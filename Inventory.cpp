#include <iostream>
#include <string>
#include <istream>
#include <vector>
#include <fstream>              
#include <sstream>
#include <cctype>
#include <conio.h>


//INVENTORY SYSTEM
using namespace std;
enum commands {
	UPDATED_INVENTORY,
	DELETED_ITEM,
	READCSV,
	EDITEDCSV,
	SEARCHED,
	DISPLAYED_INVENTORY,
};

string trim(string s) {
	while (!s.empty() && isspace((unsigned char)s.front())) s.erase(s.begin());
	while (!s.empty() && isspace((unsigned char)s.back()))  s.pop_back();
	return s;
}


class InventoryItem {
public:
	string name = "";
	int quantity = 0;

	InventoryItem(string name, int quantity) {
		this->name = name;
		this->quantity = quantity;
	}


};


class inventory {
public:
	vector<InventoryItem> Inventory;
	vector<commands>log;


	inventory() = default;

	string addItem(string name, int quantity) {
		log.push_back(UPDATED_INVENTORY);
		bool itemExists = false;
		{
			for (int i = 0; i < Inventory.size(); i++) {
				if (name == Inventory[i].name) {
					Inventory[i].quantity += quantity;
					itemExists = true;
					break;
				}
			}

			if (itemExists) return "Item already exists, quantity updated.";
			else {
				InventoryItem item(name, quantity);
				Inventory.push_back(item);
				return "Item added successfully.";
			}

		}
	}
	void displayInventory() {
		log.push_back(DISPLAYED_INVENTORY);
		for (int i = 0; i < Inventory.size(); i++) {
			cout << Inventory[i].name << ": " << Inventory[i].quantity << endl;
		}
	}
	void removeItem(string itemname) {
		log.push_back(DELETED_ITEM);
		for (int i = 0; i < Inventory.size(); i++) {
			if(itemname == Inventory[i].name) {
				Inventory.erase(Inventory.begin() + i);
				break;
			}
		}
	}
	void updateItem(string itemname, int quantity) {
		log.push_back(UPDATED_INVENTORY);
		for (int i = 0; i < Inventory.size(); i++) {
			if(itemname == Inventory[i].name) {
				Inventory[i].quantity = quantity;
				break;
			}
		}
	}

	void updateCSV(const string& filename) {
		log.push_back(EDITEDCSV);
		ofstream csv(filename);
		for (const auto& obj : Inventory) {
			csv << obj.name << "," << obj.quantity << "\n";
		}
		cout << "csv file successfully updated";
	}

	void readCSV(const string& filename) {
		log.push_back(READCSV);

		fstream csv(filename);
		string record;
		int line = 0;
		while (getline(csv, record)) {
			line++;
			istringstream s(record);
			string name, q;
			getline(s, name, ',');
			getline(s, q);
			int quantity; 
			q = trim(q);
			if (name.empty()) { cout << "no name detected at line: " << line<<endl; continue; }
			if (q.empty()) {quantity = 0; cout << "no quantity detected at line: " << line<<endl;
			}
			else { quantity = stoi(q); }
			addItem(name, quantity);

		}

	}

	void search() {
		log.push_back(SEARCHED);
		string s;
		vector<InventoryItem>matches;
		while (true) {
			cout << "enter item name to search for: ";
			cout << s << endl;
			if (_kbhit()) {

				char c = _getch();
				if (c == 8) { if (!s.empty()) s.pop_back(); }
				else {
				s += c;
			}
				matches.clear();
				for (int i = 0; i < Inventory.size(); i++) {
					if (Inventory[i].name.find(s) != string::npos) {
						matches.push_back(Inventory[i]);
					}
				}
		}
			cout << "matches: " << endl;
			for (int i = 0; i < matches.size(); i++) {
				cout << matches[i].name << ": " << matches[i].quantity << endl;
			}
			
			system("cls");
		}
	}

	void displayLog() {
		for (int i = 0; i < log.size(); i++) {
			switch (log[i]) {
			case UPDATED_INVENTORY:
				cout << "updated inventory" << endl;
				break;
			case DELETED_ITEM:
				cout << "deleted item" << endl;
				break;
			case READCSV:
				cout << "read csv file" << endl;
				break;
			case EDITEDCSV:
				cout << "edited csv file" << endl;
				break;
			case SEARCHED:
				cout << "searched inventory" << endl;
				break;
			case DISPLAYED_INVENTORY:
				cout << "displayed inventory" << endl;
				break;
			}
		}
	}



};



int main() {
	inventory inv;

	while (true) {
		cout << "1. Display Inventory" << endl;
		cout << "2. Add Item" << endl;
		cout << "3. update/create CSV file\n";
		cout << "4. read CSV file\n";
		cout << "5. search inventory\n";
		cout << "6. display log\n";
		int opt;
		cin >> opt;
		if (opt == 1) {
			cout << "Inventory:" << endl;
			inv.displayInventory();
		}
		else if (opt == 2) {
			cout << "Enter item name:";
			string name;
			cin >> name;
			cout << "Enter item amount:";
			int num;
			cin >> num;
			cout << inv.addItem(name, num) << endl;
			system("cls");
		}
		else if (opt == 3) {
			cout << "enter file name";
			string filename;
			cin >> filename;
			inv.updateCSV(filename);
		}
		else if (opt == 4) {
			cout << "enter file name: ";
			string filename;
			cin >> filename;
			inv.readCSV(filename);
		}
		else if (opt == 5) {
			inv.search();
		}
		else if (opt == 6)inv.displayLog();
	
		
	}
	return 0;
}
