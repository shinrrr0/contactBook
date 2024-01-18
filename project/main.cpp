#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>

class Person {
private:
	friend class Contact;
	std::string name;
	std::string surname;
	std::string birthdate;
	Person(): name("None"), surname("None"), birthdate("None") {}
	Person(std::string _name, std::string _surname, std::string _birthdate) : name(_name), surname(_surname), birthdate(_birthdate) {}

public:
	std::string getName() const {
		return name;
	}
	std::string getSurname() const {
		return surname;
	}
	std::string getBirthdate() const {
		return birthdate;
	}
	void setName(std::string _name) {
		name = _name;
	}
	void setSurname(std::string _surname) {
		surname = _surname;
	}
	void setBirthdate(std::string _birthdate) {
		birthdate = _birthdate;
	}
};

class Contact {
private:
	friend class ContactBook;
	Contact() {
		person = Person{};
		phone_number = "None";
		email = "None";
	};
	Contact(std::string _name, std::string _surname, std::string _birthdate, std::string _phone_number, std::string _email) {
		person = Person{ _name, _surname, _birthdate };
		phone_number = _phone_number;
		email = _email;
	};
	std::string phone_number;
	std::string email;

public:
	void setPhone(std::string _phone) {
		phone_number = _phone;
	}
	void setEmail(std::string _email) {
		email = _email;
	}
	std::string getPhone() {
		return phone_number;
	}
	std::string getEmail() {
		return email;
	}
	Person person;
	void showPersonInfo() const {
		std::cout << "Name:\t\t\t" << person.getName() << std::endl;
		std::cout << "Surname:\t\t" << person.getSurname() << std::endl;
		std::cout << "Birth date:\t\t" << person.getBirthdate() << std::endl;
		std::cout << "Phone number:\t\t" << phone_number << std::endl;
		std::cout << "Email:\t\t\t" << email << std::endl;
		std::cout << std::endl;
	}
};

class ContactBook {
private:
	// ÏÅÐÅÍÅÑÈ CONTACTS ÑÞÄÀ ÎáÐÀÒÍÎ!!!!!
public:
	std::vector<Contact> contacts;
	bool isNameValid(std::string _name, std::string _surname) {
		for (const Contact& contact : contacts) {
			if (contact.person.getName() == _name && contact.person.getSurname() == _surname) {
				return true;
			}
		}
		return false;
	}
	void showContacts() {
		std::cout << "Contacts (" << contacts.size() << "):" << std::endl << std::endl;
		for (const Contact& contact: contacts) {
			contact.showPersonInfo();
		}
	}
	Contact& getContact(std::string _name, std::string _surname) { 
		for (Contact& contact : contacts) {
			if (contact.person.getName() == _name && contact.person.getSurname() == _surname) {
				return contact;
			}
		}
	}
	void deleteContact(std::string _name, std::string _surname) {
		for (auto it = contacts.begin(); it != contacts.end(); ++it)
		{
			if (it->person.getName() == _name && it->person.getSurname() == _surname)
			{
				it = contacts.erase(it);
				break;
			}
		}
	}
	void clearContacts() {
		int size = contacts.size();
		for (int i = 0; i < size; ++i) {
			contacts.pop_back();
		}
	}
	void deleteContact(Contact& contact) {
		deleteContact(contact.person.getName(), contact.person.getSurname());
	}
	void createNewContact(std::string _name, std::string _surname, std::string _birthdate, std::string _phone_number, std::string _email) {
		Contact new_contact {_name, _surname, _birthdate, _phone_number, _email };
		contacts.push_back(new_contact);
	}
};

class Reader {
public:
	void writeRequest(const std::string& request, std::fstream& output) {
		output << request << std::endl;
	}
	std::string readRequest(const std::string& request, std::fstream& input) const {
		std::string str;
		while (!input.eof()) {
			input >> str;
			if (str == request) {
				input >> str;
				return str;
			}
		}
		return "NOT FOUND";
	}

};

// Commands -----------------------------------------------------------------------

class Command {
protected:
	std::string trigger;
	ContactBook* contact_book;
private:
	virtual void execute() = 0;
public:
	std::string getTrigger() {
		return trigger;
	}
	void operator() () {
		execute();
	}
	Command(ContactBook* contact_book): contact_book(contact_book) {}
	Command() {}
	virtual ~Command() {};
};

class DialougeCommand: public Command {
protected:
	Contact* contact;
	bool contactDialouge(Contact*& contact, std::string action) {
		std::string name, surname, tmp = "";
		while (true) {
			std::cout << "Enter the name of contact you want to " + action + ": ";
			std::cin >> name;
			std::cout << "Enter the surname of contact you want to " + action + ": ";
			std::cin >> surname;
			if (contact_book->isNameValid(name, surname)) {
				contact = &(contact_book->getContact(name, surname));
				return true;
			}
			else {
				std::cout << "No such contact\n";
				while (tmp != "/retry") {
					std::cout << "Type \"/retry\" or type \"/exit\" to finish\n";
					std::cin >> tmp;
					if (tmp == "/exit") return false;
				}
				tmp = "";
			}
		}
	}
public:
	DialougeCommand(ContactBook* contact_book) : Command(contact_book) {}
};

class ShowContacts : public Command
{	
private:
	ContactBook* contact_book;
	void execute() override {
		contact_book->showContacts();
	}
public:
	ShowContacts(ContactBook* contact_book) : contact_book(contact_book) {
		trigger = "/showContacts";
	}
};

class Help : public Command
{
private:
	void execute() override {
		std::cout << std::endl;
		std::cout << "/showContacts - show all contacts";
		std::cout << std::endl;
		std::cout << "/changeContact - allows you to change contact parameters";
		std::cout << std::endl;
		std::cout << "/deleteContact - allows you to delete contact parameters";
		std::cout << std::endl;
		std::cout << "/createContact - allows you to create a new contact parameters";
		std::cout << std::endl;
		std::cout << "/clearContacts - allows you to clear all the contact book";
		std::cout << std::endl;
		std::cout << "/loadData - complements your contact book with entries from the save file";
		std::cout << std::endl;
		std::cout << "/saveData - overwrites the save file with all entries from your current contact book";
		std::cout << std::endl;
	}
public:
	Help() {
		trigger = "/help";
	}
};

class LoadData: public Command {
private:
	void execute() override {
		Reader rd;
		std::string name, surname, birthdate, phone_number, email;
		std::fstream file;
		file.open("contacts.cnt", std::ios::in);

		std::string tmp;
	
		int number = 1;
		int limit = stoi(rd.readRequest("number", file));
		while (number <= limit) {
			name = rd.readRequest(std::to_string(number) + "name", file);
			surname = rd.readRequest(std::to_string(number) + "surname", file);
			birthdate = rd.readRequest(std::to_string(number) + "birthdate", file);
			phone_number = rd.readRequest(std::to_string(number) + "phone", file);
			email = rd.readRequest(std::to_string(number) + "email", file);
			++number;
			if (!contact_book->isNameValid(name, surname)) {
				contact_book->createNewContact(name, surname, birthdate, phone_number, email);
			}
		}

		file.close();
		std::cout << "Data loaded successful\n\n";
	}
public:
	LoadData(ContactBook* contact_book) : Command(contact_book) {
		trigger = "/loadData";
	}
};

class SaveData : public Command {
private:
	void execute() override {
		Reader rd;
		std::fstream file;
		file.open("contacts.cnt", std::ios::out);

		int number = 1;

		rd.writeRequest("number " + std::to_string(contact_book->contacts.size()), file);
		for (Contact& contact: contact_book->contacts) {
			rd.writeRequest(std::to_string(number) + "name " + contact.person.getName(), file);
			rd.writeRequest(std::to_string(number) + "surname " + contact.person.getSurname(), file);
			rd.writeRequest(std::to_string(number) + "birthdate " + contact.person.getBirthdate(), file);
			rd.writeRequest(std::to_string(number) + "phone " + contact.getPhone(), file);
			rd.writeRequest(std::to_string(number) + "email " + contact.getEmail(), file);
			++number;
		}
		file.close();
		std::cout << "Data saved successful\n\n";
	}
public:
	SaveData(ContactBook* contact_book) : Command(contact_book) {
		trigger = "/saveData";
	}
};

class ClearContacts : public Command {
	void execute() override {
		contact_book->clearContacts();
		std::cout << "Contacts were cleared successful\n\n";
	}
public:
	ClearContacts(ContactBook* contact_book) : Command(contact_book) {
		trigger = "/clearContacts";
	}
};

class ChangeContact : public DialougeCommand {
private:	
	void execute() override {
		if (!contactDialouge(contact, "change")) { return; }

		std::string tmp;
		while (true) {
			std::cout << "Enter what do you want to change (name|surname|birthdate|phone|email), type \"/exit\" to finish:\n";
			std::cin >> tmp;
			if (tmp == "name") {
				std::cout << "Enter new name: ";
				std::cin >> tmp;
				contact->person.setName(tmp);
				contact->showPersonInfo();
				tmp = "";
			}
			else if (tmp == "surname") {
				std::cout << "Enter new surname: ";
				std::cin >> tmp;
				contact->person.setSurname(tmp);
				contact->showPersonInfo();
				tmp = "";
			}
			else if (tmp == "birthdate") {
				std::cout << "Enter new birthdate: ";
				std::cin >> tmp;
				contact->person.setBirthdate(tmp);
				contact->showPersonInfo();
				tmp = "";
			}
			else if (tmp == "phone") {
				std::cout << "Enter new phone: ";
				std::cin >> tmp;
				contact->setPhone(tmp);
				contact->showPersonInfo();
				tmp = "";
			}
			else if (tmp == "email") {
				std::cout << "Enter new email: ";
				std::cin >> tmp;
				contact->setEmail(tmp);
				contact->showPersonInfo();
				tmp = "";
			}
			else if (tmp == "/exit") {
				return;
			}
		}
	}
public:
	ChangeContact(ContactBook* contact_book) : DialougeCommand(contact_book) {
		trigger = "/changeContact";
	}
};

class DeleteContact : public DialougeCommand {
private:
	void execute() override {
		if (!contactDialouge(contact, "delete")) { return; }
		std::cout << contact->person.getName() << " " << contact->person.getSurname() << " was deleted";
		std::cout << std::endl << std::endl;
		contact_book->deleteContact(*contact);
		
	}
public:
	DeleteContact(ContactBook* contact_book) : DialougeCommand(contact_book) {
		trigger = "/deleteContact";
	}
};

class CreateContact : public DialougeCommand {
private:
	void execute() override {
		std::string name, surname, birthdate, phone_number, email, tmp = "";
		std::cout << "Enter new name: ";
		std::cin >> name;
		std::cout << "Enter new surname: ";
		std::cin >> surname;
		while (contact_book->isNameValid(name, surname)) {
			std::cout << "There is already a person with that name and surname\n";
			while (tmp != "/retry") {
				std::cout << "Type \"/retry\" or type \"/exit\" to finish\n";
				std::cin >> tmp;
				if (tmp == "/exit") return;
			}
			tmp = "";
			std::cout << "Enter new name: ";
			std::cin >> name;
			std::cout << "Enter new surname: ";
			std::cin >> surname;
		}

		std::cout << "Enter new birthdate: ";
		std::cin >> birthdate;
		std::cout << "Enter new phone number: ";
		std::cin >> phone_number;
		std::cout << "Enter new phone email: ";
		std::cin >> email;

		contact_book->createNewContact(name, surname, birthdate, phone_number, email);

		std::cout << name << ' ' << surname << " was added successful\n\n";
	}
public:
	CreateContact(ContactBook* contact_book) : DialougeCommand(contact_book) {
		trigger = "/createContact";
	}
};

//---------------------------------------------------------------------------------

class UserHandler {
private:
	//{"/help", "/showContacts", "/call", "saveData"};
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
		std::cout << "Ââåäèòå êîìàíäó (/help - ñïèñîê êîìàíä)" << std::endl;
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
	//book.createNewContact( "Ñòåïàí", "Áóçèí", "7.02.2007", "+79826870863", "abab@gmail.com" );
	//book.createNewContact( "Ëåíà", "Ãîôìàí", "14.88.1337", "+79821488228", "vrvrrv@gmail.com" );
	
	UserHandler user_handler{&book};
	while (true) {
		user_handler.showStandardLabel();
		std::string command;
		std::cin >> command;
		user_handler.answerCommand(command);
	}
}
