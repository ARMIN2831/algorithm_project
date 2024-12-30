#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

void linearSearch(const vector<string>& db, const string& username) {

}

void loadUsernamesFromFile(vector<string>& database) {
    ifstream file("usernames.txt");
    string username;
    while (getline(file, username)) {
        database.push_back(username);
    }
    file.close();
}

int main() {
    vector<string> database;
    loadUsernamesFromFile(database);
    while (true) {
        int choice;
        string username;

        cout << "Enter username to search: ";
        cin >> username;

        cout << "Choose search method:\n";
        cout << "1. Linear Search\n";
        cout << "2. Binary Search\n";
        cout << "3. Hash Search\n";
        cout << "4. Bloom Filter Search\n";
        cin >> choice;

        switch (choice) {
            case 1:
                linearSearch(database, username);
                break;
            default:
                cout << "Invalid choice.\n";
                break;
        }
    }
    return 0;
}
