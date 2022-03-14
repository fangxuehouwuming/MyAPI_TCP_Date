/*
	Socket服务器端代码
	服务器监听端口8888
	建立连接后，接收Client传输的文件路径
	若文件路径存在，则发送该文件给Client，发送完毕则关闭连接。
*/
#include <tchar.h>
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>   
#include <Ws2tcpip.h>
#include<iomanip>
#include"Date.h"

#pragma comment(lib,"ws2_32.lib")

#define DATE_SIZE 7
#define BUFFER_SIZE 2048
#define FILE_NAME_MAX_SIZE 512 
#define DATE_NAME "Date.dat"


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
		创建监听用套接字，server_socket
		类型是TCP
		并检测是否创建成功
	*/
	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_socket == INVALID_SOCKET) {
		//如果创建的socket无效，则结束程序
		perror("socket error !\n");
		return 0;
	}

	/*
	   创建地址，server_addr，并设置端口和IP
	*/
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	//端口号 8888
	server_addr.sin_port = htons(8888);
	//INADDR_ANY表示本机任意IP地址
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;


	//将socket与地址server_addr绑定
	if (bind(server_socket, (LPSOCKADDR)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
	{
		perror("bind error !\n");
		return 0;
	}

	//server_socket开始监听
	if (listen(server_socket, 20) == SOCKET_ERROR)
	{
		perror("listen error !\n");
		return 0;
	}

	while (1)
	{
		printf("等待连接...\n");

		// 定义客户端的socket和socket地址结构
		SOCKET client_socket;
		sockaddr_in client_addr;
		int client_addr_length = sizeof(client_addr);

		// 接受连接请求，返回一个新的socket(描述符)，这个新socket用于同连接的客户端通信 
		// accept函数会把连接到的客户端信息写到client_addr中 
		client_socket = accept(server_socket, (SOCKADDR*)&client_addr, &client_addr_length);
		if (client_socket == INVALID_SOCKET)
		{
			perror("Socket连接建立失败:\n");
			continue;
		}

		char IP_BUFFER[256];
		memset(IP_BUFFER, 0, 256);
		InetNtop(AF_INET, &client_addr.sin_addr, IP_BUFFER, 256);
		printf("Socket连接建立，客户端IP为：%s，端口为：%d\n", IP_BUFFER, ntohs(client_addr.sin_port));


		//接受客户端请求的年月份
		char date[DATE_SIZE];
		memset(date, 0, DATE_SIZE);
		if (recv(client_socket, date, DATE_SIZE, 0) < 0)
		{
			perror("接收年月份失败:\n");
			break;
		}

		//从客户输入提取出年月份
		int year, month, i;
		string str_year, str_month;
		for (i = 0; i < 4; i++)
			str_year += date[i];
		for (i = 5; i < 7; i++)
			str_month += date[i];
		year = atoi(str_year.c_str());
		month = atoi(str_month.c_str());

		//生成日历
		Date(year, month);
		
		char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		FILE* fp;
		errno_t F_ERR = fopen_s(&fp, DATE_NAME, "rb");
		if (F_ERR != 0)
		{
			printf("文件打开失败:%s\n", DATE_NAME);
		}
		else
		{
			printf("开始传输：%s\n", DATE_NAME);
			memset(buffer, 0, BUFFER_SIZE);
			int length = 0;
			// 每读取一段数据，便将其发送给客户端，循环直到文件读完为止 
			while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
			{
				if (send(client_socket, buffer, length, 0) < 0)
				{
					printf("文件发送失败:%s/n", DATE_NAME);
					break;
				}
				memset(buffer, 0, BUFFER_SIZE);
			}

			// 关闭文件 
			fclose(fp);
			printf("传输完成:%s!\n", DATE_NAME);
		}
		
		closesocket(client_socket);
	}

	// 关闭监听用的socket 
	closesocket(server_socket);
	WSACleanup();
	return 0;
}