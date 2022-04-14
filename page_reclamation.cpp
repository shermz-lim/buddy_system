#include <iostream>
#include "page_reclamation.h"

PageReclamation::PageReclamation(int capacity) : active_lst{capacity}, inactive_lst{capacity} {}

int PageReclamation::insert(int frame_no) {
  int evicted_frame_no = -1;
  if (inactive_lst.isFull()) {
    evicted_frame_no = inactive_lst.removeTail();
  }
  inactive_lst.insertAtHead(frame_no);
  return evicted_frame_no;
}

void PageReclamation::access(int frame_no) {
  if (inactive_lst.containsKey(frame_no)) {
    inactive_lst.erase(frame_no);
    if (active_lst.isFull()) {
      // demote from active lst
      inactive_lst.insertAtHead(active_lst.removeTail());
    }
    active_lst.insertAtHead(frame_no);
  }
}

int PageReclamation::reclaim() {
  if (!inactive_lst.isEmpty()) {
    return inactive_lst.removeTail();
  } else if (!active_lst.isEmpty()) {
    return active_lst.removeTail();
  } else {
    throw PageReclaimException{};
  }
}

void PageReclamation::remove(int frame_no) {
  if (inactive_lst.containsKey(frame_no)) {
    inactive_lst.erase(frame_no);
  } else if (active_lst.containsKey(frame_no)) {
    active_lst.erase(frame_no);
  }
}

void PageReclamation::print() {
  cout << "inactive list: ";
  inactive_lst.print();
  cout << endl;
  cout << "active list: ";
  active_lst.print();
  cout << endl;
}
