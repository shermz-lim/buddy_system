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

void LRU::print() {
  for (auto it = ll.begin(); it != ll.end(); it++) {
    cout << *it;
    if (it != prev(ll.end())) {
      cout << ", ";
    }
  }
}
