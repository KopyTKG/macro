#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <format>
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
		 "items": {
			"1": {
				"path": "",
				"cmd": ""
			},
			"2": {
				"path": "",
				"cmd": ""
			},
			"3": {
				"path": "",
				"cmd": ""
			},
			"4": {
				"path": "",
				"cmd": ""
			},
			"5": {
				"path": "",
				"cmd": ""
			},
			"6": {
				"path": "",
				"cmd": ""
			}
		 } 
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
  string slot = "";
  for (int i = 1; i < argc; i++) {
    string curr = argv[i];
    string next = "";
    if (i + 1 < argc)
      next = argv[i+1];
    // set write
    if (curr == "--set")
      writeMode = true;
    if (i > 3)
      break;
    if (i == 1) {
      string path = "";
      string app = "";
      if (curr == "1") {
        path = memory["items"]["1"]["path"];
        app = memory["items"]["1"]["cmd"];
      }
      if (curr == "2") {
        path = memory["items"]["2"]["path"];
        app = memory["items"]["2"]["cmd"];
      }
      if (curr == "3") {
        path = memory["items"]["3"]["path"];
        app = memory["items"]["3"]["cmd"];
      }
      if (curr == "4") {
        path = memory["items"]["4"]["path"];
        app = memory["items"]["4"]["cmd"];
      }
      if (curr == "5") {
        path = memory["items"]["5"]["path"];
        app = memory["items"]["5"]["cmd"];
      }
      if (curr == "6") {
        path = memory["items"]["6"]["path"];
        app = memory["items"]["6"]["cmd"];
      }

      if (path != "" && app != "") {
        string cmd = "cd " + path + "/ && " + app;
        system(cmd.c_str());
      }
    }

    if (i == 2) {
      slot = curr;
    }

    if (writeMode && i == 3) {
      string memory_path = getPath() + "/memory.json";
      ofstream memWrite(memory_path);
      memory["items"][slot]["path"] = curr;
      memory["items"][slot]["cmd"] = next;
      memWrite << setw(4) << memory << endl;
      memWrite.close();
    }

    if (curr == "-l" || curr == "--list") {
      for (int k = 1; k < 6; k++) {
        string line = std::format("Project {}: {}", to_string(k),
                                  memory["items"][to_string(k)].dump());

        cout << line << endl;
      }
    }
  }

  return 0;
}
