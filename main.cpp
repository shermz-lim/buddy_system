#include <iostream>
#include <sstream>
#include "simulator.h"

int main() {
  Simulator simulator{9, 250};
  string line;
  while (getline(cin, line)) {
    stringstream ss; ss << line;
    char command;
    int seq_no, num;
    ss >> command >> seq_no >> num;
    switch (command) {
      case 'A':
        simulator.allocate(seq_no, num);
        break;
      case 'X':
        simulator.access(seq_no, num);
        break;
      case 'F':
        simulator.deallocate(seq_no, num);
        break;
      default:
        continue;
    }
  }
  simulator.print();
}
