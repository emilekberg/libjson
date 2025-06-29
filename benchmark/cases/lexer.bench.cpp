
#include "libjson/lexer.h"
#include "reg.h"
#include <benchmark/benchmark.h>
#include <fstream>

void RegisterLexerBenchmarks(const std::string &path) {
   benchmark::RegisterBenchmark(("lexer" + path).c_str(), [path](benchmark::State &state) {
      for (auto _ : state) {
         std::ifstream fstream(path);
         for (auto token : libjson::Lexer(fstream)) {
            benchmark::DoNotOptimize(token);
         }
      }
   });
}
