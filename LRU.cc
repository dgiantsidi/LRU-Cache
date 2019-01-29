#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>
#include <sstream>
#include <fstream>
using namespace std;

struct Node{
    Node* next;
    Node* prev;
    int value;
    int key;
    Node(Node* p, Node* n, int k, int val):prev(p),next(n),key(k),value(val){};
    Node(int k, int val):prev(NULL),next(NULL),key(k),value(val){};
};

class Cache{
    protected:
        map<int,Node*> mp;              //map the key to the node in the linked list
        int cp;                         //capacity
        Node* tail;                     // double linked list tail pointer
        Node* head;                     // double linked list head pointer
        virtual void set(int, int) = 0; //set function
        virtual int get(int) = 0;       //get function
};

class LRUCache : public Cache {
    public:
        LRUCache(int capacity) {
            cp = capacity;
        };

        void set (int key, int value) {
            if (mp.size() == 0) {
                Node *n = new Node(key, value);
                head = n;
                tail = n;
                mp[key] = n;
            }
            else {
                map<int,Node*>::iterator it = mp.find(key);
                if (it == mp.end()) {

                    if (mp.size() == cp) {
                      std::cout << "CACHE FULL\n";
                      Node* help_tail = tail;
                      tail = tail->prev;
                      for (map<int,Node*>::iterator i = mp.begin(); i != mp.end(); i++)
                      if (i->second == help_tail) {
                      mp.erase(i);
                      }
                      }

                    Node* n = new Node(NULL, head, key, value);
                    head->prev = n;
                    head = n;
                    mp[key] = n;
                }
                else {
                    if (mp.size() > 1) {
                        if (it->second != tail && it->second != head) {
                            // MRU
                            Node *node = it->second;
                            Node *n_prev = it->second->prev;
                            Node *n_next = it->second->next;
                            n_next->prev = n_prev;
                            n_prev->next = n_next;
                            head->prev = node;
                            node->next = head;
                            head = node;
                            node->prev = NULL;
                        }
                        else if (it->second != head) {
                            Node *n_prev = it->second->prev;
                            tail = n_prev;
                            head->prev = it->second;
                            n_prev->next = NULL;
                            it->second->next = head;
                            head = it->second;
                            head->prev = NULL;
                        }
                    }
                    head->value = value;
                }
            }
            print_cache();
            print_cache_reverse();
        };

        int get (int key) {
            map<int,Node*>::iterator it = mp.find(key);
            if (it != mp.end())
                return it->second->value;
            return -1;
        };

        void print_cache () {
            Node* temp = head;
            // std::cout<<"+++\n";
            while (temp != tail) {
                // std::cout<< "cache:" << temp->key << " " << temp->value << " \n";
                temp = temp->next;
            }
            // std::cout<< "cache:" << temp->key << " " << temp->value << " \n";
            // std::cout<<"+++\n";
        }

        void print_cache_reverse() {
            Node* temp = tail;
            // std::cout<<"---\n";
            while (temp != head) {
               // std::cout<< "cache:" << temp->key << " " << temp->value << " \n";
                temp = temp->prev;
            }
            // std::cout<< "cache:" << temp->key << " " << temp->value << " \n";
            // std::cout<<"--\n";

        }
};

int main() {
    std::ifstream inFile;
    inFile.open("file2.txt");
    int n, capacity,i;

    inFile >> n >> capacity;

    LRUCache l(capacity);
    for(i=0;i<n;i++) {
        string command;
        inFile >> command;
        if(command == "get") {
            int key;
            inFile >> key;
            cout << l.get(key) << endl;
        }
        else if(command == "set") {
            int key, value;
            inFile >> key >> value;
            l.set(key,value);
        }
    }
    return 0;
}
