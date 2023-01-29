/*
    File name: main.cpp
    Author: Aman Khadka
    Assignment: Final Project
    Date created : 01/25/2023
    Copyright (C) 2023 Aman Khadka
    C++ Version: C++17

    Usage:
        - include the json.hpp file from the nlohmann/json repository
        - g++ main.cpp -o main.exe; start-process main.exe

    Description: A simple implementation of a stock analyzer program that uses a linked list to store the stock data and a JSON file to store the client data.
*/

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>
#include <fstream>
#include <random>
#include <math.h>
#include <algorithm>
#include <vector> 

#include "includes/extra.hpp"
#include "includes/json.hpp"

using namespace std;
using json = nlohmann::json;

// Define the Client class
class Client
{
private:
    string name;
    int id;
    string address;
    string dob;

public:
    Client(string name, int id, string address, string dob) : name(name), id(id), address(address), dob(dob) {}

    string getName() { return name; }
    int getId() { return id; }
    string getAddress() { return address; }
    string getDoB() { return dob; }

    void setName(string name) { this->name = name; }
    void setId(int id) { this->id = id; }
    void setAddress(string address) { this->address = address; }
    void setDoB(string dob) { this->dob = dob; }
};

// Define the StockList class
class StockList
{
private:
    int stockId;
    string stockName;
    float marketPrice;

public:
    StockList(int stockId, string stockName, float marketPrice) : stockId(stockId), stockName(stockName), marketPrice(marketPrice) {}

    int getId() { return stockId; }
    string getStockName() { return stockName; }
    float getMarketPrice() { return marketPrice; }

    void setStockId(int stockId) { this->stockId = stockId; }
    void setStockName(string stockName) { this->stockName = stockName; }
    void setMarketPrice(float marketPrice) { this->marketPrice = marketPrice; }
};

// Define the LinkedList class
template <typename T>
class LinkedList
{
private:
    // Define the Node struct
    struct Node
    {
        T data;
        Node *next;

        Node(T data) : data(data), next(nullptr) {}
    };

    Node *head;
    int size;

public:
    LinkedList() : head(nullptr), size(0) {}

    ~LinkedList()
    {
        // Delete all nodes
        Node *current = head;
        while (current != nullptr)
        {
            Node *next = current->next;
            delete current;
            current = next;
        }
    }

    void insert(T data)
    {
        // Create a new node
        Node *newNode = new Node(data);

        // Insert the new node at the front of the list
        newNode->next = head;
        head = newNode;

        // Increment the size of the list
        size++;
    }

    void remove(int id)
    {
        if (head == nullptr)
        {
            return;
        }

        Node *current = head;
        Node *prev = nullptr;

        while (current != nullptr)
        {
            if (current->data.getId() == id)
            {
                if (prev == nullptr)
                {
                    // Removing the head node
                    head = current->next;
                }
                else
                {
                    prev->next = current->next;
                }
                delete current;
                size--;
                return;
            }
            prev = current;
            current = current->next;
        }
    }
};

class Console
{
public:
    // This function moves the console cursor to the specified position
    void gotoxy(int x, int y)
    {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    // This function draws a line of the specified length and unicode escape sequence
    void design(int x, string y)
    {
        for (int i = 0; i < x; i++)
            cout << y;
    }

    void border()
    {
        for (int i = 0; i < 108; i++)
        {
            gotoxy(i + 6, 2);
            cout << "\u2580";
            gotoxy(i + 6, 26);
            cout << "\u2584";
        }
        for (int j = 0; j < 25; j++)
        {
            gotoxy(6, j + 2);
            cout << "\u2588";
            gotoxy(113, j + 2);
            cout << "\u2588";
        }
    }
};

class Login
{
protected:
    string id;
    string password;
    string loginType; // new member variable to store the login type (admin or client)
public:
    string getID() { return id; }
    void setID(const string &newId) { id = newId; }

    string getPassword() { return password; }
    void setPassword(const string &newPassword) { password = newPassword; }

    string getLoginType() { return loginType; }
    void setLoginType(const string &newLoginType) { loginType = newLoginType; }

    bool verify()
    {
        Console c;

        // Read the login credentials from the JSON file
        ifstream loginFile("configuration/login_credentials.json");
        json loginData;
        loginFile >> loginData;

        // Check if the login is successful
        if (loginData[loginType]["username"] == id && loginData[loginType]["password"] == password)
        {
            // Login successful
            c.gotoxy(35, 18);
            c.design(48, "\u2500");
            c.gotoxy(38, 17);
            cout << "You Have Successfully Logged In : " << id;
            time_t t;
            time(&t);
            c.gotoxy(39, 19);
            cout << "Logged In Time : " << ctime(&t);
            c.gotoxy(44, 23);
            cout << "Press any key to continue .... ";
            getch();
            return true;
        }
        else
        {
            // Incorrect login credentials
            return false;
        }
    }
    virtual void login() = 0;
    virtual void logout() = 0;
};

class NepalStockAnalyzer
{
    LinkedList<Client> clients;
    LinkedList<StockList> stockLists;
    Console c;
    Login *currentUser; // add a member variable to store the current user
public:
    void displayMenu();
    void displayMenuClient();

    void addRecords();
    void removeRecords();
    void displayAllRecords();

    void updateStockPrices();
    void depositMoney();
    void purchaseStock();
    void sellStock();

    void displayTransactions();
    void buyHistory(int, int, string, int, float, float, string);
    void sellHistory(int, int, string, int, float, float, string);

    void displayClientPortfolio();

    void getChoice();
    void getChoiceClient();

    // add setter and getter functions for the current user
    void setCurrentUser(Login *user) { currentUser = user; }
    Login *getCurrentUser() { return currentUser; }
};

class ClientLogin : public Login
{
    Console c;

public:
    void login();
    void logout();
};

class AdminLogin : public Login
{
    Console c;

public:
    void login();
    void logout();
};

void ClientLogin::login()
{
    c.gotoxy(28, 4);
    c.design(15, "\u2592");
    cout << " WELCOME TO NEPAL STOCK ANALYZER ";
    c.design(15, "\u2592");
    c.border();
    c.gotoxy(18, 10);
    cout << "Enter The Client Username : ";
    string newId;
    cin >> newId;
    setID(newId);
    setLoginType("client"); // set the login type to client
    c.gotoxy(18, 12);
    cout << "Enter The Client Password : ";
    string newPassword;
    // Capture the password input from the user
    char ch;
    while ((ch = getch()) != 13)
    {
        newPassword += ch;
        cout << "*";
    }
    setPassword(newPassword); // Check if the login is successful
    if (verify())
    {
        NepalStockAnalyzer nsa;
        nsa.setCurrentUser(this); // set the current user to the client
        nsa.displayMenuClient();
    }
    else
    {
        system("CLS");
        c.gotoxy(36, 18);
        c.design(48, "\u2500");
        c.gotoxy(43, 17);
        cout << "Incorrect Username / Password !!!!" << endl;
        login();
    }
}

void ClientLogin::logout()
{
    system("cls");
    c.gotoxy(30, 4);
    c.design(25, "\u2592");
    cout << " THANK YOU ";
    c.design(25, "\u2592");
    c.border();
    c.gotoxy(40, 12);
    cout << "USER            :: " << id;
    time_t t;
    time(&t);
    c.gotoxy(40, 14);
    cout << "Logged Out Time :: " << ctime(&t);
    fflush(stdin);
    getch();
    c.gotoxy(0, 26);
    exit(0);
}

void AdminLogin::login()
{
    Console c;
    c.gotoxy(28, 4);
    c.design(15, "\u2592");
    cout << " WELCOME TO NEPAL STOCK ANALYZER ";
    c.design(15, "\u2592");
    c.border();
    c.gotoxy(18, 10);
    cout << "Enter The Admin Username : ";
    string newId;
    cin >> newId;
    setID(newId);
    setLoginType("admin"); // set the login type to admin
    c.gotoxy(18, 12);
    cout << "Enter The Admin Password : ";
    string newPassword;
    // Capture the password input from the user
    char ch;
    while ((ch = getch()) != 13)
    {
        newPassword += ch;
        cout << "*";
    }
    setPassword(newPassword);

    // Check if the login is successful
    if (verify())
    {
        NepalStockAnalyzer nsa;
        nsa.setCurrentUser(this); // set the current user to the admin
        nsa.displayMenu();
    }
    else
    {
        system("CLS");
        c.gotoxy(36, 18);
        c.design(48, "\u2500");
        c.gotoxy(43, 17);
        cout << "Incorrect Username / Password !!!!" << endl;
        login();
    }
}

void AdminLogin::logout()
{
    system("cls");
    c.gotoxy(30, 4);
    c.design(25, "\u2592");
    cout << " THANK YOU ";
    c.design(25, "\u2592");
    c.border();
    c.gotoxy(40, 12);
    cout << "USER            :: " << id;
    time_t t;
    time(&t);
    c.gotoxy(40, 14);
    cout << "Logged Out Time :: " << ctime(&t);
    fflush(stdin);
    getch();
    c.gotoxy(0, 26);
    exit(0);
}

class StockAnalyzer : public Console, public ClientLogin, public AdminLogin
{
public:
    void displayMenu()
    {
        gotoxy(28, 4);
        design(15, "\u2592");
        cout << " WELCOME TO NEPAL STOCK ANALYZER ";
        design(15, "\u2592");
        border();
        gotoxy(54, 8);
        cout << "ACCOUNT TYPE";
        gotoxy(44, 12);
        cout << "[1] . ADMINISTRATOR ";
        gotoxy(44, 14);
        cout << "[2] . CLIENT ";
        gotoxy(44, 18);
        cout << "Enter Your Choice .... ";
        char choice;
        cin >> choice;
        if (choice != '1' && choice != '2')
        {
            system("CLS");
            displayMenu();
        }
        system("CLS");

        if (choice == '1')
        {
            gotoxy(28, 4);
            design(15, "\u2592");
            cout << " WELCOME TO NEPAL STOCK ANALYZER ";
            design(15, "\u2592");
            AdminLogin::login();
        }
        else if (choice == '2')
        {
            gotoxy(28, 4);
            design(15, "\u2592");
            cout << " WELCOME TO NEPAL STOCK ANALYZER ";
            design(15, "\u2592");
            ClientLogin::login();
        }
        else
        {
            cout << "Invalid choice. Please try again.";
            displayMenu();
        }
    }
};

void NepalStockAnalyzer::displayMenu()
{
    system("CLS");
    c.gotoxy(29, 4);
    c.design(20, "\u2592");
    cout << " WELCOME TO MAIN MENU ";
    c.design(20, "\u2592");
    c.border();
    c.gotoxy(49, 8);
    cout << "[1] . View all records" << endl;
    c.gotoxy(49, 9);
    cout << "[2] . Add records" << endl;
    c.gotoxy(49, 10);
    cout << "[3] . Delete records" << endl;
    c.gotoxy(49, 11);
    cout << "[4] . Deposit Money" << endl;
    c.gotoxy(49, 12);
    cout << "[5] . Buy Stock" << endl;
    c.gotoxy(49, 13);
    cout << "[6] . Sell Stock" << endl;
    c.gotoxy(49, 14);
    cout << "[7] . View Transactions" << endl;
    c.gotoxy(49, 15);
    cout << "[8] . View Portfolio" << endl;
    c.gotoxy(49, 17);
    cout << "[9] . Log Out !!!" << endl;
    c.gotoxy(45, 22);
    cout << "Please Enter Your Choice [1-9] : ";
    getChoice();
}

void NepalStockAnalyzer::displayMenuClient()
{
    system("CLS");
    c.gotoxy(29, 4);
    c.design(20, "\u2592");
    cout << " WELCOME TO MAIN MENU ";
    c.design(20, "\u2592");
    c.border();
    c.gotoxy(49, 10);
    cout << "[1] . Deposit Money" << endl;
    c.gotoxy(49, 11);
    cout << "[2] . Buy Stock" << endl;
    c.gotoxy(49, 12);
    cout << "[3] . Sell Stock" << endl;
    c.gotoxy(49, 13);
    cout << "[4] . View Transactions" << endl;
    c.gotoxy(49, 14);
    cout << "[5] . View Portfolio" << endl;
    c.gotoxy(49, 15);
    cout << "[6] . Log Out !!!" << endl;
    c.gotoxy(45, 22);
    cout << "Please Enter Your Choice [1-6] : ";
    getChoiceClient();
}

// a set of ants to represent menu options that the user can choose from
enum MenuOption
{
    DISPLAY_ALL_RECORDS = '1',
    ADD_RECORDS,
    REMOVE_RECORDS,
    DEPOSIT_MONEY,
    PURCHASE_STOCK,
    SELL_STOCK,
    DISPLAY_TRANSACTIONS,
    DISPLAY_CLIENT_PORTFOLIO,
    QUIT
};

void NepalStockAnalyzer::getChoice()
{
    char choice;
    cin >> choice;
    system("CLS");
    switch (choice)
    {
    case DISPLAY_ALL_RECORDS:
    {
        displayAllRecords();
        break;
    }
    case ADD_RECORDS:
    {
        addRecords();
        break;
    }
    case REMOVE_RECORDS:
    {
        removeRecords();
        break;
    }
    case DEPOSIT_MONEY:
    {
        depositMoney();
        break;
    }
    case PURCHASE_STOCK:
    {
        purchaseStock();
        break;
    }
    case SELL_STOCK:
    {
        sellStock();
        break;
    }
    case DISPLAY_TRANSACTIONS:
    {
        displayTransactions();
        break;
    }
    case DISPLAY_CLIENT_PORTFOLIO:
    {
        displayClientPortfolio();
        break;
    }
    case QUIT:
    {
        // retrieve the current user
        Login *currentUser = getCurrentUser();
        // call the logout function on the current user
        currentUser->logout();
        return;
    }
    default:
    {
        displayMenu();
    }
    }
}

enum MenuOptionClient
{
    DEPOSIT_MONEY_C = '1',
    PURCHASE_STOCK_C,
    SELL_STOCK_C,
    DISPLAY_TRANSACTIONS_C,
    DISPLAY_CLIENT_PORTFOLIO_C,
    QUIT_C
};

void NepalStockAnalyzer::getChoiceClient()
{
    char choice;
    cin >> choice;
    system("CLS");
    switch (choice)
    {
    case DEPOSIT_MONEY_C:
    {
        depositMoney();
        break;
    }
    case PURCHASE_STOCK_C:
    {
        purchaseStock();
        break;
    }
    case SELL_STOCK_C:
    {
        sellStock();
        break;
    }
    case DISPLAY_TRANSACTIONS_C:
    {
        displayTransactions();
        break;
    }
    case DISPLAY_CLIENT_PORTFOLIO_C:
    {
        displayClientPortfolio();
        break;
    }
    case QUIT_C:
    {
        // retrieve the current user
        Login *currentUser = getCurrentUser();
        // call the logout function on the current user
        currentUser->logout();
        return;
    }
    default:
    {
        displayMenuClient();
    }
    }
}

void NepalStockAnalyzer::displayAllRecords()
{
    c.gotoxy(28, 4);
    c.design(25, "\u2592");
    cout << " DISPLAY MENU ";
    c.design(25, "\u2592");
    c.border();
    c.gotoxy(52, 10);
    cout << "[1] . Clients" << endl;
    c.gotoxy(52, 12);
    cout << "[2] . Stocks" << endl;
    c.gotoxy(52, 14);
    cout << "[3] . Removed Clients" << endl;
    c.gotoxy(52, 16);
    cout << "[4] . Removed Stocks" << endl;

    c.gotoxy(50, 21);
    cout << "Enter Your Choice [1-4] : ";

    int choice;
    cin >> choice;

    system("CLS");
    int i = 7;
    if (choice == 1)
    {
        // Load the data from the clients.json file and store it in the clients member variable
        json clientsJson;
        ifstream in("added_clients.json");
        if (in.good())
        {
            in >> clientsJson;
        }
        in.close();

        c.gotoxy(28, 3);
        c.design(25, "\u2592");
        cout << " CLIENTS LIST ";
        c.design(25, "\u2592");

        c.gotoxy(26, 5);
        cout << "S/N.";
        c.gotoxy(36, 5);
        cout << "Full Name";
        c.gotoxy(51, 5);
        cout << "UserID";
        c.gotoxy(64, 5);
        cout << "Address";
        c.gotoxy(85, 5);
        cout << "Birth Date";

        c.gotoxy(5, 6);
        c.design(109, "\u2550");
        for (auto &client : clientsJson)
        {
            string name = formatString(client["name"]);
            int id = client["id"];
            string address = formatString(client["address"]);
            string dob = formatString(client["dob"]);

            // automaticaly increment S.No. for each client when displaying the list
            int size = clientsJson.size();
            int sNo = 1;
            for (int i = 0; i < size; i++)
            {
                c.gotoxy(26, i + 7);
                cout << sNo << endl;
                sNo++;
            }
            c.gotoxy(36, i);
            cout << name << endl;
            c.gotoxy(51, i);
            cout << id << endl;
            c.gotoxy(64, i);
            cout << address << endl;
            c.gotoxy(85, i);
            cout << dob << endl;
            i++;
        }

        // this loop draws the top and bottom borders of the table
        for (int k = 0; k < 113; k++)
        {
            c.gotoxy(k + 3, 1);
            cout << "\u2580";
            c.gotoxy(k + 3, i + 2);
            cout << "\u2584";
        }

        // this loop draws the left and right borders of the table
        for (int j = 0; j < i + 2; j++)
        {
            c.gotoxy(3, j + 1);
            cout << "\u2588";
            c.gotoxy(116, j + 1);
            cout << "\u2588";
        }
        int x;
        c.gotoxy(42, i + 5);
        cout << "Press [Enter] to return back to main menu. ";
        x = getch();
        if (x == 13)
        {
            displayMenu();
        }
        else
        {
            system("CLS");
            displayAllRecords();
        }
    }
    else if (choice == 2)
    {
        // Load the data from the added_stocks.json file and store it in the stockLists member variable
        json stockListsJson;
        ifstream in("added_stocks.json");
        if (in.good())
        {
            in >> stockListsJson;
        }
        in.close();

        c.gotoxy(28, 3);
        c.design(25, "\u2592");
        cout << " STOCKS LIST ";
        c.design(25, "\u2592");

        c.gotoxy(34, 5);
        cout << "S/N.";
        c.gotoxy(45, 5);
        cout << "Stock Name";
        c.gotoxy(61, 5);
        cout << "Stock ID";
        c.gotoxy(73, 5);
        cout << "Market Price";

        c.gotoxy(5, 6);
        c.design(109, "\u2550");
        for (auto &stock : stockListsJson)
        {
            int stockId = stock["stockId"];
            string stockName = formatString(stock["stockName"]);
            float marketPrice = stock["marketPrice"];

            // automaticaly increment S.No. for each stock when displaying the list
            int size = stockListsJson.size();
            int sNo = 1;
            for (int i = 0; i < size; i++)
            {
                c.gotoxy(34, i + 7);
                cout << sNo << endl;
                sNo++;
            }
            c.gotoxy(45, i);
            cout << stockName << endl;
            c.gotoxy(61, i);
            cout << stockId << endl;
            c.gotoxy(73, i);
            cout << marketPrice << endl;
            i++;
        }

        // this loop draws the top and bottom borders of the table
        for (int k = 0; k < 113; k++)
        {
            c.gotoxy(k + 3, 1);
            cout << "\u2580";
            c.gotoxy(k + 3, i + 2);
            cout << "\u2584";
        }

        // this loop draws the left and right borders of the table
        for (int j = 0; j < i + 2; j++)
        {
            c.gotoxy(3, j + 1);
            cout << "\u2588";
            c.gotoxy(116, j + 1);
            cout << "\u2588";
        }

        int x;
        c.gotoxy(42, i + 5);
        cout << "Press [Enter] to return back to main menu. ";
        x = getch();
        if (x == 13)
        {
            displayMenu();
        }
        else
        {
            system("CLS");
            displayAllRecords();
        }
    }
    else if (choice == 3)
    {
        // Load the data from the removed_clients.json file and display it
        json clientsJson;
        ifstream in("removed_clients.json");
        if (in.good())
        {
            in >> clientsJson;
        }
        in.close();

        c.gotoxy(28, 3);
        c.design(25, "\u2592");
        cout << " REMOVED CLIENTS ";
        c.design(25, "\u2592");

        c.gotoxy(26, 5);
        cout << "S/N.";
        c.gotoxy(36, 5);
        cout << "Full Name";
        c.gotoxy(51, 5);
        cout << "UserID";
        c.gotoxy(64, 5);
        cout << "Address";
        c.gotoxy(85, 5);
        cout << "Date of Birth";

        c.gotoxy(5, 6);
        c.design(109, "\u2550");
        for (auto &client : clientsJson)
        {
            string name = formatString(client["name"]);
            int id = client["id"];
            string address = formatString(client["address"]);
            string dob = formatString(client["dob"]);

            // automaticaly increment S.No. for each client when displaying the list
            int size = clientsJson.size();
            int sNo = 1;
            for (int i = 0; i < size; i++)
            {
                c.gotoxy(26, i + 7);
                cout << sNo << endl;
                sNo++;
            }
            c.gotoxy(36, i);
            cout << name << endl;
            c.gotoxy(51, i);
            cout << id << endl;
            c.gotoxy(64, i);
            cout << address << endl;
            c.gotoxy(85, i);
            cout << dob << endl;
            i++;
        }

        // this loop draws the top and bottom borders of the table
        for (int k = 0; k < 113; k++)
        {
            c.gotoxy(k + 3, 1);
            cout << "\u2580";
            c.gotoxy(k + 3, i + 2);
            cout << "\u2584";
        }

        // this loop draws the left and right borders of the table
        for (int j = 0; j < i + 2; j++)
        {
            c.gotoxy(3, j + 1);
            cout << "\u2588";
            c.gotoxy(116, j + 1);
            cout << "\u2588";
        }

        int x;
        c.gotoxy(42, i + 5);
        cout << "Press [Enter] to return back to main menu. ";
        x = getch();
        if (x == 13)
        {
            displayMenu();
        }
        else
        {
            system("CLS");
            displayAllRecords();
        }
    }
    else if (choice == 4)
    {
        // Load the data from the removed_stocks.json file and display it
        json stockListsJson;
        ifstream in("removed_stocks.json");
        if (in.good())
        {
            in >> stockListsJson;
        }
        in.close();

        c.gotoxy(28, 3);
        c.design(25, "\u2592");
        cout << " REMOVED STOCKS ";
        c.design(25, "\u2592");

        c.gotoxy(34, 5);
        cout << "S/N.";
        c.gotoxy(45, 5);
        cout << "Stock Name";
        c.gotoxy(61, 5);
        cout << "Stock ID";
        c.gotoxy(73, 5);
        cout << "Market Price";

        c.gotoxy(5, 6);
        c.design(109, "\u2550");
        for (auto &stock : stockListsJson)
        {
            int stockId = stock["stockId"];
            string stockName = formatString(stock["stockName"]);
            float marketPrice = stock["marketPrice"];

            // automaticaly increment S.No. for each stock when displaying the list
            int size = stockListsJson.size();
            int sNo = 1;
            for (int i = 0; i < size; i++)
            {
                c.gotoxy(34, i + 7);
                cout << sNo << endl;
                sNo++;
            }

            c.gotoxy(45, i);
            cout << stockName << endl;
            c.gotoxy(61, i);
            cout << stockId << endl;
            c.gotoxy(73, i);
            cout << marketPrice << endl;
            i++;
        }

        // this loop draws the top and bottom borders of the table
        for (int k = 0; k < 113; k++)
        {
            c.gotoxy(k + 3, 1);
            cout << "\u2580";
            c.gotoxy(k + 3, i + 2);
            cout << "\u2584";
        }

        // this loop draws the left and right borders of the table
        for (int j = 0; j < i + 2; j++)
        {
            c.gotoxy(3, j + 1);
            cout << "\u2588";
            c.gotoxy(116, j + 1);
            cout << "\u2588";
        }

        int x;
        c.gotoxy(42, i + 5);
        cout << "Press [Enter] to return back to main menu. ";
        x = getch();
        if (x == 13)
        {
            displayMenu();
        }
        else
        {
            system("CLS");
            displayAllRecords();
        }
    }
    else
    {
        // Go back to the main menu
        displayMenu();
    }
}

void NepalStockAnalyzer::addRecords()
{
    system("CLS");
    c.gotoxy(26, 4);
    c.design(25, "\u2592");
    cout << " REGISTRATION MENU ";
    c.design(25, "\u2592");
    c.border();
    c.gotoxy(52, 10);
    cout << "[1] . Clients" << endl;
    c.gotoxy(52, 12);
    cout << "[2] . Stocks" << endl;

    c.gotoxy(50, 17);
    cout << "Enter Your Choice [1-2] : ";

    int choice;
    cin >> choice;

    system("CLS");
    if (choice == 1)
    {
        char ch;
        do
        {
            system("CLS");
            c.gotoxy(26, 4);
            c.design(20, "\u2592");
            cout << " CLIENT ACCOUNT REGISTRATION ";
            c.design(20, "\u2592");
            c.border();
            // Load the existing data from the JSON file
            json clientsJson;
            ifstream in("added_clients.json");
            if (in.good())
            {
                in >> clientsJson;
            }
            in.close();

            // Generate a random ID for the new client
            random_device rd;                       // Seed the generator
            mt19937 gen(rd());                      // Choose a generator
            uniform_int_distribution<> dis(1, 200); // Generate random numbers between 1 and 200
            int id = dis(gen);                      // Generate a random ID

            // Add a new client
            c.gotoxy(36, 9);
            cout << "[1] . Enter Client Name            : ";
            string name;
            cin.ignore();
            getline(cin, name);

            c.gotoxy(36, 11);
            cout << "[2] . Enter Client Address         : ";
            string address;
            // cin >> address;
            getline(cin, address);

            c.gotoxy(36, 13);
            cout << "[3] . Enter Client DOB (DD/MM/YYYY): ";
            string dob;
            // cin >> dob;
            getline(cin, dob);

            Client newClient(name, id, address, dob);
            clients.insert(newClient);

            // Update the data in memory
            clientsJson.push_back({{"name", name}, {"id", id}, {"address", address}, {"dob", dob}});

            // Save the updated data to the JSON file
            ofstream out("added_clients.json");
            out << clientsJson.dump(4);
            out.close();

            c.gotoxy(38, 17);
            c.design(45, "\u2500");
            c.gotoxy(40, 18);
            cout << "CLIENT ACCOUNT REGISTRATION SUCCESSFULL";
            c.gotoxy(38, 19);
            c.design(45, "\u2500");
            c.gotoxy(38, 21);
            cout << "Do You Want To Add Another Record ? (Y/N) : ";
            cin >> ch;
        } while (ch == 'y' || ch == 'Y');
        displayMenu();
    }
    else if (choice == 2)
    {
        char ch;
        do
        {
            system("CLS");
            c.gotoxy(30, 4);
            c.design(20, "\u2592");
            cout << " STOCK REGISTRATION ";
            c.design(20, "\u2592");
            c.border();
            // Load the existing data from the JSON file
            json stockListsJson;
            ifstream in("added_stocks.json");
            if (in.good())
            {
                in >> stockListsJson;
            }
            in.close();

            // Generate a random ID for the new client
            random_device rd;                       // Seed the generator
            mt19937 gen(rd());                      // Choose a generator
            uniform_int_distribution<> dis(1, 200); // Generate random numbers between 1 and 200
            int stockId = dis(gen);                 // Generate a random ID

            c.gotoxy(42, 10);
            cout << "[1] . Enter Stock Name        : ";
            string stockName;
            // cin >> stockName;
            cin.ignore();
            getline(cin, stockName);

            c.gotoxy(42, 12);
            cout << "[2] . Enter Stock Market Price: ";
            float marketPrice;
            cin >> marketPrice;

            StockList newStock(stockId, stockName, marketPrice);
            stockLists.insert(newStock);

            // Update the data in memory
            stockListsJson.push_back({{"stockId", stockId}, {"stockName", stockName}, {"marketPrice", marketPrice}});
            // Save the updated data to the JSON file
            ofstream out("added_stocks.json");
            out << stockListsJson.dump(3) << endl;
            out.close();

            c.gotoxy(38, 17);
            c.design(45, "\u2500");
            c.gotoxy(44, 18);
            cout << "STOCK REGISTRATION SUCCESSFULL";
            c.gotoxy(38, 19);
            c.design(45, "\u2500");
            c.gotoxy(38, 21);
            cout << "Do You Want To Add Another Record ? (Y/N) : ";
            cin >> ch;
        } while (ch == 'y' || ch == 'Y');
        displayMenu();
    }
    else
    {
        // Go back to the main menu
        displayMenu();
    }
}

void NepalStockAnalyzer::updateStockPrices()
{
    // Load the data from the added_stocks.json file
    json stockListsJson;
    ifstream in("added_stocks.json");
    if (in.good())
    {
        in >> stockListsJson;
    }
    in.close();

    // Iterate over the stocks in the JSON file
    for (auto &stock : stockListsJson)
    {
        // Generate a random fluctuation value
        float fluctuation = generateFluctuation(stock["marketPrice"]);

        // Update the market price of the stock
        float temp = stock["marketPrice"];
        temp += fluctuation;
        stock["marketPrice"] = temp;

        // Round the market price to the nearest 2 decimal places
        stock["marketPrice"] = floor(temp * 100 + 0.5) / 100;
    }

    // Save the updated data to the JSON file
    ofstream out("added_stocks.json");
    out << stockListsJson.dump(3) << endl;
    out.close();
}

void NepalStockAnalyzer::removeRecords()
{
    system("CLS");
    c.gotoxy(27, 4);
    c.design(25, "\u2592");
    cout << " REMOVE RECORDS ";
    c.design(25, "\u2592");
    c.border();
    c.gotoxy(52, 10);
    cout << "[1] . Clients" << endl;
    c.gotoxy(52, 12);
    cout << "[2] . Stocks" << endl;

    c.gotoxy(50, 17);
    cout << "Enter Your Choice [1-2] : ";

    int choice;
    cin >> choice;

    if (choice == 1)
    {
    re:
        system("CLS");
        c.gotoxy(24, 4);
        c.design(25, "\u2592");
        cout << " REMOVE CLIENT RECORD ";
        c.design(25, "\u2592");
        c.border();
        while (true)
        {
            // Load the data from the clients.json file
            json clientsJson;
            ifstream in("added_clients.json");
            if (in.good())
            {
                in >> clientsJson;
            }
            in.close();

            c.gotoxy(46, 10);
            cout << "Enter ID of Client to Remove: ";
            int id;
            cin >> id;

            // Remove the client from the linked list
            clients.remove(id);

            // Remove the client from the JSON object
            bool clientFound = false;
            for (auto it = clientsJson.begin(); it != clientsJson.end(); it++)
            {
                if (it->at("id") == id)
                {
                    string name = it->at("name");
                    string address = it->at("address");
                    string dob = it->at("dob");
                    clientsJson.erase(it);
                    clientFound = true;

                    // Save the updated data to the clients.json file
                    ofstream out("added_clients.json");
                    out << clientsJson.dump(4);
                    out.close();

                    // Load the data from the removed_clients.json file
                    json removedClientsJson;
                    ifstream in2("removed_clients.json");
                    if (in2.good())
                    {
                        in2 >> removedClientsJson;
                    }
                    in2.close();
                    // Add the removed client to the removed_clients.json file
                    removedClientsJson.push_back({{"name", name}, {"id", id}, {"address", address}, {"dob", dob}});
                    ofstream out2("removed_clients.json");
                    out2 << removedClientsJson.dump(4);
                    out2.close();

                    c.gotoxy(41, 14);
                    c.design(42, "\u2500");
                    c.gotoxy(48, 15);
                    cout << "CLIENT REMOVED SUCCESSFULLY";
                    c.gotoxy(41, 16);
                    c.design(42, "\u2500");
                    c.gotoxy(43, 18);
                    cout << "Want to remove another client? (Y/N) ";
                    char choice;
                    cin >> choice;
                    if (choice == 'Y' || choice == 'y')
                    {
                        goto re;
                    }
                    else
                    {
                        displayMenu();
                    }
                }
            }
            if (!clientFound)
            {
                c.gotoxy(41, 14);
                c.design(42, "\u2500");
                c.gotoxy(54, 15);
                cout << "CLIENT NOT FOUND";
                c.gotoxy(41, 16);
                c.design(42, "\u2500");
                c.gotoxy(33, 18);
                cout << "Press 'Y' to retry or any other key to return to main menu. ";
                char choice;
                cin >> choice;
                if (choice == 'Y' || choice == 'y')
                {
                    goto re;
                }
                else
                {
                    displayMenu();
                }
            }
        }
    }
    else if (choice == 2)
    {
    ye:
        system("CLS");
        c.gotoxy(24, 4);
        c.design(25, "\u2592");
        cout << " REMOVE STOCK RECORD ";
        c.design(25, "\u2592");
        c.border();
        while (true)
        {
            // Load the data from the added_stocks.json file
            json stockListsJson;
            ifstream in("added_stocks.json");
            if (in.good())
            {
                in >> stockListsJson;
            }
            in.close();

            c.gotoxy(46, 10);
            cout << "Enter ID of the Stock to Remove: ";
            int stockId;
            cin >> stockId;

            // Remove the stock from the linked list
            stockLists.remove(stockId);

            // Remove the stock from the JSON object
            bool found = false;
            for (auto it = stockListsJson.begin(); it != stockListsJson.end(); it++)
            {
                if (it->at("stockId") == stockId)
                {
                    found = true;
                    string stockName = it->at("stockName");
                    float marketPrice = it->at("marketPrice");
                    stockListsJson.erase(it);

                    // Save the updated data to the added_stocks.json file
                    ofstream out("added_stocks.json");
                    out << stockListsJson.dump(3) << endl;
                    out.close();

                    // Load the data from the removed_stocks.json file
                    json removedStockListsJson;
                    ifstream in2("removed_stocks.json");
                    if (in2.good())
                    {
                        in2 >> removedStockListsJson;
                    }
                    in2.close();
                    // Add the removed stock to the removed_stocks.json file
                    removedStockListsJson.push_back({{"stockId", stockId}, {"stockName", stockName}, {"marketPrice", marketPrice}});
                    ofstream out2("removed_stocks.json");
                    out2 << removedStockListsJson.dump(3) << endl;
                    out2.close();

                    c.gotoxy(41, 14);
                    c.design(42, "\u2500");
                    c.gotoxy(49, 15);
                    cout << "STOCK REMOVED SUCCESSFULLY";
                    c.gotoxy(41, 16);
                    c.design(42, "\u2500");
                    c.gotoxy(43, 18);
                    cout << "Want to remove another stock? (Y/N) ";
                    char choice;
                    cin >> choice;
                    if (choice == 'Y' || choice == 'y')
                    {
                        goto ye;
                    }
                    else
                    {
                        displayMenu();
                    }
                }
            }
            if (!found)
            {
                c.gotoxy(41, 14);
                c.design(42, "\u2500");
                c.gotoxy(54, 15);
                cout << "STOCK NOT FOUND";
                c.gotoxy(41, 16);
                c.design(42, "\u2500");
                c.gotoxy(33, 18);
                cout << "Press 'Y' to retry or any other key to return to main menu. ";
                char choice;
                cin >> choice;
                if (choice == 'Y' || choice == 'y')
                {
                    goto ye;
                }
                else
                {
                    displayMenu();
                }
            }
        }
    }
    else
    {
        // Go back to the main menu
        displayMenu();
    }
}

void NepalStockAnalyzer::depositMoney()
{
    while (true)
    {
        system("CLS");
        c.gotoxy(33, 4);
        c.design(20, "\u2592");
        cout << " CASH DEPOSIT ";
        c.design(20, "\u2592");
        c.border();
        c.gotoxy(46, 10);
        cout << "Enter the ID of the client: ";
        int id;
        cin >> id;

        // Load the data from the clients.json file
        json clientsJson;
        ifstream in("added_clients.json");
        if (in.good())
        {
            in >> clientsJson;
        }
        in.close();

        // Find the client with the specified ID
        bool clientFound = false;
        for (auto &client : clientsJson)
        {
            if (client["id"] == id)
            {
                clientFound = true;
                c.gotoxy(45, 13);
                cout << "Enter the amount to deposit: $ ";
                float amount;
                cin >> amount;

                // Load the data from the portfolio.json file
                json portfolioJson;
                ifstream in2("portfolio.json");
                if (in2.good())
                {
                    in2 >> portfolioJson;
                }
                in2.close();

                // Check if the client already has an entry in the portfolio.json file
                bool clientFoundInPortfolio = false;
                for (auto &clientInPortfolio : portfolioJson)
                {
                    if (clientInPortfolio["id"] == id)
                    {
                        clientFoundInPortfolio = true;
                        break;
                    }
                }

                // If the client doesn't have an entry in the portfolio.json file, add a new entry
                if (!clientFoundInPortfolio)
                {
                    string name = client["name"];
                    int balance_cents = round(amount * 100);
                    portfolioJson.push_back({{"name", name}, {"id", id}, {"balance", balance_cents / 100.0}});
                }
                else
                {
                    // If the client already has an entry in the portfolio.json file, update the balance
                    for (auto &clientInPortfolio : portfolioJson)
                    {
                        if (clientInPortfolio["id"] == id)
                        {
                            float current_balance = clientInPortfolio["balance"];
                            float updated_balance = current_balance + amount;

                            int balance_cents = round(updated_balance * 100);
                            clientInPortfolio["balance"] = balance_cents / 100.0;
                            break;
                        }
                    }
                }

                // Save the updated data to the portfolio.json file
                ofstream out2("portfolio.json");
                out2 << portfolioJson.dump(3) << endl;
                out2.close();

                c.gotoxy(39, 18);
                c.design(42, "\u2500");
                c.gotoxy(39, 20);
                c.design(42, "\u2500");
                c.gotoxy(52, 15);
                c.gotoxy(34, 19);
                cout << "Successfully deposited " << amount << " to the account of " << client["name"] << endl;
                c.gotoxy(39, 21);
                cout << "Do you want to deposit more money? (Y/N) ";
                char choice;
                cin >> choice;
                if (choice != 'Y' && choice != 'y')
                {
                    // if user is client then go to client menu, if user is admin then go to admin menu
                    if (currentUser->getLoginType() == "client")
                    {
                        displayMenuClient();
                    }
                    else if (currentUser->getLoginType() == "admin")
                    {
                        displayMenu();
                    }
                }
                else
                {
                    depositMoney();
                }
            }
        }
        if (!clientFound)
        {
            c.gotoxy(41, 15);
            c.design(42, "\u2500");
            c.gotoxy(54, 16);
            cout << "CLIENT NOT FOUND";
            c.gotoxy(41, 17);
            c.design(42, "\u2500");
            c.gotoxy(31, 19);
            cout << "Press 'Y' to retry or any other key to return to main menu. ";
            char choice;
            cin >> choice;
            if (choice != 'Y' && choice != 'y')
            {
                if (currentUser->getLoginType() == "client")
                {
                    displayMenuClient();
                }
                else if (currentUser->getLoginType() == "admin")
                {
                    displayMenu();
                }
            }
            else
            {
                depositMoney();
            }
        }
    }
}

void NepalStockAnalyzer::purchaseStock()
{
ae:
    system("CLS");
    c.gotoxy(28, 4);
    c.design(24, "\u2592");
    cout << " STOCK PURCHASE ";
    c.design(24, "\u2592");
    c.border();
    c.gotoxy(47, 7);
    cout << "Enter the ID of the client: ";
    int id;
    cin >> id;

    // Load the data from the clients.json file
    json clientsJson;
    ifstream in("added_clients.json");
    if (in.good())
    {
        in >> clientsJson;
    }
    in.close();

    // Find the client with the specified ID
    bool clientFound = false;
    for (auto &client : clientsJson)
    {
        if (client["id"] == id)
        {
            clientFound = true;

            // Load the data from the portfolio.json file
            json portfolioJson;
            ifstream in2("portfolio.json");
            if (in2.good())
            {
                in2 >> portfolioJson;
            }
            in2.close();

            // Find the client's balance
            float balance = 0;
            for (auto &entry : portfolioJson)
            {
                if (entry["id"] == id)
                {
                    balance = entry["balance"];
                    break;
                }
            }

            // Load the data from the added_stocks.json file
            json stockListsJson;
            ifstream in3("added_stocks.json");
            if (in3.good())
            {
                in3 >> stockListsJson;
            }
            in3.close();

            c.gotoxy(54, 9);
            cout << "Available stocks:" << endl;

            // first let's display headers
            c.gotoxy(38, 11);
            cout << "Stock ID";
            c.gotoxy(53, 11);
            cout << "Stock Name";
            c.gotoxy(70, 11);
            cout << "Market Price";

            c.gotoxy(35, 12);
            c.design(50, "\u2550");
            int i = 13;
            // now let's display the data
            for (auto &stock : stockListsJson)
            {
                int stockId = stock["stockId"];
                string stockName = formatString(stock["stockName"]);
                float marketPrice = stock["marketPrice"];

                c.gotoxy(38, i);
                cout << stockId << endl;
                c.gotoxy(53, i);
                cout << stockName << endl;
                c.gotoxy(70, i);
                cout << marketPrice << endl;
                i++;
            }

            // calculate the number of rows in the table
            int numberOfRows = i - 12;

            c.gotoxy(44, 13 + numberOfRows);
            cout << "Enter ID of Stock to Purchase: ";
            int stockId;
            cin >> stockId;

            // Find the stock with the specified ID
            bool stockFound = false;
            float marketPrice = 0;
            string stockName;
            for (auto &stock : stockListsJson)
            {
                if (stock["stockId"] == stockId)
                {
                    stockFound = true;
                    marketPrice = stock["marketPrice"];
                    stockName = stock["stockName"];
                    break;
                }
            }

            if (!stockFound)
            {
                c.gotoxy(39, numberOfRows + 14);
                c.design(42, "\u2500");
                c.gotoxy(52, numberOfRows + 15);
                cout << "STOCK NOT FOUND";
                c.gotoxy(39, numberOfRows + 16);
                c.design(42, "\u2500");
                c.gotoxy(30, numberOfRows + 18);
                cout << "Press 'Y' to retry or any other key to return to main menu. ";
                char choice;
                cin >> choice;
                if (choice != 'Y' && choice != 'y')
                {
                    displayMenu();
                }
                else
                {
                    goto ae;
                }
            }

            c.gotoxy(40, numberOfRows + 14);
            cout << "Enter the number of stocks to purchase: ";
            int numStocks;
            cin >> numStocks;

            // Calculate the total cost of the purchase
            float totalCost = marketPrice * numStocks;

            if (balance < totalCost)
            {
                c.gotoxy(41, numberOfRows + 15);
                c.design(42, "\u2500");
                c.gotoxy(39, numberOfRows + 16);
                cout << "INSUFFICIENT BALANCE TO COMPLETE THE PURCHASE";
                c.gotoxy(41, numberOfRows + 17);
                c.design(42, "\u2500");
                c.gotoxy(34, numberOfRows + 19);
                cout << "Press 'Y' to retry or any other key to return to main menu. ";
                char choice;
                cin >> choice;
                if (choice != 'Y' && choice != 'y')
                {
                    if (currentUser->getLoginType() == "admin")
                    {
                        displayMenu();
                    }
                    else
                    {
                        displayMenuClient();
                    }
                }
                else
                {
                    goto ae;
                }
            }
            // Update the client's balance
            for (auto &entry : portfolioJson)
            {
                if (entry["id"] == id)
                {
                    float balance = entry["balance"].get<float>();
                    float updated_balance = balance - totalCost;
                    int balance_cents = round(updated_balance * 100);
                    entry["balance"] = balance_cents / 100.0;
                    break;
                }
            }
            // Save the updated data to the portfolio.json file
            ofstream out2("portfolio.json");
            out2 << portfolioJson.dump(4);
            out2.close();

            // Get the current time
            time_t t;
            time(&t);
            string purchaseTime = ctime(&t);

            // Add the purchased stock to the client's portfolio
            bool portfolioEntryFound = false;
            for (auto &entry : portfolioJson)
            {
                if (entry["id"] == id)
                {
                    portfolioEntryFound = true;
                    bool stockAlreadyExists = false;
                    for (auto &stock : entry["stocks"])
                    {
                        if (stock["stockId"] == stockId)
                        {
                            stockAlreadyExists = true;
                            int numberOfShares = stock["numberOfShares"];
                            numberOfShares += numStocks;
                            stock["numberOfShares"] = numberOfShares;
                            int purchaseRateCents = round(marketPrice * 100);
                            stock["purchaseRate"] = purchaseRateCents / 100.0;
                            stock["purchaseTime"] = purchaseTime;
                            break;
                        }
                    }
                    if (!stockAlreadyExists)
                    {
                        json newStock;
                        newStock["stockId"] = stockId;
                        newStock["stockName"] = stockName;
                        newStock["numberOfShares"] = numStocks;
                        int purchaseRateCents = round(marketPrice * 100);
                        newStock["purchasedRate"] = purchaseRateCents / 100.0;
                        newStock["purchaseTime"] = purchaseTime;
                        entry["stocks"].push_back(newStock);
                    }
                    break;
                }
            }

            // Save the updated data to the portfolio.json file
            ofstream out3("portfolio.json");
            out3 << portfolioJson.dump(4);
            out3.close();

            // Call the buyHistory function to store the purchase transaction in the transactions.json file
            buyHistory(id, stockId, stockName, numStocks, marketPrice, totalCost, purchaseTime);

            c.gotoxy(41, numberOfRows + 15);
            c.design(42, "\u2500");
            c.gotoxy(32, numberOfRows + 16);
            cout << "Successfully Purchased " << numStocks << " Shares of " << stockName << " for a Total of " << totalCost;
            c.gotoxy(41, numberOfRows + 17);
            c.design(42, "\u2500");
            c.gotoxy(44, numberOfRows + 19);
            cout << "Want to Purchase More Stocks? (Y/N) ";
            char choice;
            cin >> choice;
            if (choice == 'Y' || choice == 'y')
            {
                goto ae;
            }
            else
            {
                // if the user is admin then go to admin menu else go to client menu
                if (currentUser->getLoginType() == "admin")
                    displayMenu();
                else
                    displayMenuClient();
            }
        }
    }
    if (!clientFound)
    {
        c.gotoxy(39, 14);
        c.design(42, "\u2500");
        c.gotoxy(52, 15);
        cout << "CLIENT NOT FOUND";
        c.gotoxy(39, 16);
        c.design(42, "\u2500");
        c.gotoxy(30, 18);
        cout << "Press 'Y' to retry or any other key to return to main menu. ";
        char choice;
        cin >> choice;
        if (choice == 'Y' || choice == 'y')
        {
            goto ae;
        }
        else
        {
            if (currentUser->getLoginType() == "admin")
                displayMenu();
            else
                displayMenuClient();
        }
    }
}

void NepalStockAnalyzer::sellStock()
{
be:
    system("CLS");
    c.gotoxy(29, 4);
    c.design(25, "\u2592");
    cout << " SELL STOCK ";
    c.design(25, "\u2592");
    c.border();
    c.gotoxy(47, 6);
    cout << "Enter the ID of the client: ";
    int id;
    cin >> id;

    // Load the data from the clients.json file
    json clientsJson;
    ifstream in("added_clients.json");
    if (in.good())
    {
        in >> clientsJson;
    }
    in.close();

    // Find the client with the specified ID
    bool clientFound = false;
    for (auto &client : clientsJson)
    {
        if (client["id"] == id)
        {
            clientFound = true;
            break;
        }
    }

    if (!clientFound)
    {
        c.gotoxy(39, 14);
        c.design(42, "\u2500");
        c.gotoxy(52, 15);
        cout << "CLIENT NOT FOUND";
        c.gotoxy(39, 16);
        c.design(42, "\u2500");
        c.gotoxy(30, 18);
        cout << "Press 'Y' to retry or any other key to return to main menu. ";
        char choice;
        cin >> choice;
        if (choice == 'Y' || choice == 'y')
        {
            goto be;
        }
        else
        {
            if (currentUser->getLoginType() == "admin")
            {
                displayMenu();
            }
            else
            {
                displayMenuClient();
            }
        }
    }

    // Load the data from the portfolio.json file
    json portfolioJson;
    ifstream in2("portfolio.json");
    if (in2.good())
    {
        in2 >> portfolioJson;
    }
    in2.close();

    // Find the client's portfolio
    bool portfolioFound = false;
    for (auto &entry : portfolioJson)
    {
        if (entry["id"] == id)
        {
            portfolioFound = true;
            break;
        }
    }

    if (!portfolioFound)
    {
        c.gotoxy(41, 14);
        c.design(42, "\u2500");
        c.gotoxy(44, 15);
        cout << "PORTFOLIO NOT FOUND";
        c.gotoxy(41, 16);
        c.design(42, "\u2500");
        c.gotoxy(42, 18);
        cout << "Press 'Y' to retry or any other key to return to main menu. ";
        char choice;
        cin >> choice;
        if (choice == 'Y' || choice == 'y')
        {
            goto be;
        }
        else
        {
            if (currentUser->getLoginType() == "admin")
                displayMenu();
            else
                displayMenuClient();
        }
    }

    // Load the data from the added_stocks.json file
    json stockListsJson;
    ifstream in3("added_stocks.json");
    if (in3.good())
    {
        in3 >> stockListsJson;
    }
    in3.close();

    // display the header
    c.gotoxy(19, 8);
    cout << "ID";
    c.gotoxy(27, 8);
    cout << "Stock Name";
    c.gotoxy(41, 8);
    cout << "Stock Count";
    c.gotoxy(57, 8);
    cout << "Market Price";
    c.gotoxy(73, 8);
    cout << "Purchased Rate";
    c.gotoxy(90, 8);
    cout << "Total Value";

    c.gotoxy(12, 9);
    c.design(96, "\u2550");
    int i = 10;
    for (auto &entry : portfolioJson)
    {
        if (entry["id"] == id)
        {
            for (auto &stock : entry["stocks"])
            {
                c.gotoxy(19, i);
                cout << stock["stockId"];
                c.gotoxy(27, i);
                cout << stock["stockName"];
                c.gotoxy(41, i);
                cout << stock["numberOfShares"];
                // find the market price of the stock from added_stocks.json
                double marketPrice = 0;
                for (auto &stockList : stockListsJson)
                {
                    if (stockList["stockId"] == stock["stockId"])
                    {
                        marketPrice = stockList["marketPrice"];
                        break;
                    }
                }
                c.gotoxy(57, i);
                cout << marketPrice;
                c.gotoxy(73, i);
                cout << stock["purchasedRate"];

                // calculate the total value of the stock
                double totalValue = marketPrice * stock["numberOfShares"].get<double>();
                c.gotoxy(90, i);
                cout << totalValue;
                i++;
            }
            break;
        }
    }

    // count number of stocks in the portfolio
    int numberOfStocks = 0;
    for (auto &entry : portfolioJson)
    {
        if (entry["id"] == id)
        {
            numberOfStocks = entry["stocks"].size();
            break;
        }
    }

    c.gotoxy(18, numberOfStocks + 11);
    cout << "Enter ID of Stock to Sell     : ";
    int stockId;
    cin >> stockId;

    // Find the stock with the specified ID
    bool stockFound = false;
    float marketPrice = 0;
    string stockName;
    for (auto &stock : stockListsJson)
    {
        if (stock["stockId"] == stockId)
        {
            stockFound = true;
            marketPrice = stock["marketPrice"];
            stockName = stock["stockName"];
            break;
        }
    }

    if (!stockFound)
    {
        c.gotoxy(39, numberOfStocks + 14);
        c.design(42, "\u2500");
        c.gotoxy(41, numberOfStocks + 15);
        cout << "STOCK WITH THE SPECIFIED ID NOT FOUND";
        c.gotoxy(39, numberOfStocks + 16);
        c.design(42, "\u2500");
        c.gotoxy(30, numberOfStocks + 18);
        cout << "Press 'Y' to retry or any other key to return to main menu. ";
        char choice;
        cin >> choice;
        if (choice == 'Y' || choice == 'y')
        {
            goto be;
        }
        else
        {
            if (currentUser->getLoginType() == "admin")
                displayMenu();
            else
                displayMenuClient();
        }
    }

    c.gotoxy(18, numberOfStocks + 12);
    cout << "Enter Number of Stocks to Sell: ";
    int numStocks;
    cin >> numStocks;

    // Check if the client has the stock in their portfolio
    int stockIndex = -1;
    int numberOfShares = 0;
    float purchaseRate = 0;
    for (auto &entry : portfolioJson)
    {
        if (entry["id"] == id)
        {
            for (int i = 0; i < entry["stocks"].size(); i++)
            {
                if (entry["stocks"][i]["stockId"] == stockId)
                {
                    stockIndex = i;
                    numberOfShares = entry["stocks"][i]["numberOfShares"];
                    purchaseRate = entry["stocks"][i]["purchasedRate"];
                    break;
                }
            }
        }
    }
    if (numStocks > numberOfShares)
    {
        c.gotoxy(39, numberOfStocks + 15);
        c.design(42, "\u2500");
        c.gotoxy(39, numberOfStocks + 16);
        cout << "CLIENT DOES NOT HAVE ENOUGH SHARES TO SELL";
        c.gotoxy(39, numberOfStocks + 17);
        c.design(42, "\u2500");
        c.gotoxy(30, numberOfStocks + 19);
        cout << "Press 'Y' to retry or any other key to return to main menu. ";
        char choice;
        cin >> choice;
        if (choice == 'Y' || choice == 'y')
        {
            goto be;
        }
        else
        {
            if (currentUser->getLoginType() == "admin")
                displayMenu();
            else
                displayMenuClient();
        }
    }

    // Calculate the total earnings from the sale
    float totalEarnings = marketPrice * numStocks;

    c.gotoxy(47, numberOfStocks + 14);
    cout << "The Selling Price is: " << marketPrice;
    c.gotoxy(33, numberOfStocks + 15);
    cout << "Purchased Stock at " << purchaseRate << " and Earned " << totalEarnings << " From the Sale";

    // Calculate the profit or loss
    float profitLoss = totalEarnings - (purchaseRate * numStocks);

    // Display the profit or loss
    c.gotoxy(51, numberOfStocks + 16);
    cout << "Profit/loss: " << profitLoss << endl;

    c.gotoxy(44, numberOfStocks + 18);
    cout << "Want to Sell the Stock? (Y/N): ";
    char sellDecision;
    cin >> sellDecision;

    if (sellDecision == 'n')
    {
        if (currentUser->getLoginType() == "admin")
            displayMenu();
        else
            displayMenuClient();
    }

    // Update the client's balance
    for (auto &entry : portfolioJson)
    {
        if (entry["id"] == id)
        {
            float balance = entry["balance"].get<float>();
            balance += totalEarnings;
            int balance_cents = round(balance * 100);
            entry["balance"] = balance_cents / 100.0;
            break;
        }
    }

    // Update the client's portfolio
    for (auto &entry : portfolioJson)
    {
        if (entry["id"] == id)
        {
            int numberOfShares = entry["stocks"][stockIndex]["numberOfShares"].get<int>();
            int newNumberOfShares = numberOfShares - numStocks;
            if (newNumberOfShares == 0)
            {
                // Remove the stock from the portfolio if the client no longer has any shares
                entry["stocks"].erase(entry["stocks"].begin() + stockIndex);
            }
            else
            {
                // Update the number of shares if the client still has some left
                entry["stocks"][stockIndex]["numberOfShares"] = newNumberOfShares;
            }
            break;
        }
    }

    // Save the updated data to the portfolio.json file
    ofstream out2("portfolio.json");
    out2 << portfolioJson.dump(4);
    out2.close();

    // Get the current time
    time_t t;
    time(&t);
    string sellTime = ctime(&t);

    // // Call the sellHistory function to store the transaction in the transactions.json file
    sellHistory(id, stockId, stockName, numStocks, marketPrice, totalEarnings, sellTime);

    c.gotoxy(41, numberOfStocks + 19);
    c.design(42, "\u2500");
    c.gotoxy(33, numberOfStocks + 20);
    cout << "Successfully Sold " << numStocks << " shares of " << stockName << " at a price of " << totalEarnings << endl;
    c.gotoxy(41, numberOfStocks + 21);
    c.design(42, "\u2500");
    c.gotoxy(44, numberOfStocks + 22);
    cout << "Want to Sell another Stock? (Y/N) ";
    char choice;
    cin >> choice;
    if (choice == 'y')
    {
        goto be;
    }
    else
    {
        if (currentUser->getLoginType() == "admin")
            displayMenu();
        else
            displayMenuClient();
    }
}

void NepalStockAnalyzer::buyHistory(int id, int stockId, string stockName, int numStocks, float price, float totalCost, string time)
{
    // Load the data from the transactions.json file
    json transactionsJson;
    ifstream in("transactions.json");
    if (in.good())
    {
        in >> transactionsJson;
    }
    in.close();

    // Load the data from the clients.json file
    json clientsJson;
    ifstream in2("added_clients.json");
    if (in2.good())
    {
        in2 >> clientsJson;
    }
    in2.close();

    // Find the client's name
    string clientName;
    for (auto &client : clientsJson)
    {
        if (client["id"] == id)
        {
            clientName = client["name"];
            break;
        }
    }

    // Check if the client already has an entry in the transactionsJson object
    bool clientFound = false;
    for (auto &client : transactionsJson)
    {
        if (client.find(clientName) != client.end())
        {
            clientFound = true;
            int priceCents = round(price * 100);
            int totalCostCents = round(totalCost * 100);
            client[clientName].push_back({{"id", id}, {"stockId", stockId}, {"stockName", stockName}, {"numberOfShares", numStocks}, {"price", priceCents / 100.0}, {"totalCost", totalCostCents / 100.0}, {"type", "purchase"}, {"time", time}});
            break;
        }
    }

    if (!clientFound)
    {
        int priceCents = round(price * 100);
        int totalCostCents = round(totalCost * 100);
        // Add a new entry for the client
        transactionsJson.push_back({{"name", clientName}, {"transactions", {{{"id", id}, {"stockId", stockId}, {"stockName", stockName}, {"numberOfShares", numStocks}, {"price", priceCents / 100.0}, {"totalCost", totalCostCents / 100.0}, {"type", "purchase"}, {"time", time}}}}});
    }

    // Save the updated data to the transactions.json file
    ofstream out("transactions.json");
    out << transactionsJson.dump(4);
    out.close();
}

void NepalStockAnalyzer::sellHistory(int id, int stockId, string stockName, int numStocks, float price, float totalCost, string time)
{
    // Load the data from the transactions.json file
    json transactionsJson;
    ifstream in("transactions.json");
    if (in.good())
    {
        in >> transactionsJson;
    }
    in.close();

    // Load the data from the clients.json file
    json clientsJson;
    ifstream in2("added_clients.json");
    if (in2.good())
    {
        in2 >> clientsJson;
    }
    in2.close();

    // Find the client's name
    string clientName;
    for (auto &client : clientsJson)
    {
        if (client["id"] == id)
        {
            clientName = client["name"];
            break;
        }
    }

    // Check if the clientalready has an entry in the transactionsJson object
    bool clientFound = false;
    for (auto &entry : transactionsJson)
    {
        if (entry["name"] == clientName)
        {
            clientFound = true;
            int priceCents = round(price * 100);
            int totalCostCents = round(totalCost * 100);
            // Add the sell transaction to the client's entry in the transactionsJson object
            entry["transactions"].push_back({{"id", id}, {"stockId", stockId}, {"stockName", stockName}, {"numberOfShares", numStocks}, {"price", priceCents / 100.0}, {"totalCost", totalCostCents / 100.0}, {"type", "sell"}, {"time", time}});
            break;
        }
    }
    // If the client doesn't have an entry in the transactionsJson object, create a new entry for them
    if (!clientFound)
    {
        int priceCents = round(price * 100);
        price = priceCents / 100.0;
        int totalCostCents = round(totalCost * 100);
        totalCost = totalCostCents / 100.0;
        transactionsJson.push_back({{"name", clientName}, {"transactions", {{{"id", id}, {"stockId", stockId}, {"stockName", stockName}, {"numberOfShares", numStocks}, {"price", price}, {"totalCost", totalCost}, {"type", "sell"}, {"time", time}}}}});
    }
    // Save the updated transactionsJson object to the transactions.json file
    ofstream out("transactions.json");
    out << transactionsJson.dump(4);
    out.close();
}

void NepalStockAnalyzer::displayTransactions()
{
ce:
    system("CLS");
    c.gotoxy(24, 4);
    c.design(24, "\u2592");
    cout << " SEARCH CLIENT ACCOUNT ";
    c.design(24, "\u2592");
    c.border();
    c.gotoxy(43, 7);
    cout << "Enter The Client ID To Search: ";
    int id;
    cin >> id;

    // Load the data from the transactions.json file
    json transactionsJson;
    ifstream in("transactions.json");
    if (in.good())
    {
        in >> transactionsJson;
    }
    in.close();

    // Load the data from the clients.json file
    json clientsJson;
    ifstream in2("added_clients.json");
    if (in2.good())
    {
        in2 >> clientsJson;
    }
    in2.close();

    // Find the client's name
    string clientName;
    bool clientFound = false;
    for (auto &client : clientsJson)
    {
        if (client["id"] == id)
        {
            clientName = client["name"];
            clientFound = true;
            break;
        }
    }

    // count the number of transactions for the client
    int numTransactions = 0;

    // Iterate through the top level of the JSON object
    for (auto &client : transactionsJson)
    {
        // Get the transactions array for the current client
        auto transactions = client["transactions"];
        // Iterate through the transactions array
        for (auto &transaction : transactions)
        {
            // Check if the ID of the client in the current transaction matches the user-entered ID
            if (transaction["id"] == id)
            {
                // If it matches, increment the transactionCount variable
                numTransactions++;
            }
        }
    }

    if (!clientFound)
    {
        c.gotoxy(39, numTransactions + 15);
        c.design(42, "\u2500");
        c.gotoxy(52, numTransactions + 16);
        cout << "CLIENT NOT FOUND";
        c.gotoxy(39, numTransactions + 17);
        c.design(42, "\u2500");
        c.gotoxy(30, numTransactions + 18);
        cout << "Press 'Y' to retry or any other key to return to main menu. ";
        char choice;
        cin >> choice;
        if (choice == 'Y' || choice == 'y')
        {
            goto ce;
        }
        else
        {
            if (currentUser->getLoginType() == "admin")
            {
                displayMenu();
            }
            else
            {
                displayMenuClient();
            }
        }
    }
    else
    {
        c.gotoxy(39, numTransactions + 15);
        c.design(42, "\u2500");
        c.gotoxy(49, numTransactions + 16);
        cout << "Account Number Matched";
        c.gotoxy(39, numTransactions + 17);
        c.design(42, "\u2500");
    }

    // Display the transactions for the client
    c.gotoxy(49, 9);
    cout << "Transactions for ";
    transform(clientName.begin(), clientName.end(), ostream_iterator<char>(std::cout), ::toupper);
    cout << ":" << endl;

    c.gotoxy(15, 11);
    cout << "ID";
    c.gotoxy(22, 11);
    cout << "Stock Name";
    c.gotoxy(35, 11);
    cout << "Shares";
    c.gotoxy(45, 11);
    cout << "Price";
    c.gotoxy(55, 11);
    cout << "Cost";
    c.gotoxy(65, 11);
    cout << "Type";
    c.gotoxy(78, 11);
    cout << "Purchase Time";

    c.gotoxy(11, 12);
    c.design(98, "\u2550");
    int i = 13;
    for (auto &client : transactionsJson)
    {
        for (auto &transaction : client["transactions"])
        {
            if (transaction["id"] == id)
            {
                c.gotoxy(15, i);
                cout << transaction["stockId"];
                c.gotoxy(22, i);
                cout << transaction["stockName"];
                c.gotoxy(35, i);
                cout << transaction["numberOfShares"];
                c.gotoxy(45, i);
                cout << transaction["price"];
                c.gotoxy(55, i);
                cout << transaction["totalCost"];
                c.gotoxy(65, i);
                cout << transaction["type"];
                c.gotoxy(78, i);
                cout << transaction["time"];
                i++;
            }
        }
    }
    c.gotoxy(30, numTransactions + 18);
    cout << "Press 'Y' to retry or any other key to return to main menu. ";
    char choice;
    cin >> choice;
    if (choice == 'Y' || choice == 'y')
    {
        goto ce;
    }
    else
    {
        if (currentUser->getLoginType() == "admin")
        {
            displayMenu();
        }
        else
        {
            displayMenuClient();
        }
    }
}

void NepalStockAnalyzer::displayClientPortfolio()
{
de:
    system("CLS");
    c.gotoxy(24, 4);
    c.design(24, "\u2592");
    cout << " SEARCH CLIENT ACCOUNT ";
    c.design(24, "\u2592");
    c.border();
    c.gotoxy(43, 7);
    cout << "Enter The Client ID To Search: ";
    int id;
    cin >> id;

    // Load the data from the portfolio.json file
    json portfolioJson;
    ifstream in("portfolio.json");
    if (in.good())
    {
        in >> portfolioJson;
    }
    in.close();

    json addedStocksJson;
    ifstream addedStocksIn("added_stocks.json");
    if (addedStocksIn.good())
    {
        addedStocksIn >> addedStocksJson;
    }
    addedStocksIn.close();

    // Search for the client with the given ID
    string clientName;
    vector<pair<string, int>> stockPurchased;
    bool clientFound = false;
    for (auto &client : portfolioJson)
    {
        if (client["id"] == id)
        {
            clientName = client["name"];
            for (auto &stock : client["stocks"])
            {
                string stockName = stock["stockName"];
                int stockQuantity = stock["numberOfShares"];
                stockPurchased.push_back({stockName, stockQuantity});
            }
            clientFound = true;
            break;
        }
    }

    int numStocks = 0;
    for (auto &client : portfolioJson)
    {
        if (client["id"] == id)
        {
            numStocks = client["stocks"].size();
            break;
        }
    }

    // Display the client's portfolio if they were found
    if (clientFound)
    {
        c.gotoxy(39, numStocks + 14);
        c.design(42, "\u2500");
        c.gotoxy(49, numStocks + 15);
        cout << "Account Number Matched";
        c.gotoxy(39, numStocks + 16);
        c.design(42, "\u2500");
        c.gotoxy(46, 10);
        cout << "Detail Information of ";
        transform(clientName.begin(), clientName.end(), ostream_iterator<char>(std::cout), ::toupper);
        cout << ":" << endl;

        c.gotoxy(28, 11);
        c.design(65, "\u2550");
        c.gotoxy(52, 12);
        cout << "Stock Purchased" << endl;
        int i = 13;
        for (auto &stock : stockPurchased)
        {
            // Find the market price of the stock
            float marketPrice = 0;
            for (auto &addedStock : addedStocksJson)
            {
                if (addedStock["stockName"] == stock.first)
                {
                    marketPrice = addedStock["marketPrice"];
                    break;
                }
            }
            // Calculate the value of the stock
            float stockValue = marketPrice * stock.second;
            c.gotoxy(37, i);
            cout << "Stock Name: " << stock.first << ", Shares: " << stock.second << ", Value: " << stockValue << endl;
            // cout << stock.first << ": " << stock.second << " (" << stockValue << ")" << endl;
            i++;
        }
        c.gotoxy(30, numStocks + 17);
        cout << "Press 'Y' to retry or any other key to return to main menu. ";
        char choice;
        cin >> choice;
        if (choice == 'Y' || choice == 'y')
        {
            goto de;
        }
        else
        {
            if (currentUser->getLoginType() == "admin")
            {
                displayMenu();
            }
            else
            {
                displayMenuClient();
            }
        }
    }
    else
    {
        c.gotoxy(39, 17);
        c.design(42, "\u2500");
        c.gotoxy(52, 18);
        cout << "CLIENT NOT FOUND";
        c.gotoxy(39, 19);
        c.design(42, "\u2500");
        c.gotoxy(30, 20);
        cout << "Press 'Y' to retry or any other key to return to main menu. ";
        char choice;
        cin >> choice;
        if (choice == 'Y' || choice == 'y')
        {
            goto de;
        }
        else
        {
            if (currentUser->getLoginType() == "admin")
            {
                displayMenu();
            }
            else
            {
                displayMenuClient();
            }
        }
    }
}

int main()
{
    // set background color to black and text color to white
    system("color 0F");

    StockAnalyzer sa;
    NepalStockAnalyzer nsa;

    // to update market prices of stocks
    nsa.updateStockPrices();

    // to display the menu
    sa.displayMenu();
    return 0;
}