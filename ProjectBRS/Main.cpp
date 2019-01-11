/*
Project: BRS (Banking Record System):

File handling has been effectively used for each feature of this project.Here, I am going to describe these features in brief.

Add Record :
For this feature void read_data() function has been used to add banking record into the file.It asks for information such as account number, first name, last name and balance to be entered.

Show / List Data :
With the information provided in add record, the void show_data() function in this banking record system project in C++ show the record corresponding to a particular account number, first name and last name.Current balance of the account holder is displayed.

Search Record :
When the function for this feature is first executed, it shows the total records in the file, and the user can then search by record number.If the record searched for is not found, the banking record system project in C++ displays the message – “Error in opening!File Not Found!!”

Edit Record :
This works in similar manner to the Search feature.When the function for Edit Record is first executed, it shows the total records in the file, and the user can edit the information by providing record number.Then, the C++ project shows all the data in that record, and the user can enter any data to modify.If the record to be edited for is not found, it displays the message – “Error in opening!File Not Found!!”

Delete Record :
First of all, when the function of this feature is executed, it shows all the records in the file, and the user can enter the record number to delete.If the record was not found, this banking record system project in C++ displays the message – “Error in opening!File Not Found!!”
*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void read_data() {

	int acc_num;
	string first_name;
	string last_name;
	double balance;
	cout << "Account number: "; cin >> acc_num;
	cout << "First name: ";     cin >> first_name;
	cout << "Last name: ";      cin >> last_name;
	cout << "Balance: ";        cin >> balance;

	//keep track of accounts
	ofstream accounts("accounts.txt");                               
	accounts << first_name + "_" + last_name + ".txt";
	accounts.close();

	//create new file for new accounts 
	ofstream entering(first_name + "_" + last_name + ".txt");        
	entering << acc_num << " " << first_name << " " << last_name << " " << balance;
	entering.close();

}

void show_data() {

	cout << "search by:\n";


}

void search_record() {

}

void edit_record() {

}

void delete_record() {

}

void invalid_input() {
	cout << "invalid option. Please try again.\n";
	cin.clear();
	cin.ignore(10000, '\n');
}

int main()
{

	cout << "BANKING RECORD SYSTEM\n" << endl;
	cout << "1 - add record\n";
	cout << "2 - show / list data\n";
	cout << "3 - search record\n";
	cout << "4 - edit record\n";
	cout << "5 - delete record\n";

	//input checking
	int option;
	do
	{
		cin >> option;
		if (option > 5 || option < 1) {
			invalid_input();
		}
	} while (option>5||option<1);


	switch (option) {
	case 1:
		read_data();
		break;

	case 2:
		show_data();
		break;

	case 3:
		search_record();
		break;
	case 4:
		edit_record();
		break;
	case 5:
		delete_record();
		break;
	}
	//read_data();


}