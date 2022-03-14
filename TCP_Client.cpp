
/*
	Socket�ͻ��˴���
	������127.0.0.1ͨ�ţ��˿�8888
	�������Ӻ󣬷��͸�����������Ҫ������ļ�·��
	���ļ�·�����ڣ����շ��������͵��ļ��������������ر����ӡ�
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
	��ʼ��WSA��ʹ�ó�����Ե���windows socket
	*/
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	/*
	�����׽��֣�client_socket
	������Ƿ񴴽��ɹ�
	*/
	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client_socket == INVALID_SOCKET) {
		//���������socket��Ч�����������
		perror("socket error !");
		return 0;
	}

	/*
	������ַ�ṹ��server_addr�������ö˿ں�IP
	*/
	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	//Ҫ���ӵķ������˿ں� 8888
	server_addr.sin_port = htons(8888);
	//ָ���������ĵ�ַ127.0.0.1
	InetPton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);

	//���ַserver_addr��������
	if (connect(client_socket, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)))
	{
		perror("connect error !\n");
		return 0;
	}

	char DATE_NEED[DATE_NAME_MAX_SIZE + 1];
	memset(DATE_NEED, 0, DATE_NAME_MAX_SIZE + 1);
	printf("������Ҫ��ȡ���������·�:\n");
	scanf_s("%s", DATE_NEED, DATE_NAME_MAX_SIZE);

	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	strncpy_s(buffer, DATE_NEED, strlen(DATE_NEED) > BUFFER_SIZE ? BUFFER_SIZE : strlen(DATE_NEED));

	// �����������buffer�е����� 
	if (send(client_socket, buffer, BUFFER_SIZE, 0) < 0)
	{
		perror("�����ļ���ʧ��:");
		exit(1);
	}

	// �ӷ������������ݵ�buffer��,��������Ļ 
	memset(buffer, 0, BUFFER_SIZE);
	int length = 0;
	while ((length = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0)
	{
		cout << buffer;
		memset(buffer, 0, BUFFER_SIZE);
	}


	// ���ճɹ��󣬹ر��ļ����ر�socket��WSA 
	closesocket(client_socket);
	WSACleanup();
	system("pause");
	return 0;

}