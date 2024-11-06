#include "headers/memory.hpp"

static string HELPMENU = R"(Usage: macro [name]

    -s, --set               		Set macro
      -o				Set override
    -d, --delete			Delete macro
    -l, --list               		Display all macros
    -h, --help                  	Display this help message
    --version               		Display the version of lea
 
If nothing is provided at all macro will display help.

Any errors please report to: <https://github.com/kopytkg/macro/issues>

usage

get macro
$ macro [name]

set macro
$ macro -s [name] [cmd] [path]

set macro with override
$ macro -s -o [name] [cmd] [path]

delete macro
$ macro -d [name]

list macros
$ macro -l)";

static string VERSION = R"(v2.0.0)";

void processArgs(int argc, char *argv[], vector<macro> *memory) {
  if (argc < 2) {
    return;
  }

  if (argv[1][0] != '-') {
    if (!argv[1])
      return;
    auto item = string(argv[1]);
    cout << "No macro named (" + item + ") was found" << endl;
  } else {
    for (int i = 1; i <= argc; i++) {
      bool valid = false;

      if (!argv[i])
        break;
      auto item = string(argv[i]);

      // HELP menu output
      if (item == "-h" || item == "--help") {
        cout << HELPMENU << endl;
        valid = true;
      }

      // Version output
      if (item == "--version") {
        cout << VERSION << endl;
        valid = true;
      }

      // List output
      if (item == "-l" || item == "--list") {
        for (int k = 0; k < int(memory->size()); k++) {
          cout << (*memory)[k].name + "|" + (*memory)[k].macro.cmd + ":" +
                      (*memory)[k].macro.path
               << endl;
        }
        valid = true;
      }

      if (!valid && item[0] == '-') {
        cout << item + " is not valid command. Use macro -h" << endl;
        break;
      } else if (!valid) {
      }
    }
  }
}

int main(int argc, char *argv[]) {
  vector<macro> memory = {};
  init(&memory);
  if (argc >= 2) {
    processArgs(argc, argv, &memory);
  } else {
    cout << HELPMENU << endl;
  }

  dump(&memory);
  return 0;
}
