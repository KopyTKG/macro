#include "headers/memory.hpp"

int main(int argc, char *argv[]) {
  vector<macro> memory = {};
  init(&memory);

  cout << memory.size() << endl;

  for (int i = 0; i < int(memory.size()); i++) {
	cout <<memory[i].name<< "    " << memory[i].macro.cmd << "    " << memory[i].macro.path<< endl;
  }
  return 0;
}
