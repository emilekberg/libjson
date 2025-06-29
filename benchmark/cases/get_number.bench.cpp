
#include "libjson/json-types.h"
#include <benchmark/benchmark.h>
#include <libjson/parse.h>
#include <sstream>

void RegisterJsonGetNumberBenchmarks() {
   benchmark::RegisterBenchmark("JsonGetNumber_", [](benchmark::State &state) {
      std::istringstream stream(R"([1,2,3,4,5,6,7,8,9,10])");
      auto               array = libjson::parse(stream).get<libjson::JsonArray>();
      for (auto _ : state) {
         uint64_t sum = 0;
         for (auto o : array) {
            sum += o.get<uint64_t>();
         }
         benchmark::DoNotOptimize(sum);
      }
   });
}
void RegisterJsonGetObjectBenchmarks() {
   benchmark::RegisterBenchmark("JsonGetObject_", [](benchmark::State &state) {
      std::istringstream stream(
          R"([{"key":"value"},{"key":"value"},{"key":"value"},{"key":"value"}])"
      );
      auto array = libjson::parse(stream).get<libjson::JsonArray>();
      for (auto _ : state) {
         uint64_t sum = 0;
         for (auto o : array) {
            auto obj = o.get<libjson::JsonObject>();
            benchmark::DoNotOptimize(obj);
         }
      }
   });
}
