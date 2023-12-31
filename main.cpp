#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>

using namespace std;

class BankAccount {
    int account_number;
    char name[50];
    char type;
    int money_deposit;

public:
    int retacno() const {
        return account_number;
    }
    void create_account();
    void display_account();
    void report() const;
    void deposit(int amount);
    void withdraw(int amount);
    void modify_account();
};

void BankAccount::report() const {
    cout << account_number << setw(10) << " " << name << setw(6) << type << setw(6) << money_deposit << endl;
}

void display_all() {
    BankAccount ac;
    ifstream infile;
    infile.open("Account.dat", ios::binary);
    if (!infile) {
        cout << "File could not be opened. Press any key..";
        return;
    }
    cout << "\t Bank Account Holder List" << endl;
    cout << "\t  ***************************************" << endl;
    cout << "A/C NO.         NAME       TYPE    BALANCE" << endl;
    cout << "\t  ***************************************" << endl;
    while (infile.read(reinterpret_cast<char *>(&ac), sizeof(BankAccount))) {
        ac.report();
    }
}

void BankAccount::deposit(int amount) {
    money_deposit += amount;
    cout << "\tAmount deposited successfully." << endl;
}

void BankAccount::withdraw(int amount) {
    if (money_deposit >= amount) {
        money_deposit -= amount;
        cout << "\tAmount withdrawn successfully." << endl;
    } else {
        cout << "\tInsufficient funds." << endl;
    }
}

void BankAccount::modify_account() {
    cout << "\tModify Menu:" << endl;
    cout << "\t1. Modify Name" << endl;
    cout << "\t2. Modify Type" << endl;
    cout << "\t3. Modify Initial Deposit" << endl;
    cout << "\tEnter your choice (1-3): ";

    int choice;
    cin >> choice;

    switch (choice) {
        case 1:
            cout << "\tEnter new name: ";
            cin.ignore();
            cin.getline(name, 50);
            cout << "\tName modified successfully." << endl;
            break;
        case 2:
            cout << "\tEnter new type (C/S): ";
            cin >> type;
            type = toupper(type);
            cout << "\tType modified successfully." << endl;
            break;
        case 3:
            cout << "\tEnter new initial deposit: ";
            cin >> money_deposit;
            cout << "\tInitial deposit modified successfully." << endl;
            break;
        default:
            cout << "\tInvalid choice." << endl;
            break;
    }
}

void write_account() {
    BankAccount ac;
    ofstream outfile;
    outfile.open("Account.dat", ios::binary | ios::app);
    ac.create_account();
    outfile.write(reinterpret_cast<char *>(&ac), sizeof(BankAccount));
    outfile.close();
}

void delete_account(int n) {
    BankAccount ac;
    ifstream infile;
    ofstream outfile;
    infile.open("Account.dat", ios::binary);
    if (!infile) {
        cout << "File could not be opened. Press any key.";
        return;
    }
    outfile.open("Temp.dat", ios::binary);
    infile.seekg(0, ios::beg);

    while (infile.read(reinterpret_cast<char *>(&ac), sizeof(BankAccount))) {
        if (ac.retacno() != n) {
            outfile.write(reinterpret_cast<char *>(&ac), sizeof(BankAccount));
        }
    }

    infile.close();
    outfile.close();
    remove("Account.dat");
    rename("Temp.dat", "Account.dat");
    cout << "\tRecord deleted..." << endl;
}

void display_details(int n) {
    BankAccount ac;
    bool flag = false;
    ifstream infile;
    infile.open("Account.dat", ios::binary);
    if (!infile) {
        cout << "File could not be opened. Press any key." << endl;
        return;
    }
    cout << "\tBank Account Details" << endl;
    while (infile.read(reinterpret_cast<char *>(&ac), sizeof(BankAccount))) {
        if (ac.retacno() == n) {
            ac.display_account();
            flag = true;
        }
    }

    infile.close();
    if (!flag) {
        cout << "\tAccount number does not exist" << endl;
    }
}

void BankAccount::create_account() {
    cout << "Enter the account number: ";
    cin >> account_number;
    cout << "Enter the name of the account holder: ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "\tEnter the type of the account (C/S): ";
    cin >> type;
    type = toupper(type);
    cout << "\tEnter the initial amount (>=500 for saving and >= 1000 for current): ";
    cin >> money_deposit;
    cout << endl;
    cout << "\tAccount created...:)" << endl;
    cout << endl;
}

void BankAccount::display_account() {
    cout << "\tBank Account Number: " << account_number << endl;
    cout << "\tAccount Holder Name: " << name << endl;
    cout << "\tBalance Amount: " << money_deposit << endl;
}

int main() {
    int ch;
    int num;

    do {
        cout << "\t\t..............................." << endl;
        cout << "\t\tWelcome to the Bank Management System" << endl;
        cout << "\t\t..............................." << endl;
        cout << endl;
        cout << "\t--Main Menu---" << endl;
        cout << "\t1. Create Account" << endl;
        cout << "\t2. Deposit Money" << endl;
        cout << "\t3. Withdraw Money" << endl;
        cout << "\t4. Balance Enquiry" << endl;
        cout << "\t5. All Account Holder List" << endl;
        cout << "\t6. Close an Account" << endl;
        cout << "\t7. Modify an Account" << endl;
        cout << "\t8. Exit" << endl;
        cout << endl;
        cout << "\tEnter your choice (1-8): ";
        cin >> ch;

        switch (ch) {
            case 1:
                write_account();
                break;
            case 2:
                cout << "\tEnter the account number: ";
                cin >> num;
                {
                    BankAccount ac;
                    int amount;
                    cout << "\tEnter the amount to deposit: ";
                    cin >> amount;
                    while (amount < 0) {
                        cout << "\tPlease enter a valid amount: ";
                        cin >> amount;
                    }
                    if (amount > 0) {
                        ac.deposit(amount);
                        fstream file;
                        file.open("Account.dat", ios::binary | ios::in | ios::out);
                        while (file.read(reinterpret_cast<char*>(&ac), sizeof(BankAccount))) {
                            if (ac.retacno() == num) {
                                int pos = file.tellg();
                                file.seekp(pos - static_cast<int>(sizeof(BankAccount)), ios::beg);
                                file.write(reinterpret_cast<char*>(&ac), sizeof(BankAccount));
                                break;
                            }
                        }
                        file.close();
                    }
                }
                break;
            case 3:
                cout << "\tEnter the account number: ";
                cin >> num;
                {
                    BankAccount ac;
                    int amount;
                    cout << "\tEnter the amount to withdraw: ";
                    cin >> amount;
                    while (amount < 0) {
                        cout << "\tPlease enter a valid amount: ";
                        cin >> amount;
                    }
                    if (amount > 0) {
                        ac.withdraw(amount);
                        fstream file;
                        file.open("Account.dat", ios::binary | ios::in | ios::out);
                        while (file.read(reinterpret_cast<char*>(&ac), sizeof(BankAccount))) {
                            if (ac.retacno() == num) {
                                int pos = file.tellg();
                                file.seekp(pos - static_cast<int>(sizeof(BankAccount)), ios::beg);
                                file.write(reinterpret_cast<char*>(&ac), sizeof(BankAccount));
                                break;
                            }
                        }
                        file.close();
                    }
                }
                break;
            case 4:
                cout << "\tEnter the account number: ";
                cin >> num;
                display_details(num);
                break;
            case 5:
                display_all();
                break;
            case 6:
                cout << "\tEnter the account number: ";
                cin >> num;
                delete_account(num);
                break;
            case 7:
                cout << "\tEnter the account number: ";
                cin >> num;
                {
                    BankAccount ac;
                    fstream file;
                    file.open("Account.dat", ios::binary | ios::in | ios::out);
                    if (!file) {
                        cout << "File could not be opened. Press any key." << endl;
                        return 0;
                    }
                    while (file.read(reinterpret_cast<char*>(&ac), sizeof(BankAccount))) {
                        if (ac.retacno() == num) {
                            ac.modify_account();
                            int pos = file.tellg();
                            file.seekp(pos - static_cast<int>(sizeof(BankAccount)), ios::beg);
                            file.write(reinterpret_cast<char*>(&ac), sizeof(BankAccount));
                            break;
                        }
                    }
                    file.close();
                }
                break;
            case 8:
                cout << "\tThank you for using Bank Management System" << endl;
                break;
            default:
                cout << "\tInvalid choice. Please enter a number between 1 and 8." << endl;
        }

        cin.ignore();
        cin.get();

    } while (ch != 8);

    return 0;
}
