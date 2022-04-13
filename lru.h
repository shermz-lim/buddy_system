#include <list>
#include <unordered_map>

using namespace std;

// Generic LRU class
class LRU {
 private:
  list<int> ll;
  unordered_map<int,list<int>::iterator> key_node_map;
  int capacity;

 public:
  LRU(int capacity);
  // insert a key at head of LRU
  void insertAtHead(int key);
  // erase a key from LRU
  void erase(int key);
  // remove key at tail of LRU and return it
  int removeTail();
  bool isFull();
  bool isEmpty();
  bool containsKey(int key);
  void print();
};
