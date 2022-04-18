#include <iostream>
#include "buddy_allocator.h"

int BuddyAllocator::getNextPowTwo(int x) {
  x--;
  int pow = 0;
  while (x > 0) {
    x >>= 1;
    pow++;
  }
  return pow;
}

bool BuddyAllocator::isBuddy(int addr1, int addr2, int k) {
  return (addr1 ^ addr2) == (1 << k);
}

BuddyAllocator::BuddyAllocator(int size_two_pow) : free_areas(size_two_pow + 1) {
  free_areas[size_two_pow].push_front(0);
};

pair<int,int> BuddyAllocator::allocPages(int size) {
  int k = getNextPowTwo(size);
  if (free_areas[k].empty()) {
    // no free block found  -> have to find the next free block
    int free_k = k;
    while (free_k < free_areas.size() && free_areas[free_k].empty()) {
      free_k++;
    }

    if (free_k == free_areas.size()) { // cannot find free block
      throw AllocFailException{};
    }

    // keep splitting blocks
    for (int split_k = free_k; split_k > k; split_k--) {
      int addr_split_block = free_areas[split_k].front();
      free_areas[split_k].pop_front();
      free_areas[split_k - 1].push_front(addr_split_block + (1 << (split_k - 1)));
      free_areas[split_k - 1].push_front(addr_split_block);
    }
  }

  int base_addr = free_areas[k].front();
  free_areas[k].pop_front();
  return make_pair(base_addr, 1 << k);
}

void BuddyAllocator::freePages(int base_addr, int size) {
  int k = getNextPowTwo(size);
  list<int>& free_area = free_areas[k];
  for (auto it = free_area.begin(); it != free_area.end(); it++) {
    int other_addr = *it;
    if (isBuddy(base_addr, other_addr, k)) {
      free_area.erase(it);
      freePages(min(base_addr, other_addr), size << 1);
      return;
    }
  }

  // buddy not found
  free_area.insert(lower_bound(free_area.begin(), free_area.end(), base_addr), base_addr);
}

void BuddyAllocator::printBuddyLists() {
  cout << "------------------- Buddy Lists -------------------\n";
  cout << "i | list of free blocks [start_addr, end_addr]\n";
  for (int i = free_areas.size() - 1; i >= 0; i--) {
    cout << i << " : [";
    for (int addr : free_areas[i]) {
      cout << "[" << addr << ", " << (addr + (1 << i) - 1) << "]";
      if (addr != free_areas[i].back()) {
        cout << ", ";
      }
    }
    cout << "]\n";
  }
}
