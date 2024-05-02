#pragma once
#ifndef DATABASE
#define DATABASE
#include "Account.h"
#include "UI.h"

class Database {
private:
	struct Participant {
		string name = "";
		string surName = "";
		string country = "";
		int age = 0;
		int weight = 0;
		int height = 0;
		bool isParticipant = true;

		int rate = 0;
		vector<string> rated;
		vector<int> ratings;

		Participant() {};
		Participant(string name, string surName, string country, int age, int height, int weight, int rate, bool isParticipant) {
			this->name = name;
			this->surName = surName;
			this->country = country;
			this->age = age;
			this->height = height;
			this->weight = weight;
			this->rate = rate;
			this->isParticipant = isParticipant;
		}
		void addToFile() const
		{
			fstream file("participants.txt", ios::app);
			file << "p " << name << ' ' << surName << ' ' << country << ' ' << age << ' ' << height << ' ' << weight << ' ' << rate << ' ' << isParticipant << '\n';
			for (int i = 0; i < rated.size(); i++)
				file << "r " << rated.at(i) << ' ' << ratings.at(i) << '\n';

			file.close();
		}
		static bool byRate(const Participant* pt1, const Participant* pt2) {
			return pt1->rate < pt2->rate;
		}
		static bool byHeight(const Participant* pt1, const Participant* pt2) {
			return pt1->height < pt2->height;
		}
		static bool byAge(const Participant* pt1, const Participant* pt2) {
			return pt1->age < pt2->age;
		}
		static bool byWeight(const Participant* pt1, const Participant* pt2) {
			return pt1->weight < pt2->weight;
		}
		static bool byCountry(const Participant* pt1, const Participant* pt2) {
			return pt1->country.compare(pt2->country) < 0;
		}

		void addRating(string login, int rating) {
			for (int i = 0; i < rated.size(); i++)
				if (rated.at(i) == login) {
					ratings.at(i) = rating;
					return;
				}
			rated.emplace_back(login);
			ratings.emplace_back(rating);
		}
		void calculateRating() {
			if (ratings.empty()) return;
			rate = 0;
			for (int r : ratings)
				rate += r;
			rate /= ratings.size();
		}
	};
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
	const bool isLoggedIn() const { return currentAccount; }

	const bool checkLogin(string login) {
		for (Account* ac : accounts) {
			if (ac->login == login) return true;
		}
		return false;
	}

	const string getLogin() const { return currentAccount->login; }

	// ������� ������ //
	void login();
	void deauthorize() { currentAccount = nullptr; }

	void showAccounts();
	void addAccount(int type = -1);
	void removeAccount();

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

			getline(file, temp, '\n');
			int accountType = stoi(temp);

			accounts.emplace_back(new Account(login, enDecrypt(password), accountType));
		}
		file.close();
	}

	// ��������� //
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
};// ����������� �������

void Database::calculateParticipantsRatings()
{
	for (Participant* p : participants)
		p->calculateRating();
}

Database::Database(UI* ui) {
	readParticipantsFromFile();
	calculateParticipantsRatings();

	this->ui = ui;
}
Database::~Database() {
	writeAccountsToFile();
	calculateParticipantsRatings();
	writeParticipantsToFile();
}

void Database::login() {
	while (true) {
		if (currentAccount) {
			ui->printColor("&1������������ ��� �����������");
			return;
		}
		readAccountsFromFile();
		if (accounts.empty())
		{
			ui->printColor("&1��� ��������� ��� �����������");
			ui->printColor("�������� ������� �������������� ��� ������ ������ � ����� ������");
			addAccount(1);
			currentAccount = accounts.at(0);
			ui->printColor("&3����������� �������");
			return;
		}
		bool isRegistered = ui->inputRange("�� ��� ���������������� � �������? (&10-���&0,&31-��&0)", 0, 1);

		if (!isRegistered) {
			bool wantRegister = ui->inputRange("������ ������������������ � �������� �������? (&10-���&0,&31-��&0)", 0, 1);
			if (wantRegister) {
				addAccount(0);
				currentAccount = accounts.at(accounts.size() - 1);
				ui->printColor("&3����������� �������");
			}
		}
		else {
			ui->printColor("������� � �������");
			string login;

			do {
				login = ui->input<string>("������� �����");
				if (checkLogin(login)) {
					break;
				}
				ui->printColor("&1�������� �����!");
			} while (true);

			int accountID = findID(login);
			int attempts = 3;
			string password;

			do {
				password = ui->_input("������� ������");
				if (checkPassword(accountID, password) || attempts-- < 0) {
					break;
				}
				ui->printColor("&1�������� ������!");
			} while (true);

			if (attempts < 0) {
				ui->printColor("&1�� ������� ����� ������ ������ 3 ���, ���������� �����.");
				continue;
			}

			currentAccount = accounts.at(accountID);
			ui->printColor("&3����������� �������");
		}
		return;
	}
}

void Database::showAccounts() {
	if (accounts.empty())
	{
		ui->printColor("&1������ ����");
		return;
	}
	for (int i = 0; i < accounts.size(); i++) {
		ui->printColor(to_string(i) + " " + accounts.at(i)->login);

		switch (accounts.at(i)->accountType) {
		case 0:
			ui->printColor(" �����", false);
			break;
		case 1:
			ui->printColor(" �������������", false);
			break;
		case 2:
			ui->printColor(" �����", false);
			break;
		}
	}
}
void Database::addAccount(int type) {
	string login;
	while (true) {
		login = ui->input<string>("������� �����");
		if (!checkLogin(login)) break;
		ui->printColor("&1���� ������������ ��� ����������");
	}
	string password = ui->input<string>("������� ������");

	int accountType = type;
	if (type < 0 || type > 2)
		accountType = ui->input<int>("������� ��� �������� (&10-�������&0,&31-�������������&0,&42-�����&0)");

	accounts.emplace_back(new Account(login, password, accountType));

	writeAccountsToFile();
}
void Database::removeAccount() {
	showAccounts();

	if (accounts.empty()) {
		ui->printColor("&1 ������ �������");
		return;
	}
	int id = ui->inputRange<int>("�������� ����� ���������� ��������", 0, accounts.size() - 1);

	if (!ui->confirm()) return;

	if (currentAccount == accounts.at(id)) {
		system("cls");
		ui->printColor("&1�� �� ������ ������� ������� � ������� ������������");
		return;
	}

	delete accounts.at(id);
	accounts.erase(accounts.begin() + id);

	writeAccountsToFile();

	ui->printColor("&3������� ������� �����");
}

void Database::showGenericParticipantInfo() {
	if (participants.empty())
	{
		ui->printColor("&1������ ���������� ����");
		return;
	}
	for (int i = 0; i < participants.size(); i++) {
		if (participants.at(i)->isParticipant)
			ui->printColor("ID: " + to_string(i) + " " + participants.at(i)->name + " " + participants.at(i)->surName
				+ "\n\tCountry: " + participants.at(i)->country
				+ "\n\tAge: " + to_string(participants.at(i)->age));
	}
}
void Database::showDetailParticipantInfo() {
	if (participants.empty())
	{
		ui->printColor("&1������ ���������� ����");
		return;
	}
	for (int i = 0; i < participants.size(); i++) {
		if (participants.at(i)->isParticipant)
			ui->printColor("ID: " + to_string(i) + " " + participants.at(i)->name + " " + participants.at(i)->surName
				+ "\n\tCountry: " + participants.at(i)->country
				+ "\n\tAge: " + to_string(participants.at(i)->age)
				+ "\n\tWeight: " + to_string(participants.at(i)->weight) + " kg "
				+ "\n\tHeight: " + to_string(participants.at(i)->height) + " sm "
				+ "\n\tRate: " + to_string(participants.at(i)->rate));
	}
}
void Database::showRateParticipantInfo() {
	if (participants.empty())
	{
		ui->printColor("&1������ ���������� ����");
		return;
	}
	for (int i = 0; i < participants.size(); i++) {
		if (participants.at(i)->isParticipant)
			ui->printColor("ID: " + to_string(i) + " " + participants.at(i)->name + " " + participants.at(i)->surName
				+ "\n\tCountry: " + participants.at(i)->country
				+ "\n\tRate: " + to_string(participants.at(i)->rate));
	}
}
void Database::showBannedParticipantInfo() {
	if (participants.empty())
	{
		ui->printColor("&1������ ���������� ����");
		return;
	}
	bool someoneBanned = false;
	for (int i = 0; i < participants.size(); i++) {
		if (!participants.at(i)->isParticipant) {
			ui->printColor("ID: " + to_string(i) + " " + participants.at(i)->name + " " + participants.at(i)->surName
				+ "\n\tCountry: " + participants.at(i)->country
				+ "\n\tAge: " + to_string(participants.at(i)->age)
				+ "\n\tWeight: " + to_string(participants.at(i)->weight) + " kg "
				+ "\n\tHeight: " + to_string(participants.at(i)->height) + " sm "
				+ "\n\tRate: " + to_string(participants.at(i)->rate));
			someoneBanned = true;
		}
	}
	if (!someoneBanned)
	{
		ui->printColor("&1������ �������� ���������� ����");
	}
}

void Database::addParticipant() {
	string name = ui->input<string>("������� ���");
	string surName = ui->input<string>("������� �������");
	string country = ui->input<string>("������� ������");
	int age = ui->input<int>("������� �������");
	int weight = ui->input<int>("������� ���");
	int height = ui->input<int>("������� ����");

	participants.emplace_back(new Participant(name, surName, country, age, height, weight, 0, true));

	writeParticipantsToFile();
}
void Database::removeParticipant() {
	showGenericParticipantInfo();

	if (participants.empty()) {
		ui->printColor("&1 ������ �������");
		return;
	}
	int id = ui->inputRange<int>("�������� ����� ��������� ���������", 0, participants.size() - 1);

	if (!ui->confirm()) return;

	delete participants.at(id);
	participants.erase(participants.begin() + id);

	writeParticipantsToFile();

	ui->printColor("&3��������� ������� �������");
}
void Database::banParticipant()
{
	showGenericParticipantInfo();

	if (participants.empty()) {
		ui->printColor("&1 ������ �������");
		return;
	}
	int id = ui->inputRange<int>("�������� ����� ���������, ������� ��������", 0, participants.size() - 1);

	if (!ui->confirm()) return;

	participants.at(id)->isParticipant = false;

	writeParticipantsToFile();
}
void Database::unbanParticipant()
{
	showBannedParticipantInfo();

	if (participants.empty()) {
		ui->printColor("&1 ������ �������");
		return;
	}
	int id = ui->inputRange<int>("�������� ����� ������������ ���������", 0, participants.size() - 1);

	if (!ui->confirm()) return;

	participants.at(id)->isParticipant = true;

	writeParticipantsToFile();
}
void Database::rateParticipant()
{
	if (participants.empty())
	{
		ui->printColor("&1������ ���������");
		return;
	}
	showGenericParticipantInfo();

	int id = ui->inputRange<int>("�������� ����� ����������� ���������", 0, participants.size() - 1);
	int rating = ui->inputRange<int>("������� ������", 0, 100);

	participants.at(id)->addRating(getLogin(), rating);
	participants.at(id)->calculateRating();

	writeParticipantsToFile();
}

void Database::findParticipantSurname() {
	if (participants.empty())
	{
		ui->printColor("&1������ �������� ����");
		return;
	}
	string surName = ui->input<string>("������� ������� ��� ������: ");
	for (int i = 0; i < participants.size(); i++) {
		if (participants.at(i)->isParticipant && participants.at(i)->surName == surName)
			ui->printColor(participants.at(i)->name + " " + participants.at(i)->surName
				+ "\n\tCountry: " + participants.at(i)->country
				+ "\n\tAge: " + to_string(participants.at(i)->age)
				+ "\n\tRate: " + to_string(participants.at(i)->rate));
	}
}
void Database::findParticipantAge() {
	if (participants.empty())
	{
		ui->printColor("&1������ ���������� ����");
		return;
	}
	int age = ui->input<int>("������� ������� ��� ������: ");
	for (int i = 0; i < participants.size(); i++) {
		if (participants.at(i)->isParticipant && participants.at(i)->age == age)
			ui->printColor(participants.at(i)->name + " " + participants.at(i)->surName
				+ "\n\tCountry: " + participants.at(i)->country
				+ "\n\tAge: " + to_string(participants.at(i)->age)
				+ "\n\tRate: " + to_string(participants.at(i)->rate));
	}
}
void Database::findParticipantCountry() {
	if (participants.empty())
	{
		ui->printColor("&1������ ���������� ����");
		return;
	}
	string country = ui->input<string>("������� ������ ��� ������: ");
	for (int i = 0; i < participants.size(); i++) {
		if (participants.at(i)->isParticipant && participants.at(i)->country == country)
			ui->printColor(participants.at(i)->name + " " + participants.at(i)->surName
				+ "\n\tCountry: " + participants.at(i)->country
				+ "\n\tAge: " + to_string(participants.at(i)->age)
				+ "\n\tRate: " + to_string(participants.at(i)->rate));
	}
}

#endif // !DATABASE
