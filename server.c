//cau lenh chay server
// gcc -o server server.c ./src/libunp.a `mysql_config --cflags --libs`

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <poll.h>

#include "define.c"
#include "src/lib/unp.h"
#include "sql.c"
#include "support.c"

// Gui respone thanh cong/ that bai ve cho client ------------------------------

//Gui phan hoi de thong bao da ok
void sendResponeSuccess(int connfd, Object obj) {
  obj.status = 1;
  send(connfd, &obj, sizeof(obj), 0);
}

//Gui phan hoi de thong bao da ok
void sendResponeFailure(int connfd, Object obj) {
  obj.status = 0;
  send(connfd, &obj, sizeof(obj), 0);
}

// Thuc hien yeu cau cua client ------------------------------------------------

// Gui file cho client
int sendFile(int connfd, Object *obj) {
  char* path = (char*)malloc(sizeof(char) * PATH_LEN);
  char* id = (char*)malloc(sizeof(char) * ID_LEN);
  sprintf(id, "%d", obj->userID);
  strcpy(path, UPLOADS);
  strcat(path, id);
  strcat(path, "/");
  strcat(path, obj->fileName);
  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    return 0;
  } else {
    fseek(file, 0L, SEEK_END);
    int length = ftell(file);
    obj->fileSize = length;
    rewind(file);
    while(fread(obj->file, obj->fileSize, 1, file));
  }
  fclose(file);
  return 1;
}

// Gui danh sach file cho client
int sendListFile(int connfd, Object *obj) {
  Object object = getListFile(connectMysql(), obj->userID);
  if (object.sizeListFile == 0) {
    return 0;
  }
  obj->sizeListFile = object.sizeListFile;
  memcpy(obj->listFile, object.listFile, sizeof(object.listFile));
  return 1;
}

// kiem tra login tu client
int logIn(Object *obj) {
  Object object = getAccount(connectMysql(), obj->userName);
  if (object.userID == 0) {
    return 0;
  }
  if (strcompare(object.passWord, obj->passWord)) {
    obj->userID = object.userID;
    return 1;
  }
  return 0;
}

// kiem tra login tu client
int signUp(Object *obj) {
  Object object = getAccount(connectMysql(), obj->userName);
  char* id = malloc(sizeof(char) * ID_LEN);
  char* table = malloc(sizeof(char) * TABLE_LEN);
  strcpy(table, "users");

  // neu chua co pass tuc la chua co account
  if (object.userID == 0) {
    obj->userID = getMaxId(connectMysql(), table) + 1;
    sprintf(id, "%d", obj->userID);
    insertUser(connectMysql(), id, obj->userName, obj->passWord);

    struct stat st = {0};
    char *path = malloc(sizeof(char) * PATH_LEN);
    strcpy(path, UPLOADS);
    strcat(path, id);
    if (stat(path, &st) == -1) {
      mkdir(path, 0700);
    }
    return 1;
  }
  return 0;
}

// Nhan file tu client
int saveFile(Object obj) {
  char* userid = malloc(sizeof(char) * ID_LEN);
  char* table = malloc(sizeof(char) * TABLE_LEN);
  strcpy(table, "userfiles");
  sprintf(userid, "%d", obj.userID);
  char* path = malloc(sizeof(char) * PATH_LEN);
  strcpy(path, UPLOADS);
  strcat(path, userid);
  struct stat st = {0};
  if (stat(path, &st) == -1) {
    mkdir(path, 0700);
  }
  strcat(path, "/");
  strcat(path, obj.fileName);
  FILE *file = fopen(path, "wb");
  if (file) {
    fwrite(obj.file, obj.fileSize, 1, file);
    fclose(file);
    char* id = malloc(sizeof(char) * ID_LEN);
    int _id = getMaxId(connectMysql(), table) + 1;
    sprintf(id, "%d", _id);
    insertUserFile(connectMysql(), id, obj.fileName, userid);
    return 1;
  }
  return 0;
}

// Lang nghe trao doi voi client -----------------------------------------------

// Trao doi du lieu voi client
void exchangeClient(int connfd) {
  Object obj;
  int status;
  for( ; ; ) {
    while(recv(connfd, &obj, sizeof(Object), 0)) {
      if (strcompare(obj.method, "GET")) {
        if (strcompare(obj.typeData, "DATA")) {
          status = sendFile(connfd, &obj);
        } else if (strcompare(obj.typeData, "DATAS")) {
          status = sendListFile(connfd, &obj);
        }
      } else if (strcompare(obj.method, "POST")) {
          if (strcompare(obj.typeData, "LOG_IN")) {
            status = logIn(&obj);
          } else if (strcompare(obj.typeData, "SIGN_UP")) {
            status = signUp(&obj);
          } else if (strcompare(obj.typeData, "DATA")) {
            status = saveFile(obj);
          }
      }
      if (status) {
        sendResponeSuccess(connfd, obj);
      } else {
        sendResponeFailure(connfd, obj);
      }
    }
  }
}

// Server bat dau lang nghe
void initSocket(int port) {
  int listenfd;
  pid_t childpid, connfd;
  socklen_t clilen;
  struct sockaddr_in  cliaddr, servaddr;
  void sig_chld(int);

  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

  Listen(listenfd, LISTENQ);

  for ( ; ; ) {
    clilen = sizeof(cliaddr);
    if ((connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) {
      if (errno == EINTR)
        continue;
      else
        err_sys(ERROR_ACCEPT);
    }

    if ((childpid = Fork()) == 0) {
      close(listenfd);
      exchangeClient(connfd);
      exit(0);
    }
    close(connfd);
  }
}

void main(int argc, char **argv) {
  if (argc != 2)
    err_quit("usage: server <Port>");
  initSocket(atoi(argv[1]));
}
