#include <string>
#include <vector>
#include <Windows.h>
#include "commands.cpp"


int main() {
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	ContactBook book; 

	std::cout << "This your contact book\n";
	UserHandler user_handler{&book};
	while (true) {
		user_handler.showStandardLabel();
		std::string command;
		std::cin >> command;
		user_handler.answerCommand(command);
	}
}
