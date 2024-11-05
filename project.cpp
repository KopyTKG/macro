#include "headers/memory.hpp"

int main(int argc, char *argv[]) {
  vector<macro> memory = {};
  init(&memory);

    cout << memory.size() << endl;
  return 0;
}
