#include <iostream>
#include <cassert>
#include <chrono>
#include <vector>

#include "array_processor.h"

constexpr size_t CPU_AMOUNT = 4;

void MeasureLenght(const size_t LENGHT)
{
    std::vector<int> vector;
    for (size_t i = 0; i < LENGHT; ++i) {
        vector.push_back(std::rand());
    }
    std::vector<int> out_buffer;
    ArrayProcessingSingleThread<CPU_AMOUNT> ap_s(vector);
    ArrayProcessingMultiThread<CPU_AMOUNT> ap_m(vector);
    std::cout << LENGHT << " | ";
    
    // Measure multi thread
    auto start_time = std::chrono::high_resolution_clock::now();
    ap_m.ProcessArray(&out_buffer);
    auto stop_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    std::cout << duration.count() << " | ";

    // Measure single thread
    start_time = std::chrono::high_resolution_clock::now();
    ap_s.ProcessArray(&out_buffer);
    stop_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    std::cout << duration.count() << std::endl;
}

int main()
{
    assert(false && "DO NOT RUN BENCHMARKS IN DEBUG MODE");
    std::srand(std::time(nullptr));

    std::cout << "Size | Multithread, us | Singlethread, us" << std::endl;
    for(size_t i = 1000; i < 100000000; i*=2) {
        MeasureLenght(i);
    }

    return 0;
}