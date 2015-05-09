#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#ifndef _WINSOCKAPI_ 
#include <winsock2.h>
#endif
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

const int SLEEP = 500;
const char *DIR_FOR_SAVING = "d:/";

using namespace std;

SOCKET init_data(SOCKET FTPsocket,char *addressOfServer) 
{
	SOCKET ds;
	char buff[65536]= "";

	send(FTPsocket, "PASV\r\n", strlen("PASV\r\n"), 0);
	//cout << endl << "C: PASV";
	recv(FTPsocket, buff, 1024, 0);
	//cout << endl << "Server: " << buff; для получения контрольных цифр 

	int a, b;
	char *tmp_char;
	tmp_char = strtok(buff, "(");
	tmp_char = strtok(NULL, "(");
	tmp_char = strtok(tmp_char, ")");
	int c, d, e, f;
	sscanf(tmp_char, "%d,%d,%d,%d,%d,%d", &c, &d, &e, &f, &a, &b);
	int len;
	sockaddr_in address;

	int result;
	int port = a * 256 + b;

	ds = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(addressOfServer); 
	address.sin_port = htons(port);

	len = sizeof(address);
	result = connect(ds, (sockaddr *)&address, len);
	if (result == -1) {
		perror("Client error!");
		return -1;
	}

	return ds;
}

void sending(char *str, SOCKET FTPsocket)
{
	send(FTPsocket, str, strlen(str), 0);
	cout << "\nClient: " << str;
}

void readServ(SOCKET FTPsocket)
{
	char buff[1024];
	for (int i = 0; i<1024; i++)
		buff[i] = '\0';
	recv(FTPsocket, buff, 1024, 0);
	cout << "Server: " << buff;
	Sleep(SLEEP);
}

int init_sock (char *addressOfServer) 
{

	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
	{
		cout << "Ошибка инициализации сокета." <<endl;
		return -1;
	}

	SOCKET FTPSocket;
	sockaddr_in address;
	struct hostent *host;

	FTPSocket = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(addressOfServer);  
	address.sin_port = htons(21);

	if (address.sin_addr.s_addr == INADDR_NONE)
	{
		host = NULL;
		host = gethostbyname(addressOfServer);

		if (host == NULL)
		{
			cout << " Ошибка подключения к серверу." << endl;
			return -1;
		}
		memcpy(&address.sin_addr, host->h_addr_list[0], host->h_length);
	}

	int len;
	int result;
	len = sizeof(address);
	result = connect(FTPSocket, (sockaddr *)&address, len);
	if (result == -1)
	{
		cout << "Ошибка соединения." <<endl;
		return -1;
	}

	cout << "Соединение установлено.\n";
	readServ(FTPSocket);
	return FTPSocket;
}

char *getdata(SOCKET FTPSocket)
{
	char file[65536];
	for (int i = 0; i<65536; i++)
		file[i] = '\0';
	recv(FTPSocket, file, sizeof(file), 0);
	closesocket(FTPSocket);
	return file;
}

void authorization(SOCKET FTPsocket)
{
	char str[512];
	char choice;

	cout << "Хотите авторизоваться?" << endl;
	cout << "1.Нет" << endl;
	cout << "2.Да" << endl;
	cin >> choice;

	if (choice == '2')
	{
		char login[64];
		char pass[64];
		cout << "Введите логин:" << endl;
		cin >> login;
		sprintf(str, "USER %s\r\n", login);
		sending(str, FTPsocket);
		readServ(FTPsocket);

		cout << "Введите пароль:" << endl;
		cin >> pass;
		sprintf(str, "PASS %s\r\n", pass);
		sending(str, FTPsocket);
		readServ(FTPsocket);
	}
	else
	{
		char login[64] = "anonymous";
		sprintf(str, "USER %s\r\n", login);
		sending(str, FTPsocket);
		readServ(FTPsocket);

		char pass[64] = "anonymous@gmail.com";
		sprintf(str, "PASS %s\r\n", pass);
		sending(str, FTPsocket);
		readServ(FTPsocket);
	}
}

void getList(SOCKET FTPsocket, SOCKET DATA, char *adressOfServer)
{
	DATA = init_data(FTPsocket,adressOfServer);
	sending("LIST\r\n", FTPsocket);
	Sleep(SLEEP);
	readServ(FTPsocket);

	char file[65536];
	strcpy(file, getdata(DATA));
	cout << file << endl;
	Sleep(SLEEP);
}

void getFile(SOCKET FTPsocket, SOCKET DATA,char *adressOfServer)
{
	// !!! sending("NLST\r\n", FTPsocket);

	char inquiry[1000];
	char fileName[400];
	char outputFile[1010];

	cout << "Введите имя файла (целиком) для скачивания" << endl;
	cin >> fileName;

	DATA = init_data(FTPsocket,adressOfServer);

	sprintf(inquiry, "RETR %s\r\n", fileName);

	sending(inquiry, FTPsocket);
	readServ(FTPsocket);
	Sleep(SLEEP);

	sprintf(outputFile, "%s%s",DIR_FOR_SAVING ,fileName);
	cout <<outputFile <<endl;

	FILE *file;
	file = fopen(outputFile, "wb");  
	int read = 0;
	int fullSize;

	do 
	{
		char buff[2048];   
		int readed = recv(DATA,buff,sizeof(buff),0);   
		fwrite(buff,1,readed,file);  
		fullSize = read;
		read += readed;  
    } while (fullSize != read);

	fclose(file);
}

void deleteFile(SOCKET FTPsocket)
{
	char inquiry[1000];
	char fileForDelete[1000];

	cout << "Выберите файл для удаления" << endl;
	cin >> fileForDelete;

	sprintf(inquiry, "DELE %s\r\n", fileForDelete);
	sending(inquiry, FTPsocket);
	Sleep(SLEEP);
	readServ(FTPsocket);
}

void goToDirectory(SOCKET FTPsocket, SOCKET DATA, char *nameOfDirectory,char *adressOfServer)
{
	char inquiry[1000];

	sprintf(inquiry, "CWD %s\r\n", nameOfDirectory);
	sending(inquiry, FTPsocket);
	Sleep(SLEEP);
	readServ(FTPsocket);
	getList(FTPsocket, DATA,adressOfServer);
}

void returnToParentsDir(SOCKET FTPsocket, SOCKET DATA,char *addressOfServer)
{
	sending("XCUP\r\n", FTPsocket);
	Sleep(SLEEP);
	readServ(FTPsocket);
	getList(FTPsocket, DATA,addressOfServer);
}

void sendFile(SOCKET FTPsocket, SOCKET DATA,char *addressOfServer)
{
		/*ifstream  file;
		file.open("d:/2.mp3",ios::binary);
		char singleBuffer[4096];
		while (!file.eof())
		{
			file >> singleBuffer[0];
			send(FTPsocket, singleBuffer, sizeof(singleBuffer), 0);
		}
		file.close();
		return 1;*/
	DATA = init_data(FTPsocket,addressOfServer);
		sending("STOR 33.mp3\r\n", FTPsocket);
		readServ(FTPsocket);
		system("pause");

		FILE *file;
		file = fopen("d:/33.mp3", "rb");  
		int read = 0;
		int fullSize=0;
		do 
		{
			char buff[2048];   
			int readed = send(DATA,buff,sizeof(buff),0);   
			fread(buff,1,readed,file);  
			fullSize++;
			read += readed;  
		} while (fullSize<1711);

		fclose(file);
} 

int main()
{
	setlocale(LC_ALL, "russian");
	char addressOfServer[100];
	SOCKET DATA;
	SOCKET FTPsocket;


	cout << "Введите адрес FTP-сервера" <<endl;//195.178.216.8   86.57.151.3    127.0.0.1
	cin >> addressOfServer;

	FTPsocket = init_sock(addressOfServer);

	authorization(FTPsocket);

	DATA = init_data(FTPsocket,addressOfServer);

	getList(FTPsocket, DATA,addressOfServer);
	system("pause");


	sendFile (FTPsocket, DATA,addressOfServer);
	system("pause");

	//getFile(FTPsocket, DATA,addressOfServer);
    //system("pause");

	//deleteFile(FTPsocket);
	//system("pause");

	//goToDirectory(FTPsocket, DATA, "123456");
	//system("pause");

	//goToDirectory(FTPsocket, DATA, "222");
	//system("pause");

	//returnToParentsDir(FTPsocket, DATA);
	//system("pause");

	//strcpy(pass, "/");
	/*sending("LIST\r\n", S);
	Sleep(444);
	readServ(S);//Ответ
	system("pause");

	char file[65536];
	strcpy(file, getdata(DATA));//Список
	cout << file << endl;//Ответ -список

	//sending("TYPE I\r\n", S);
	//readServ(S);
	//system("pause");
	//sending("CWD ! ! Новинки КИНО\r\n", S);
	//readServ(S);
	//sending("CWD Шеф (комедия) [2012]\r\n", S);
	//readServ(S);
	Sleep(444);

	//sending("STOR Mumiy_trol-03_Utekay.mp3\r\n", S);
	//readServ(S);
	//system("pause");

	DATA = init_data(S);
	sending("LIST\r\n", S);
	Sleep(444);
	readServ(S);
	strcpy(file, getdata(DATA));//Список
	cout << file << endl;//Ответ -список
	system("pause");
	//-------------------------------------------------Файл!!!*/

	/*int k;
	DATA = init_data(S);
	sending("STOR 2.mp3\r\n", S);
	readServ(S);
	Sleep(444);
	k = sent(DATA);*/
	/*sending("DELE 5.mp3\r\n", S);
	Sleep(444);
	readServ(S);

	sending("CWD 123456\r\n", S);
	Sleep(444);
	readServ(S);

	DATA = init_data(S);
	sending("LIST\r\n", S);
	Sleep(444);
	readServ(S);
	strcpy(file, getdata(DATA));//Список
	cout << file << endl;//Ответ -список*/

	sending("QUIT\r\n", FTPsocket);
	readServ(FTPsocket);
	system("pause");

	closesocket(FTPsocket);  ///закрытие соединения
	WSACleanup();
	return 0;
}
