#include "lru.h"

class PageReclaimException : std::exception {
  const char* what() const throw() {
    return "page reclamation failed. No more frames to reclaim.";
  }
};

class PageReclamation {
 private:
  LRU active_lst;
  LRU inactive_lst;

 public:
  PageReclamation(int capacity);
  // insert frame into inactive_lst, and return evicted frame_no if any (else return -1)
  int insert(int frame_no);
  // access the frame no -> promote to active list if currently in inactive list
  void access(int frame_no);
  // reclaim a frame. Take from tail of inactive_lst, else the tail of active_lst. Returns frame reclaimed.
  // Throws an exception when there are no more frames to reclaim.
  int reclaim();
  // remove a frame from the two lists
  void remove(int frame_no);
  void print();
};
