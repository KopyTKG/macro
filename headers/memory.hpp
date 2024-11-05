#ifndef MACRO_H
#define MACRO_H

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
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



static void load(string path,vector<macro>* memory) {
  ifstream mem(path);
  stringstream buffer;
  buffer << mem.rdbuf();
  string data = buffer.str();

while (!data.empty()) {
	string::size_type index = data.find_first_of("\n");
        string selected = data.substr(0, index);
        data = (index == string::npos) ? "" : data.substr(index + 1);

        if (selected.empty()) continue;

	string::size_type nameI = selected.find("|");
	string::size_type cmdI = selected.find(":");
        
        if (nameI == string::npos || cmdI == string::npos) {
            cerr << "Error: Invalid format in line: " << selected << endl;
            continue;
        }

        macro m;
        m.name = selected.substr(0, nameI); // Name part
        m.macro.cmd = selected.substr(nameI + 1, cmdI - nameI - 1); // Command part (between | and :)
        m.macro.path = selected.substr(cmdI + 1); // Path part (after :)

        memory->push_back(m);
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
