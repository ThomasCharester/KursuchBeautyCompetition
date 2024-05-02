#pragma once
#ifndef USERINTERFACE
#define USERINTERFACE
#include "Database.h"

class UI {
private:
	Database* db = nullptr;
public:
	UI(){
		setlocale(LC_ALL, "Russian");
	}
	void setDatabase(Database* db) { this->db = db; }
	// Цвета 0-Белый,1-Красный,2-Синий,3-Зелёный,4-Оранжевый,5-Розовый,6-Жёлтый.
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
			SetConsoleCP(1251);
			cin >> input;
			SetConsoleCP(866);
			if (cin.fail()) {
				printColor("&1Неправильный тип данных!");
				cin.clear();
				cin.ignore(100500, '\n');
			}
			else {
				return input;
			}
		}
	}
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
	template<typename T>
	T inputRange(string text, T min, T max, bool newLine = true) {
		T input{};
		while (true) {
			printColor(text + " : ", newLine);
			cin >> input;
			if (cin.fail()) {
				printColor("&1Неправильный тип данных!");
				cin.clear();
				cin.ignore(100500, '\n');
			}
			else if ((max != min) && (input > max || input < min)) {
				printColor("&1Введите значение в диапазоне от " + to_string(min) + " до " + to_string(max));
			}
			else {
				return input;
			}
		}
	}
	template <typename T>
	void print(T data, bool newLine = true) {
		SetConsoleCP(1251);
		if (newLine)  cout << '\n';
		cout << data;
		SetConsoleCP(866);
	}
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
		setColor(White);
	}
	void pressAnyButton() {
		cout << '\n';
		system("pause");
	}
	const bool confirm() {
		return input<bool>("&6Вы уверены? (&10-Нет&0,&31-Да&0)");
	}

	// Наборы меню	
	void adminMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printColor("&6Добро пожаловать " + db->getLogin());
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
	void spectatorMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printColor("&6Добро пожаловать " + db->getLogin());
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
	void judgeMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printColor("&6Добро пожаловать " + db->getLogin());
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

	void participantsEditing() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printColor("Редактировать список участниц");
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
	void accountsEditing() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printColor("Редактировать список аккаунтов");
			printColor("1 - Добавить аккаунт");
			printColor("2 - Удалить аккаунт");
			printColor("3 - Посмотреть список аккаунтов");
			printColor("0 - Выйти");
			int choice = inputRange<int>("Выберите действие из списка", 0, 3);
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
			printColor("Порядки сортировки");
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
	void searchMenu() {
		while (true) {
			if (!db->isLoggedIn()) return;
			system("cls");
			printColor("Признаки поиска");
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
		if (currentAccount) {
			ui->printColor("&1Пользователь уже авторизован");
			return;
		}
		readAccountsFromFile();
		if (accounts.empty())
		{
			ui->printColor("&1Нет аккаунтов для авторизации");
			ui->printColor("Создайте аккаунт администратора для начала работы с базой данных");
			addAccount(1);
			currentAccount = accounts.at(0);
			ui->printColor("&3Авторизация успешна");
			return;
		}
		bool isRegistered = ui->inputRange("Вы уже зарегистрированы в системе? (&10-Нет&0,&31-Да&0)", 0, 1);

		if (!isRegistered) {
			bool wantRegister = ui->inputRange("Хотите зарегистрироваться в качестве зрителя? (&10-Нет&0,&31-Да&0)", 0, 1);
			if (wantRegister) {
				addAccount(0);
				currentAccount = accounts.at(accounts.size() - 1);
				ui->printColor("&3Авторизация успешна");
			}
		}
		else {
			ui->printColor("Войдите в аккаунт");
			string login;

			do {
				login = ui->input<string>("Введите логин");
				if (checkLogin(login)) {
					break;
				}
				ui->printColor("&1Неверный логин!");
			} while (true);

			int accountID = findID(login);
			int attempts = 3;
			string password;

			do {
				password = ui->_input("Введите пароль");
				if (checkPassword(accountID, password) || attempts-- < 0) {
					break;
				}
				ui->printColor("&1Неверный пароль!");
			} while (true);

			if (attempts < 0) {
				ui->printColor("&1Вы неверно ввели пароль больше 3 раз, попробуйте снова.");
				continue;
			}

			currentAccount = accounts.at(accountID);
			ui->printColor("&3Авторизация успешна");
		}
		return;
	}
}

void Database::showAccounts() {
	if (accounts.empty())
	{
		ui->printColor("&1Список пуст");
		return;
	}
	for (int i = 0; i < accounts.size(); i++) {
		ui->printColor(to_string(i) + " " + accounts.at(i)->login);

		switch (accounts.at(i)->accountType) {
		case 0:
			ui->printColor(" гость", false);
			break;
		case 1:
			ui->printColor(" администратор", false);
			break;
		case 2:
			ui->printColor(" судья", false);
			break;
		}
	}
}
void Database::addAccount(int type) {
	string login;
	while (true) {
		login = ui->input<string>("Введите логин");
		if (!checkLogin(login)) break;
		ui->printColor("&1Этот пользователь уже существует");
	}
	string password = ui->input<string>("Введите пароль");

	int accountType = type;
	if (type < 0 || type > 2)
		accountType = ui->input<int>("Введите тип аккаунта (&10-Зритель&0,&31-Администратор&0,&42-Судья&0)");

	accounts.emplace_back(new Account(login, password, accountType));

	writeAccountsToFile();
}
void Database::removeAccount() {
	showAccounts();

	if (accounts.empty()) {
		ui->printColor("&1 Нечего удалять");
		return;
	}
	int id = ui->inputRange<int>("Выберите номер удаляемого аккаунта", 0, accounts.size() - 1);

	if (!ui->confirm()) return;

	if (currentAccount == accounts.at(id)) {
		system("cls");
		ui->printColor("&1Вы не можете удалить аккаунт в котором авторизованы");
		return;
	}

	delete accounts.at(id);
	accounts.erase(accounts.begin() + id);

	writeAccountsToFile();

	ui->printColor("&3Аккаунт успешно удалён");
}

void Database::showGenericParticipantInfo() {
	if (participants.empty())
	{
		ui->printColor("&1Список участников пуст");
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
		ui->printColor("&1Список участников пуст");
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
		ui->printColor("&1Список участников пуст");
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
		ui->printColor("&1Список участников пуст");
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
		ui->printColor("&1Список выбывших участников пуст");
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
		ui->printColor("&1 Нечего удалять");
		return;
	}
	int id = ui->inputRange<int>("Выберите номер удаляемой участницы", 0, participants.size() - 1);

	if (!ui->confirm()) return;

	delete participants.at(id);
	participants.erase(participants.begin() + id);

	writeParticipantsToFile();

	ui->printColor("&3Участница успешно удалена");
}
void Database::banParticipant()
{
	showGenericParticipantInfo();

	if (participants.empty()) {
		ui->printColor("&1 Нечего удалять");
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
		ui->printColor("&1 Нечего удалять");
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
		ui->printColor("&1Некого оценивать");
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
		ui->printColor("&1Список участниц пуст");
		return;
	}
	string surName = ui->input<string>("Введите фамилию для поиска: ");
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
		ui->printColor("&1Список участников пуст");
		return;
	}
	int age = ui->input<int>("Введите возраст для поиска: ");
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
		ui->printColor("&1Список участников пуст");
		return;
	}
	string country = ui->input<string>("Введите страну для поиска: ");
	for (int i = 0; i < participants.size(); i++) {
		if (participants.at(i)->isParticipant && participants.at(i)->country == country)
			ui->printColor(participants.at(i)->name + " " + participants.at(i)->surName
				+ "\n\tCountry: " + participants.at(i)->country
				+ "\n\tAge: " + to_string(participants.at(i)->age)
				+ "\n\tRate: " + to_string(participants.at(i)->rate));
	}
}

#endif // !UI
