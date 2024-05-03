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
	// Цвета 0-Белый,1-Красный,2-Синий,3-Зелёный,4-Оранжевый,5-Розовый,6-Жёлтый.
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
	// Конструктор класса интерфейс,
	UI(UI::colors defaultColor = White) {
		setlocale(LC_ALL, "Russian");
		this->defaultColor = defaultColor;
		setColor(defaultColor);
	}
	// Задать указатель на базу данных.
	void setDatabase(Database* db) { this->db = db; }
	// Цвет интерфейса по умолчанию.
	UI::colors defaultColor = White;
	// Задать цвет вывода в консоль.
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
	// Ввести данные заданного типа.
	template<typename T>
	T input(string text, bool newLine = true) {
		T input{};
		while (true) {
			printColor(text + " : ", newLine);
			SetConsoleCP(1251);
			cin >> input;
			SetConsoleCP(866);
			if (cin.fail()) {
				printColor("&2Неправильный тип данных!");
				cin.clear();
				cin.ignore(100500, '\n');
			}
			else {
				return input;
			}
		}
	}
	// Ввести строку со скрытием введёных символов.
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
	// Ввести данные заданного типа в диапозоне.
	template<typename T>
	T inputRange(string text, T min, T max, bool newLine = true) {
		T input{};
		while (true) {
			printColor(text + " : ", newLine);
			cin >> input;
			if (cin.fail()) {
				printColor("&2Неправильный тип данных!");
				cin.clear();
				cin.ignore(100500, '\n');
			}
			else if ((max != min) && (input > max || input < min)) {
				printColor("&2Введите значение в диапазоне от " + to_string(min) + " до " + to_string(max));
			}
			else {
				return input;
			}
		}
	}
	// Вывести информацию заданного типа.
	template <typename T>
	void print(T data, bool newLine = true) {
		SetConsoleCP(1251);
		if (newLine)  cout << '\n';
		cout << data;
		SetConsoleCP(866);
	}
	// Вывести строку с изменением цвета, с помощью символа & и идущей за ним цифры, соответствующей номеру цвета в перечислении colors. Пример: "&1Привет&3!".
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
	// Вывести набор строк в формате таблицы.
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
	// Вывести строку в линии
	void printHeader(string data, int precision, char fillingChar = '=') {
		cout << '\n';

		int test = (precision - data.size()) / 2;

		for (int i = 0; i < test; i++)
			cout << fillingChar;
		
		printColor(data, false);

		for (int i = 0; i < test; i++)
			cout << fillingChar;
	}
	// Ожидание ввода любой клавиши.
	void pressAnyButton() {
		cout << '\n';
		system("pause");
	}
	// Диалог подтверждения действия.
	const bool confirm() {
		return input<bool>("&6Вы уверены? (&20-Нет&6,&41-Да&6)");
	}

	// Наборы меню	
	
	// Меню действий организатора.
	void adminMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printHeader("&6Добро пожаловать " + db->getLogin(),50);
			printColor("1 - Редактировать список участниц");
			printColor("2 - Редактировать список аккаунтов");
			//printColor("2 - Редактировать список аккаунтов");
			printColor("0 - Выйти");
			int choice = inputRange<int>("Выберите действие из списка", 0, 2);
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
	// Меню действий зрителя.
	void spectatorMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printHeader("&6Добро пожаловать " + db->getLogin(),50);
			printColor("1 - Посмотреть на список участниц");
			printColor("2 - Посмотреть на список участниц по рейтингу");
			printColor("3 - Поиск в списке участниц");
			printColor("0 - Выйти");
			int choice = inputRange<int>("Выберите действие из списка", 0, 3);
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
	// Меню действий судьи.
	void judgeMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printHeader("&6Добро пожаловать " + db->getLogin(),50);
			printColor("1 - Посмотреть на список участниц");
			printColor("2 - Посмотреть на список выбывших участниц");
			printColor("3 - Оценить участницу");
			printColor("4 - Дисквалифицировать участницу");
			printColor("5 - Вернуть дисквалифицированную участницу");
			printColor("0 - Выйти");
			int choice = inputRange<int>("Выберите действие из списка", 0, 5);
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
	// Меню редактирования списка участниц.
	void participantsEditing() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printHeader("Редактировать список участниц",50);
			printColor("1 - Добавить участницу");
			printColor("2 - Удалить участницу");
			printColor("3 - Посмотреть список участниц");
			printColor("4 - Отсортировать список участниц");
			printColor("0 - Выйти");
			int choice = inputRange<int>("Выберите действие из списка", 0, 4);
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
	// Меню редактирования списка аккаунтов.
	void accountsEditing() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printHeader("Редактировать список аккаунтов",50);
			printColor("1 - Добавить аккаунт");
			printColor("2 - Удалить аккаунт");
			printColor("3 - Посмотреть список аккаунтов");
			printColor("4 - Редактировать параметр доступа аккаунтов");
			printColor("0 - Выйти");
			int choice = inputRange<int>("Выберите действие из списка", 0, 4);
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
	// Меню выбора сортировки.
	void sortMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printHeader("Порядки сортировки",50);
			printColor("1 - По рейтингу");
			printColor("2 - По возрасту");
			printColor("3 - По стране");
			printColor("4 - По весу");
			printColor("5 - По росту");
			printColor("0 - Выйти");

			int choice = inputRange<int>("Выберите действие из списка", 0, 5);
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
	// Меню выбора поиска
	void searchMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printHeader("Признаки поиска",50);
			printColor("1 - По стране");
			printColor("2 - По фамилии");
			printColor("3 - По возрасту");
			printColor("0 - Выйти");

			int choice = inputRange<int>("Выберите действие из списка", 0, 3);
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

// Определения методов

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
		// Обработка исключительных ситуаций
		if (currentAccount) {
			ui->printColor("&2Пользователь уже авторизован");
			return;
		}
		readAccountsFromFile();
		if (accounts.empty())
		{
			ui->printColor("&2Нет аккаунтов для авторизации");
			ui->printColor("Создайте аккаунт администратора для начала работы с базой данных");
			addAccount(1);
			currentAccount = accounts.at(0);
			accounts.at(0)->access = true;
			ui->printColor("&4Авторизация успешна");
			return;
		}
		// Предложить регистрацию
		bool isRegistered = ui->inputRange("Вы уже зарегистрированы в системе? (&20-Нет&0,&41-Да&0)", 0, 1);

		if (!isRegistered) {
			bool wantRegister = ui->inputRange("Хотите зарегистрироваться? (&20-Нет&0,&41-Да&0)", 0, 1);
			if (wantRegister) {
				addAccount();
				ui->printColor("&4Регистрация завершена, ожидайте подтверждения организатором");
			}
		}
		// Авторизация
		else {
			system("cls");
			ui->printHeader("В какую учётную запись вы хотите зайти?", 50);
			int authType = ui->inputRange("&20-Зритель\n&41-Организатор\n&52-Судья&0\nВаш выбор:", 0, 2);
			system("cls");
			ui->printColor("Войдите в аккаунт");
			string login;

			do {
				login = ui->input<string>("Введите логин");
				if (checkLogin(login) && whoIs(login) == authType) {
					break;
				}
				else if (!checkLogin(login)) ui->printColor("&2Неверный логин!");
				else ui->printColor("&2Этот аккаунт - другого типа!");
			} while (true);

			int accountID = findID(login);
			if (!accounts.at(accountID)->access) {
				ui->printColor("&2У этого аккаунта нету доступа. Ожидайте подтверждения организатором.");
				continue;
			}

			int attempts = 3;
			string password;

			do {
				password = ui->_input("Введите пароль");
				if (checkPassword(accountID, password) || attempts-- < 0) {
					break;
				}
				ui->printColor("&2Неверный пароль!");
			} while (true);

			if (attempts < 0) {
				ui->printColor("&2Вы неверно ввели пароль больше 3 раз, попробуйте снова.");
				continue;
			}

			currentAccount = accounts.at(accountID);
			ui->printColor("&4Авторизация успешна");
		}
		return;
	}
}

void Database::showAccounts() {
	if (accounts.empty())
	{
		ui->printColor("&2Список пуст");
		return;
	}
	vector<string> table;

	SetConsoleCP(1251);
	table.emplace_back("ID");
	table.emplace_back("Логин");
	table.emplace_back("Роль");
	table.emplace_back("Доступ");

	ui->printTable(table, 15, true);

	for (int i = 0; i < accounts.size(); i++) {
		table.clear();
		table.emplace_back(to_string(i));
		table.emplace_back(accounts.at(i)->login);

		switch (accounts.at(i)->accountType) {
		case 0:
			table.emplace_back("Гость");
			break;
		case 1:
			table.emplace_back("Организатор");
			break;
		case 2:
			table.emplace_back("Судья");
			break;
		}
		if (accounts.at(i)->access) table.emplace_back("Разрешён");
		else table.emplace_back("Запрещён");

		ui->printTable(table, 15);
	}
	SetConsoleCP(866);
}
void Database::addAccount(int type) {
	string login;
	while (true) {
		login = ui->input<string>("Введите логин");
		if (!checkLogin(login)) break;
		ui->printColor("&2Этот пользователь уже существует");
	}
	string password = ui->input<string>("Введите пароль");

	int accountType = type;
	if (type < 0 || type > 2)
		accountType = ui->input<int>("Введите тип аккаунта (&20-Зритель&0,&41-Организатор&0,&42-Судья&0)");

	if (isLoggedIn()) accounts.emplace_back(new Account(login, password, accountType, true));
	else accounts.emplace_back(new Account(login, password, accountType, false));

	writeAccountsToFile();
}
void Database::removeAccount() {
	showAccounts();

	if (accounts.empty()) {
		ui->printColor("&2 Нечего удалять");
		return;
	}
	int id = ui->inputRange<int>("Выберите номер удаляемого аккаунта", 0, accounts.size() - 1);

	if (!ui->confirm()) return;

	if (currentAccount == accounts.at(id)) {
		system("cls");
		ui->printColor("&2Вы не можете удалить аккаунт в котором авторизованы");
		return;
	}

	delete accounts.at(id);
	accounts.erase(accounts.begin() + id);

	writeAccountsToFile();

	ui->printColor("&4Аккаунт успешно удалён");
}
void Database::grantAccess()
{
	showAccounts();
	int id = ui->inputRange<int>("Выберите номер аккаунта для изменения параметра доступа", 0, accounts.size() - 1);

	if (!ui->confirm()) return;

	if (currentAccount == accounts.at(id)) {
		system("cls");
		ui->printColor("&2Вы не можете отобрать доступ у аккаунта, в котором авторизованы");
		return;
	}

	accounts.at(id)->access = !accounts.at(id)->access;

	writeAccountsToFile();

	ui->printColor("&4Параметр доступа изменён");
}

void Database::showGenericParticipantInfo() {
	if (participants.empty())
	{
		ui->printColor("&2Список участников пуст");
		return;
	}

	vector<string> table;

	SetConsoleCP(1251);
	table.emplace_back("ID");
	table.emplace_back("Имя");
	table.emplace_back("Фамилия");
	table.emplace_back("Страна");
	table.emplace_back("Возраст");

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
		ui->printColor("&2Список участников пуст");
		return;
	}

	vector<string> table;

	SetConsoleCP(1251);
	table.emplace_back("ID");
	table.emplace_back("Имя");
	table.emplace_back("Фамилия");
	table.emplace_back("Страна");
	table.emplace_back("Возраст");
	table.emplace_back("Вес");
	table.emplace_back("Рост");
	table.emplace_back("Рейтинг");

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
		ui->printColor("&2Список участников пуст");
		return;
	}
	vector<string> table;

	SetConsoleCP(1251);
	table.emplace_back("ID");
	table.emplace_back("Имя");
	table.emplace_back("Фамилия");
	table.emplace_back("Страна");
	table.emplace_back("Рейтинг");

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
		ui->printColor("&2Список участников пуст");
		return;
	}
	bool someoneBanned = false;
	vector<string> table;

	SetConsoleCP(1251);
	table.emplace_back("ID");
	table.emplace_back("Имя");
	table.emplace_back("Фамилия");
	table.emplace_back("Страна");
	table.emplace_back("Возраст");
	table.emplace_back("Вес");
	table.emplace_back("Рост");
	table.emplace_back("Рейтинг");

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
		ui->printColor("&2Список выбывших участников пуст");
	}
}

void Database::addParticipant() {
	string name = ui->input<string>("Введите имя");
	string surName = ui->input<string>("Введите фамилию");
	string country = ui->input<string>("Введите страну");
	int age = ui->input<int>("Введите возраст");
	int weight = ui->input<int>("Введите вес");
	int height = ui->input<int>("Введите рост");

	participants.emplace_back(new Participant(name, surName, country, age, height, weight, 0, true));

	writeParticipantsToFile();
}
void Database::removeParticipant() {
	showGenericParticipantInfo();

	if (participants.empty()) {
		ui->printColor("&2 Нечего удалять");
		return;
	}
	int id = ui->inputRange<int>("Выберите номер удаляемой участницы", 0, participants.size() - 1);

	if (!ui->confirm()) return;

	delete participants.at(id);
	participants.erase(participants.begin() + id);

	writeParticipantsToFile();

	ui->printColor("&4Участница успешно удалена");
}
void Database::banParticipant()
{
	showGenericParticipantInfo();

	if (participants.empty()) {
		ui->printColor("&2 Нечего удалять");
		return;
	}
	int id = ui->inputRange<int>("Выберите номер участницы, которая выбывает", 0, participants.size() - 1);

	if (!ui->confirm()) return;

	participants.at(id)->isParticipant = false;

	writeParticipantsToFile();
}
void Database::unbanParticipant()
{
	showBannedParticipantInfo();

	if (participants.empty()) {
		ui->printColor("&2 Нечего удалять");
		return;
	}
	int id = ui->inputRange<int>("Выберите номер возвращаемой участницы", 0, participants.size() - 1);

	if (!ui->confirm()) return;

	participants.at(id)->isParticipant = true;

	writeParticipantsToFile();
}
void Database::rateParticipant()
{
	if (participants.empty())
	{
		ui->printColor("&2Некого оценивать");
		return;
	}
	showGenericParticipantInfo();

	int id = ui->inputRange<int>("Выберите номер оцениваемой участницы", 0, participants.size() - 1);
	int rating = ui->inputRange<int>("Введите оценку", 0, 100);

	participants.at(id)->addRating(getLogin(), rating);
	participants.at(id)->calculateRating();

	writeParticipantsToFile();
}

void Database::findParticipantSurname() {
	if (participants.empty())
	{
		ui->printColor("&2Список участниц пуст");
		return;
	}
	string surName = ui->input<string>("Введите фамилию для поиска: ");

	vector<string> table;

	SetConsoleCP(1251);
	table.emplace_back("ID");
	table.emplace_back("Имя");
	table.emplace_back("Фамилия");
	table.emplace_back("Страна");
	table.emplace_back("Возраст");
	table.emplace_back("Рейтинг");

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
		ui->printColor("&2Список участников пуст");
		return;
	}
	int age = ui->input<int>("Введите возраст для поиска: ");

	vector<string> table;

	SetConsoleCP(1251);
	table.emplace_back("ID");
	table.emplace_back("Имя");
	table.emplace_back("Фамилия");
	table.emplace_back("Страна");
	table.emplace_back("Возраст");
	table.emplace_back("Рейтинг");

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
		ui->printColor("&2Список участников пуст");
		return;
	}
	string country = ui->input<string>("Введите страну для поиска: ");

	vector<string> table;

	SetConsoleCP(1251);
	table.emplace_back("ID");
	table.emplace_back("Имя");
	table.emplace_back("Фамилия");
	table.emplace_back("Страна");
	table.emplace_back("Возраст");
	table.emplace_back("Рейтинг");

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
