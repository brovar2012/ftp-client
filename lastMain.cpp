#include "FTP.h"

int main()
{
	FTP *client;
	client = new FTP();

	setlocale(LC_ALL, "russian");

	char addressOfServer[100];
	int error;
	cout << "Введите адрес FTP-сервера" <<endl;//195.178.216.8   86.57.151.3    127.0.0.1
	cin >> addressOfServer;
	client->getAddress(addressOfServer);
	error = client->initSock();
	if (error == 0 )
		cout << "Соединение с сервером установлено."<< endl;
	else
		cout << "Ошибка соединения с сервером.Проверьте введенные данные." <<endl;
	error = client->authorization();
	if (error == -1)
	{
		cout << "Ошибка авторизации.Повторите попытку либо зайдите анонимно." <<endl;
		return 0;
	}
	client->getList();
	client->goToDirectory("debian-backports");
	error = client->deleteFile("ls-lR.gz");
	if (error == -1)
		cout << "Ошибка удаления.Возможно вы не обладаете правами доступа." <<endl;
	client->goToDirectory();
	client->getFile();
	client->close();
	return 0;
}
