#include <benchmark/benchmark.h>
#include <format>
#include <fstream>
#include <libjson/parse.h>
#include <sstream>

static std::string loadFile(const std::string &path) {
  std::ifstream fstream(path);
  std::ostringstream ss;
  ss << fstream.rdbuf();
  return ss.str();
}
void RegisterParserBenchmark(const std::string &name, const std::string &path) {
  benchmark::RegisterBenchmark(name.c_str(), [path](benchmark::State &state) {
    // std::string json = loadFile(path);
    for (auto _ : state) {

      std::ifstream fstream(path);
      auto lexer = libjson::Lexer(fstream);
      auto result = libjson::parseValue(lexer);
      benchmark::DoNotOptimize(result);
    }
  });
}

void RegisterLexerBenchmark(const std::string &name, const std::string &path) {
  benchmark::RegisterBenchmark(name.c_str(), [path](benchmark::State &state) {
    // std::string json = loadFile(path);
    for (auto _ : state) {
      std::ifstream fstream(path);
      for (auto token : libjson::Lexer(fstream)) {
        benchmark::DoNotOptimize(token);
      }
    }
  });
}
int main(int argc, char **argv) {

  std::vector<std::string> files = {"data/large-file.json", "data/00.json"};

  for (const auto &file : files) {
    RegisterLexerBenchmark(std::format("MB_Lexer_{}", file), file);
    RegisterParserBenchmark(std::format("MB_Parse_{}", file), file);
  }
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}
