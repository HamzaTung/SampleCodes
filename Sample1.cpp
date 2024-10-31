// Muhammad Hamza Ijaz Tung
// 21I-1562
// Assignment: 01
// Section: DS-B
// Question: 01

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// node structure for the cache
struct CacheNode {
    int address;
    string data; 
    bool dBit; // dirty bit for write policies
    CacheNode* next;
    CacheNode(int addr, const string& d) : address(addr), data(d), dBit(false), next(nullptr) {}
};

class Cache {
private:
    CacheNode* head; // points to the first node 
    string policyHit, policyMiss;

    // hexa to decimal conversion
    int HexToDec(const string& hexStr) {
        int decValue = 0;
        for (char c : hexStr) {
            decValue *= 16; 
            if (c >= '0' && c <= '9') {
                decValue += c - '0';
            } 
            else if (c >= 'A' && c <= 'F') {
                decValue += c - 'A' + 10;
            } 
            else if (c >= 'a' && c <= 'f') {
                decValue += c - 'a' + 10;
            }
        }
        return decValue;
    }

    // decimal to hexa conversion
    string DecToHex(int decValue) {
        string hexStr;
        while (decValue > 0) {
            int remainder = decValue % 16;
            if (remainder < 10) {
                hexStr = char(remainder + '0') + hexStr;
            } 
            else {
                hexStr = char(remainder - 10 + 'A') + hexStr;
            }
            decValue /= 16;
        }
        return hexStr.empty() ? "0" : hexStr; 
    }

public:
    Cache() : head(nullptr) {}

    // cache policies for hits and misses
    void setPolicies(const string& hitPolicy, const string& missPolicy) {
        policyHit = hitPolicy;
        policyMiss = missPolicy;
    }

    // check if address is in cache
    bool load(int address) {
        CacheNode* temp = head;
        while (temp != nullptr) {
            if (temp->address == address) {
                cout << "Cache Hit: Address found.\n";
                return true;
            }
            temp = temp->next;
        }
        cout << "Cache Miss: Address not found.\n";
        return false;
    }

    // store data in cache and update memory (only if required)
    void store(int address, const string& hexData, fstream& memoryFile) {
        int data = HexToDec(hexData);
        CacheNode* temp = head;
        while (temp != nullptr) {
            if (temp->address == address) {
                cout << "Cache Hit: Updating data in cache.\n";
                temp->data = to_string(data);
                if (policyHit == "Through") {
                    updateMemory(address, hexData, memoryFile);
                } 
                else {
                    temp->dBit = true;
                }
                return;
            }
            temp = temp->next;
        }
        cout << "Cache Miss: Storing new address in cache.\n";
        appendCache(address, to_string(data));
        if (policyMiss == "Allocate") {
            updateMemory(address, hexData, memoryFile);
        }
    }

    // add new node to cache
    void appendCache(int address, const string& data) {
        CacheNode* newNode = new CacheNode(address, data);
        if (head == nullptr) {
            head = newNode;
        } 
        else {
            CacheNode* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    // update the memory file with the new data
    void updateMemory(int address, const string& hexData, fstream& memoryFile) {
        string line;
        ofstream tempFile("temp.csv");
        memoryFile.clear(); // resets file flags
        memoryFile.seekg(0); // moves to the start of the file
        while (getline(memoryFile, line)) {
            if (line.substr(0, line.find(',')) == to_string(address)) {
                tempFile << address << "," << hexData << "\n"; 
            } 
            else {
                tempFile << line << "\n";
            }
        }
        memoryFile.close();
        tempFile.close();
        remove("Memory.csv");
        rename("temp.csv", "Memory.csv");
        memoryFile.open("Memory.csv", ios::in | ios::out);
    }
};

int main() {
    Cache cache;
    fstream memoryFile("Memory.csv", ios::in | ios::out);
    if (!memoryFile.is_open()) {
        cerr << "Failed to open Memory.csv file.\n";
        return 1;
    }

    string hitPolicy, missPolicy;
    cout << "Enter Cache Hit Policy (Through or Back): ";
    cin >> hitPolicy;
    cout << "Enter Cache Miss Policy (Allocate or Around): ";
    cin >> missPolicy;
    cache.setPolicies(hitPolicy, missPolicy);
    
    int address;
    string data;
    while (true) {
        cout << "Enter address (decimal) and data (hexa) or -1 to end: ";
        cin >> address;
        if (address == -1) break;
        cin >> data;
        if (!cache.load(address)) {
            cache.store(address, data, memoryFile);
        }
    }
    memoryFile.close();
    
    return 0;
}
