//So sanh 2 xau
int strcompare(char *a, char *b) {
  return !strcmp(a, b);
}

// Cat ten file tu 1 duong dan
char *convertFileName(char fileName[]) {
  char *str = (char*)malloc(CMD_LEN * sizeof(char));
  for (char *p = strtok(fileName, "/"); p; p = strtok(NULL, "/")){
    str = p;
  }
  return str;
}
