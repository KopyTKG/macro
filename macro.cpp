#include "headers/memory.hpp"

void processArgs(int argc, char *argv[], vector<Macro::macro> *memory,
                 bool lockdown) {
  // use Macro
  if (argv[1][0] != '-' && !lockdown) {
    if (!argv[1])
      return;
    string name = string(argv[1]);
    int index = Macro::indexOf(memory, name);
    if (index == -1) {
      cout << "No macro named (" + name + ") was found" << endl;
      return;
    } else {
      Macro::macro item = (*memory)[index];
      string cmd = "cd " + item.macro.path + " && " + item.macro.cmd;
      system(cmd.c_str());
    }
    // Use command
  } else {
    int last = 0;
    bool set, override, del = false;

    for (int i = 1; i <= argc; i++) {
      bool valid = false;

      // Argv overflow
      if (!argv[i])
        break;

      auto item = string(argv[i]);

      // HELP menu output
      if (item == "-h" || item == "--help") {
        cout << Macro::HELPMENU << endl;
        valid = true;
      }

      // Version output
      if (item == "-v" || item == "--version") {
        cout << Macro::VERSION << endl;
        valid = true;
      }

      // Setter flag
      if (item == "-s" || item == "--set") {
        last = i;
        set = true;
        break;
      }

      // Override flag
      if (item == "-o" || item == "--override") {
        last = i;
        override = true;
        break;
      }

      // Delete flag
      if (item == "-d" || item == "--delete") {
        last = i;
        del = true;
        break;
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

      // Missing command check
      if (!valid && item[0] == '-') {
        cout << item + " is not valid command. Use macro -h" << endl;
        break;
      } else if (!valid) {
      }
    }

    if (del && !lockdown) {
      Macro::remove(memory, argv[last + 1]);
      return;
    }

    if ((!set && !override) || lockdown)
      return;

    if ((argc - last) < 2) {
      cout << "Not enought arguments" << endl;
      return;
    }

    if (set || override) {
      string name = argv[last + 1];
      string cmd = argv[last + 2];
      string path = "";
      if ((argc - last) == 2) {
        path = argv[last + 3];
      }
      Macro::add(memory, name, cmd, path, override);
    }
  }
}

int main(int argc, char *argv[]) {
  vector<Macro::macro> memory = {};
  bool lockdown = false;
  try {
    Macro::init(&memory);
  } catch (const std::exception &) {
    lockdown = true;
  }
  if (argc >= 2) {
    processArgs(argc, argv, &memory, lockdown);
  } else {
    cout << Macro::HELPMENU << endl;
  }
  if (!lockdown)
    Macro::dump(&memory);
  return 0;
}
