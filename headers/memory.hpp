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
namespace Macro {
static string VERSION = R"(v2.0.3)";

static string getUser() {
  char *username = getlogin();
  if (username == nullptr) {
    throw std::runtime_error("No user. Switching to lockdown mode");
  }
  return string(username);
}

static string DIRPATH = "";
static string FILEPATH = "";
static string HELPMENU = R"(Usage: macro [name]

    -s, --set               		Set macro
    -o, --override			Set override
    -d, --delete			Delete macro
    -l, --list               		Display all macros
    -h, --help                  	Display this help message
    -v, --version               	Display the version of lea
 
If nothing is provided at all macro will display help.

Any errors please report to: <https://github.com/kopytkg/macro/issues>

usage

get macro
$ macro [name]

set macro
$ macro -s [name] [cmd] [path]

set macro with override
$ macro -o [name] [cmd] [path]

delete macro
$ macro -d [name]

list macros
$ macro -l)";


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
  string user = getUser();
  DIRPATH = "/home/" + user + "/.config/Macro";
  FILEPATH = DIRPATH + "/memory.mem";

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

static int indexOf(vector<macro> *memory, string name) {
  for (int i = 0; i < int(memory->size()); i++) {
    if ((*memory)[i].name == name)
      return i;
  }
  return -1;
}

static void add(vector<macro> *memory, string name, string cmd, string path,
                bool override = false) {
  int index = indexOf(memory, name);

  if (index != -1 && !override) {
    cout << "Macro (" + name +
                ") already exits. Use override flag to modify it."
         << endl;
    return;
  }

  if (override) {
    macro item = (*memory)[index];
    item.name = name;
    item.macro.cmd = cmd;
    item.macro.path = path;
    (*memory)[index] = item;
    return;
  }

  macro m;
  m.name = name;
  m.macro.cmd = cmd;
  m.macro.path = path;

  memory->push_back(m);
  return;
}

static void remove(vector<macro> *memory, string name) {
  int index = indexOf(memory, name);
  if (index == -1) {
    cout << "Cannot find (" + name + ") macro.";
    return;
  }
  auto old = (*memory);
  vector<macro> newMem = {};
  for (int i = 0; i < int(memory->size()); i++) {
    if (old[i].name == name)
      continue;

    newMem.push_back(old[i]);
  }
  (*memory) = newMem;
}

} // namespace Macro
#endif
