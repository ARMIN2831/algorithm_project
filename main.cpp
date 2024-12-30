#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
using namespace std;

float TimeTaken;

void linearSearch(const vector<string>& db, const string& username) {
    auto start = chrono::steady_clock::now(); // Start time
    for (const auto& user : db) {
        if (user == username) {
            auto end = chrono::steady_clock::now(); // End time
            cout << "Username found (Linear Search).\n";
            chrono::duration<double> duration = end - start;
            cout << "Time taken: " << duration.count() << " seconds.\n";
            TimeTaken = duration.count(); // Time in seconds
            return;
        }
    }
    auto end = chrono::steady_clock::now(); // End time after loop
    cout << "Username not found (Linear Search).\n";
    chrono::duration<double> duration = end - start;
    cout << "Time taken: " << duration.count() << " seconds.\n";
    TimeTaken = duration.count(); // Time in seconds
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

    // Save results to file
    ofstream resultFile("search_results.txt");

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

        resultFile << "Searching for: " << username << endl << "Time taken: " << TimeTaken << " seconds" << endl << "Search method: " << choice << endl; // Save search details to file
        resultFile << "----------------------------------------\n"; // Separator
        cout << "----------------------------------------\n";
    }
    resultFile.close(); // Close the file after finishing
    return 0;
}
