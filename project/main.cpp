#include <string>
#include <vector>
#include <Windows.h>
#include "commands.cpp"


class UserHandler {
private:
	ContactBook* contact_book;
	std::vector<Command*> commands{
		new Help,
		new ShowContacts{ contact_book },
		new ChangeContact{ contact_book },
		new DeleteContact{ contact_book }, 
		new CreateContact{ contact_book },
		new ClearContacts{ contact_book },
		new SaveData{ contact_book },
		new LoadData{ contact_book } };
public:
	void showStandardLabel() {
		std::cout << "Введите команду (/help - список команд)" << std::endl;
	}
	void answerCommand(const std::string command) {
		for (Command* elem : commands) {
			if (elem->getTrigger() == command) {
				(*elem)();
			}
		}
	}
	UserHandler(ContactBook* contact_book) : contact_book(contact_book) {}
	~UserHandler() {
		for (Command* elem : commands) {
			delete elem;
		}
	}
};


int main() {
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	ContactBook book; 
	
	UserHandler user_handler{&book};
	while (true) {
		user_handler.showStandardLabel();
		std::string command;
		std::cin >> command;
		user_handler.answerCommand(command);
	}
}
