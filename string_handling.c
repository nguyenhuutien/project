#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "define.c"

// muc tieu cua ham nay la tao ra string co dang nhu duoi
// "SELECT PassWord FROM Account WHERE UserName = 'le tat tung'"
char* commandSelectAccount(char *name){
  char *command = malloc(sizeof(char) * CMD_LEN);
  strcpy(command, SELECT_ACCOUNT);
  strcat(command, "\'");
  strcat(command, name);
  strcat(command, "\'");

  return command;
}

char* commandSelectListFile(int id){
  char *command = malloc(sizeof(char) * CMD_LEN);
  char id_account[ID_LEN];
  sprintf(id_account, "%d", id);
  strcpy(command, SELECT_LIST_FILE_NAME);
  strcat(command, "\'");
  strcat(command, id_account);
  strcat(command, "\'");

  return command;
}

// muc tieu cua ham nay la tao ra string co dang nhu duoi
// "INSERT INTO Account VALUES(1,'cao van minh','minh123')"
char* commandInsertUser(char* id, char* name, char *pass){
  char* command = malloc(sizeof(char) * CMD_LEN);
  strcpy(command, INSERT_USER);
  strcat(command, id);
  strcat(command, ",\'");
  strcat(command, name);
  strcat(command, "\',\'");
  strcat(command, pass);
  strcat(command, "\')");

  return command;
}

char* commandSelectUserFile(char* filename, char* userid) {
  char *command = malloc(sizeof(char) * CMD_LEN);
  strcpy(command, SELECT_USER_FILE);
  strcat(command, "\'");
  strcat(command, filename);
  strcat(command, "\'");
  strcat(command, " AND userid = ");
  strcat(command, "\'");
  strcat(command, userid);
  strcat(command, "\'");

  return command;
}

char* commandInsertUserFile(char* id, char* filename, char *userid){
  char *command = malloc(sizeof(char) * CMD_LEN);
  strcpy(command, INSERT_USER_FILE);
  strcat(command, id);
  strcat(command, ",\'");
  strcat(command, filename);
  strcat(command, "\',\'");
  strcat(command, userid);
  strcat(command, "\')");

  return command;
}
