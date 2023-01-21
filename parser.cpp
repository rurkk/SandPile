#include "parser.h"
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

Arguments FillArguments(int argc, const char **argv) {
  Arguments result;

  for (int i = 1; i < argc; i++) {
	if (argv[i][0] == '-') {
	  std::string argument = argv[i];
	  if (argument == "-l" or argument == "--length")  {
		result.length = std::atoi(argv[i + 1]);
	  } else if (argument == "-w" or argument == "--width") {
		result.width = std::atoi(argv[i + 1]);
	  } else if (argument == "-i" or argument == "--input") {
		result.input = argv[i + 1];
	  } else if (argument == "-o" or argument == "--output") {
		result.output = argv[i + 1];
	  } else if (argument == "-m" or argument == "--max-iter") {
		result.max_iter = std::atoi(argv[i + 1]);
	  } else if (argument == "-f" or argument == "--freq") {
		result.freq = std::atoi(argv[i + 1]);
	  }
	}
  }

  if (result.length == 0 or result.width == 0 or result.input == "" or result.output == "") {
	std::cout << "Error\n";
  }

  if (!std::filesystem::exists(std::filesystem::path(result.output))) {
	std::filesystem::create_directory(std::filesystem::path(result.output));
	std::cout << "Output file was not found, the required directory has been created\n";
  }
  return result;
}

void FileReader(uint64_t** sand_pile, uint64_t** sand_pile2, const std::basic_string<char>& arg){
  std::ifstream fin;
  fin.open(arg);
  if (!fin.is_open()){
    std::cout << "Error: No such file in this directory";
  }
  int x;
  int y;
  int val;
  while (fin >> x >> y >> val) {
    sand_pile[x][y] = val;
    sand_pile2[x][y] = val;
  }
}
