#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include "windows.h"


using namespace std;

class UI;

string enDecrypt(string text) {
	char key = text.size() % 10;

	for (char& ch : text)
		ch ^= key;
	return text;
}

struct Account {
	// 0 - �������, 1 - �����������, 2 - �����
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
class Database {
private:
	struct Participant {
		string name = "";
		string surName = "";
		string country = "";
		int age = 0;
		int weight = 0;
		int height = 0;
		int rate = 0;
		bool isParticipant = true;
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
			file << name << ' ' << surName << ' ' << country << ' ' << age << ' ' << height << ' ' << weight << ' ' << rate << ' ' << isParticipant << '\n';
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

	void calculateParticipantRate();
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

		for (int i = 0; i < size; i++)
		{
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
		file.close();
	}
};
class UI {
private:
	Database* db = nullptr;
public:
	UI() {
		setlocale(LC_ALL, "Russian");
	}
	void setDatabase(Database* db) { this->db = db; }
	// ����� 0-�����,1-�������,2-�����,3-������,4-���������,5-�������,6-Ƹ����.
	enum colors : int {
		White,
		Red,
		Blue,
		Green,
		Orange,
		Pink,
		Yellow
	};
	void setColor(UI::colors color) {
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		switch (color) {
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
			SetConsoleTextAttribute(handle, 7);
			break;
		}
	}
	template<typename T>
	T input(string text, bool newLine = true) {
		T input{};
		while (true) {
			printColor(text + " : ", newLine);
			cin >> input;
			if (cin.fail()) {
				printColor("&1������������ ��� ������!");
				cin.clear();
				cin.ignore();
			}
			else {
				return input;
			}
		}
	}
	template<typename T>
	T inputRange(string text, T min, T max, bool newLine = true) {
		T input{};
		while (true) {
			printColor(text + " : ", newLine);
			cin >> input;
			if (cin.fail()) {
				printColor("&1������������ ��� ������!");
				cin.clear();
				cin.ignore();
			}
			else if ((max != min) && (input > max || input < min)) {
				printColor("&1������� �������� � ��������� �� " + to_string(min) + " �� " + to_string(max));
			}
			else {
				return input;
			}
		}
	}
	template <typename T>
	void print(T data, bool newLine = true) {
		if (newLine)  cout << '\n';
		cout << data;
	}
	void printColor(string str, bool newLine = true) {
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
		setColor(White);
	}
	void pressAnyButton() {
		cout << '\n';
		system("pause");
	}
	const bool confirm() {
		return input<bool>("&6�� �������? (&10-���&0,&31-��&0)");
	}

	// ������ ����	
	void adminMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printColor("&6����� ���������� " + db->getLogin());
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

	void spectatorMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printColor("&6����� ���������� " + db->getLogin());
			printColor("1 - ���������� �� ������ ��������");
			printColor("2 - ���������� �� ������ �������� �� ��������");
			printColor("0 - �����");
			int choice = inputRange<int>("�������� �������� �� ������", 0, 2);
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
			}
		}
	}

	void judgeMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printColor("&6����� ���������� " + db->getLogin());
			printColor("1 - ���������� �� ������ ��������");
			printColor("2 - ���������� �� ������ �������� ��������");
			printColor("0 - �����");
			int choice = inputRange<int>("�������� �������� �� ������", 0, 2);
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
			}
		}
	}
	void participantsEditing() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printColor("������������� ������ ��������");
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

	void accountsEditing() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printColor("������������� ������ ���������");
			printColor("1 - �������� �������");
			printColor("2 - ������� �������");
			printColor("3 - ���������� ������ ���������");
			printColor("0 - �����");
			int choice = inputRange<int>("�������� �������� �� ������", 0, 3);
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
			}
		}
	}
	void sortMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printColor("������� ����������");
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
};

int main() {

	UI ui;
	Database database(&ui);

	ui.setDatabase(&database);

	while (true) {
		system("cls");

		database.login();

		switch (database.whoIsNow()) {
		case 0:
			ui.spectatorMenu();
			break;
		case 1:
			ui.adminMenu();
			break;
		case 2:
			ui.judgeMenu();
			break;
		}
		bool choice = ui.input<bool>("������� &1�����&0 ��� &3���������� &0������������� ���������? (&10-�����&0/&31-����������&0)");

		if (!choice) {
			if (!ui.confirm()) continue; /// XD
			ui.printColor("&6�������� ���!");
			return 0;
		}
	}

	return 0;
}


// ����������� �������

Database::Database(UI* ui) {
	readParticipantsFromFile();

	this->ui = ui;
	//ui->startUI();
}

Database::~Database() {
	writeAccountsToFile();
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
			ui->printColor("�������� ������� ��� �����������");
			addAccount();
			currentAccount = accounts.at(0);
			ui->printColor("&3����������� �������");
			return;
		}
		bool isRegistered = ui->inputRange("�� ��� ���������������� � �������? (&10-���&0,&31-��&0)", 0, 1);

		if (!isRegistered) {
			bool wantRegister = ui->inputRange("������ ������������������ � �������� �������? (&10-���&0,&31-��&0)", 0, 1);
			if (wantRegister) {
				addAccount(0);
				currentAccount = accounts.at(0);
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
				password = ui->input<string>("������� ������");
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
		ui->printColor("&1�� ������� �������, � ������� ���� ������������");
		deauthorize();
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