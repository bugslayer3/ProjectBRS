/*
Project: BRS (Banking Record System):

File handling has been effectively used for each feature of this project.Here, I am going to describe these features in brief.

Show / List Data :
With the information provided in add record, the void show_data() function in this banking record system project in C++ show the record corresponding to a particular account number, first name and last name.Current balance of the account holder is displayed.

Search Record :
When the function for this feature is first executed, it shows the total records in the file, and the user can then search by record number.If the record searched for is not found, the banking record system project in C++ displays the message – “Error in opening!File Not Found!!”

Edit Record :
This works in similar manner to the Search feature.When the function for Edit Record is first executed, it shows the total records in the file, and the user can edit the information by providing record number.Then, the C++ project shows all the data in that record, and the user can enter any data to modify.If the record to be edited for is not found, it displays the message – “Error in opening!File Not Found!!”

Delete Record :
First of all, when the function of this feature is executed, it shows all the records in the file, and the user can enter the record number to delete.If the record was not found, this banking record system project in C++ displays the message – “Error in opening!File Not Found!!”
*/
#include "Main.h"

int main()
{
	// Define records
	std::ofstream records;

	// Check to see if Records file exists
	if (fileExists("records.txt") == true) {}
	// Else Create file Records
	else
	{
		records.open("records.txt");
		records << "[Records]";
		records.close();
	}

	std::cout << "BANKING RECORD SYSTEM\n"
		<< "--------------------\n\n"
		<< "Select one option from below\n"
		<< "1 - add record\n"
		<< "2 - show / list record\n"
		<< "3 - search record\n"
		<< "4 - edit record\n"
		<< "5 - update record\n"
		<< "6 - delete record\n" 
		<< std::endl;

	//input checking
	int option;

	std::cin >> option;

	switch (option) 
	{
	case 1:
		read_data(records);
		break;

	case 2:
		//read_record();
		break;

	case 3:
		//search_record();
		break;
	case 4:
		//edit_record();
		break;
	case 5:
		//update_record();
		break;
	case 6:
		//delete_record();
		break;
	default:
		std::cout << "Please enter a correct number: ";
	}
	//read_data();

	return 0;
}

bool fileExists(const std::string& filename)
{
	struct stat buf;
	if (stat(filename.c_str(), &buf) != -1)
	{
		return true;
	}
	return false;
}

// Add Record :
// For this feature void read_data() function has been used to add banking record into the file. It asks for information such as account number, first name, last name and balance to be entered.
void read_data(std::ofstream record)
{
	// Account Number, First Name, Last Name and Balance to be entered
	double accountNumber, accountBalance;
	std::string firstName, lastName;

	std::cout << "You have selected Option 1. - Add Record\n"
		<< "Please enter the account number: ";
	std::cin >> accountNumber;

	std::cout << "Please enter the first name of the Client: ";
	std::cin >> firstName;

	std::cout << "Please enter the last name of the Client: ";
	std::cin >> lastName;

	std::cout << "Please enter the Clients new bank balance: ";
	std::cin >> accountBalance;
	std::cout << std::endl;

	std::cout << "Account number: " << accountNumber << "\nClient: " << firstName << " " << lastName << "\n" 
		<< "Bank Balance: " << accountBalance << std::endl
		<< "Please confirm this information by pressing 1: ";

	int input;
	std::cin >> input;

	if (input == 1)
	{
		// Process Record
		record.open("records.txt");
		record << "[Records]";
		record.close();
	}
}