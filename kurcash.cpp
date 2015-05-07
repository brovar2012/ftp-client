#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#ifndef _WINSOCKAPI_ 
#include <winsock2.h>
#endif

#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>

#define SLEEP 500

#pragma comment(lib, "ws2_32.lib")

char buff[65536];

using namespace std;

struct hostent *host, *host2;

char addr[] = "127.0.0.1";

std::fstream stream;

unsigned int file_size;

char file_name[256];

SOCKET init_data(SOCKET s) {
	SOCKET ds;
	send(s, "PASV\r\n", strlen("PASV\r\n"), 0);
	cout << endl << "C: PASV";
	recv(s, buff, 1024, 0);
	cout << endl << "S: " << buff; ////выводим на экран полученную от сервера строку

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
	address.sin_addr.s_addr = inet_addr(addr);   ///addr - у меня глобальная переменная с адресом сервера
	address.sin_port = htons(port);

	len = sizeof(address);
	result = connect(ds, (sockaddr *)&address, len);
	if (result == -1) {
		perror("oops: client");
		return -1;
	}

	return ds;
}
void sending(char *a, SOCKET FTPsocket)
{
	send(FTPsocket, a, strlen(a), 0);
	cout << "\nClient: " << a;
}

void readServ(SOCKET FTPsocket)
{
	for (int i = 0; i<1024; i++)
		buff[i] = '\0';
	recv(FTPsocket, buff, 1024, 0);
	cout << "Server: " << buff;
	Sleep(SLEEP);
}

int init_sock() {

	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
	{
		cout << "786";
		// getch();
		// exit(0);
	}

	SOCKET S;
	sockaddr_in address;
	S = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(addr);// was 77.246.101.195    87.224.183.162    87.224.147.100    134.169.34.11 lin   
	address.sin_port = htons(21);
	if (address.sin_addr.s_addr == INADDR_NONE)
	{
		host = NULL;
		host = gethostbyname(addr);

		if (host == NULL)
		{
			//            MessageBox::Show("SERVER CONNECTION ERROR");
			exit(0);
		}
		memcpy(&address.sin_addr, host->h_addr_list[0], host->h_length);
	}

	int len;
	int result;
	len = sizeof(address);
	result = connect(S, (sockaddr *)&address, len);
	if (result == -1)
	{
		cout << "87698";
		//        getch();
		exit(0);
	}
	cout << "Connection Estabilished!\n";
	readServ(S);
	return S;

}

char *getdata(SOCKET ds)
{
	char file[65536];
	for (int i = 0; i<65536; i++)
		file[i] = '\0';
	recv(ds, file, sizeof(file), 0);
	closesocket(ds);
	return file;
}

/*int sent(SOCKET FTPsocket2)
{
/*ifstream  file;
file.open("d:/2.mp3",ios::binary);
char singleBuffer[4096];
while (!file.eof())
{
file >> singleBuffer[0];
send(FTPsocket2, singleBuffer, sizeof(singleBuffer), 0);
}

file.close();*/
//return 1;
/*char *buffer;
ifstream  file;
file.open("d:/2.mp3", ios::binary);
long n = sizeof(file);
buffer = (char*)malloc(n * sizeof(char));

//file.read(buffer, n);
while (send(FTPsocket2, buffer, sizeof(buffer), 0))
{
file.read(buffer, n);

}
file.close();
return 1;*/

/*int getfile(SOCKET ds)
{
char *tmp_char;
tmp_char = strtok(buff, "\r\n");
char t[256];
memset(t, '\0', sizeof(t));
strcpy(t, tmp_char);
int i = 44, u = 0;
char n[256];
memset(n, '\0', sizeof(n));
while (t[i] != '\0'){
n[u] = t[i];
i++; u++;
}
u -= 8; i = 0;
char e[256];
memset(e, '\0', sizeof(e));
while (n[u] != '('){
e[i] = n[u];
i++; u--;
}
reverse(e);
file_size = atoi(e);
int n_len = u - 1;
for (int i = 0; i<n_len; i++)
file_name[i] = n[i];

stream.open(file_name, ios::out | ios::binary | ios::trunc);
if (!stream){
cout << "Error open\n";
//        getch();
return 0;
}

char file[4096];
int num = 0;
unsigned int k = 0;
int размер_файла = 0;
while (num = recv(ds, file, 4096, 0)){
if (num<0){
cout << "Error";
printf(" %d\n", WSAGetLastError());
break;
}

// записывается сразу в файл
stream.write(file, num);
размер_файла += num;
cout << "Recieved " << num << " bytes\n";
Sleep(5);
}

std::cout << "\nRecieved байт" << размер_файла;
closesocket(ds);
return 1;
}*/

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

void getList(SOCKET FTPsocket, SOCKET DATA)
{
	DATA = init_data(FTPsocket);
	sending("LIST\r\n", FTPsocket);
	Sleep(444);
	readServ(FTPsocket);//Ответ

	char file[65536];
	strcpy(file, getdata(DATA));//Список
	cout << file << endl;//Ответ -список
	Sleep(444);
}

int getfile1(SOCKET FTPsocket2,char *s)
{
	fstream file;
	file.open(s, ios::out | ios::binary);

	char buffer[256];
	while (recv(FTPsocket2, buffer, sizeof(buffer), 0))
		file << buffer;

	file.close();
	return 1;
}

//////////////////////
/*int getfile1(SOCKET FTPsocket2)
{
	fstream file;
	file.open("d:/1.mp3", ios::out | ios::binary);

	char buffer[256];

	while (recv(FTPsocket2, buffer, sizeof(buffer), 0))
		file << buffer;

	file.close();
	return 1;
}*/

void getFile(SOCKET FTPsocket, SOCKET DATA)
{
	// !!! sending("NLST\r\n", FTPsocket);

	char inquiry[1000];
	char fileName[400];
	char outputFile[1010];
	fstream file;

	cout << "Введите имя файла (целиком) для скачивания" << endl;
	cin >> fileName;

	DATA = init_data(FTPsocket);

	sprintf(inquiry, "RETR %s\r\n", fileName);

	sending(inquiry, FTPsocket);
	readServ(FTPsocket);
	Sleep(444);

	sprintf(outputFile, "d:/%s", fileName);

	file.open(outputFile, ios::out | ios::binary);

	char buffer[64];
	while (recv(DATA, buffer, sizeof(buffer), 0))
		file << buffer;

	file.close();
}

void deleteFile(SOCKET FTPsocket)
{
	char inquiry[1000];
	char fileForDelete[1000];

	cout << "Выберите файл для удаления" << endl;
	cin >> fileForDelete;

	sprintf(inquiry, "DELE %s\r\n", fileForDelete);
	sending(inquiry, FTPsocket);
	Sleep(444);
	readServ(FTPsocket);
}

void goToDirectory(SOCKET FTPsocket, SOCKET DATA, char *nameOfDirectory)
{
	char inquiry[1000];

	sprintf(inquiry, "CWD %s\r\n", nameOfDirectory);
	sending(inquiry, FTPsocket);
	Sleep(444);
	readServ(FTPsocket);
	getList(FTPsocket, DATA);
}

void returnToParentsDir(SOCKET FTPsocket, SOCKET DATA)
{
	sending("XCUP\r\n", FTPsocket);
	Sleep(444);
	readServ(FTPsocket);
	getList(FTPsocket, DATA);
}

int main()
{
	setlocale(LC_ALL, "russian");
	char addressOfServer[100];
	SOCKET DATA;
	SOCKET FTPsocket;

	cout << "Введите адрес FTP-сервера";
	cin >> addressOfServer;

	FTPsocket = init_sock();
	char str[512];

	authorization(FTPsocket);

	DATA = init_data(FTPsocket);

	getList(FTPsocket, DATA);
	system("pause");

	//getFile(FTPsocket, DATA);
	//system("pause");

	//deleteFile(FTPsocket);
	//system("pause");

	goToDirectory(FTPsocket, DATA, "123456");
	system("pause");

	goToDirectory(FTPsocket, DATA, "222");
	system("pause");

	returnToParentsDir(FTPsocket, DATA);
	system("pause");

	closesocket(FTPsocket);
	WSACleanup();

	stream.close();
	sending("QUIT\r\n", FTPsocket);
	Sleep(444);
	readServ(FTPsocket);
	system("pause");
	//    getch();
	return 0;
}
