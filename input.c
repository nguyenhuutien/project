#include <string.h>
#include "object.c"
#include "support.c"

int checkInput(char *buff) {
  int length = PASS_WORD_LEN;
  for (int i = 0; i < strlen(buff); ++i) {
    if (buff[i] == ' ') {
      return 0;
    }
  }
  return (strlen(buff) >= 1 && strlen(buff) <= length);
}

// Kiem tra do dai ten file toi da
int checkFileNameLength(char *fileName) {
  return (strlen(fileName) <= FILE_NAME_LEN);
}

Object inputData() {
  Object object;

  printf("Nhap thong tin cho tai khoan\n");
  while(1) {
    printf("Nhap username: ");
    scanf("%[^\n]", object.userName); DEL();
    if(!checkInput(object.userName)) {
      printf("Dau vao sai. Moi nhap lai!!\n");
    } else break;
  }

  while(1) {
    printf("Nhap password: ");
    scanf("%[^\n]", object.passWord); DEL();
    if(!checkInput(object.passWord)) {
      printf("Dau vao sai. Moi nhap lai!!\n");
    } else break;
  }

  return object;
}

Object inputFile(Object object) {
  int checkLength = 0;
  char str[FILE_NAME_LEN];
  do {
    printf("Nhap ten file: ");
    scanf("%[^\n]", object.fileName); DEL();
    strcpy(str, object.fileName);
    checkLength = checkFileNameLength(convertFileName(str));
  } while(!checkLength);
  return object;
}
