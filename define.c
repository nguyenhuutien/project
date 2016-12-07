#define DEL() while (getchar() != '\n') // chong luu dau enter tu xau truoc

#define ADDR_SERVER "127.0.0.1"
#define PORT 1995
#define BUFLEN 512  //Kich thuoc toi da 1 lan truyen
#define METHOD_LEN 5
#define TYPE_DATA_LEN 10
#define FILE_LEN 1024
#define FILE_NAME_LEN 200
#define USER_NAME_LEN 30
#define PASS_WORD_LEN 30
#define ARRAY_FILE_LEN 30
#define CMD_LEN 100
#define PATH_LEN 50
#define ID_LEN 3
#define TABLE_LEN 10

#define SLASH "|"
#define SIGN_UP "SIGN_UP"
#define LOG_IN "LOG_IN"
#define UPLOADS "uploads/"
#define DOWNLOADS "downloads/"
#define LOG_IN_SUCCESS "Dang nhap thanh cong!!\n"
#define LOG_IN_FAIL "Dang nhap that bai!!\n"
#define SIGN_UP_SUCCESS "Dang ki thanh cong!!\n"
#define SIGN_UP_FAIL "Dang ki that bai!!\n"
#define UP_FILE_SUCCESS "Up file thanh cong!!\n"
#define UP_FILE_FAIL "Up file that bai!!\n"
#define FILE_NOT_FOUND "Khong tim thay file!!\n"
#define GET_FILE_SUCCESS "Down file thanh cong!!\n"
#define GET_FILE_FAIL "Down file that bai!!\n"
#define GET_LIST_FILE_SUCCESS "Lay danh sach file thanh cong!!\n"
#define GET_LIST_FILE_FAIL "Khong co file nao!!\n"

//DATABASE
#define DB_USER_NAME "root"
#define DB_PASS_WORD "123456"
#define DB_NAME "socket"

// cac lenh truy van den database
#define SELECT_ACCOUNT "SELECT id, password FROM users WHERE username = "
#define SELECT_MAX_ID "SELECT MAX(id) FROM "
#define SELECT_LIST_FILE_NAME "SELECT filename FROM userfiles WHERE userid = "
#define SELECT_USER_FILE "SELECT id FROM userfiles WHERE filename = "
#define INSERT_USER "INSERT INTO users VALUES("
#define INSERT_USER_FILE "INSERT INTO userfiles VALUES("

// Cac loi
#define ERROR_ACCEPT "Loi chap nhan ket noi client"
#define SYSTEM_BUSY "He thong dang ban!!"
