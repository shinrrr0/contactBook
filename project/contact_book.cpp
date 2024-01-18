#include <iostream>
#include <string>
#include <vector>

class Person {
private:
	friend class Contact;
	std::string name;
	std::string surname;
	std::string birthdate;
	Person() : name("None"), surname("None"), birthdate("None") {}
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
		for (const Contact& contact : contacts) {
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
		Contact new_contact{ _name, _surname, _birthdate, _phone_number, _email };
		contacts.push_back(new_contact);
	}
};