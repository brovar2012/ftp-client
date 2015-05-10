#ifndef FTP_H_
#define FTP_H_H

#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

static int SLEEP = 500;
static char *DIR_FOR_SAVING = "d:/";

class FTP 
{
private:
	SOCKET FTPsocket;
	SOCKET DATA;
	char addressOfServer[100];
public:

	void getAddress(char *address)
	{
		for(int i =0;address[i];i++)
			addressOfServer[i] = address[i];
	}
	void sending(char *str)
	{
		send(FTPsocket, str, strlen(str), 0);
		cout << "\nClient: " << str;
	}
	void readServ()
	{
		char buff[1024];
		for (int i = 0; i<1024; i++)
			buff[i] = '\0';
		recv(FTPsocket, buff, 1024, 0);
		cout << "Server: " << buff;
		Sleep(SLEEP);
	}

	int initData();
	int initSock ();
	int authorization();
	char *getdata();
	void getList();
	void getFile(char *fileName);
	int deleteFile(char *fileForDelete);
	void goToDirectory(char *nameOfDirectory);
	void returnToParentsDir();
	void abort();
	void close ();
	void putFile(char *fileName);
};

#endif
