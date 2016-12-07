//cau lenh chay server
// gcc -o client client.c ./src/libunp.a

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#include "src/lib/unp.h"
#include "display.c"
#include "input.c"

int status = 0;
int userID = 0;
int sendData = 1;

// Xu li co the gui du lieu di hay k? ------------------------------------------

//Kiem tra duoc gui tiep hay k?
int acceptSend() {
  return sendData;
}

// Cho phep client dc gui du lieu
void canSend() {
  sendData = 1;
}

// Ngan chan client duoc gui du lieu do he thong dang ban
void canntSend() {
  sendData = 0;
}

// Gui du lieu di
int sendToServer(int sockfd, Object obj) {
  int n = send(sockfd, &obj, sizeof(obj), 0);
  if (n == sizeof(obj)) {
    canntSend();
    return 1;
  } else {
    canSend();
    return 0;
  }
}

// Xu li phu phan tra ve -------------------------------------------------------------------

// Kiem tra log in hay chua
int userSignedIn() {
  return status;
}

// Luu file khi tai ve
void saveFile(Object obj) {
  char* userid = malloc(sizeof(char) * ID_LEN);
  sprintf(userid, "%d", obj.userID);
  char* path = malloc(sizeof(char) * PATH_LEN);
  strcpy(path, DOWNLOADS);
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
  }
}

// Hien thi danh sach file da upload
void printListFile(Object obj) {
  for (int i = 0; i < obj.sizeListFile; ++i)
  {
    printf("%d. %s\n", i + 1, obj.listFile[i]);
  }
  printf("\n");
}

// Thiet lap du lieu khi login thanh cong
void loggedIn(Object obj) {
  status = 1;
  userID = obj.userID;
}

// Thiet lap du lieu khi signup thanh cong
void signedUp(Object obj) {
  status = 1;
  userID = obj.userID;
}

// Thiet lap du lieu khi dang xuat
void loggedOut() {
  status = 0;
  userID = 0;
}

// Nhan du lieu tra ve ---------------------------------------------------------

// Respone getFile
void responeGetFile(Object obj) {
  if (obj.status) {
    printf("%s\n", GET_FILE_SUCCESS);
    saveFile(obj);
  } else {
    printf("%s\n", GET_FILE_FAIL);
  }
}

// Respone getListFile
void responeGetListFile(Object obj) {
  if (obj.status) {
    printf("%s\n", GET_LIST_FILE_SUCCESS);
    printListFile(obj);
  } else {
    printf("%s\n", GET_LIST_FILE_FAIL);
  }
}

// Respone login
void responeLogin(Object obj) {
  if (obj.status) {
    printf("%s\n", LOG_IN_SUCCESS);
    loggedIn(obj);
  } else {
    printf("%s\n", LOG_IN_FAIL);
  }
}

// Respone signup
void responeSignUp(Object obj) {
  if (obj.status) {
    printf("%s\n", SIGN_UP_SUCCESS);
    signedUp(obj);
  } else {
    printf("%s\n", SIGN_UP_FAIL);
  }
}

// Respone up file
void responeUpFile(Object obj) {
  if(obj.status) {
    printf("%s\n", UP_FILE_SUCCESS);
  } else {
    printf("%s\n", UP_FILE_FAIL);
  }
}

// Gui du lieu di --------------------------------------------------------------

// Gui yeu cau lay file
int getFile(int sockfd, Object obj) {
  obj.userID = userID;
  strcpy(obj.method, "GET");
  strcpy(obj.typeData, "DATA");
  strcpy(obj.fileName, convertFileName(obj.fileName));
  return sendToServer(sockfd, obj);
}

// Gui yeu cau lay danh sach file
int getListFile(int sockfd, Object obj) {
  obj.userID = userID;
  strcpy(obj.method, "GET");
  strcpy(obj.typeData, "DATAS");
  return sendToServer(sockfd, obj);
}

// ham login
int logIn(int sockfd, Object obj) {
  strcpy(obj.method, "POST");
  strcpy(obj.typeData, "LOG_IN");
  return sendToServer(sockfd, obj);
}

// ham sign up
int signUp(int sockfd, Object obj) {
  strcpy(obj.method, "POST");
  strcpy(obj.typeData, "SIGN_UP");
  return sendToServer(sockfd, obj);
}

// ham send file
int sendFile(int sockfd, Object obj) {
  obj.userID = userID;
  strcpy(obj.method, "POST");
  strcpy(obj.typeData, "DATA");

  FILE *file = fopen(obj.fileName, "rb");
  if (file == NULL) {
    printf("%s\n", FILE_NOT_FOUND);
    return 0;
  }

  fseek(file, 0L, SEEK_END);
  int length = ftell(file);
  obj.fileSize = length;
  rewind(file);
  strcpy(obj.fileName, convertFileName(obj.fileName));
  while(fread(obj.file, obj.fileSize, 1, file));
  fclose(file);
  return sendToServer(sockfd, obj);
}


// Lang nghe repsone tu server -------------------------------------------------

// Trao doi du lieu voi server
void exchangeServer(int sockfd) {
  Object obj;
  for ( ; ; ) {
    while(recv(sockfd, &obj, sizeof(Object), 0)) {
      if (strcompare(obj.method, "GET")) {
        if (strcompare(obj.typeData, "DATA")) {
          responeGetFile(obj);
        } else if (strcompare(obj.typeData, "DATAS")) {
          responeGetListFile(obj);
        }
      } else if (strcompare(obj.method, "POST")) {
          if (strcompare(obj.typeData, "LOG_IN")) {
            responeLogin(obj);
          } else if (strcompare(obj.typeData, "SIGN_UP")) {
            responeSignUp(obj);
          } else if (strcompare(obj.typeData, "DATA")) {
            responeUpFile(obj);
          }
      }
      break;
    }
    canSend();
    break;
  }
}

// ham khoi tao socket
int initSocket(char *addressServer, int port) {
  int sockfd;
  struct sockaddr_in servaddr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  inet_pton(AF_INET, addressServer, &servaddr.sin_addr);
  if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) == -1)
    return 0;

  return sockfd;
}

void menu(int sockfd) {
  int choice = 0;
  Object obj;

  start: while (!userSignedIn()) {
    choice = displayLock();
    if (acceptSend()) {
      switch(choice) {
        case 1:
          obj = inputData();
          if (signUp(sockfd, obj))
            exchangeServer(sockfd);
          break;
        case 2:
          obj = inputData();
          if (logIn(sockfd, obj))
            exchangeServer(sockfd);
          break;
        case 3:
          exit(0);
          break;
      }
    } else {
      printf("%s\n", SYSTEM_BUSY);
    }
  }

  while (userSignedIn()) {
    choice = displayUnlock();
    if (acceptSend()) {
      switch(choice) {
        case 1:
          if (getListFile(sockfd, obj))
            exchangeServer(sockfd);
          break;
        case 2:
          obj = inputFile(obj);
          if (getFile(sockfd, obj))
            exchangeServer(sockfd);
          break;
        case 3:
          obj = inputFile(obj);
          if (sendFile(sockfd, obj))
            exchangeServer(sockfd);
          break;
        case 4:
          loggedOut();
          goto start;
          break;
      }
    } else {
      printf("%s\n", SYSTEM_BUSY);
    }
  }
}

void main(int argc, char **argv) {
  if (argc != 3)
    err_quit("usage: client <IPaddress> <Port>");
  int sockfd = initSocket(argv[1], atoi(argv[2]));
  if (sockfd) {
    menu(sockfd);
  } else {
    err_quit("Invalid <IPaddress> or <Port> or Server not ready!!");
  }
}
