#include "define.c"

typedef struct Obj {
  char method[METHOD_LEN];
  char typeData[TYPE_DATA_LEN];
  char fileName[FILE_NAME_LEN];
  char file[FILE_LEN];
  int fileSize;
  char listFile[ARRAY_FILE_LEN][FILE_NAME_LEN];
  int sizeListFile;
  int userID;
  char userName[USER_NAME_LEN];
  char passWord[PASS_WORD_LEN];
  int status;
} Object;
