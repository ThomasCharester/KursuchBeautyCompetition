#pragma once
#ifndef DATABASE
#define DATABASE

#include "Account.h"
#include "Participant.h"
#include <fstream>
#include <algorithm>

// Прототип класса интерфейса, чтобы класс базы данных, знал о наличии интерфейса.
class UI;
// Класс базы данных.
class Database {
private:
	// Указатель на текущую учёнтую запись.
	Account* currentAccount = nullptr;
	// Вектор указателей на объекты аккаунтов.
	vector<Account*> accounts;
	// Вектор указателей на объекты участниц.
	vector<Participant*> participants;
	// Указатель на объект класса интерфейс.
	UI* ui;
	// Найти номер аккаунта в векторе по логину.
	int findID(string login) {
		for (int i = 0; i < accounts.size(); i++) {
			if (accounts.at(i)->login == login) return i;
		}
		return -1;
	}
	// Проверить правильность введённого пароля.
	const bool checkPassword(int id, string password) {
		return accounts.at(id)->password == password;
	}
	// Перерассчитать рейтинги участниц.
	void calculateParticipantsRatings();
public:
	// Конструктор базы данных, который задаёт указатель на интерфейс и читает данные из файлов.
	Database(UI* ui);
	// Деструктор базы данных, записывает данные в файл.
	~Database();

	// Аккаунты

	// Какую роль имеет авторизованный пользователь.
	const int whoIsNow() const {
		if (isLoggedIn()) return currentAccount->accountType;
		else return -1;
	}
	// Какую роль имеет пользователь с данным логиком.
	const int whoIs(string login) const {
		for (Account* ac : accounts) {
			if (ac->login == login) return ac->accountType;
		}
		return -1;
	}
	// Авторизован ли пользователь. Т.е. вошёл ли пользователь в аккаунт.
	const bool isLoggedIn() const { return currentAccount; }
	// Проверить есть ли аккаунт с таким логином.
	const bool checkLogin(string login) {
		for (Account* ac : accounts) {
			if (ac->login == login) return true;
		}
		return false;
	}
	// Получить логин текущего пользователя.
	const string getLogin() const { return currentAccount->login; }
	// Авторизоваться в программе.
	void login();
	// Деавторизоваться.
	void deauthorize() { currentAccount = nullptr; }
	// Показать список аккаунтов, их логин, тип, есть у него доступ к базе или нет.
	void showAccounts();
	// Создать новый аккаунт.
	void addAccount(int type = -1);
	// Удалить аккаунт.
	void removeAccount();
	// Предоставить доступ аккаунту.
	void grantAccess();
	// Сохранить все данные об аккаунтах в файл.
	void writeAccountsToFile()
	{
		fstream file("accounts.txt", ios::out);
		file << accounts.size() << '\n';
		file.close();

		for (int i = 0; i < accounts.size(); i++)
			accounts.at(i)->addToFile();
	}
	// Прочитать данные об аккаунтах из файла.
	void readAccountsFromFile()
	{
		accounts.clear();
		fstream file("accounts.txt", ios::in);

		if (file.eof() || !file.is_open())
		{
			file.close();
			return;
		}

		string temp;

		getline(file, temp, '\n');
		int size = stoi(temp);

		for (int i = 0; i < size; i++)
		{
			string login, password;
			getline(file, login, ' ');
			getline(file, password, ' ');

			getline(file, temp, ' ');
			int accountType = stoi(temp);

			getline(file, temp, '\n');
			bool access = stoi(temp);

			accounts.emplace_back(new Account(login, enDecrypt(password), accountType, access));
		}
		file.close();
	}

	// Участницы

	// Показать общую информацию об участницах.
	void showGenericParticipantInfo();
	// Показать детальную информацию об участницах.
	void showDetailParticipantInfo();
	// Показать рейтинг участниц.
	void showRateParticipantInfo();
	// Показать выбывших участниц.
	void showBannedParticipantInfo();
	// Добавить участницу.
	void addParticipant();
	// Удалить участницу.
	void removeParticipant();
	// Икслючить участницу.
	void banParticipant();
	// Вернуть исключённую учёстницу.
	void unbanParticipant();
	// Оценить участницу.
	void rateParticipant();

	// Сортировки

	// Сортировать участниц по рейтингу.
	void sortParticipantRate() { sort(participants.begin(), participants.end(), Participant::byRate); }
	// Сортировать участниц по возрасту.
	void sortParticipantAge() { sort(participants.begin(), participants.end(), Participant::byAge); }
	// Сортировать участниц по стране.
	void sortParticipantCountry() { sort(participants.begin(), participants.end(), Participant::byCountry); }
	// Сортировать участниц по росту.
	void sortParticipantHeight() { sort(participants.begin(), participants.end(), Participant::byHeight); }
	// Сортировать участниц по весу.
	void sortParticipantWeight() { sort(participants.begin(), participants.end(), Participant::byWeight); }

	// Поиск участниц

	// Поиск участниц по фамилии.
	void findParticipantSurname();
	// Поиск участниц по стране.
	void findParticipantCountry();
	// Поиск участниц по возрасту.
	void findParticipantAge();
	// Сохранить информацию об участницах в файл.
	void writeParticipantsToFile()
	{
		fstream file("participants.txt", ios::out);
		file << participants.size() << '\n';
		file.close();

		for (int i = 0; i < participants.size(); i++)
			participants.at(i)->addToFile();
	}
	// Прочитать информацию об участницах из файла.
	void readParticipantsFromFile()
	{
		participants.clear();
		fstream file("participants.txt", ios::in);

		if (file.eof() || !file.is_open())
		{
			file.close();
			return;
		}

		string temp;

		getline(file, temp, '\n');
		int size = stoi(temp);

		for (int i = 0; i < size; )
		{
			getline(file, temp, ' ');
			if (temp == "p") {
				i++;
				string name, surName, country;
				int age, weight, height, rate;
				bool isParticipant;
				getline(file, name, ' ');
				getline(file, surName, ' ');
				getline(file, country, ' ');
				getline(file, temp, ' ');
				age = stoi(temp);
				getline(file, temp, ' ');
				height = stoi(temp);
				getline(file, temp, ' ');
				weight = stoi(temp);
				getline(file, temp, ' ');
				rate = stoi(temp);
				getline(file, temp, '\n');
				isParticipant = stoi(temp);
				participants.emplace_back(new Participant(name, surName, country, age, height, weight, rate, isParticipant));
			}
			else if (temp == "r") {
				string login;
				int rating;
				getline(file, login, ' ');
				getline(file, temp, '\n');
				rating = stoi(temp);
				participants.at(participants.size() - 1)->addRating(login, rating);
			}
		}
		file.close();
	}
};

#endif // !DATABASE
