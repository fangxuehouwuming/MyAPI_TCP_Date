
/*
	Socket客户端代码
	服务器127.0.0.1通信，端口8888
	建立连接后，发送给服务器，需要传输的文件路径
	若文件路径存在，接收服务器发送的文件流，发送完毕则关闭连接。
*/

#include <tchar.h>
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>  
#include <string.h>  
#include <Ws2tcpip.h>
#include<iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")

#define BUFFER_SIZE 2048
#define DATE_NAME_MAX_SIZE 512 

int main(int argc, char* argv[])
{
	/*
	初始化WSA，使得程序可以调用windows socket
	*/
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	/*
	创建套接字，client_socket
	并检测是否创建成功
	*/
	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client_socket == INVALID_SOCKET) {
		//如果创建的socket无效，则结束程序
		perror("socket error !");
		return 0;
	}

	/*
	创建地址结构，server_addr，并设置端口和IP
	*/
	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	//要连接的服务器端口号 8888
	server_addr.sin_port = htons(8888);
	//指定服务器的地址127.0.0.1
	InetPton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);

	//与地址server_addr建立连接
	if (connect(client_socket, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)))
	{
		perror("connect error !\n");
		return 0;
	}

	char DATE_NEED[DATE_NAME_MAX_SIZE + 1];
	memset(DATE_NEED, 0, DATE_NAME_MAX_SIZE + 1);
	printf("请输入要获取的日历年月份:\n");
	scanf_s("%s", DATE_NEED, DATE_NAME_MAX_SIZE);

	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	strncpy_s(buffer, DATE_NEED, strlen(DATE_NEED) > BUFFER_SIZE ? BUFFER_SIZE : strlen(DATE_NEED));

	// 向服务器发送buffer中的数据 
	if (send(client_socket, buffer, BUFFER_SIZE, 0) < 0)
	{
		perror("发送文件名失败:");
		exit(1);
	}

	// 从服务器接收数据到buffer中,并流向屏幕 
	memset(buffer, 0, BUFFER_SIZE);
	int length = 0;
	while ((length = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0)
	{
		cout << buffer;
		memset(buffer, 0, BUFFER_SIZE);
	}


	// 接收成功后，关闭文件，关闭socket、WSA 
	closesocket(client_socket);
	WSACleanup();
	system("pause");
	return 0;

}