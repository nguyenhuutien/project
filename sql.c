//lenh chay
//gcc -o sql sql.c `mysql_config --cflags --libs`
// insert data
#include <my_global.h>
#include <mysql.h>
#include "string_handling.c"
#include "define.c"
#include "object.c"

// finish neu xay ra loi khi ket noi den mysql
void finishWithError(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
}

MYSQL* connectMysql() {
  MYSQL *con = mysql_init(NULL);

  if (con == NULL)
  {
    fprintf(stderr, "mysql_init() failed\n");
    exit(1);
  }

  if (mysql_real_connect(con, "localhost", DB_USER_NAME, DB_PASS_WORD,
    DB_NAME, 0, NULL, 0) == NULL)
  {
    finishWithError(con);
  }
  return con;
}

MYSQL_RES* excuteSQL(MYSQL* con, char* command) {
  if (mysql_query(con, command))
  {
    finishWithError(con);
    return NULL;
  }

  MYSQL_RES *result = mysql_store_result(con);

  if (result == NULL)
  {
    finishWithError(con);
    return NULL;
  }

  return result;
}

Object getAccount(MYSQL* con, char* name) {
  Object obj;
  obj.userID = 0;

  MYSQL_RES *result;
  result = excuteSQL(con, commandSelectAccount(name));
  if (result == NULL)
    return obj;

  MYSQL_ROW row;
  while((row = mysql_fetch_row(result))) {
    if (row[0] != NULL)
      obj.userID = atoi(row[0]);
    strcpy(obj.passWord, row[1]);
  }

  mysql_free_result(result);
  mysql_close(con);
  return obj;
}

int getMaxId(MYSQL* con, char *table) {
  MYSQL_RES *result;
  char *command = malloc(sizeof(char) *CMD_LEN);
  strcpy(command, SELECT_MAX_ID);
  strcat(command, table);
  result = excuteSQL(con, command);
  if (result == NULL)
    return 0;

  int id = 0;
  MYSQL_ROW row;
  while((row = mysql_fetch_row(result))) {
    if (row[0] != NULL)
      id = atoi(row[0]);
  }

  mysql_free_result(result);
  mysql_close(con);
  return id;
}

void insertUser(MYSQL* con, char* id, char* name, char* pass) {
  if (mysql_query(con, commandInsertUser(id, name, pass))) {
    finishWithError(con);
  }
  mysql_close(con);
}

int getUserFile(MYSQL* con, char* filename, char* userid) {
  MYSQL_RES *result = excuteSQL(con, commandSelectUserFile(filename, userid));
  int num_rows = mysql_num_rows(result);
  return num_rows;
}

void insertUserFile(MYSQL* con, char* id, char* filename, char* userid) {
  if (!getUserFile(con, filename, userid)) {
    if (mysql_query(con, commandInsertUserFile(id, filename, userid))) {
      finishWithError(con);
    }
  }
  mysql_close(con);
}

Object getListFile(MYSQL* con, int id) {
  Object obj;
  MYSQL_RES *result;
  result = excuteSQL(con, commandSelectListFile(id));
  if (result == NULL) {
    return obj;
  }

  int num_rows = mysql_num_rows(result);
  obj.sizeListFile = num_rows;
  MYSQL_ROW row;
  int count = 0;
  while ((row = mysql_fetch_row(result)))
  {
    if (row[0] != NULL)
      strcpy(obj.listFile[count++], row[0]);
  }

  mysql_free_result(result);
  mysql_close(con);
  return obj;
}
