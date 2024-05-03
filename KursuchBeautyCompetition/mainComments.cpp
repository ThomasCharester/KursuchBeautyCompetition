#include "UserInterface.h"

int main() {
	// Создаём объект класса интерфейс.
	UI ui(UI::colors::Pink);
	// Создаём объект класса базы данных.
	Database database(&ui);
	// Задаём интерфейсу указатель на созданную базу данных.
	ui.setDatabase(&database);
	// Основной цикл программы, где вызывается меню для пользователя и происходит авторизация.
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
		bool choice = ui.input<bool>("Желаете &2выйти&0 или &4продолжить &9использование программы? (&20-Выйти&0/&41-Продолжить&0)");

		if (!choice) {
			if (!ui.confirm()) continue; /// XD
			ui.printColor("&6Хорошего дня!");
			return 0;
		}
	}

	return 0;
}


