#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.c"

int displayLock() {
  int choice;

  printf("\t\tTrang dang nhap\n");
  printf("\t1. Dang ki tai khoan\n");
  printf("\t2. Dang nhap\n");
  printf("\t3. Thoat\n");
  do {
    printf("Ban chon: ");
    scanf("%d", &choice); DEL();
    if (choice < 1 || choice > 3) {
      printf("Ban chon sai.\n");
    }
  } while (choice < 1 || choice > 3);
  return choice;
}

int displayUnlock() {
  int choice;

  printf("\t\tTrang quan ly file\n");
  printf("\t1. Danh sach cac file ma ban da upload\n");
  printf("\t2. Download file\n");
  printf("\t3. Upload file\n");
  printf("\t4. Dang xuat\n");
  do {
    printf("Ban chon: ");
    scanf("%d", &choice); DEL();
    if (choice < 1 || choice > 4) {
      printf("Ban chon sai.\n");
    }
  } while (choice < 1 || choice > 4);
  return choice;
}
