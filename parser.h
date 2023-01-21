#pragma once
#include <string>

struct Arguments {
  uint16_t length = 0;
  uint16_t width = 0;
  std::string input = "1233";
  std::string output = "123";
  uint64_t max_iter = 0;
  uint64_t freq = 0;
};

Arguments FillArguments(int argc, const char **argv);

void FileReader(uint64_t** sand_pile, uint64_t** sand_pile2, const std::basic_string<char>& arg);