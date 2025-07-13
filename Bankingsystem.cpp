#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include<fstream>
#include <unordered_map>
using namespace std;

class Account {
public:
    string name;
    string mobile;
    string address;
    string accountType;
    int accountNumber;
    string password;
    float balance;

    Account() {
        balance = 0.0;
    }

    void create(int accNo) {
        accountNumber = accNo;
        cin.ignore();  // Clear input buffer

        cout << "Enter Full Name: ";
        getline(cin, name);

        cout << "Enter Mobile Number: ";
        getline(cin, mobile);

        cout << "Enter Address: ";
        getline(cin, address);

        cout << "Enter Account Type (Saving/Current): ";
        getline(cin, accountType);

        cout << "Create Password: ";
        getline(cin, password);

        balance = 0.0;

        cout << "\n Your Account created successfully....!" << endl;
        viewAccount();
    }

    void viewAccount() {
        cout << "\n***------ Account Details ------***" << endl;
        cout << "Name         : " << name << endl;
        cout << "Mobile No.   : " << mobile << endl;
        cout << "Address      : " << address << endl;
        cout << "Account Type : " << accountType << endl;
        cout << "Account No.  : " << accountNumber << endl;
        cout << "Balance      : " << balance << endl;
    }

    void deposit(float amount) {
        if (amount > 0) {
            balance += amount;
            cout << " " << amount << "Money deposited successfully....!" << endl;
        } else {
            cout << " Invalid deposit amount!" << endl;
        }
    }

    void withdraw(float amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << " " << amount << "Money withdrawn successfully....!" << endl;
        } else {
            cout << " Insufficient balance or invalid amount....!" << endl;
        }
    }

    bool transfer(Account &receiver, float amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            receiver.balance += amount;
            return true;
        }
        return false;
    }
};

// Store accounts in global map
unordered_map<int, Account> accounts;

// Get account by number
Account* getAccount(int accNo) {
    if (accounts.find(accNo) != accounts.end()) {
        return &accounts[accNo];
    }
    return nullptr;
}

// 🔐 Check user login before operations
Account* check_user() {
    int accNo;
    string pass;

    cout << "\nEnter Account Number: ";
    cin >> accNo;
    cin.ignore(); // clear newline

    if (accounts.find(accNo) == accounts.end()) {
        cout << " Account not found....!" << endl;
        return nullptr;
    }

    cout << "Enter Password: ";
    getline(cin, pass);

    if (accounts[accNo].password == pass) {
        return &accounts[accNo];
    } else {
        cout << " Incorrect password....!" << endl;
        return nullptr;
    }
}
int loadLastAccountNumber() {
    ifstream infile("acc_number.txt");
    int lastAccNo = 1000;  // Start from 1001 if file doesn't exist
    if (infile >> lastAccNo) {
        return lastAccNo;
    }
    return 1000;
}

void saveLastAccountNumber(int lastAccNo) {
    ofstream outfile("acc_number.txt");
    outfile << lastAccNo;
}


int main() {
    int choice;
   int accNumberCounter = loadLastAccountNumber();


    cout << " Hi, how can I help you?\n";

    do {
        cout << "\n========  BANK MENU =========" << endl;
        cout << "1. Create Account" << endl;
        cout << "2. View Account" << endl;
        cout << "3. Deposit Money" << endl;
        cout << "4. Withdraw Money" << endl;
        cout << "5. Transfer Money" << endl;
        cout << "6. Exit" << endl;
        cout << "============================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout<<"_____Creating Bank Account_____"<<endl;
            Account newAcc;
            newAcc.create(accNumberCounter);
            accounts[accNumberCounter] = newAcc;
            saveLastAccountNumber(accNumberCounter); // Save updated number
            break;
        }

        case 2: {
            cout<<"_____View Account Information_____"<<endl;
            Account* acc = check_user();
            if (acc) acc->viewAccount();
            break;
        }

        case 3: {
            cout<<"_____Deposit Money_____"<<endl;
            Account* acc = check_user();
            if (acc) {
                float amt;
                cout << "Enter amount to deposit: ";
                cin >> amt;
                acc->deposit(amt);
            }
            break;
        }

        case 4: {
            cout<<"_____Withdraw Money_____"<<endl;
            Account* acc = check_user();
            if (acc) {
                float amt;
                cout << "Enter amount to withdraw: ";
                cin >> amt;
                acc->withdraw(amt);
            }
            break;
        }

        case 5: {
            cout<<"_____Transafer Money______"<<endl;
            Account* sender = check_user();
            if (!sender) break;

            int toAcc;
            cout << "Enter Receiver's Account Number: ";
            cin >> toAcc;

            Account* receiver = getAccount(toAcc);
            if (!receiver) {
                cout << " Receiver account not found!" << endl;
                break;
            }

            float amt;
            cout << "Enter amount to transfer: ";
            cin >> amt;

            if (sender->transfer(*receiver, amt)) {
                cout << " " << amt << " transferred successfully!" << endl;
            } else {
                cout << " Transfer failed. Insufficient balance or invalid amount!" << endl;
            }
            break;
        }

        case 6:
            cout << " Thank you for using our banking system. Goodbye!\n";
            break;

        default:
            cout << " Invalid choice. Please try again.\n";
        }

    } while (choice != 6);

    return 0;
}
