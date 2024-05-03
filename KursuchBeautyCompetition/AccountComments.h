#pragma once
#ifndef ACCOUNT
#define ACCOUNT

#include <string>
#include <fstream>

using namespace std;

// Шифрование/Дешифрование пароля.
string enDecrypt(string text) {
	char key = text.size() % 10;

	for (char& ch : text)
		ch ^= key;
	return text;
}
// Класс аккаунта пользвателя, может быть зрителем,судьёй или организатором.
struct Account {
	// 0 - Зритель, 1 - Организатор, 2 - Судья.
	int accountType = 0;
	// Пароль аккаунта.
	string password = "";
	// Логин аккаунта.
	string login = "";
	// Имеет ли доступ к базе данных этот аккаунт.
	bool access = false;
	// Конструктор по умолчанию.
	Account() {};
	// Конструктор с задачей всех полей.
	Account(string login, string password, int accountType, bool access) {
		this->login = login;
		this->password = password;
		this->accountType = accountType;
		this->access = access;
	}
	// Перегрузка оператора сравнения.
	bool operator == (const Account& account) {
		return login == account.login && password == account.password;
	}
	// Внесение всех полей аккаунта в файл.
	void addToFile() const
	{
		fstream file("accounts.txt", ios::app);
		file << login << ' ' << enDecrypt(password) << ' ' << accountType << ' '<< access <<'\n';
		file.close();
	}
};
#endif // !ACCOUNT