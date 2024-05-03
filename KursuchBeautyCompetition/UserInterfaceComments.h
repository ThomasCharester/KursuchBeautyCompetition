#pragma once
#ifndef USERINTERFACE
#define USERINTERFACE
#include "Database.h"
#include <iomanip>
#include <iostream>
#include <conio.h>
#include "Windows.h"

class UI {
private:
	Database* db = nullptr;
public:
	// ����� 0-�����,1-�������,2-�����,3-������,4-���������,5-�������,6-Ƹ����.
	enum colors : int {
		Default,
		White,
		Red,
		Blue,
		Green,
		Orange,
		Pink,
		Yellow
	};
	// ����������� ������ ���������,
	UI(UI::colors defaultColor = White) {
		setlocale(LC_ALL, "Russian");
		this->defaultColor = defaultColor;
		setColor(defaultColor);
	}
	// ������ ��������� �� ���� ������.
	void setDatabase(Database* db) { this->db = db; }
	// ���� ���������� �� ���������.
	UI::colors defaultColor = White;
	// ������ ���� ������ � �������.
	void setColor(UI::colors color) {
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		switch (color) {
		case UI::colors::White:
			SetConsoleTextAttribute(handle, 7);
			break;
		case UI::colors::Red:
			SetConsoleTextAttribute(handle, 12);
			break;
		case UI::colors::Green:
			SetConsoleTextAttribute(handle, 10);
			break;
		case UI::colors::Blue:
			SetConsoleTextAttribute(handle, 9);
			break;
		case UI::colors::Yellow:
			SetConsoleTextAttribute(handle, 14);
			break;
		case UI::colors::Orange:
			SetConsoleTextAttribute(handle, 6);
			break;
		case UI::colors::Pink:
			SetConsoleTextAttribute(handle, 13);
			break;
		default:
			setColor(defaultColor);
			break;
		}
	}
	// ������ ������ ��������� ����.
	template<typename T>
	T input(string text, bool newLine = true) {
		T input{};
		while (true) {
			printColor(text + " : ", newLine);
			SetConsoleCP(1251);
			cin >> input;
			SetConsoleCP(866);
			if (cin.fail()) {
				printColor("&2������������ ��� ������!");
				cin.clear();
				cin.ignore(100500, '\n');
			}
			else {
				return input;
			}
		}
	}
	// ������ ������ �� �������� ������� ��������.
	string _input(string text, bool newLine = true) {
		string input = "";
		printColor(text + " : ", newLine);
		SetConsoleCP(1251);
		char ch = _getch();
		do {
			cout << '*';
			input += ch;
			ch = _getch();
		} while (ch != 13);
		SetConsoleCP(866);
		return input;
	}
	// ������ ������ ��������� ���� � ���������.
	template<typename T>
	T inputRange(string text, T min, T max, bool newLine = true) {
		T input{};
		while (true) {
			printColor(text + " : ", newLine);
			cin >> input;
			if (cin.fail()) {
				printColor("&2������������ ��� ������!");
				cin.clear();
				cin.ignore(100500, '\n');
			}
			else if ((max != min) && (input > max || input < min)) {
				printColor("&2������� �������� � ��������� �� " + to_string(min) + " �� " + to_string(max));
			}
			else {
				return input;
			}
		}
	}
	// ������� ���������� ��������� ����.
	template <typename T>
	void print(T data, bool newLine = true) {
		SetConsoleCP(1251);
		if (newLine)  cout << '\n';
		cout << data;
		SetConsoleCP(866);
	}
	// ������� ������ � ���������� �����, � ������� ������� & � ������ �� ��� �����, ��������������� ������ ����� � ������������ colors. ������: "&1������&3!".
	void printColor(string str, bool newLine = true) {
		SetConsoleCP(1251);
		if (newLine)  cout << '\n';
		bool flag = false;
		for (char ch : str) {
			if (!flag)
				if (ch != '&')
					cout << ch;
				else flag = true;
			else {
				setColor(static_cast<UI::colors>((ch - 48)));
				flag = false;
			}
		}
		SetConsoleCP(866);
		setColor(defaultColor);
	}
	// ������� ����� ����� � ������� �������.
	int printTable(vector<string> data, int precision = -1, bool header = false) {
		int max = data.at(0).length();
		for (string str : data) {
			if (str.length() > max) max = str.length();
		}
		if (precision >= 0) max = precision;
		cout << '\n' << '|';
		SetConsoleOutputCP(1251);
		for (string str : data) {
			cout << setw(max) << left << str << '|';
		}
		SetConsoleOutputCP(866);
		cout << '\n';
		for (int i = 0; i < data.size() * max + data.size() + 1; i++)
			if (header) cout << '=';
			else cout << '-';
		return max;
	}
	// ������� ������ � �����
	void printHeader(string data, int precision, char fillingChar = '=') {
		cout << '\n';

		int test = (precision - data.size()) / 2;

		for (int i = 0; i < test; i++)
			cout << fillingChar;
		
		printColor(data, false);

		for (int i = 0; i < test; i++)
			cout << fillingChar;
	}
	// �������� ����� ����� �������.
	void pressAnyButton() {
		cout << '\n';
		system("pause");
	}
	// ������ ������������� ��������.
	const bool confirm() {
		return input<bool>("&6�� �������? (&20-���&6,&41-��&6)");
	}

	// ������ ����	
	
	// ���� �������� ������������.
	void adminMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printHeader("&6����� ���������� " + db->getLogin(),50);
			printColor("1 - ������������� ������ ��������");
			printColor("2 - ������������� ������ ���������");
			//printColor("2 - ������������� ������ ���������");
			printColor("0 - �����");
			int choice = inputRange<int>("�������� �������� �� ������", 0, 2);
			system("cls");
			switch (choice) {
			case 0:
				db->deauthorize();
				return;
				break;
			case 1:
				participantsEditing();
				break;
			case 2:
				accountsEditing();
				break;
			}
		}
	}
	// ���� �������� �������.
	void spectatorMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printHeader("&6����� ���������� " + db->getLogin(),50);
			printColor("1 - ���������� �� ������ ��������");
			printColor("2 - ���������� �� ������ �������� �� ��������");
			printColor("3 - ����� � ������ ��������");
			printColor("0 - �����");
			int choice = inputRange<int>("�������� �������� �� ������", 0, 3);
			system("cls");
			switch (choice) {
			case 0:
				db->deauthorize();
				return;
				break;
			case 1:
				db->showGenericParticipantInfo();
				pressAnyButton();
				break;
			case 2:
				db->showRateParticipantInfo();
				pressAnyButton();
				break;
			case 3:
				searchMenu();
				break;
			}
		}
	}
	// ���� �������� �����.
	void judgeMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printHeader("&6����� ���������� " + db->getLogin(),50);
			printColor("1 - ���������� �� ������ ��������");
			printColor("2 - ���������� �� ������ �������� ��������");
			printColor("3 - ������� ���������");
			printColor("4 - ������������������ ���������");
			printColor("5 - ������� �������������������� ���������");
			printColor("0 - �����");
			int choice = inputRange<int>("�������� �������� �� ������", 0, 5);
			system("cls");
			switch (choice) {
			case 0:
				db->deauthorize();
				return;
				break;
			case 1:
				db->showDetailParticipantInfo();
				pressAnyButton();
				break;
			case 2:
				db->showBannedParticipantInfo();
				pressAnyButton();
				break;
			case 3:
				system("cls");
				db->rateParticipant();
				break;
			case 4:
				system("cls");
				db->banParticipant();
				break;
			case 5:
				system("cls");
				db->unbanParticipant();
				break;
			}
		}
	}
	// ���� �������������� ������ ��������.
	void participantsEditing() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printHeader("������������� ������ ��������",50);
			printColor("1 - �������� ���������");
			printColor("2 - ������� ���������");
			printColor("3 - ���������� ������ ��������");
			printColor("4 - ������������� ������ ��������");
			printColor("0 - �����");
			int choice = inputRange<int>("�������� �������� �� ������", 0, 4);
			system("cls");
			switch (choice) {
			case 0:
				return;
				break;
			case 1:
				db->addParticipant();
				break;
			case 2:
				db->removeParticipant();
				break;
			case 3:
				system("cls");
				db->showDetailParticipantInfo();
				pressAnyButton();
				break;
			case 4:
				sortMenu();
				break;
			}
		}
	}
	// ���� �������������� ������ ���������.
	void accountsEditing() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printHeader("������������� ������ ���������",50);
			printColor("1 - �������� �������");
			printColor("2 - ������� �������");
			printColor("3 - ���������� ������ ���������");
			printColor("4 - ������������� �������� ������� ���������");
			printColor("0 - �����");
			int choice = inputRange<int>("�������� �������� �� ������", 0, 4);
			system("cls");
			switch (choice) {
			case 0:
				return;
				break;
			case 1:
				db->addAccount();
				break;
			case 2:
				db->removeAccount();
				break;
			case 3:
				system("cls");
				db->showAccounts();
				pressAnyButton();
				break;
			case 4:
				db->grantAccess();
				break;
			}
		}
	}
	// ���� ������ ����������.
	void sortMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printHeader("������� ����������",50);
			printColor("1 - �� ��������");
			printColor("2 - �� ��������");
			printColor("3 - �� ������");
			printColor("4 - �� ����");
			printColor("5 - �� �����");
			printColor("0 - �����");

			int choice = inputRange<int>("�������� �������� �� ������", 0, 5);
			system("cls");
			switch (choice) {
			case 0:
				return;
				break;
			case 1:
				db->sortParticipantRate();
				break;
			case 2:
				db->sortParticipantAge();
				break;
			case 3:
				db->sortParticipantCountry();
				break;
			case 4:
				db->sortParticipantWeight();
				break;
			case 5:
				db->sortParticipantHeight();
				break;
			}
			return;
		}
	}
	// ���� ������ ������
	void searchMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printHeader("�������� ������",50);
			printColor("1 - �� ������");
			printColor("2 - �� �������");
			printColor("3 - �� ��������");
			printColor("0 - �����");

			int choice = inputRange<int>("�������� �������� �� ������", 0, 3);
			system("cls");
			switch (choice) {
			case 0:
				return;
				break;
			case 1:
				system("cls");
				db->findParticipantCountry();
				pressAnyButton();
				break;
			case 2:
				system("cls");
				db->findParticipantSurname();
				pressAnyButton();
				break;
			case 3:
				system("cls");
				db->findParticipantAge();
				pressAnyButton();
				break;
			}
		}
	}
};

// ����������� �������

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
		// ��������� �������������� ��������
		if (currentAccount) {
			ui->printColor("&2������������ ��� �����������");
			return;
		}
		readAccountsFromFile();
		if (accounts.empty())
		{
			ui->printColor("&2��� ��������� ��� �����������");
			ui->printColor("�������� ������� �������������� ��� ������ ������ � ����� ������");
			addAccount(1);
			currentAccount = accounts.at(0);
			accounts.at(0)->access = true;
			ui->printColor("&4����������� �������");
			return;
		}
		// ���������� �����������
		bool isRegistered = ui->inputRange("�� ��� ���������������� � �������? (&20-���&0,&41-��&0)", 0, 1);

		if (!isRegistered) {
			bool wantRegister = ui->inputRange("������ ������������������? (&20-���&0,&41-��&0)", 0, 1);
			if (wantRegister) {
				addAccount();
				ui->printColor("&4����������� ���������, �������� ������������� �������������");
			}
		}
		// �����������
		else {
			system("cls");
			ui->printHeader("� ����� ������� ������ �� ������ �����?", 50);
			int authType = ui->inputRange("&20-�������\n&41-�����������\n&52-�����&0\n��� �����:", 0, 2);
			system("cls");
			ui->printColor("������� � �������");
			string login;

			do {
				login = ui->input<string>("������� �����");
				if (checkLogin(login) && whoIs(login) == authType) {
					break;
				}
				else if (!checkLogin(login)) ui->printColor("&2�������� �����!");
				else ui->printColor("&2���� ������� - ������� ����!");
			} while (true);

			int accountID = findID(login);
			if (!accounts.at(accountID)->access) {
				ui->printColor("&2� ����� �������� ���� �������. �������� ������������� �������������.");
				continue;
			}

			int attempts = 3;
			string password;

			do {
				password = ui->_input("������� ������");
				if (checkPassword(accountID, password) || attempts-- < 0) {
					break;
				}
				ui->printColor("&2�������� ������!");
			} while (true);

			if (attempts < 0) {
				ui->printColor("&2�� ������� ����� ������ ������ 3 ���, ���������� �����.");
				continue;
			}

			currentAccount = accounts.at(accountID);
			ui->printColor("&4����������� �������");
		}
		return;
	}
}

void Database::showAccounts() {
	if (accounts.empty())
	{
		ui->printColor("&2������ ����");
		return;
	}
	vector<string> table;

	SetConsoleCP(1251);
	table.emplace_back("ID");
	table.emplace_back("�����");
	table.emplace_back("����");
	table.emplace_back("������");

	ui->printTable(table, 15, true);

	for (int i = 0; i < accounts.size(); i++) {
		table.clear();
		table.emplace_back(to_string(i));
		table.emplace_back(accounts.at(i)->login);

		switch (accounts.at(i)->accountType) {
		case 0:
			table.emplace_back("�����");
			break;
		case 1:
			table.emplace_back("�����������");
			break;
		case 2:
			table.emplace_back("�����");
			break;
		}
		if (accounts.at(i)->access) table.emplace_back("��������");
		else table.emplace_back("��������");

		ui->printTable(table, 15);
	}
	SetConsoleCP(866);
}
void Database::addAccount(int type) {
	string login;
	while (true) {
		login = ui->input<string>("������� �����");
		if (!checkLogin(login)) break;
		ui->printColor("&2���� ������������ ��� ����������");
	}
	string password = ui->input<string>("������� ������");

	int accountType = type;
	if (type < 0 || type > 2)
		accountType = ui->input<int>("������� ��� �������� (&20-�������&0,&41-�����������&0,&42-�����&0)");

	if (isLoggedIn()) accounts.emplace_back(new Account(login, password, accountType, true));
	else accounts.emplace_back(new Account(login, password, accountType, false));

	writeAccountsToFile();
}
void Database::removeAccount() {
	showAccounts();

	if (accounts.empty()) {
		ui->printColor("&2 ������ �������");
		return;
	}
	int id = ui->inputRange<int>("�������� ����� ���������� ��������", 0, accounts.size() - 1);

	if (!ui->confirm()) return;

	if (currentAccount == accounts.at(id)) {
		system("cls");
		ui->printColor("&2�� �� ������ ������� ������� � ������� ������������");
		return;
	}

	delete accounts.at(id);
	accounts.erase(accounts.begin() + id);

	writeAccountsToFile();

	ui->printColor("&4������� ������� �����");
}
void Database::grantAccess()
{
	showAccounts();
	int id = ui->inputRange<int>("�������� ����� �������� ��� ��������� ��������� �������", 0, accounts.size() - 1);

	if (!ui->confirm()) return;

	if (currentAccount == accounts.at(id)) {
		system("cls");
		ui->printColor("&2�� �� ������ �������� ������ � ��������, � ������� ������������");
		return;
	}

	accounts.at(id)->access = !accounts.at(id)->access;

	writeAccountsToFile();

	ui->printColor("&4�������� ������� ������");
}

void Database::showGenericParticipantInfo() {
	if (participants.empty())
	{
		ui->printColor("&2������ ���������� ����");
		return;
	}

	vector<string> table;

	SetConsoleCP(1251);
	table.emplace_back("ID");
	table.emplace_back("���");
	table.emplace_back("�������");
	table.emplace_back("������");
	table.emplace_back("�������");

	ui->printTable(table, 15, true);

	for (int i = 0; i < participants.size(); i++) {
		table.clear();
		if (participants.at(i)->isParticipant) {
			table.emplace_back(to_string(i));
			table.emplace_back(participants.at(i)->name);
			table.emplace_back(participants.at(i)->surName);
			table.emplace_back(participants.at(i)->country);
			table.emplace_back(to_string(participants.at(i)->age));
			ui->printTable(table, 15);
		}
	}
	SetConsoleCP(866);
}
void Database::showDetailParticipantInfo() {
	if (participants.empty())
	{
		ui->printColor("&2������ ���������� ����");
		return;
	}

	vector<string> table;

	SetConsoleCP(1251);
	table.emplace_back("ID");
	table.emplace_back("���");
	table.emplace_back("�������");
	table.emplace_back("������");
	table.emplace_back("�������");
	table.emplace_back("���");
	table.emplace_back("����");
	table.emplace_back("�������");

	ui->printTable(table, 15, true);

	for (int i = 0; i < participants.size(); i++) {
		table.clear();
		if (participants.at(i)->isParticipant) {
			table.emplace_back(to_string(i));
			table.emplace_back(participants.at(i)->name);
			table.emplace_back(participants.at(i)->surName);
			table.emplace_back(participants.at(i)->country);
			table.emplace_back(to_string(participants.at(i)->age));
			table.emplace_back(to_string(participants.at(i)->weight));
			table.emplace_back(to_string(participants.at(i)->height));
			table.emplace_back(to_string(participants.at(i)->rate));
			ui->printTable(table, 15);
		}
	}
	SetConsoleCP(866);
}
void Database::showRateParticipantInfo() {
	if (participants.empty())
	{
		ui->printColor("&2������ ���������� ����");
		return;
	}
	vector<string> table;

	SetConsoleCP(1251);
	table.emplace_back("ID");
	table.emplace_back("���");
	table.emplace_back("�������");
	table.emplace_back("������");
	table.emplace_back("�������");

	ui->printTable(table, 15, true);

	for (int i = 0; i < participants.size(); i++) {
		table.clear();
		if (participants.at(i)->isParticipant) {
			table.emplace_back(to_string(i));
			table.emplace_back(participants.at(i)->name);
			table.emplace_back(participants.at(i)->surName);
			table.emplace_back(participants.at(i)->country);
			table.emplace_back(to_string(participants.at(i)->rate));
			ui->printTable(table, 15);
		}
	}
	SetConsoleCP(866);
}
void Database::showBannedParticipantInfo() {
	if (participants.empty())
	{
		ui->printColor("&2������ ���������� ����");
		return;
	}
	bool someoneBanned = false;
	vector<string> table;

	SetConsoleCP(1251);
	table.emplace_back("ID");
	table.emplace_back("���");
	table.emplace_back("�������");
	table.emplace_back("������");
	table.emplace_back("�������");
	table.emplace_back("���");
	table.emplace_back("����");
	table.emplace_back("�������");

	ui->printTable(table, 15, true);

	for (int i = 0; i < participants.size(); i++) {
		table.clear();
		if (!participants.at(i)->isParticipant) {
			table.emplace_back(to_string(i));
			table.emplace_back(participants.at(i)->name);
			table.emplace_back(participants.at(i)->surName);
			table.emplace_back(participants.at(i)->country);
			table.emplace_back(to_string(participants.at(i)->age));
			table.emplace_back(to_string(participants.at(i)->weight));
			table.emplace_back(to_string(participants.at(i)->height));
			table.emplace_back(to_string(participants.at(i)->rate));
			ui->printTable(table, 15);
			someoneBanned = true;
		}
	}
	SetConsoleCP(866);
	if (!someoneBanned)
	{
		ui->printColor("&2������ �������� ���������� ����");
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
		ui->printColor("&2 ������ �������");
		return;
	}
	int id = ui->inputRange<int>("�������� ����� ��������� ���������", 0, participants.size() - 1);

	if (!ui->confirm()) return;

	delete participants.at(id);
	participants.erase(participants.begin() + id);

	writeParticipantsToFile();

	ui->printColor("&4��������� ������� �������");
}
void Database::banParticipant()
{
	showGenericParticipantInfo();

	if (participants.empty()) {
		ui->printColor("&2 ������ �������");
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
		ui->printColor("&2 ������ �������");
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
		ui->printColor("&2������ ���������");
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
		ui->printColor("&2������ �������� ����");
		return;
	}
	string surName = ui->input<string>("������� ������� ��� ������: ");

	vector<string> table;

	SetConsoleCP(1251);
	table.emplace_back("ID");
	table.emplace_back("���");
	table.emplace_back("�������");
	table.emplace_back("������");
	table.emplace_back("�������");
	table.emplace_back("�������");

	ui->printTable(table, 15, true);

	for (int i = 0; i < participants.size(); i++) {
		table.clear();
		if (participants.at(i)->isParticipant && participants.at(i)->surName == surName) {
			table.emplace_back(to_string(i));
			table.emplace_back(participants.at(i)->name);
			table.emplace_back(participants.at(i)->surName);
			table.emplace_back(participants.at(i)->country);
			table.emplace_back(to_string(participants.at(i)->age));
			table.emplace_back(to_string(participants.at(i)->rate));
			ui->printTable(table, 15);
		}
	}
	SetConsoleCP(866);
}
void Database::findParticipantAge() {
	if (participants.empty())
	{
		ui->printColor("&2������ ���������� ����");
		return;
	}
	int age = ui->input<int>("������� ������� ��� ������: ");

	vector<string> table;

	SetConsoleCP(1251);
	table.emplace_back("ID");
	table.emplace_back("���");
	table.emplace_back("�������");
	table.emplace_back("������");
	table.emplace_back("�������");
	table.emplace_back("�������");

	ui->printTable(table, 15, true);

	for (int i = 0; i < participants.size(); i++) {
		table.clear();
		if (participants.at(i)->isParticipant && participants.at(i)->age == age) {
			table.emplace_back(to_string(i));
			table.emplace_back(participants.at(i)->name);
			table.emplace_back(participants.at(i)->surName);
			table.emplace_back(participants.at(i)->country);
			table.emplace_back(to_string(participants.at(i)->age));
			table.emplace_back(to_string(participants.at(i)->rate));
			ui->printTable(table, 15);
		}
	}
	SetConsoleCP(866);
}
void Database::findParticipantCountry() {
	if (participants.empty())
	{
		ui->printColor("&2������ ���������� ����");
		return;
	}
	string country = ui->input<string>("������� ������ ��� ������: ");

	vector<string> table;

	SetConsoleCP(1251);
	table.emplace_back("ID");
	table.emplace_back("���");
	table.emplace_back("�������");
	table.emplace_back("������");
	table.emplace_back("�������");
	table.emplace_back("�������");

	ui->printTable(table, 15, true);

	for (int i = 0; i < participants.size(); i++) {
		table.clear();
		if (participants.at(i)->isParticipant && participants.at(i)->country == country) {
			table.emplace_back(to_string(i));
			table.emplace_back(participants.at(i)->name);
			table.emplace_back(participants.at(i)->surName);
			table.emplace_back(participants.at(i)->country);
			table.emplace_back(to_string(participants.at(i)->age));
			table.emplace_back(to_string(participants.at(i)->rate));
			ui->printTable(table, 15);
		}
	}
	SetConsoleCP(866);
}

#endif // !UI
