#include <iostream>

using namespace std;

class Text {
private:
	class State {
	private:
		friend class Text;
		int number;

		State() = default;
		State(const State* st) {
			number = st->number;
		}
	};
public:
	class Memento {
	public:
		void restoreState() {
			org->state = new State(state);
		}
	private:
		friend class Text;
		Memento(Text* org) : org(org), state(new State(org->state)) {}
		Text* org;
		State* state;
	};
	
	Text() {
		state = new State{};
	}
	Memento* createMemento() {
		return new Memento{this};
	}
	void add() {
		++(state->number);
	}
	void show() {
		cout << state->number << endl;
	}
private:
	State* state;
};



//int main()
//{
//	Text text;
//	text.add();
//	text.show();
//	auto m1 = text.createMemento();
//	text.add();
//	text.show();
//	m1->restoreState();
//	text.show();
//	cout << "restored" << endl;
//	text.add();
//	text.add();
//	text.show();
//	auto m2 = text.createMemento();
//	text.add();
//	text.show();
//	m2->restoreState();
//	text.show();
//	cout << "restored" << endl;
//}