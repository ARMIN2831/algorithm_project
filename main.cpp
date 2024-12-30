#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <random>
#include <cmath>

using namespace std;

float TimeTaken;
double pi = 0.1;
vector<bool> bloomFilter; // Using vector<bool> instead of bitset

// Function to calculate the False Positive Rate for Bloom Filter
double calculateFalsePositiveRate(int n, int filterSize, int hashCount) {
    return pow(1 - exp(-((double)hashCount * n) / filterSize), hashCount);
}

// Function to calculate the number of hash functions
int getHashFunctionCount(int n, float p) {
    return round((bloomFilter.size() / n) * log(2));
}

// Function to calculate the size of the Bloom filter
int getBloomFilterSize(int n, float p) {
    return round(- (n * log(p)) / (log(2) * log(2)));
}

int hash1(const string& key, int filterSize) {
    int hash = 0;
    for (char c : key) {
        hash = (hash * 31 + c) % filterSize;
    }
    return hash;
}

int hash2(const string& key, int filterSize) {
    int hash = 0;
    for (char c : key) {
        hash = (hash * 17 + c) % filterSize;
    }
    return hash;
}

void binarySearch(const vector<string>& sortedDb, const string& username) {
    auto start = chrono::steady_clock::now(); // Start time
    int left = 0;
    int right = sortedDb.size() - 1;
    bool found = false;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (sortedDb[mid] == username) {
            found = true;
            break;
        }
        if (sortedDb[mid] < username) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    auto end = chrono::steady_clock::now(); // End time
    if (found) {
        cout << "Username found (Binary Search).\n";
    } else {
        cout << "Username not found (Binary Search).\n";
    }
    chrono::duration<double> duration = end - start;
    cout << "Time taken: " << fixed << setprecision(10) << duration.count() << " seconds.\n";
    TimeTaken = duration.count(); // Time in seconds
}

void linearSearch(const vector<string>& db, const string& username) {
    auto start = chrono::steady_clock::now(); // Start time
    for (const auto& user : db) {
        if (user == username) {
            auto end = chrono::steady_clock::now(); // End time
            cout << "Username found (Linear Search).\n";
            chrono::duration<double> duration = end - start;
            cout << "Time taken: " << fixed << setprecision(10) << duration.count() << " seconds.\n";
            TimeTaken = duration.count(); // Time in seconds
            return;
        }
    }
    auto end = chrono::steady_clock::now(); // End time after loop
    cout << "Username not found (Linear Search).\n";
    chrono::duration<double> duration = end - start;
    cout << "Time taken: " << fixed << setprecision(10) << duration.count() << " seconds.\n";
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
    cout << "Time taken: " << fixed << setprecision(10) << duration.count() << " seconds.\n";
    TimeTaken = duration.count(); // Time in seconds
}

void bloomFilterSearch(const string& username, int filterSize, int hashCount) {
    auto start = chrono::steady_clock::now(); // Start time
    bool ch = true;
    for (int i = 0; i < hashCount; ++i) {
        int h = hash1(username, filterSize) + i * hash2(username, filterSize); // Combining hash functions
        ch = ch && bloomFilter[h % filterSize];
    }
    auto end = chrono::steady_clock::now(); // End time
    if (ch) {
        cout << "Username found (Bloom Filter Search).\n";
    } else {
        cout << "Username not found (Bloom Filter Search).\n";
    }
    chrono::duration<double> duration = end - start;
    cout << "Time taken: " << fixed << setprecision(10) << duration.count() << " seconds.\n";
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

void generateRandomUsernamesFromFile(const vector<string>& database, int count, const vector<string>& sortedDatabase, const unordered_map<string, bool>& hashMap, int hashCount) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, database.size() - 1);
    vector<string> randomUsernames;

    for (int i = 0; i < count; ++i) {
        int index = dis(gen);
        randomUsernames.push_back(database[index]);
    }
    double one,two,three,four;
    int c = 1;
    ofstream timeFile("execution_times.csv", ios::app);
    for (const auto& username : randomUsernames) {
        timeFile << c++ << endl;
        
        linearSearch(database, username);
        //timeFile << username << "," << database.size() << "," << fixed << setprecision(10) << TimeTaken << "," << "Linear Search" << endl;
        one += TimeTaken;
        binarySearch(sortedDatabase, username);
        //timeFile << username << "," << database.size() << "," << fixed << setprecision(10) << TimeTaken << "," << "Binary Search" << endl;
        two += TimeTaken;
        hashSearch(hashMap, username);
        //timeFile << username << "," << database.size() << "," << fixed << setprecision(10) << TimeTaken << "," << "Hash Search" << endl;
        three += TimeTaken;
        bloomFilterSearch(username, bloomFilter.size(), hashCount);
        //timeFile << username << "," << database.size() << "," << fixed << setprecision(10) << TimeTaken << "," << "Bloom Filter Search" << endl;
        four += TimeTaken;
    }
    timeFile << "Linear Search: " << one/count << endl
             << "Binary Search: " << two/count << endl
             << "Hash Search: " << three/count << endl
             << "Bloom Filter Search: " << four/count << endl;
    timeFile.close();

    cout << "Search results for 10000 random usernames have been saved to 'execution_times.csv'.\n";
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

    // Calculate the number of hash functions and the size of the Bloom filter
    int filterSize = getBloomFilterSize(database.size(), pi);
    bloomFilter.resize(filterSize); // Now using resize for vector<bool>

    // Number of hash functions
    int hashCount = getHashFunctionCount(database.size(), pi);

    int actualFalsePositiveRate = round(calculateFalsePositiveRate(database.size(), filterSize, hashCount) * 100);

    // Fill the Bloom filter
    for (const auto& user : database) {
        for (int i = 0; i < hashCount; ++i) {
            int h = hash1(user, filterSize) + i * hash2(user, filterSize); // Combining hash functions
            bloomFilter[h % filterSize] = true;
        }
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
        cout << "5. Make 10000 random usernames for all methods\n";
        cin >> choice;
        int bloomFilter = 0;
        switch (choice) {
            case 1:
                linearSearch(database, username);
                break;
            case 2:
                binarySearch(database, username);
                break;
            case 3:
                hashSearch(hashMap, username);
                break;
            case 4:
                bloomFilter = 1;
                bloomFilterSearch(username, filterSize, hashCount);
                break;
            case 5:
                generateRandomUsernamesFromFile(database, 1000, sortedDatabase, hashMap, hashCount);
                break;
            default:
                cout << "Invalid choice!\n";
        }
        resultFile << "Searching for: " << username << endl
           << "Time taken: " << fixed << setprecision(10) << TimeTaken << " seconds" << endl
           << "Search method: " << choice << endl; // Save search details to file
        if (bloomFilter == 1){
            resultFile << "filter Size: " << filterSize << endl << "Hash functions: " << hashCount << endl << "False Positive Rate: 10" << actualFalsePositiveRate << endl;
            cout << "filter Size: " << filterSize << endl << "Hash functions: " << hashCount << endl << "False Positive Rate: " << actualFalsePositiveRate << endl;
        } 
           
        resultFile << "----------------------------------------\n"; // Separator
        cout << "----------------------------------------\n";
    }
    resultFile.close();
    return 0;
}
