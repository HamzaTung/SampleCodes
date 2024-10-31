// Muhammad Hamza Ijaz Tung
// 21I-1562
// Assignment: 01
// Section: DS-B
// Question: 02

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

// node structure for browser history
struct Node {
    string url;
    Node* prev;
    Node* next;
    int visitCount;  // tracks how many times a URL is visited
    Node(const string& url) : url(url), prev(nullptr), next(nullptr), visitCount(1) {}
};

// browser history class handles navigation and history
class BrowserHistory {
private:
    Node* head;   // start of history
    Node* tail;   // end of history
    Node* current; // points to the current URL

public:
    BrowserHistory() : head(nullptr), tail(nullptr), current(nullptr) {}
    ~BrowserHistory() {
        Node* temp;
        while (head) {
            temp = head;
            head = head->next;
            delete temp;
        }
    }

    // visit a new URL, updating the history
    void visit(const string& url) {
        string fileName = url + ".html";
        fstream htmlFile(fileName);
        if (!htmlFile.is_open()) {
            cerr << "HTML file not found: " << fileName << endl;
            return;
        }
        if (!current || current->url != url) {
            Node* newNode = new Node(url);
            if (current) {
                if (current->next) {
                    current->next->prev = nullptr;
                }
                while (current->next) {
                    Node* temp = current->next;
                    current->next = nullptr;
                    delete temp; 
                }
            } 
            else {
                head = newNode; 
            }

            newNode->prev = current;
            if (current) {
                current->next = newNode;
            }
            current = newNode; 
        } 
        else {
            current->visitCount++; 
        }
        openHtmlFile(url); // open the webpage
        displayHistory(); // display the history
    }

    // move back in history 
    string back(int steps) {
        while (steps-- > 0 && current && current->prev) {
            current = current->prev; 
        }
        if (current) {
            cout << "Moved back to: " << current->url << endl;
            openHtmlFile(current->url);
            displayHistory(); 
            return current->url;
        }
        return "";
    }

    // move forward in history
    string forward(int steps) {
        while (steps-- > 0 && current && current->next) {
            current = current->next; 
        }
        if (current) {
            cout << "Moved forward to: " << current->url << endl;
            openHtmlFile(current->url);
            displayHistory(); 
            return current->url;
        }
        return "";
    }

    // display full browsing history
    void displayHistory() {
        Node* temp = head;
        cout << "Browser History:\n";
        while (temp) {
            cout << " - " << temp->url << " (" << temp->visitCount << " visits)" 
                      << (temp == current ? " (current)" : "") << "\n";
            temp = temp->next;
        }
        cout << endl;
    }

    // open the corresponding HTML file for the URL
    void openHtmlFile(const string& url) {
        string fileName = url + ".html"; 
        fstream htmlFile(fileName);
        if (!htmlFile.is_open()) {
            cerr << "HTML file not found: " << fileName << endl;
            return;
        }

#ifdef _WIN32
        string command = "start " + fileName;  
#elif __linux__
        string command = "xdg-open " + fileName; 
#elif __APPLE__
        string command = "open " + fileName; 
#else
        cerr << "Unsupported operating system" << endl;
        return;
#endif
        system(command.c_str()); 
    }
};

int main() {
    BrowserHistory browser;
    // visiting websites
    browser.visit("Facebook");  
    browser.visit("Safari");    
    browser.visit("YouTube"); 
    browser.back(1);            
    browser.forward(1);        

    return 0;
}
