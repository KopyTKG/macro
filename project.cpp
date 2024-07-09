#include <cstdlib>
#include <format>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <unistd.h>

#include "headers/json.hpp"

using namespace std;
using json = nlohmann::json;

string getPath() {
  string username = getlogin();
  return "/home/" + username + "/.config/project";
}

void createBase() {
  string project_path = getPath();
  auto check = filesystem::exists(project_path);
  if (!check) {
    filesystem::create_directory(project_path);
    ofstream createFile(project_path + "/memory.json");
    json memory = json::parse(R"(
		{
		 "paths": 
		    [
		      "",
		      "",
		      "",
		      "",
		      "",
		      ""
		    ]
		})");
    createFile << setw(4) << memory << endl;
    createFile.close();
  }
}

json loadMemory() {
  string memory_path = getPath() + "/memory.json";
  ifstream mem(memory_path);
  stringstream buffer;
  buffer << mem.rdbuf();
  return json::parse(buffer.str());
}


int main(int argc, char *argv[]) {
  createBase();

  json memory = loadMemory();

  bool writeMode = false;
  int slot = -1;
  for (int i = 1; i < argc; i++) {
	string curr = argv[i]; 
	// set write
	if(curr == "--set") writeMode = true;

	if(i == 1) {
		string path = "";
		if(curr == "1") path = memory["paths"][stoi(curr) -1];
		if(curr == "2") path = memory["paths"][stoi(curr) -1];
		if(curr == "3") path = memory["paths"][stoi(curr) -1];
		if(curr == "4") path = memory["paths"][stoi(curr) -1];
		if(curr == "5") path = memory["paths"][stoi(curr) -1];
		if(curr == "6") path = memory["paths"][stoi(curr) -1];

		if(path != "") {
			const string cmd = "nvim " + path;
			system(cmd.c_str());
		}
	}

	if(i == 2 ) {
	  if(curr == "1") slot = stoi(curr) - 1;	
	  if(curr == "2") slot = stoi(curr) - 1;	
	  if(curr == "3") slot = stoi(curr) - 1;	
	  if(curr == "4") slot = stoi(curr) - 1;	
	  if(curr == "5") slot = stoi(curr) - 1;	
	  if(curr == "6") slot = stoi(curr) - 1;	
	}
	
	if(writeMode && i == 3) {
	  string memory_path = getPath() + "/memory.json";
	  ofstream memWrite(memory_path);
	  memory["paths"][slot] = argv[i];
	  memWrite << setw(4) << memory << endl;
	  memWrite.close();
	}

	if(curr == "-l" || curr == "--list") {
		for(int k = 0; k < 6; k++) {
			string line = std::format("Project {}: {}", to_string(k + 1), memory["paths"][k].dump()); 

			cout << line << endl;
		}
	}

  }

  return 0;
}
