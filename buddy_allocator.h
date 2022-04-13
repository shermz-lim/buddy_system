#include <list>
#include <vector>

using namespace std;

struct AllocFailException : public std::exception {
  const char* what() const throw() {
    return "Allocation failed";
  }
};

// A buddy system allocator that allocates contiguous physical frames 
// All size/base addresses are in units of pages
class BuddyAllocator {
 private:
  vector<list<int>> free_areas;
  static int getNextPowTwo(int x);
  static bool isBuddy(int addr1, int addr2, int k);

 public:
  // construct buddy allocator with 2^size_two_pow pages in total
  BuddyAllocator(int size_two_pow);
  // allocate size contiguous page frames. Returns base_addr, no. of pages frames allocated
  pair<int,int> allocPages(int size);
  // free contiguous page frames with base_addr and of size pages (size must be power of 2)
  void freePages(int base_addr, int size);

  void printBuddyLists();
};
