//
// Copyright [2020] <Timoshenko Yulia>
//

#ifndef INCLUDE_CACHEINFO_HPP_
#define INCLUDE_CACHEINFO_HPP_
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
#include <string>

namespace cacheInfo {

std::vector<int> generateCacheInfo();
int straightExp(int size);
int reverseExp(int size);
int randomExp(int size);
void printReport(std::string& method, std::vector<int>& cacheInfo);
void printReports();
void printHeader(std::string& method);
void printResult(int size, int durat, int number);
int investigate(std::string& order, int size);
} // namespace cacheInfo
#endif  // INCLUDE_CACHEINFO_HPP_
