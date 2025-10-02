#ifndef __UTILS_HPP__
#define __UTILS_HPP__
#include "poscode.hpp"
#include <string>
void quick_sort(Poscode *A, size_t n);
void merge_sort(Poscode *A, size_t n);
void radix_sort(Poscode *A, size_t n);

Poscode *readCodes(const std::string &strfile, size_t n);
void deleteCodes(Poscode *codes);
#endif

