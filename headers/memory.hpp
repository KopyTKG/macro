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

const vector<macro> MEMORY = {};



static void load(string path) {
  ifstream mem(path);
  stringstream buffer;
  buffer << mem.rdbuf();
  string data = buffer.str();

}

static void init() {
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
  load(FILEPATH);
}

#endif
