#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <WinSock2.h>
//#pragma comment(lib,"ws2_32.lib")  
struct DataPackage
{
	char name[32];
	int age;
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
	//recive
	while (1)
	{
		char cmdBuf[128] = {};
		scanf("%s", cmdBuf);
		if (0 == strcmp(cmdBuf, "exit"))
		{
			printf("exit");
			break;
		}
		else
		{
			send(serverSock, cmdBuf, strlen(cmdBuf) + 1, 0);
		}
		char recvBuf[256] = {};
		int revLen = 0;
		revLen = recv(serverSock, recvBuf, 256, 0);
		if (revLen > 0)
		{
			DataPackage *dp = (DataPackage *)recvBuf;
			printf("recive : name:%s  age:%d\n", dp->name, dp->age);
		}
	}
	closesocket(serverSock);
	WSACleanup();
	printf("close socket\n");
	getchar();
	return 0;

}
