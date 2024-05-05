#include "UserInterface.h"

int main() {

	UI ui(UI::colors::Pink);
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
		bool choice = ui.inputRangeInstant("Желаете &2выйти&0 или &4продолжить &9использование программы? (&20-Выйти&0/&41-Продолжить&0)",0,1);

		if (!choice) {
			if (!ui.confirm()) continue; /// XD
			ui.printColor("&6Хорошего дня!");
			return 0;
		}
	}

	return 0;
}


