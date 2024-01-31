#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Guest {
public:
    string name;
    int roomNumber;

    Guest() : name(""), roomNumber(-1) {}

    Guest(string n, int room) : name(n), roomNumber(room) {}
};

class Stack {
private:
    Guest* data;
    int size;
    int capacity;

public:
    Stack(int cap = 10) {
        capacity = cap;
        size = 0;
        data = new Guest[capacity];
    }

    ~Stack() {
        delete[] data;
    }

    void push(const Guest& guest) {
        if (size < capacity) {
            data[size++] = guest;
        }
        else {
            cout << "/n Sorry no room available /n";
        }
    }

    void pop() {
        if (size > 0) {
            --size;
        }
        else
        {
            cout << "/n Sorry no room Booked /n";
        }
    }

    Guest top() const {
        if (size > 0) {
            return data[size - 1];
        }
        else {
            return Guest("", -1);
        }
    }

    bool isEmpty() const {
        return size == 0;
    }
};

class Queue {
private:
    Guest* data;
    int front;
    int rear;
    int size;
    int capacity;

public:
    Queue(int cap = 10) {
        front = 0;
        rear = -1;
        size = 0;
        capacity = cap;
        data = new Guest[capacity];
    }

    ~Queue() {
        delete[] data;
    }

    void enqueue(const Guest& guest) {
        if (size < capacity) {
            rear = (rear + 1) % capacity;
            data[rear] = guest;
            ++size;
        }
        else {
            cout << "/n Sorry no room available /n";
        }
    }

    void dequeue() {
        if (size > 0) {
            front = (front + 1) % capacity;
            --size;
        }
        else {
            cout << "/n Sorry no room Booked /n";
        }
    }

    Guest frontGuest() const {
        if (size > 0) {
            return data[front];
        }
        else {
            return Guest("", -1);
        }
    }

    bool isEmpty() const {
        return size == 0;
    }
};

class HashMap {
private:
    static const int TABLE_SIZE = 10;
    struct Node {
        Guest data;
        Node* next;

        Node(const Guest& guest) {
            data = guest;
            next = nullptr;
        }
    };

    Node* table[TABLE_SIZE];

public:
    HashMap() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            table[i] = nullptr;
        }
    }

    ~HashMap() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            Node* current = table[i];
            while (current) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    void insert(const Guest& guest) {
        int index = guest.roomNumber % TABLE_SIZE;
        Node* newNode = new Node(guest);
        newNode->next = table[index];
        table[index] = newNode;
    }

    bool remove(int roomNumber) {
        int index = roomNumber % TABLE_SIZE;
        Node* current = table[index];
        Node* prev = nullptr;

        while (current) {
            if (current->data.roomNumber == roomNumber) {
                if (prev) {
                    prev->next = current->next;
                }
                else {
                    table[index] = current->next;
                }

                delete current;
                return true;
            }

            prev = current;
            current = current->next;
        }

        return false;
    }

    void display() const {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            Node* current = table[i];
            while (current) {
                cout << "Room: " << current->data.roomNumber << ", Guest: " << current->data.name << endl;
                current = current->next;
            }
        }
    }
};

class HotelManagementSystem {
private:
    Stack checkInStack;
    Queue checkInQueue;
    HashMap guestHashMap;

public:
    const string FILE_NAME = "guests.txt";
    HotelManagementSystem() {

        loadGuestsFromFile();
    }

    ~HotelManagementSystem() {

        saveGuestsToFile();
    }
    void checkIn(const Guest& guest) {
        checkInStack.push(guest);
        checkInQueue.enqueue(guest);
        guestHashMap.insert(guest);
        cout << "Guest checked in successfully." << endl;
    }

    void checkOut() {
        if (!checkInStack.isEmpty()) {
            Guest guest = checkInStack.top();
            checkInStack.pop();
            checkInQueue.dequeue();
            guestHashMap.remove(guest.roomNumber);
            cout << "Guest checked out successfully." << endl;
        }
    }

    void displayGuests() const {
        cout << "\nHotel Management System - Guest Information\n";
        cout << "-----------------------------------------\n";


        cout << "\nStack System Guests:\n";
        cout << "Top of stack: Room: " << checkInStack.top().roomNumber << ", Guest: " << checkInStack.top().name << endl;


        cout << "\nQueue System Guests:\n";
        cout << "Front of queue: Room: " << checkInQueue.frontGuest().roomNumber << ", Guest: " << checkInQueue.frontGuest().name << endl;


        cout << "\nHash Map System Guests:\n";
        guestHashMap.display();

        cout << "-----------------------------------------\n";
    }
    void loadGuestsFromFile() {
        ifstream inputFile(FILE_NAME);

        if (inputFile.is_open()) {
            Guest guest;
            while (inputFile >> guest.name >> guest.roomNumber) {
                checkInStack.push(guest);
                checkInQueue.enqueue(guest);
                guestHashMap.insert(guest);
            }
            inputFile.close();
        }
    }

    void saveGuestsToFile() {
        ofstream outputFile(FILE_NAME);

        if (outputFile.is_open()) {
            while (!checkInStack.isEmpty()) {
                Guest guest = checkInStack.top();
                outputFile << guest.name << " " << guest.roomNumber << endl;
                checkInStack.pop();
            }
            outputFile.close();
        }
    }

};

int main() {
    HotelManagementSystem hotel;

    int choice;

    do {
        cout << "\tWelcome To Hotel Management System Application\n\n";
        cout << "\n\t_Hotel Management System";
        cout << "\n\nS.No    Functions                  Description\n\n";
        cout << "\n1.\tCheck-In\t\t\tInsert New Room";
        cout << "\n2.\tCheck-Out\t\t\t\n";
        cout << "\n3.\tDisplay Guests \t\t\tShow Room Records that(we added)";

        cout << "\n4. \tExit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            system("cls");
            string name;
            int roomNumber;

            cout << "Enter guest name: ";
            cin.ignore();
            getline(cin, name);

            cout << "Enter room number: ";
            cin >> roomNumber;

            hotel.checkIn(Guest(name, roomNumber));
            system("pause");
            system("cls");
            break;
        }
        case 2:

            hotel.checkOut();
            system("pause");
            system("cls");
            break;
        case 3:
            system("cls");
            hotel.displayGuests();
            system("pause");
            system("cls");
            break;
        case 4:
            cout << "Exiting the program.\n";
            system("pause");

            break;
        default:

            cout << "Invalid choice. Please enter a valid option.\n";
            system("pause");
            system("cls");
        }
    } while (choice != 4);

    return 0;
}