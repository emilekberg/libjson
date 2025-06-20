
#include "reg.h"
#include <benchmark/benchmark.h>
#include <fstream>
#include <libjson/parse.h>

void RegisterParserBenchmarks(const std::string &path) {
  benchmark::RegisterBenchmark(path.c_str(), [path](benchmark::State &state) {
    for (auto _ : state) {
      std::ifstream fstream(path);
      auto lexer = libjson::Lexer(fstream);
      auto result = libjson::parseValue(lexer);
      benchmark::DoNotOptimize(result);
    }
  });
}
