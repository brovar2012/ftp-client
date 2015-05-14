#include "FTP.h"
#include <conio.h>

void showMenu()
{
	cout << "Введите номер операции:" << endl;
	cout << "1. Скачать файл." << endl
		<< "2. Загрузить файл. " << endl
		<< "3. Открыть дерикторию." << endl
		<< "4. Удалить файл." << endl
		<< "5. Обновить список." << endl
		<< "6. Вернуться в родительский каталог." << endl
		<< "7. Закрыть соединение." << endl;
}

void showList(char *nameOfDir, FTP *client, char *list)
{
	if (!(strcmp(nameOfDir, "NULL")))
	{
		cout << "Список файлов на сервере: " << endl;
		client->getList(list);
		cout << list;
	}
	else
	{
		cout << "Список файлов в директории " << nameOfDir << endl;
		client->getList(list);
		cout << list;
	}
}

int main()
{
	FTP *client;
	char list[65536];
	char nameOfFile[100];
	char nameOfDir[100] = "NULL";
	int error;
	char ch;

	setlocale(LC_ALL, "russian");

	do
	{
		cout << "1.Подключиться к серверу." << endl;
		cout << "2.Выйти из программы." << endl;
		fflush(stdin);
		ch = getchar();
		fflush(stdin);
		if (ch == '1')
		{
			do
			{
				client = new FTP();
				char addressOfServer[100] = "";
				cout << "Введите адрес FTP-сервера" << endl;//195.178.216.8     127.0.0.1   89.108.120.88
				fflush(stdin);
				cin >> addressOfServer;
				fflush(stdin);
				client->getAddress(addressOfServer);
				error = client->initSock();
				if (error == 0)
					cout << "Соединение с сервером установлено." << endl;
				else
					cout << "Ошибка соединения с сервером.Проверьте введенные данные." << endl;
			} while (error != 0);
			do
			{
				error = client->authorization();
				if (error == -1)
					cout << "Ошибка авторизации.Повторите попытку либо зайдите анонимно." << endl;

			} while (error != 0);

			system("pause");
			system("cls");
			showList(nameOfDir, client, list);
			system("pause");

			char choise;
			do
			{
				int error;
				showMenu();
				fflush(stdin);
				choise = getchar();
				fflush(stdin);

				switch (choise)
				{
				case '1':
					cout << "Введите имя файла для скачивания (целиком): " << endl;
					fflush(stdin);
					cin >> nameOfFile;
					fflush(stdin);
					if (!(strstr(list, nameOfFile)))
					{
						system("cls");
						showList(nameOfDir, client, list);
						cout << "Файла с таким именем не сущеcтвует либо вы ввели неправильное название." << endl;
						break;
					}
					error = client->getFile(nameOfFile);
					if (error == -1)
					{
						system("cls");
						showList(nameOfDir, client, list);
						cout << "Файл не скачан." << endl;
						break;
					}
					showList(nameOfDir, client, list);
					break;
				case '2':
					cout << "Введите имя файла для загрузки (целиком) из дериктории: " << DIR_FOR_SAVING << endl;
					fflush(stdin);
					cin >> nameOfFile;
					fflush(stdin);
					error = client->putFile(nameOfFile);
					if (error == -1)
					{
						system("cls");
						showList(nameOfDir, client, list);
						cout << "Файл не загружен." << endl;
						break;
					}
					cout << "Файл " << nameOfFile << " успешно загружен." << endl;
					showList(nameOfDir, client, list);
					break;
				case '3':
					cout << "Введите название директории для открытия на сервере: " << endl;
					fflush(stdin);
					cin >> nameOfDir;
					fflush(stdin);
					if (!(strstr(list, nameOfDir)))
					{
						system("cls");
						showList(nameOfDir, client, list);
						cout << "Папки с таким именем не сущеcтвует либо вы ввели неправильное название." << endl;
						strcpy(nameOfDir, "NULL");
						break;
					}
					client->goToDirectory(nameOfDir);
					showList(nameOfDir, client, list);
					break;
				case '4':
					cout << "Введите имя файла для удаления:" << endl;
					fflush(stdin);
					cin >> nameOfFile;
					fflush(stdin);
					if (!(strstr(list, nameOfFile)))
					{
						system("cls");
						showList(nameOfDir, client, list);
						cout << "Файла с таким именем не существует в данной директории." << endl;
						break;
					}
					error = client->deleteFile(nameOfFile);
					if (error == -1)
					{
						cout << "Файл не удален." << endl;
						client->getList(list);
						break;
					}
					cout << "Файл удален успешно." << endl;
					showList(nameOfDir, client, list);
					break;
				case '5':
					system("cls");
					showList(nameOfDir, client, list);
					break;
				case '7':
					client->close();
					break;
				case '6':
					client->returnToParentsDir();
					strcpy(nameOfDir, "NULL");
					showList(nameOfDir, client, list);
					break;
				default:
					cout << "Ошибка ввода. Повторите выбор операции снова." << endl;
					break;
				}
			} while (choise != '7');
		}
	} while (ch != '2');
	return 0;
}
