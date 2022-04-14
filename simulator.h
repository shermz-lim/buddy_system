#include "buddy_allocator.h"
#include "page_reclamation.h"

// track allocations of simulator
struct Allocation {
  int seq_no;
  int size;
  int freed;
};

struct Page {
  int seq_no;
  int id;

  bool operator==(const Page &other) const {
    return seq_no == other.seq_no && id == other.id;
  }
};

template<>
struct hash<Page> {
  size_t operator()(const Page& p) const {
    return hash<int>{}(p.seq_no) ^ (hash<int>{}(p.id) << 1);
  }
};

class Simulator {
 private:
  BuddyAllocator buddy_allocator;
  PageReclamation reclaim_system;
  unordered_map<Page, int> page_table;
  unordered_map<int, Page> inverted_page_table;
  unordered_map<int, Allocation> allocations;
 
  // alloc pages and reclaim if not enough
  pair<int,int> allocPagesWithReclaim(int size); 
  void mapPageToFrame(Page page, int frame_no);
  void freeFrame(int frame_no);

 public:
  Simulator(int mem_sz_two_pow, int rclm_lst_sz);
  void allocate(int seq_no, int num);
  void access(int seq_no, int num);
  void deallocate(int seq_no, int num);
  void print();
};
