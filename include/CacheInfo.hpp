//
// Copyright [2020] <Timoshenko Yulia>
//

#ifndef INCLUDE_CACHEINFO_HPP_
#define INCLUDE_CACHEINFO_HPP_
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <vector>

class CacheInfo {
 private:
  std::vector<int> sizeOfBuf;
  int k;

 public:
  CacheInfo();
  int straightExp(int size);
  int reverseExp(int size);
  int randomExp(int size);
  void printReport(std::string& method);
  void printReports();
  static void printHeader(std::string& method);
  static void printResult(int size, int durat, int number);
  int investigate(std::string& order, int size);
};
#endif  // INCLUDE_CACHEINFO_HPP_
