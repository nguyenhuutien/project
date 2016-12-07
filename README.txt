Đầu tiên vào terminal, di chuyên tới thư mục project

Bên server để biên dịch chương trình, chạy câu lệnh:
gcc -o server server.c ./src/libunp.a `mysql_config --cflags --libs`
Sau khi biên dịch, chạy ./server <Port> để chạy server
 - Trong <Port> là cổng để server lắng nghe yêu cầu từ client

Bên client để biên dịch chương trình, chạy câu lệnh
gcc -o client client.c ./src/libunp.a
Sau khi biên dịch chạy ./client <IPaddress> <Port>  để chạy client
 - Trong đó IPadress là địa chỉ IP của server
 - Port là cổng mà server lắng nghe

Giao diện đăng nhập:
Chọn 1: Để đăng kí tài khoản
	- Nhập username và password
		+ Nếu username chưa tồn tại thì sẽ đăng kí thành công
		+ Ngược lại sẽ đăng kí thất bại
Chọn 2: Để đăng nhập
	- Nhập username và password
		+ Nếu tài khoản hợp lệ, sẽ chuyển tới giao diện chính
		+ Thất bại sẽ trở lại giao diện đăng nhập
Chọn 3: Để thoát chương trình

Giao diện chính (Sau khi đăng nhập thành công)
Chọn 1: Để lấy danh sách file mà người dùng đã up lên server
Chọn 2: Để tải file từ server về
	- Nhập tên file cần tải
		+ Nếu đúng tên file đã tải thì sẽ báo thành công, và file tải về sẽ được lưu trong mục dowloads/{ID của người đang đăng nhập}
		+ Sai tên file thì sẽ báo thất bại
Chọn 3: Để tải file
	- Nhập tên đường dẫn tới file
		+ File tồn tại sẽ báo thành công, và file đã up sẽ được lưu trong mục uploads/{ID của người đang đăng nhập}
		+ Nếu file không tồn tại sẽ báo lỗi
Chọn 4: Để thoát đăng nhập
	- Sau khi thoát sẽ trở về giao diện đăng nhập
