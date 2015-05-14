#include "ftp.h" 

int FTP::authorization()
{
	char str[MAX_STRING_LENGTH];
	char choice;

	cout << "Хотите авторизоваться?" << endl;
	cout << "1.Нет" << endl;
	cout << "2.Да" << endl;
	cin >> choice;

	if (choice == '2')
	{
		char login[MAX_STRING_LENGTH];
		char pass[MAX_STRING_LENGTH];
		cout << "Введите логин:" << endl;
		cin >> login;
		sprintf(str, "USER %s\r\n", login);
		sending(str);
		readServ();

		cout << "Введите пароль:" << endl;
		cin >> pass;
		sprintf(str, "PASS %s\r\n", pass);
		sending(str);

		char buff[MAX_BUFFER_LENGTH];
		for (int i = 0; i<MAX_BUFFER_LENGTH; i++)
			buff[i] = '\0';
		recv(FTPsocket, buff, MAX_BUFFER_LENGTH, 0);
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
		char login[MAX_STRING_LENGTH] = "anonymous";
		sprintf(str, "USER %s\r\n", login);
		sending(str);
		readServ();

		char pass[MAX_STRING_LENGTH] = "anonymous@gmail.com";
		sprintf(str, "PASS %s\r\n", pass);
		sending(str);
		readServ();
		return 0;
	}
}

int FTP::initData()
{
	SOCKET ds;
	char buff[MAX_BUFFER_LENGTH] = "";

	send(FTPsocket, "PASV\r\n", strlen("PASV\r\n"), 0);
	//cout << endl << "Client: PASV";
	recv(FTPsocket, buff, MAX_BUFFER_LENGTH, 0);
	//cout << endl << "Server: " << buff; //для получения контрольных цифр 

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

int FTP::initSock()
{

	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
	{
		cout << "Ошибка инициализации сокета." << endl;
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
	char file[MAX_LIST_LENGTH];
	for (int i = 0; i<MAX_LIST_LENGTH; i++)
		file[i] = '\0';
	recv(DATA, file, sizeof(file), 0);
	closesocket(DATA);
	return file;
}

void FTP::getList(char *list)
{
	initData();
	sending("LIST\r\n");
	Sleep(SLEEP);
	readServ();
	strcpy(list, getdata());
	Sleep(SLEEP);
}

int FTP::getFile(char *fileName)
{
	char inquiry[MAX_STRING_LENGTH];
	char outputFile[MAX_STRING_LENGTH];

	initData();

	sprintf(inquiry, "RETR %s\r\n", fileName);

	sending(inquiry);
	char buff[MAX_BUFFER_LENGTH];
	for (int i = 0; i<MAX_BUFFER_LENGTH; i++)
		buff[i] = '\0';
	recv(FTPsocket, buff, MAX_BUFFER_LENGTH, 0);

	if (buff[0] == '5')
	{
		cout << buff << endl;
		return -1;
	}

	else
	{
		sprintf(outputFile, "%s%s", DIR_FOR_SAVING, fileName);
		FILE *file;
		file = fopen(outputFile, "wb");
		int read = 0;
		int fullSize;

		do
		{
			char buff[MAX_BUFFER_LENGTH];
			int readed = recv(DATA, buff, sizeof(buff), 0);
			fwrite(buff, 1, readed, file);
			fullSize = read;
			read += readed;
		} while (fullSize != read);

		readServ();
		cout << "Сохранено в " << DIR_FOR_SAVING << endl;
		fclose(file);
		return 0;
	}
}

int FTP::deleteFile(char *fileForDelete)
{
	char inquiry[MAX_STRING_LENGTH];

	sprintf(inquiry, "DELE %s\r\n", fileForDelete);
	sending(inquiry);
	Sleep(SLEEP);
	char buff[MAX_BUFFER_LENGTH];
	for (int i = 0; i<MAX_BUFFER_LENGTH; i++)
		buff[i] = '\0';
	recv(FTPsocket, buff, MAX_BUFFER_LENGTH, 0);
	if (buff[0] == '5')
	{
		cout << buff << endl;
		return -1;
	}
	else
	{
		cout << "Server: " << buff;
		Sleep(SLEEP);
		return 0;
	}
}

void FTP::goToDirectory(char *nameOfDirectory)
{
	char inquiry[MAX_STRING_LENGTH];

	sprintf(inquiry, "CWD %s\r\n", nameOfDirectory);
	sending(inquiry);
	Sleep(SLEEP);
	readServ();
}

void FTP::returnToParentsDir()
{
	sending("XCUP\r\n");
	Sleep(SLEEP);
	readServ();
}

void FTP::close()
{
	sending("QUIT\r\n");
	readServ();
	system("pause");

	strcpy(addressOfServer, "");
	closesocket(FTPsocket);
	closesocket(DATA);
	WSACleanup();
}

void FTP::abort()
{
	sending("ABOR\r\n");
	Sleep(SLEEP);
	readServ();
	cout << "Передача данных прервана." << endl;
}

int FTP::putFile(char *fileName)
{
	initData();

	char inquiry[1000];

	sprintf(inquiry, "STOR %s\r\n", fileName);
	sending(inquiry);
	Sleep(SLEEP);
	char buff[MAX_BUFFER_LENGTH];
	for (int i = 0; i<MAX_BUFFER_LENGTH; i++)
		buff[i] = '\0';
	recv(FTPsocket, buff, MAX_BUFFER_LENGTH, 0);
	if (buff[0] == '5')
	{
		cout << buff << endl;
		return -1;
	}

	char fileN[1000];
	sprintf(fileN, "%s%s",DIR_FOR_SAVING, fileName);

	FILE *file;
	file = fopen(fileN, "rb");
	if (file == 0)
	{
		cout << "Такого файла не существует в " << DIR_FOR_SAVING  << endl;
		return -1;
	}

	fseek(file, 0, SEEK_END);
	long size = ftell(file);

	int read = 0;
	int fullSize=0;
	int reading;

	rewind(file);

	if (size > MAX_BUFFER_LENGTH)
	{
		do
		{
			char buff[MAX_BUFFER_LENGTH];
			fread(buff, sizeof(buff), 1, file);
			reading = send(DATA, buff, sizeof(buff), 0);
			fullSize += reading;
		} while (fullSize < size - MAX_BUFFER_LENGTH);
	}

	do
	{
		char buff[1];
		fread(buff, sizeof(buff), 1, file);
		reading = send(DATA, buff, sizeof(buff), 0);
		fullSize += reading;
	} while (fullSize!=size);

	closesocket(DATA);
	fclose(file);
	readServ();
	return 0;
}
