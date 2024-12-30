#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <bitset>
#include <chrono>
#include <fstream>
using namespace std;

const int BLOOM_FILTER_SIZE = 4000000;
float TimeTaken;
bitset<BLOOM_FILTER_SIZE> bloomFilter;

int hash1(const string& key) {
    int hash = 0;
    for (char c : key) {
        hash = (hash * 31 + c) % BLOOM_FILTER_SIZE;
    }
    return hash;
}

int hash2(const string& key) {
    int hash = 0;
    for (char c : key) {
        hash = (hash * 17 + c) % BLOOM_FILTER_SIZE;
    }
    return hash;
}

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

void binarySearch(const vector<string>& sortedDb, const string& username) {
    auto start = chrono::steady_clock::now(); // Start time
    bool ch = binary_search(sortedDb.begin(), sortedDb.end(), username);
    auto end = chrono::steady_clock::now(); // End time
    if (ch) {
        cout << "Username found (Binary Search).\n";
    } else {
        cout << "Username not found (Binary Search).\n";
    }
    chrono::duration<double> duration = end - start;
    cout << "Time taken: " << duration.count() << " seconds.\n";
    TimeTaken = duration.count(); // Time in seconds
}

void hashSearch(const unordered_map<string, bool>& hashMap, const string& username) {
    auto start = chrono::steady_clock::now(); // Start time
    bool ch = hashMap.find(username) != hashMap.end();
    auto end = chrono::steady_clock::now(); // End time
    if (ch) {
        cout << "Username found (Hash Search).\n";
    } else {
        cout << "Username not found (Hash Search).\n";
    }
    chrono::duration<double> duration = end - start;
    cout << "Time taken: " << duration.count() << " seconds.\n";
    TimeTaken = duration.count(); // Time in seconds
}

void bloomFilterSearch(const string& username) {
    auto start = chrono::steady_clock::now(); // Start time
    int h1 = hash1(username);
    int h2 = hash2(username);
    bool ch = bloomFilter[h1] && bloomFilter[h2];
    auto end = chrono::steady_clock::now(); // End time
    if (ch) {
        cout << "Username might be in the database (Bloom Filter Search).\n";
    } else {
        cout << "Username definitely not in the database (Bloom Filter Search).\n";
    }
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
    loadUsernamesFromFile(database); // Load usernames from file
    vector<string> sortedDatabase = database; // Sorted version for binary search
    unordered_map<string, bool> hashMap;      // Hash map for hash search

    sort(sortedDatabase.begin(), sortedDatabase.end());

    // Fill hash map
    for (const auto& user : database) {
        hashMap[user] = true;
    }

    // Fill bloom filter
    for (const auto& user : database) {
        int h1 = hash1(user);
        int h2 = hash2(user);
        bloomFilter[h1] = true;
        bloomFilter[h2] = true;
    }

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
            case 2:
                binarySearch(sortedDatabase, username);
                break;
            case 3:
                hashSearch(hashMap, username);
                break;
            case 4:
                bloomFilterSearch(username);
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
