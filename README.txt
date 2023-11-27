I: Lỗi 

1. PortNumber không kết thúc bằng kí tự null

Line 19:		char PortNumber[6];
Line 27: 		strncpy(PortNumber, argv[1], 6);


2. Sử dụng các hàm không an toàn 

a, Sử dụng strlen (nếu argv[1] không có kí tự null ở cuối strlen sẽ tiếp tục đọc qua bộ nhớ cho đến khi gặp ký tự null, có thể dẫn đến undefined behavior)

Line 26: 		if ((atoi(argv[1]) > 0) && (atoi(argv[1]) < 65536) && (strlen(argv[1]) < 7)){...}
Line 204:		if (strlen(RecvBuf) > 3950) {}


b, Sử dụng hàm strcpy (trong Function1, Function2, Function3, và Function4) mà không kiểm tra độ dài Input -> buffer overflow 

Line 116 -> 134:
			void Function1(char* Input) {
				char Buffer2S[140];
				strcpy(Buffer2S, Input);
			}

			void Function2(char* Input) {
				char Buffer2S[60];
				strcpy(Buffer2S, Input);
			}

			void Function3(char* Input) {
				char Buffer2S[2000];
				strcpy(Buffer2S, Input);
			}

			void Function4(char* Input) {
				char Buffer2S[1000];
				strcpy(Buffer2S, Input);
			}

3. Không kiểm tra null khi cấp phát bộ nhớ bằng malloc

4. Biến không sử dụng: BigEmpty


II: Cách khai thác lỗ hổng

1. Gây tràn bộ đệm

Số byte tối đa đối với đầu vào từng lệnh để gây tràn bộ đệm
- KSTET > 60 byte
- TRUN: > 2000 byte cộng với đối số đầu vào phải chứa ký tự '.'
- GMON: > 3950 byte và đối số đầu vào phải chứa ký tự '/'.
- GTER: > 140 byte
- HTER: > 1000 byte
- LTER: > 2000 byte cộng với đối số đầu vào phải chứa ký tự '.'

2. Thực thi mã độc

Ghi đè lên thanh ghi EIP để chuyển hướng luồng thực thi đến địa chỉ nằm trong thanh ghi ESP nơi sẽ chứa shellcode độc hại.

III: Code đã sửa

	file project1_fix.c