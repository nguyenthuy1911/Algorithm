#define _WIN32_WINNT 0x501

#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define VERSION "1.00"
#define DEFAULT_BUFLEN 4096
#define DEFAULT_PORT "9999"


void Function1(char* Input);
void Function2(char* Input);
void Function3(char* Input);
void Function4(char* Input);
DWORD WINAPI ConnectionHandler(LPVOID CSocket);


int main(int argc, char* argv[]) {
	char PortNumber[6];
	const char Usage[94] = "Usage: %s [port_number]\n\nIf no port number is provided, the default port of %s will be used.\n";
	if (argc > 2) {
		printf(Usage, argv[0], DEFAULT_PORT);
		return 1;
	}
	else if (argc == 2) {
		//===insecure code====
		/*
		if ((atoi(argv[1]) > 0) && (atoi(argv[1]) < 65536) && (strlen(argv[1]) < 7)) {
			strncpy(PortNumber, argv[1], 6);
		}
		*/

		//===fix====
		if ((atoi(argv[1]) > 0) && (atoi(argv[1]) < 65536) && (strnlen(argv[1], 7) < 7)) {
			strncpy(PortNumber, argv[1], 5);
			PortNumber[5] = '\0';
		}
		//==========

		else {
			printf(Usage, argv[0], DEFAULT_PORT);
			return 1;
		}
	}
	else {
		strncpy(PortNumber, DEFAULT_PORT, 6);
	}
	printf("Starting vulnserver version %s\n", VERSION);
	EssentialFunc1(); // Call function from external dll
	printf("\nThis is vulnerable software!\nDo not allow access from untrusted systems or networks!\n\n");
	WSADATA wsaData;
	SOCKET ListenSocket = INVALID_SOCKET,
		ClientSocket = INVALID_SOCKET;
	struct addrinfo* result = NULL, hints;
	int Result;
	struct sockaddr_in ClientAddress;
	int ClientAddressL = sizeof(ClientAddress);

	Result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (Result != 0) {
		printf("WSAStartup failed with error: %d\n", Result);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	Result = getaddrinfo(NULL, PortNumber, &hints, &result);
	if (Result != 0) {
		printf("Getaddrinfo failed with error: %d\n", Result);
		WSACleanup();
		return 1;
	}

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	Result = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (Result == SOCKET_ERROR) {
		printf("Bind failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	Result = listen(ListenSocket, SOMAXCONN);
	if (Result == SOCKET_ERROR) {
		printf("Listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	while (ListenSocket) {
		printf("Waiting for client connections...\n");

		ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientAddress, &ClientAddressL);
		if (ClientSocket == INVALID_SOCKET) {
			printf("Accept failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		printf("Received a client connection from %s:%u\n", inet_ntoa(ClientAddress.sin_addr), htons(ClientAddress.sin_port));
		CreateThread(0, 0, ConnectionHandler, (LPVOID)ClientSocket, 0, 0);

	}

	closesocket(ListenSocket);
	WSACleanup();

	return 0;
}

//===insecure code====
/*
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
*/
		
//===fix====check_input
void Function1(char* Input) {
	char Buffer2S[140];
	strncpy(Buffer2S, Input, sizeof(Buffer2S) - 1);
	Buffer2S[sizeof(Buffer2S) - 1] = '\0';
}

void Function2(char* Input) {
	char Buffer2S[60];
	strncpy(Buffer2S, Input, sizeof(Buffer2S) - 1);
	Buffer2S[sizeof(Buffer2S) - 1] = '\0';
}

void Function3(char* Input) {
	char Buffer2S[2000];
	strncpy(Buffer2S, Input, sizeof(Buffer2S) - 1);
	Buffer2S[sizeof(Buffer2S) - 1] = '\0';
}

void Function4(char* Input) {
	char Buffer2S[1000];
	strncpy(Buffer2S, Input, sizeof(Buffer2S) - 1);
	Buffer2S[sizeof(Buffer2S) - 1] = '\0';
}
//=========

DWORD WINAPI ConnectionHandler(LPVOID CSocket) {
	int RecvBufLen = DEFAULT_BUFLEN;
	//===code====
	/*
	char* RecvBuf = malloc(DEFAULT_BUFLEN);
	char BigEmpty[1000];
	char* GdogBuf = malloc(1024);
	int Result, SendResult, i, k;
	memset(BigEmpty, 0, 1000);
	memset(RecvBuf, 0, DEFAULT_BUFLEN);
	*/

	//===recommend====Use calloc instead of malloc and memset, delete unused variable
	char* RecvBuf = (char*)calloc(DEFAULT_BUFLEN, sizeof(char));
	char* GdogBuf = (char*)calloc(1024, sizeof(char));
	int Result, SendResult, i, k;	
	//==========

	SOCKET Client = (SOCKET)CSocket;
	SendResult = send(Client, "Welcome to Vulnerable Server! Enter HELP for help.\n", 51, 0);
	if (SendResult == SOCKET_ERROR) {
		printf("Send failed with error: %d\n", WSAGetLastError());
		closesocket(Client);
		return 1;
	}
	while (CSocket) {
		Result = recv(Client, RecvBuf, RecvBufLen, 0);
		if (Result > 0) {
			if (strncmp(RecvBuf, "HELP ", 5) == 0) {
				const char NotImplemented[47] = "Command specific help has not been implemented\n";
				SendResult = send(Client, NotImplemented, sizeof(NotImplemented), 0);
			}
			else if (strncmp(RecvBuf, "HELP", 4) == 0) {
				const char ValidCommands[251] = "Valid Commands:\nHELP\nSTATS [stat_value]\nRTIME [rtime_value]\nLTIME [ltime_value]\nSRUN [srun_value]\nTRUN [trun_value]\nGMON [gmon_value]\nGDOG [gdog_value]\nKSTET [kstet_value]\nGTER [gter_value]\nHTER [hter_value]\nLTER [lter_value]\nKSTAN [lstan_value]\nEXIT\n";
				SendResult = send(Client, ValidCommands, sizeof(ValidCommands), 0);
			}
			else if (strncmp(RecvBuf, "STATS ", 6) == 0) {
				//===insecure code====
				/*
				char* StatBuf = malloc(120);
				memset(StatBuf, 0, 120);
				strncpy(StatBuf, RecvBuf, 120);
				*/

				//===fix===check_null
				char* StatBuf = malloc(120);
				if (StatBuf != NULL) {
					memset(StatBuf, 0, 120);
					strncpy(StatBuf, RecvBuf, 120);
				}
				//=========

				SendResult = send(Client, "STATS VALUE NORMAL\n", 19, 0);
			}
			else if (strncmp(RecvBuf, "RTIME ", 6) == 0) {
				//===insecure code====
				/*
				char* RtimeBuf = malloc(120);
				memset(RtimeBuf, 0, 120);
				strncpy(RtimeBuf, RecvBuf, 120);
				*/

				//===fix===check_null
				char* RtimeBuf = malloc(120);
				if (RtimeBuf != NULL) {
					memset(RtimeBuf, 0, 120);
					strncpy(RtimeBuf, RecvBuf, 120);
				}
				//=========
				
				SendResult = send(Client, "RTIME VALUE WITHIN LIMITS\n", 26, 0);
			}
			else if (strncmp(RecvBuf, "LTIME ", 6) == 0) {
				//===insecure code====
				/*
				char* LtimeBuf = malloc(120);
				memset(LtimeBuf, 0, 120);
				strncpy(LtimeBuf, RecvBuf, 120);
				*/

				//===fix===check_null
				char* LtimeBuf = malloc(120);
				if (LtimeBuf != NULL) {
					memset(LtimeBuf, 0, 120);
					strncpy(LtimeBuf, RecvBuf, 120);
				}
				//=========
				
				SendResult = send(Client, "LTIME VALUE HIGH, BUT OK\n", 25, 0);
			}
			else if (strncmp(RecvBuf, "SRUN ", 5) == 0) {
				//===insecure code====
				/*
				char* SrunBuf = malloc(120);
				memset(SrunBuf, 0, 120);
				strncpy(SrunBuf, RecvBuf, 120);
				*/

				//===fix===check_null
				char* SrunBuf = malloc(120);
				if (SrunBuf != NULL) {
					memset(SrunBuf, 0, 120);
					strncpy(SrunBuf, RecvBuf, 120);
				}
				//=========
				
				SendResult = send(Client, "SRUN COMPLETE\n", 14, 0);
			}
			else if (strncmp(RecvBuf, "TRUN ", 5) == 0) {
				char* TrunBuf = malloc(3000);
				//===insecure code====
				/*
				memset(TrunBuf, 0, 3000);
				for (i = 5; i < RecvBufLen; i++) {
					if ((char)RecvBuf[i] == '.') {
						strncpy(TrunBuf, RecvBuf, 3000);
						Function3(TrunBuf);
						break;
					}
				}
				memset(TrunBuf, 0, 3000);
				*/

				//===fix===check_null
				if (TrunBuf != NULL) {
					memset(TrunBuf, 0, 3000);
					for (i = 5; i < RecvBufLen; i++) {
						if ((char)RecvBuf[i] == '.') {
							strncpy(TrunBuf, RecvBuf, 3000);
							Function3(TrunBuf);
							break;
						}
					}
					memset(TrunBuf, 0, 3000);
				}
				//=========
				
				SendResult = send(Client, "TRUN COMPLETE\n", 14, 0);
			}
			else if (strncmp(RecvBuf, "GMON ", 5) == 0) {
				char GmonStatus[13] = "GMON STARTED\n";
				for (i = 5; i < RecvBufLen; i++) {
					if ((char)RecvBuf[i] == '/') {
						//===insecure code====
						/*
						if (strlen(RecvBuf) > 3950) {
							Function3(RecvBuf);
						}
						*/

						//===fix====
						if (strnlen(RecvBuf, 3950) > 3095) {
							Function3(RecvBuf);
						}
						//========
						break;
					}
				}
				SendResult = send(Client, GmonStatus, sizeof(GmonStatus), 0);
			}
			else if (strncmp(RecvBuf, "GDOG ", 5) == 0) {
				strncpy(GdogBuf, RecvBuf, 1024);
				SendResult = send(Client, "GDOG RUNNING\n", 13, 0);
			}
			else if (strncmp(RecvBuf, "KSTET ", 6) == 0) {
				char* KstetBuf = malloc(100);
				//===insecure code====
				/*
				strncpy(KstetBuf, RecvBuf, 100);
				memset(RecvBuf, 0, DEFAULT_BUFLEN);
				Function2(KstetBuf);
				*/

				//===fix===check_null
				if (KstetBuf != NULL) {
					strncpy(KstetBuf, RecvBuf, 100);
					memset(RecvBuf, 0, DEFAULT_BUFLEN);
					Function2(KstetBuf);
				}
				//=========
				
				SendResult = send(Client, "KSTET SUCCESSFUL\n", 17, 0);
			}
			else if (strncmp(RecvBuf, "GTER ", 5) == 0) {
				char* GterBuf = malloc(180);
				//===insecure code====
				/*
				memset(GdogBuf, 0, 1024);
				strncpy(GterBuf, RecvBuf, 180);
				memset(RecvBuf, 0, DEFAULT_BUFLEN);
				Function1(GterBuf);
				*/

				//===fix===check_null
				if (GterBuf != NULL && GterBuf != NULL) {
					memset(GdogBuf, 0, 1024);
					strncpy(GterBuf, RecvBuf, 180);
					memset(RecvBuf, 0, DEFAULT_BUFLEN);
					Function1(GterBuf);
				}
				//=========
				
				SendResult = send(Client, "GTER ON TRACK\n", 14, 0);
			}
			else if (strncmp(RecvBuf, "HTER ", 5) == 0) {
				char THBuf[3];
				memset(THBuf, 0, 3);
				char* HterBuf = malloc((DEFAULT_BUFLEN + 1) / 2);
				//===insecure code====
				/*
				memset(HterBuf, 0, (DEFAULT_BUFLEN + 1) / 2);
				i = 6;
				k = 0;
				while ((RecvBuf[i]) && (RecvBuf[i + 1])) {
					memcpy(THBuf, (char*)RecvBuf + i, 2);
					unsigned long j = strtoul((char*)THBuf, NULL, 16);
					memset((char*)HterBuf + k, (byte)j, 1);
					i = i + 2;
					k++;
				}
				Function4(HterBuf);
				memset(HterBuf, 0, (DEFAULT_BUFLEN + 1) / 2);
				*/

				//===fix===check_null
				if (HterBuf != NULL) {
					memset(HterBuf, 0, (DEFAULT_BUFLEN + 1) / 2);
					i = 6;
					k = 0;
					while ((RecvBuf[i]) && (RecvBuf[i + 1])) {
						memcpy(THBuf, (char*)RecvBuf + i, 2);
						unsigned long j = strtoul((char*)THBuf, NULL, 16);
						memset((char*)HterBuf + k, (byte)j, 1);
						i = i + 2;
						k++;
					}
					Function4(HterBuf);
					memset(HterBuf, 0, (DEFAULT_BUFLEN + 1) / 2);
				}
				//=========
				
				SendResult = send(Client, "HTER RUNNING FINE\n", 18, 0);
			}
			else if (strncmp(RecvBuf, "LTER ", 5) == 0) {
				char* LterBuf = malloc(DEFAULT_BUFLEN);
				//===insecure code====
				/*
				memset(LterBuf, 0, DEFAULT_BUFLEN);
				i = 0;
				while (RecvBuf[i]) {
					if ((byte)RecvBuf[i] > 0x7f) {
						LterBuf[i] = (byte)RecvBuf[i] - 0x7f;
					}
					else {
						LterBuf[i] = RecvBuf[i];
					}
					i++;
				}
				for (i = 5; i < DEFAULT_BUFLEN; i++) {
					if ((char)LterBuf[i] == '.') {
						Function3(LterBuf);
						break;
					}
				}
				memset(LterBuf, 0, DEFAULT_BUFLEN);
				*/

				//===fix===check_null
				if (LterBuf != NULL) {
					memset(LterBuf, 0, DEFAULT_BUFLEN);
					i = 0;
					while (RecvBuf[i]) {
						if ((byte)RecvBuf[i] > 0x7f) {
							LterBuf[i] = (byte)RecvBuf[i] - 0x7f;
						}
						else {
							LterBuf[i] = RecvBuf[i];
						}
						i++;
					}
					for (i = 5; i < DEFAULT_BUFLEN; i++) {
						if ((char)LterBuf[i] == '.') {
							Function3(LterBuf);
							break;
						}
					}
					memset(LterBuf, 0, DEFAULT_BUFLEN);
				}
				//=========
				
				SendResult = send(Client, "LTER COMPLETE\n", 14, 0);
			}
			else if (strncmp(RecvBuf, "KSTAN ", 6) == 0) {
				SendResult = send(Client, "KSTAN UNDERWAY\n", 15, 0);
			}
			else if (strncmp(RecvBuf, "EXIT", 4) == 0) {
				SendResult = send(Client, "GOODBYE\n", 8, 0);
				printf("Connection closing...\n");
				closesocket(Client);
				return 0;
			}
			else {
				SendResult = send(Client, "UNKNOWN COMMAND\n", 16, 0);
			}
			if (SendResult == SOCKET_ERROR) {
				printf("Send failed with error: %d\n", WSAGetLastError());
				closesocket(Client);
				return 1;
			}
		}
		else if (Result == 0) {
			printf("Connection closing...\n");
			closesocket(Client);
			return 0;
		}
		else {
			printf("Recv failed with error: %d\n", WSAGetLastError());
			closesocket(Client);
			return 1;
		}

	}
}