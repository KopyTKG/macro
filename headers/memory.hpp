#ifndef MACRO_H
#define MACRO_H

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

using namespace std;

static string getUser() {
  string username = getlogin();
  return username;
}

const string DIRPATH = "/home/" + getUser() + "/.config/Macro";
const string FILEPATH = DIRPATH + "/memory.mem";

struct cell {
  string cmd;
  string path;
};

struct macro {
  string name;
  cell macro;
};

static void load(string path, vector<macro> *memory) {
  ifstream mem(path);
  stringstream buffer;
  buffer << mem.rdbuf();
  string data = buffer.str();

  while (!data.empty()) {
    string::size_type index = data.find_first_of("\n");
    string selected = data.substr(0, index);
    data = (index == string::npos) ? "" : data.substr(index + 1);

    if (selected.empty())
      continue;

    string::size_type nameI = selected.find("|");
    string::size_type cmdI = selected.find(":");

    if (nameI == string::npos || cmdI == string::npos) {
      cerr << "Error: Invalid format in line: " << selected << endl;
      continue;
    }

    macro m;
    m.name = selected.substr(0, nameI);
    m.macro.cmd = selected.substr(nameI + 1, cmdI - nameI - 1);
    m.macro.path = selected.substr(cmdI + 1);

    memory->push_back(m);
  }
}

static void dump(vector<macro> *memory) {
  ofstream mem(FILEPATH);
  while (!(*memory).empty()) {
    auto first = (*memory)[0];
    string line = first.name + "|" + first.macro.cmd + ":" + first.macro.path;
    mem << line << endl;

    vector<macro> oldMem = (*memory);
    (*memory) = vector<macro>{};
    for (int i = 1; i < int(oldMem.size()); i++) {
      memory->push_back(oldMem[i]);
    }
  }
}

static void init(vector<macro> *memory) {
  auto dirExist = filesystem::exists(DIRPATH);
  auto fileExist = filesystem::exists(FILEPATH);
  if (!dirExist) {
    filesystem::create_directory(DIRPATH);
  }
  if (!fileExist) {
    ofstream createFile(FILEPATH);
    createFile << setw(4) << "" << endl;
    createFile.close();
  }
  load(FILEPATH, memory);
}

#endif
