#include <iostream>
#include "lru.h"

LRU::LRU(int capacity) : capacity(capacity) {}

void LRU::insertAtHead(int key) {
  ll.push_front(key);
  key_node_map[key] = ll.begin();
}

void LRU::erase(int key) {
  ll.erase(key_node_map.at(key));
  key_node_map.erase(key);
}

int LRU::removeTail() {
  int key = ll.back();
  ll.pop_back();
  key_node_map.erase(key);  
  return key;
}

bool LRU::isFull() {
  return ll.size() >= capacity;
}

bool LRU::isEmpty() {
  return ll.empty();
}

bool LRU::containsKey(int key) {
  return key_node_map.count(key) > 0;
}

void LRU::print() {
  cout << "Size: " << ll.size() << endl;
  cout << "Tail ~ [";
  for (auto it = ll.rbegin(); it != ll.rend(); it++) {
    cout << *it;
    if (it != prev(ll.rend())) {
      cout << ", ";
    }
  }
  cout << "] ~ Head";
}
