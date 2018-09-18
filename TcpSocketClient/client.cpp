#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include <WinSock2.h>
#include <stdio.h>

struct DataPackage
{
	char name[32];
	int age;
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
	//����
	if (SOCKET_ERROR == listen(_sock, 5))
	{
		printf("listen socket error\n");
	}
	sockaddr_in clientAddr = {};
	int clientAddrLen = sizeof(sockaddr_in);
	SOCKET clientSock = INVALID_SOCKET;
	//��������
	clientSock = accept(_sock, (sockaddr*)&clientAddr, &clientAddrLen);
	if (INVALID_SOCKET == clientSock)
	{
		printf("accept error\n");
	}
	printf("clientaddr:%s\n", inet_ntoa(clientAddr.sin_addr));
	char recvBuf[256] = {};
	while (1)
	{
		int recvLen = recv(clientSock, recvBuf, 256, 0);
		if (recvLen <= 0)
		{
			printf("client exit\n");
			break;
		}
		if (0 == strcmp(recvBuf, "getInfo"))
		{
			DataPackage dp = { "pct",18 };
			send(clientSock, (const char*)&dp, sizeof(dp), 0);
		}
		else
		{
			char msgBuf[] = { "???" };
			send(clientSock, msgBuf, strlen(msgBuf) + 1, 0);
		}

	}
	closesocket(_sock);
	printf("close socket\n");
	WSAStartup(ver, &data);
	WSACleanup();
	getchar();
	return 0;
}