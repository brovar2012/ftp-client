#include "ftp.h" 

int FTP::authorization()
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
		sending(str);
		readServ();

		cout << "Введите пароль:" << endl;
		cin >> pass;
		sprintf(str, "PASS %s\r\n", pass);
		sending(str);

		char buff[1024];
		for (int i = 0; i<1024; i++)
			buff[i] = '\0';
		recv(FTPsocket, buff, 1024, 0);
		if (buff[0] == '5')
			return -1;
		else
		{
			cout << "Server: " << buff;
			Sleep(SLEEP);
			return 0;
		}
	}
	else
	{
		char login[64] = "anonymous";
		sprintf(str, "USER %s\r\n", login);
		sending(str);
		readServ();

		char pass[64] = "anonymous@gmail.com";
		sprintf(str, "PASS %s\r\n", pass);
		sending(str);
		readServ();
		return 0;
	}
}

int FTP::initData() 
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
		if (result == -1) 
			return -1;

		DATA = ds;
		return 0;
	}

int FTP::initSock () 
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
		return -1;
	}

	FTPsocket = FTPSocket;
	readServ();
	return 0;
}

char* FTP::getdata()
{
	char file[65536];
	for (int i = 0; i<65536; i++)
		file[i] = '\0';
	recv(DATA, file, sizeof(file), 0);
	closesocket(DATA);
	return file;
}

void FTP::getList()
{
	initData();
	sending("LIST\r\n");
	Sleep(SLEEP);
	readServ();

	char file[65536];
	strcpy(file, getdata());
	cout << file << endl;
	Sleep(SLEEP);
}

void FTP::getFile(char *fileName)
{
	// !!! sending("NLST\r\n", FTPsocket);

	char inquiry[500];
	char outputFile[510];

	initData();

	sprintf(inquiry, "RETR %s\r\n", fileName);

	sending(inquiry);
	readServ();
	Sleep(SLEEP);

	sprintf(outputFile, "%s%s",DIR_FOR_SAVING ,fileName);

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

	readServ();
	fclose(file);
}

int FTP::deleteFile(char *fileForDelete)
{
	char inquiry[1000];

	sprintf(inquiry, "DELE %s\r\n", fileForDelete);
	sending(inquiry);
	Sleep(SLEEP);
	char buff[1024];
    for (int i = 0; i<1024; i++)
		buff[i] = '\0';
	recv(FTPsocket, buff, 1024, 0);
	if (buff[0]=='5')
		return -1;
	else
	{
		cout << "Server: " << buff;
		Sleep(SLEEP);
		return 0;
	}
}

void FTP::goToDirectory(char *nameOfDirectory)
{
	char inquiry[1000];

	sprintf(inquiry, "CWD %s\r\n", nameOfDirectory);
	sending(inquiry);
	Sleep(SLEEP);
	readServ();
	getList();
}

void FTP::returnToParentsDir()
{
	sending("XCUP\r\n");
	Sleep(SLEEP);
	readServ();
	getList();
}

void FTP::close ()
{
	sending("QUIT\r\n");
	readServ();
	system("pause");

	closesocket(FTPsocket); 
	WSACleanup();
}

void FTP:: abort ()
{
	sending("ABOR\r\n");
	Sleep(SLEEP);
	readServ();
	cout << "Передача данных прервана."<< endl;
}

void FTP::putFile(char *fileName)
{
	initData();

	char inquiry[1000];
	sprintf(inquiry, "STOR %s\r\n", fileName);
	sending(inquiry);
	Sleep(SLEEP);
	readServ();

	char fileN [1000];
	sprintf(fileN, "d:/%s\r\n", fileName);

	FILE *file;
	file = fopen(fileN, "wb");  
	int read = 0;
	int fullSize;

	while (file)
	{
		char singleBuffer[2048];
		fread(singleBuffer,1,2048,file); 
		send(FTPsocket, singleBuffer, sizeof(singleBuffer), 0);
	}
	readServ();
	fclose(file);
} 
