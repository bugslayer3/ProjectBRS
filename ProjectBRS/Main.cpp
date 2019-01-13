#include <tuple>
#include <iostream>
#include <sqlite3.h>
using namespace std;
/*
Project: BRS (Banking Record System):

File handling has been effectively used for each feature of this project.Here, I am going to describe these features in brief.

Add Record :
For this feature void read_data() function has been used to add banking record into the file.It asks for information such as account number, first name, last name and balance to be entered.

Show / List Data :
With the information provided in add record, the void show_data() function in this banking record system project in C++ show the record corresponding to a particular account number, first name and last name.Current balance of the account holder is displayed.

Search Record :
When the function for this feature is first executed, it shows the total records in the file, and the user can then search by record number.If the record searched for is not found, the banking record system project in C++ displays the message � �Error in opening!File Not Found!!�

Edit Record :
This works in similar manner to the Search feature.When the function for Edit Record is first executed, it shows the total records in the file, and the user can edit the information by providing record number.Then, the C++ project shows all the data in that record, and the user can enter any data to modify.If the record to be edited for is not found, it displays the message � �Error in opening!File Not Found!!�

Delete Record :
First of all, when the function of this feature is executed, it shows all the records in the file, and the user can enter the record number to delete.If the record was not found, this banking record system project in C++ displays the message � �Error in opening!File Not Found!!�
*/

const int pageSize = 10;

struct Record
{
    int id;
    string accountNum;
    string firstname;
    string lastname;
    double balance;
};

tuple<Record, bool> read_data(bool newData = true)
{
    Record user;
    if (newData)
    {
        cout << "Enter account number: ";
        cin >> user.accountNum;
    }
    auto ok = true;

    cout << "Enter firstname: ";
    cin >> user.firstname;
    cout << "Enter lastname: ";
    cin >> user.lastname;
    cout << "Enter balance: ";
    cin >> user.balance;
    cout << "invalid balance" << endl;
    ok = cin.good();
    if (!ok)
    {
        cin.clear();
        cin.ignore(1000, '\n');
    }

    return make_tuple(user, ok);
}

void insert_record(sqlite3 *db, Record &user)
{
    sqlite3_stmt *stmt;
    const char *query = R"(
        INSERT INTO `records` (`accountNum`, `firstname`, `lastname`, `balance`)
        VALUES (?, ?, ?, ?)
    )";
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "failed to prepare query: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        sqlite3_finalize(stmt);
        return;
    }

    int rc;
    sqlite3_bind_text(stmt, 1, user.accountNum.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user.firstname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, user.lastname.c_str(), -1, SQLITE_STATIC);
    rc = sqlite3_bind_double(stmt, 4, user.balance);

    rc = sqlite3_step(stmt);
    switch (rc)
    {
    case SQLITE_DONE:
        cerr << "account successfully created: " << user.accountNum << endl;
        break;
    case SQLITE_CONSTRAINT:
        cerr << "account number is not available, please use a different account number" << endl;
        break;
    default:
        cerr << "an error ocurred while inserting into database: " << rc << endl;
        cerr << "**" << sqlite3_errstr(rc) << endl;
        break;
    }

    sqlite3_finalize(stmt);
}

int count_records(sqlite3 *db)
{
    sqlite3_stmt *stmt;
    const char *query = "SELECT count(*) FROM `records`";

    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "failed to prepare query: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        sqlite3_finalize(stmt);
        return 0;
    }

    int rc;
    int count = 0;
    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        count = sqlite3_column_int(stmt, 0);
    }
    else if (rc != SQLITE_DONE)
    {
        cerr << "an error ocurred while counting records: " << rc << endl;
        cerr << "**" << sqlite3_errstr(rc) << endl;
    }
    else
    {
        cout << "empty database" << endl;
    }

    sqlite3_finalize(stmt);
    return count;
}

void list_records(sqlite3 *db, string search, int offset)
{
    sqlite3_stmt *stmt;
    const char *query =
        "SELECT `id`, `accountNum`, `firstname`, `lastname`, `balance` FROM `records` "
        "WHERE `firstname` LIKE ? OR `lastname` LIKE ? LIMIT ? OFFSET ?";

    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "failed to prepare query: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        sqlite3_finalize(stmt);
        return;
    }

    int rc;
    string searchParam = "%" + search + "%";
    sqlite3_bind_text(stmt, 1, searchParam.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, searchParam.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, pageSize);
    sqlite3_bind_int(stmt, 4, offset);

    if (rc != SQLITE_OK)
    {
        cerr << "failed to bind parameter: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        sqlite3_finalize(stmt);
        return;
    }

    int id = 0;
    const unsigned char *accountNum = 0;
    const unsigned char *firstname = 0;
    double balance = 0;

    cout << "account numbers "
         << ", page " << (offset / pageSize) + 1 << endl;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        id = sqlite3_column_int(stmt, 0);
        accountNum = sqlite3_column_text(stmt, 1);
        balance = sqlite3_column_double(stmt, 4);
        cout << "" << id << ". " << accountNum << endl;
    }

    sqlite3_finalize(stmt);
}

bool valid_record(sqlite3 *db, string accountNum)
{
    sqlite3_stmt *stmt;
    const char *query = "SELECT * FROM `records` WHERE accountNum=?";
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "failed to prepare query: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        sqlite3_finalize(stmt);
        return false;
    }

    int rc;
    rc = sqlite3_bind_text(stmt, 1, accountNum.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
        cerr << "failed to bind parameter: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        sqlite3_finalize(stmt);
        return false;
    }

    auto valid = false;
    rc = sqlite3_step(stmt);
    switch (rc)
    {
    case SQLITE_ROW:
        valid = true;
        break;
    case SQLITE_DONE:
        break;
    default:
        cerr << "an error ocurred while searching database: " << rc << endl;
        cerr << "**" << sqlite3_errstr(rc) << endl;
    }

    sqlite3_finalize(stmt);
    return valid;
}
void search_record(sqlite3 *db, string accountNum)
{
    sqlite3_stmt *stmt;
    const char *query = "SELECT `accountNum`, `firstname`, `lastname`, `balance` FROM `records` WHERE accountNum=?";
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "failed to prepare query: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        sqlite3_finalize(stmt);
        return;
    }

    int rc;
    rc = sqlite3_bind_text(stmt, 1, accountNum.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
        cerr << "failed to bind parameter: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        sqlite3_finalize(stmt);
        return;
    }

    cout << "searching for " << accountNum << endl;
    const unsigned char *firstname = 0;
    const unsigned char *lastname = 0;
    double balance = 0;

    rc = sqlite3_step(stmt);
    switch (rc)
    {
    case SQLITE_ROW:
        firstname = sqlite3_column_text(stmt, 1);
        lastname = sqlite3_column_text(stmt, 2);
        balance = sqlite3_column_double(stmt, 3);
        cout << "# account details:" << endl;
        cout << "firstname: " << firstname << endl;
        cout << "lastname: " << lastname << endl;
        cout << "balance: " << balance << endl;
        break;
    case SQLITE_DONE:
        cout << "No matching record found." << endl;
        break;
    default:
        cerr << "an error ocurred while searching database: " << rc << endl;
        cerr << "**" << sqlite3_errstr(rc) << endl;
    }

    sqlite3_finalize(stmt);
}

void delete_record(sqlite3 *db, string accountNum)
{
    sqlite3_stmt *stmt;
    const char *query = "DELETE FROM `records` WHERE accountNum=?";
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "failed to prepare query: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_bind_text(stmt, 1, accountNum.c_str(), -1, SQLITE_STATIC);
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        cerr << "an error ocurred while deleting a record: " << rc << endl;
        cerr << "**" << sqlite3_errstr(rc) << endl;
    }
    else
    {
        cout << "record deleted: " << accountNum << endl;
    }

    sqlite3_finalize(stmt);
}

void update_record(sqlite3 *db, Record *user)
{
    sqlite3_stmt *stmt;
    const char *query = R"(
        UPDATE `records` SET `firstname`=?, `lastname`=?, `balance`=?
        WHERE `accountNum` = ?
    )";
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "failed to prepare query: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        sqlite3_finalize(stmt);
        return;
    }

    int rc;
    sqlite3_bind_text(stmt, 1, user->firstname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user->lastname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, user->balance);
    sqlite3_bind_text(stmt, 4, user->accountNum.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    switch (rc)
    {
    case SQLITE_DONE:
        cerr << "account successfully updated: " << user->accountNum << endl;
        break;
    default:
        cerr << "an error ocurred while inserting into database: " << rc << endl;
        cerr << "**" << sqlite3_errstr(rc) << endl;
        break;
    }

    sqlite3_finalize(stmt);
}

int main()
{
    sqlite3 *db;
    if (sqlite3_open("bank.db", &db) != SQLITE_OK)
    {
        cerr << "nope, failed to open file";
        return 2;
    }

    char *errMessage = 0;
    auto query = R"(
            CREATE TABLE IF NOT EXISTS `records` (
               `id` INTEGER PRIMARY KEY,
               `accountNum` VARCHAR(128) UNIQUE,
               `firstname` VARCHAR(128) NOT NULL,
               `lastname` VARCHAR(128) NOT NULL,
               `balance` REAL NOT NULL
            )
        )";
    auto rc = sqlite3_exec(db, query, nullptr, 0, &errMessage);

    if (rc)
    {
        cerr << "error while creating table: " << errMessage << endl;
    }

    cout << "Welcome!\n";
    for (;;)
    {
        string choice;
        cout << "\n"
                "[1]Create "
                "[2]Browse "
                "[3]Info\n"
                "[4]Edit "
                "[5]Delete "
                "[6]Quit\n";
        cout << ">> ";

        getline(cin, choice);

        if (choice == "1")
        {
            cout << "creating new account" << endl;
            Record user;
            bool inputValid;
            tie(user, inputValid) = read_data();
            if (inputValid)
            {
                insert_record(db, user);
            }
        }
        else if (choice == "2")
        {
            int offset = 0;
            string filter = "";
            auto total = count_records(db);
            cout << "total:" << total << endl;
            while (true)
            {
                list_records(db, "", offset);
                offset += pageSize;

                if (offset > total)
                {
                    break;
                }

                cout << "<enter> - next, q - done" << endl;
                getline(cin, choice);
                if (choice == "q")
                {
                    break;
                }
            }
        }
        else if (choice == "3")
        {
            string accountNum;
            cout << "enter account number: ";
            getline(cin, accountNum);
            search_record(db, accountNum);
        }
        else if (choice == "4")
        {
            string accountNum;
            cout << "enter account number: ";
            getline(cin, accountNum);
            if (!valid_record(db, accountNum))
            {
                cout << "invalid account number" << endl;
            }
            else
            {
                Record user;
                bool inputValid;
                tie(user, inputValid) = read_data(false);
                if (inputValid)
                {
                    update_record(db, &user);
                }
            }
        }
        else if (choice == "5")
        {
            string accountNum;
            cout << "enter account number: ";
            getline(cin, accountNum);
            if (!valid_record(db, accountNum))
            {
                cout << "invalid account number" << endl;
            }
            else
            {
                delete_record(db, accountNum);
            }
        }
        else if (choice == "6" || cin.eof())
        {
            break;
        }
        else
        {
            cout << "huh." << endl;
        }
    }

    sqlite3_close(db);
    return 0;
}
