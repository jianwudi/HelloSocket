
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <WinSock2.h>
//#pragma comment(lib,"ws2_32.lib")  

enum Cmd
{
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_ERROR
};
struct DataHeader
{
	short dataLength;
	short cmd;
};
struct Login :public DataHeader
{
	Login()
	{
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	char name[32];
	char password[32];
};
struct LoginResult :public DataHeader
{
	LoginResult()
	{
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;
	}
	int result;
};
struct Logout :public DataHeader
{
	Logout()
	{
		dataLength = sizeof(Logout);
		cmd = CMD_LOGOUT;
	}
	char name[32];
};
struct LogoutResult :public DataHeader
{
	LogoutResult()
	{
		dataLength = sizeof(LogoutResult);
		cmd = CMD_LOGOUT_RESULT;
		result = 0;
	}
	int result;
};
int main()
{
	WORD ver = MAKEWORD(2, 2);
	WSADATA data;
	WSAStartup(ver, &data);
	SOCKET serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == serverSock)
	{
		printf("create socket failed\n");
	}
	else
	{
		printf("create socket success\n");
	}
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	_sin.sin_port = htons(4567);
	//connect
	int ret = connect(serverSock, (sockaddr*)(&_sin), sizeof(sockaddr_in));
	if (SOCKET_ERROR == ret)
	{
		printf("connect fail\n");
	}
	while (1)
	{
		char cmdBuf[128] = {};
		scanf("%s", cmdBuf);
		if (0 == strcmp(cmdBuf, "exit"))
		{
			printf("exit");
			break;
		}
		else if (0 == strcmp(cmdBuf, "login"))
		{
			Login login;
			strcpy(login.name, "pct");
			strcpy(login.password,"123");
			send(serverSock, (char *)&login, sizeof(Login), 0);
			LoginResult retResult;
			recv(serverSock, (char *)&retResult, sizeof(LoginResult), 0);
			printf("login result:%d \n", retResult.result);

		}
		else if (0 == strcmp(cmdBuf, "logout"))
		{
			Logout logout;
			strcpy(logout.name, "pct");
			send(serverSock, (char *)&logout, sizeof(Logout), 0);
			LogoutResult retResult;
			recv(serverSock, (char *)&retResult, sizeof(LogoutResult), 0);
			printf("logout result:%d \n", retResult.result);

		}
		else
		{
			printf("error cmd\n");
		}
	}
	closesocket(serverSock);
	WSACleanup();
	printf("close socket\n");
	getchar();
	return 0;

}
