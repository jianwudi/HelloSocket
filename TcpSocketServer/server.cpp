#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include <WinSock2.h>
#include <stdio.h>

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
struct Login:public DataHeader
{
	Login()
	{
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	char name[32];
	char password[32];
};
struct LoginResult:public DataHeader
{
	LoginResult()
	{
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;
	}
	int result;
};
struct Logout:public DataHeader
{
	Logout()
	{
		dataLength = sizeof(Logout);
		cmd = CMD_LOGOUT;
	}
	char name[32];
};
struct LogoutResult:public DataHeader
{
	LogoutResult()
	{
		dataLength = sizeof(LogoutResult);
		cmd = CMD_LOGOUT_RESULT;
		result = 0;
	}
	int result;
};
//#pragma comment(lib,"ws2_32.lib")  
int main()
{
	WORD ver = MAKEWORD(2, 2);
	WSADATA data;
	WSAStartup(ver, &data);
	sockaddr_in _sin = {};
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);
	_sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(_sock, (sockaddr*)&_sin, sizeof(_sin)) == SOCKET_ERROR)
	{
		printf("bind socket error\n");
	}
	else
	{
		printf("bind socket success\n");
	}
	if (SOCKET_ERROR == listen(_sock, 5))
	{
		printf("listen socket error\n");
	}
	sockaddr_in clientAddr = {};
	int clientAddrLen = sizeof(sockaddr_in);
	SOCKET clientSock = INVALID_SOCKET;
	//接收请求
	clientSock = accept(_sock, (sockaddr*)&clientAddr, &clientAddrLen);
	if (INVALID_SOCKET == clientSock)
	{
		printf("accept error\n");
	}
	printf("clientaddr:%s\n", inet_ntoa(clientAddr.sin_addr));
	char recvBuf[256] = {};
	while (1)
	{
		DataHeader dataHeader = {};
		int recvLen = recv(clientSock, (char*)&dataHeader, sizeof(DataHeader), 0);
		if (recvLen <= 0)
		{
			printf("client exit\n");
			break;
		}
		printf("CMD:%d length:%d\n", dataHeader.cmd, dataHeader.dataLength);
		switch (dataHeader.cmd)
		{
		case CMD_LOGIN:
		{
			//忽略用户名 密码验证;
			Login login = {};
			recv(clientSock, (char*)&login+ sizeof(DataHeader), sizeof(login)- sizeof(DataHeader), 0);
			printf("CMD_LOGIN length:%d username:%s password:%s\n", login.dataLength,login.name,login.password);
			LoginResult ret;
			send(clientSock, (char*)&ret, sizeof(LoginResult), 0);
			break;
		}
		case CMD_LOGOUT:
		{
			Logout logout = {};
			recv(clientSock, (char*)&logout + sizeof(DataHeader), sizeof(Logout)- sizeof(DataHeader), 0);
			//忽略用户名验证;
			printf("CMD_LOGIN length:%d username:%s\n", logout.dataLength, logout.name);
			LoginResult ret;
			send(clientSock, (char*)&ret, sizeof(LogoutResult), 0);
			break;
		}
		default:
			dataHeader.cmd = CMD_ERROR;
			dataHeader.dataLength = 0;
			send(clientSock, (char*)&dataHeader, sizeof(DataHeader), 0);
			break;

		}

	}
	closesocket(_sock);
	printf("close socket\n");
	WSAStartup(ver, &data);
	WSACleanup();
	getchar();
	return 0;
}