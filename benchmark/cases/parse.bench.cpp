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
    std::string json = loadFile(path);
    auto tokens = libjson::tokenize(json);
    for (auto _ : state) {
      // reset position of tokenizer.
      tokens.pos = 0;
      auto result = libjson::parseValue(tokens.next(), tokens);
      benchmark::DoNotOptimize(result);
    }
  });
}

void RegisterLexerBenchmark(const std::string &name, const std::string &path) {
  benchmark::RegisterBenchmark(name.c_str(), [path](benchmark::State &state) {
    std::string json = loadFile(path);
    for (auto _ : state) {
      auto tokens = libjson::tokenize(json);
      benchmark::DoNotOptimize(tokens);
    }
  });
}
int main(int argc, char **argv) {
  std::vector<std::string> files = {"data/large-file.json"};

  for (const auto &file : files) {
    RegisterLexerBenchmark(std::format("MB_Lexer_{}", file), file);
    RegisterParserBenchmark(std::format("MB_Parse_{}", file), file);
  }
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}
