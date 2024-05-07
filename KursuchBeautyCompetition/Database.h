#pragma once
#ifndef DATABASE
#define DATABASE

#include "Account.h"
#include "Participant.h"
#include <fstream>
#include <algorithm>

class UI;

class Database {
private:
	Account* currentAccount = nullptr;
	vector<Account*> accounts;

	vector<Participant*> participants;

	UI* ui;

	int findID(string login) {
		for (int i = 0; i < accounts.size(); i++) {
			if (accounts.at(i)->login == login) return i;
		}
		return -1;
	}
	const bool checkPassword(int id, string password) {
		return accounts.at(id)->password == password;
	}

	void calculateParticipantsRatings();

public:
	Database(UI* ui);

	~Database();

	const int whoIsNow() const {
		if (isLoggedIn()) return currentAccount->accountType;
		else return -1;
	}
	const int whoIs(string login) const {
		for (Account* ac : accounts) {
			if (ac->login == login) return ac->accountType;
		}
		return -1;
	}
	const bool isLoggedIn() const { return currentAccount; }

	const bool checkLogin(string login) {
		for (Account* ac : accounts) {
			if (ac->login == login) return true;
		}
		return false;
	}
	const string getLogin() const { return currentAccount->login; }

	// Учётные записи //
	void login();
	void deauthorize() { currentAccount = nullptr; }

	void showAccounts();
	void addAccount(int type = -1);
	void removeAccount();
	void grantAccess();
	void changePassword();

	void writeAccountsToFile()
	{
		fstream file("accounts.txt", ios::out);
		file << accounts.size() << '\n';
		file.close();

		for (int i = 0; i < accounts.size(); i++)
			accounts.at(i)->addToFile();
	}
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

	// Участники //
	void showGenericParticipantInfo();
	void showDetailParticipantInfo();
	void showRateParticipantInfo();
	void showBannedParticipantInfo();

	void addParticipant();
	void removeParticipant();
	void banParticipant();
	void unbanParticipant();
	void rateParticipant();

	void sortParticipantRate() { sort(participants.begin(), participants.end(), Participant::byRate); }
	void sortParticipantAge() { sort(participants.begin(), participants.end(), Participant::byAge); }
	void sortParticipantCountry() { sort(participants.begin(), participants.end(), Participant::byCountry); }
	void sortParticipantHeight() { sort(participants.begin(), participants.end(), Participant::byHeight); }
	void sortParticipantWeight() { sort(participants.begin(), participants.end(), Participant::byWeight); }

	void findParticipantSurname();
	void findParticipantCountry();
	void findParticipantAge();

	void writeParticipantsToFile()
	{
		fstream file("participants.txt", ios::out);
		file << participants.size() << '\n';
		file.close();

		for (int i = 0; i < participants.size(); i++)
			participants.at(i)->addToFile();
	}
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
