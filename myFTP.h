#ifndef FTP_H_
#define FTP_H_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int MAX_STRING_LENGTH = 256;
const int MAX_LIST_LENGTH = 65536;
const int MAX_BUFFER_LENGTH = 2048;

static int SLEEP = 10;
static char *DIR_FOR_SAVING = "d:/FOR FTP/";

class FTP
{
private:
	SOCKET FTPsocket;
	SOCKET DATA;
	char addressOfServer[MAX_STRING_LENGTH];
public:
	void getAddress(char *address)
	{
		for (int i = 0; address[i]; i++)
			addressOfServer[i] = address[i];
	}
	void sending(char *str)
	{
		send(FTPsocket, str, strlen(str), 0);
		cout << "\nClient: " << str;
	}
	void readServ()
	{
		char buff[MAX_BUFFER_LENGTH];
		for (int i = 0; i<MAX_BUFFER_LENGTH; i++)
			buff[i] = '\0';
		recv(FTPsocket, buff, MAX_BUFFER_LENGTH, 0);
		cout << "Server: " << buff;
		Sleep(SLEEP);
	}

	int initData();
	int initSock();
	int authorization();
	char *getdata();
	void getList(char *list);
	int getFile(char *fileName);
	int deleteFile(char *fileForDelete);
	void goToDirectory(char *nameOfDirectory);
	void returnToParentsDir();
	void abort();
	void close();
	int putFile(char *fileName);

};

#endif
