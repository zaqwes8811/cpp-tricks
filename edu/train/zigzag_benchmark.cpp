//
// Created by zaqwes on 17.09.2022.
//

#include <benchmark/benchmark.h>

#include "common/data_generators.hpp"
#include "zigzag.hpp"

// Define another benchmark
static const int g_size = 256;
static const int g_num_row = 5;
static void BM_ZigZag_base(benchmark::State& state) {
    std::string x = gen_random(state.range(0));
    for (auto _ : state) {
        auto result = leetcode::convert_base(x, g_num_row);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_ZigZag_base)->Arg(64);

static void BM_ZigZag_best(benchmark::State& state) {
    std::string x = gen_random(state.range(0));
    for (auto _ : state) {
        auto result = leetcode::convert_best(x, g_num_row);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_ZigZag_best)->Arg(64);

static void BM_ZigZag_best1(benchmark::State& state) {
    std::string x = gen_random(state.range(0));
    for (auto _ : state) {
        auto result = leetcode::convert_best1(x, g_num_row);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_ZigZag_best1)->Arg(64);

static void BM_ZigZag_forge(benchmark::State& state) {
    std::string x = gen_random(state.range(0));
    for (auto _ : state) {
        auto result = leetcode::convert_forge(x, g_num_row);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_ZigZag_forge)->Arg(64);

static void BM_ZigZag_arena(benchmark::State& state) {
    std::string x = gen_random(state.range(0));
    for (auto _ : state) {
        auto result = leetcode::convert_arena(x, g_num_row);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_ZigZag_arena)->Arg(64);

static void BM_ZigZag_ref(benchmark::State& state) {
    std::string x = gen_random(state.range(0));
    for (auto _ : state) {
        auto result = leetcode::convert_ref(x, g_num_row);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_ZigZag_ref)->Arg(64);
