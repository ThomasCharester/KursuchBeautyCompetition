#pragma once
#ifndef UI
#define UI
#include "Database.h"

class UI {
private:
	Database* db = nullptr;
public:
	UI() {
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


#endif // !UI
