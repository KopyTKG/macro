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



static void load(string path,vector<macro> *memory) {
  ifstream mem(path);
  stringstream buffer;
  buffer << mem.rdbuf();
  string data = buffer.str();

  while (data != "") {
    int index = data.find_first_of("\n");
    string selected = data.substr(0, index);
    data = data.substr(index, data.length());
    if(index == 0 ) {
    	selected = data;
	data = "";
    }
    while (selected != "") {
	int nameI = selected.find("|");	
	int cmdI = selected.find(":");

	vector<macro> tmp = {macro()};
	tmp[0].name = selected.substr(0, nameI);
	tmp[0].macro = cell();
	tmp[0].macro.cmd = selected.substr(nameI, cmdI);
	tmp[0].macro.path = selected.substr(cmdI, selected.length()-1);
	(*memory).insert((*memory).end(), tmp.begin(), tmp.end());
	selected = "";
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
