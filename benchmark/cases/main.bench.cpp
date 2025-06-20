#include "reg.h"
#include <benchmark/benchmark.h>
int main(int argc, char **argv) {
  std::vector<std::string> files = {"data/large-file.json", "data/00.json"};

  for (const auto &file : files) {
    RegisterLexerBenchmarks(file);
    RegisterParserBenchmarks(file);
  }
  RegisterJsonGetNumberBenchmarks();
  RegisterJsonGetObjectBenchmarks();
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}
