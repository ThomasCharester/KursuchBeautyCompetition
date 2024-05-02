#pragma once
#ifndef ACCOUNT
#define ACCOUNT

#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include "windows.h"

using namespace std;

string enDecrypt(string text) {
	char key = text.size() % 10;

	for (char& ch : text)
		ch ^= key;
	return text;
}

struct Account {
	// 0 - Зритель, 1 - Организатор, 2 - Судья
	int accountType = 0;
	string password = "";
	string login = "";

	Account() {};
	Account(string login, string password, int accountType) {
		this->login = login;
		this->password = password;
		this->accountType = accountType;
	}
	bool operator == (const Account& account) {
		return login == account.login && password == account.password;
	}
	void addToFile() const
	{
		fstream file("accounts.txt", ios::app);
		file << login << ' ' << enDecrypt(password) << ' ' << accountType << '\n';
		file.close();
	}
};
#endif // !ACCOUNT