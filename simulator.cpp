#include "simulator.h"
#include <iostream>

static const int NULL_FRAME = -1;

pair<int,int> Simulator::allocPagesWithReclaim(int size) {
  while (true) {    
    try {
      return buddy_allocator.allocPages(size);
    } catch (const AllocFailException& e) {
      // reclaim if not enough space
      int frame_reclaimed = reclaim_system.reclaim();
      freeFrame(frame_reclaimed);
    }
  }
}

void Simulator::mapPageToFrame(Page page, int frame_no) {
  page_table[page] = frame_no;
  inverted_page_table[frame_no] = page;
}

void Simulator::freeFrame(int frame_no) {
  if (inverted_page_table.count(frame_no) == 0) return;
  const Page& page = inverted_page_table[frame_no];
  page_table[page] = NULL_FRAME;
  inverted_page_table.erase(frame_no);
  buddy_allocator.freePages(frame_no, 1);
}

Simulator::Simulator(int mem_sz_two_pow, int rclm_lst_sz) :
  buddy_allocator{mem_sz_two_pow},
  reclaim_system{rclm_lst_sz} {}

void Simulator::allocate(int seq_no, int num) {
  pair<int,int> alloc = allocPagesWithReclaim(num);
  int base_addr = alloc.first;
  int alloc_size = alloc.second;

  // free up unused pages
  for (int i = num; i < alloc_size; i++) {
    buddy_allocator.freePages(base_addr + i, 1);
  }

  // map page to frames
  for (int i = 0; i < num; i++) {
    int frame_no = base_addr + i;
    mapPageToFrame({seq_no, i}, frame_no);
    int evicted_frame = reclaim_system.insert(frame_no);
    if (evicted_frame != NULL_FRAME) freeFrame(evicted_frame);
  }
}

void Simulator::access(int seq_no, int num) {
  Page page{seq_no, num};
  if (page_table.count(page) == 0) return; // accessing freed page

  int frame_no = page_table[page];
  if (frame_no != NULL_FRAME) {
    // no page fault
    reclaim_system.access(frame_no);

  } else {
    // page fault -> get a free frame
    pair<int,int> alloc = allocPagesWithReclaim(1);
    frame_no = alloc.first;
    mapPageToFrame(page, frame_no);
    int evicted_frame = reclaim_system.insert(frame_no);
    if (evicted_frame != -1) freeFrame(evicted_frame);
  }
}

void Simulator::deallocate(int seq_no, int num) {
  Page page{seq_no, num};
  // page has already been freed
  if (page_table.count(page) == 0) return;

  int free_frame_no = page_table[page];
  // free frame associated with page if it exists
  if (free_frame_no != NULL_FRAME) {
    reclaim_system.remove(free_frame_no);
    freeFrame(free_frame_no);
  }

  // free page
  page_table.erase(page);
}

void Simulator::print() {
  buddy_allocator.printBuddyLists();
}
