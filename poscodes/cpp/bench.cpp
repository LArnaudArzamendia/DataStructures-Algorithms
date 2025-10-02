#include "poscode.hpp"
#include "utils.hpp"
#include <chrono>
#include <cmath>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

using Clock = std::chrono::high_resolution_clock;

static double ms_since(const Clock::time_point& t0, const Clock::time_point& t1){
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

template <typename SortFn>
std::pair<double,double> time_sort(const std::string& file, size_t n, int runs, SortFn sorter){
    std::vector<double> times;
    times.reserve(runs);

    // Cargar dataset.
    Poscode* base = readCodes(file, n);
    if (!base){ std::cerr << "No pude leer " << file << "\n"; return {NAN, NAN}; }

    for (int r = 0; r < runs; ++r){
        Poscode* A = new Poscode[n];
        for (size_t i = 0; i < n; ++i) {
            A[i] = base[i];
        }

        auto t0 = Clock::now();
        sorter(A, n);
        auto t1 = Clock::now();

        times.push_back(ms_since(t0, t1));
        delete[] A;
    }
    deleteCodes(base);

    double mu = 0.0;
    for (double v : times) mu += v;
    mu /= times.size();

    double var = 0.0;
    for (double v : times) var += (v - mu)*(v - mu);
    var /= times.size();
    double sigma = std::sqrt(var);
    return {mu, sigma};
}

int main(int argc, char** argv){
    if (argc < 3){
        std::cerr << "Uso: bench <path_txt> <N> [runs=5]\n";
        return 1;
    }
    std::string path = argv[1];
    size_t n = static_cast<size_t>(std::stoull(argv[2]));
    int runs = (argc >= 4) ? std::stoi(argv[3]) : 5;

    std::pair<double,double> r1 = time_sort(path, n, runs, [](Poscode* A, size_t N){ radix_sort(A, N); });
    double mur = r1.first, sdr = r1.second;
    std::pair<double,double> r2 = time_sort(path, n, runs, [](Poscode* A, size_t N){ quick_sort(A, N); });
    double muq = r2.first, sdq = r2.second;
    std::pair<double,double> r3 = time_sort(path, n, runs, [](Poscode* A, size_t N){ merge_sort(A, N); });
    double mum = r3.first, sdm = r3.second;

    // Tamaño | Radix | Quick | Merge
    // N | μr ± σr | μq ± σq | μm ± σm
    std::cout.setf(std::ios::fixed); std::cout.precision(2);
    std::cout << "Tamano | Radix (ms) | Quick (ms) | Merge (ms)\n";
    std::cout << n << " | " << mur << " +/- " << sdr
              << " | " << muq << " +/- " << sdq
              << " | " << mum << " +/- " << sdm << "\n";
    return 0;
}