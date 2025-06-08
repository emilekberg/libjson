#include <benchmark/benchmark.h>
#include <fstream>
#include <libjson/parse.h>
#include <sstream>

static std::string loadFile(const std::string &path) {
  std::ifstream fstream(path);
  std::ostringstream ss;
  ss << fstream.rdbuf();
  return ss.str();
}
void RegisterJsonBenchmark(const std::string &name, const std::string &path) {
  benchmark::RegisterBenchmark(name.c_str(), [path](benchmark::State &state) {
    std::string json = loadFile(path);
    for (auto _ : state) {
      auto result = libjson::parse(json);
      benchmark::DoNotOptimize(result);
    }
  });
}
int main(int argc, char **argv) {
  RegisterJsonBenchmark("MB_Parse_LargeJson", "data/large-file.json");
  RegisterJsonBenchmark("BM_ParseMtCars(32 rows)", "data/mtcars.json");
  RegisterJsonBenchmark("BM_ParseIris(150 rows)", "data/iris.json");
  RegisterJsonBenchmark("BM_ParseTitanic(891 rows)", "data/titanic.json");
  RegisterJsonBenchmark("BM_ParseHousePrice(1 460 rows)",
                        "data/house-price.json");
  RegisterJsonBenchmark("BM_ParseWeather(2 922 rows)", "data/weather.json");

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}
