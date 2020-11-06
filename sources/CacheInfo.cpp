//
// Copyright [2020] <Timoshenko Yulia>
//

#include "CacheInfo.hpp"
const int intsize = 4;
const size_t numIter = 1000;
const int zero = 0;
const size_t cacheLine = 16;

CacheInfo::CacheInfo() {
  sizeOfBuf = std::vector<int>();
  for (int i = 64; i <= 4096; i *= 2) {
    sizeOfBuf.push_back(i * intsize);
  }
  sizeOfBuf.push_back(2048 * 3 * intsize);
  k = 0;
}

int CacheInfo::straightExp(int Size) {
  int *arr = new int[Size];
  for (int i = zero; i < Size; i += cacheLine)  // прогрев
    k = arr[i];
  std::chrono::microseconds base =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch());
  for (size_t t = zero; t < numIter; t++) {
    for (int i = zero; i < Size; i += cacheLine)  // чтение
      k = arr[i];
  }
  std::chrono::microseconds delta =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch()) -
      base;
  return delta.count();
}

int CacheInfo::reverseExp(int Size) {
  int *arr = new int[Size];
  for (int i = Size - 1; i >= 0; i -= cacheLine)  // прогрев
    k = arr[i];
  std::chrono::microseconds base =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch());
  for (size_t t = zero; t < numIter; t++) {
    for (int i = Size - 1; i >= 0; i -= cacheLine)  // чтение
      k = arr[i];
  }
  std::chrono::microseconds delta =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch()) -
      base;
  return delta.count();
}

int CacheInfo::randomExp(int size) {
  std::vector<int> indOrder = std::vector<int>();
  for (int i = zero; i < size; i += cacheLine) {
    indOrder.push_back(i);
  }
  auto random = std::default_random_engine{};
  std::shuffle(std::begin(indOrder), std::end(indOrder), random);

  int *arr = new int[size];
  for (size_t i = zero; i < indOrder.size(); i++)  // прогрев
    k = arr[indOrder[i]];
  std::chrono::microseconds base =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch());
  for (int t = zero; t < numIter; t++) {
    for (size_t i = zero; i < indOrder.size(); i++) k = arr[indOrder[i]];
  }
  std::chrono::microseconds delta =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch()) -
      base;
  return delta.count();
}

int CacheInfo::investigate(std::string &method, int size) {
  int r;
  if (method == "straight") {
    r = straightExp(size);
  } else if (method == "random") {
    r = randomExp(size);
  } else if (method == "reverse") {
    r = reverseExp(size);
  } else {
    throw std::invalid_argument("invalid name of method");
  }
  return r;
}
void CacheInfo::printHeader(std::string &method) {
  std::cout << "investigation:" << '\n';
  std::cout << "  travel_variant: " << method << '\n';
  std::cout << "  experiments:" << '\n';
}

void CacheInfo::printResult(int size, int durat, int number) {
  std::cout << "  - experiment:" << '\n';
  std::cout << "  number:" << number + 1 << '\n';
  std::cout << "  input_data:" << '\n';
  if (size > 4095)
    std::cout << "    buffer_size: " << size / 4096 << "mb" << '\n';
  else
    std::cout << "    buffer_size: " << size / intsize << "kb" << '\n';
  std::cout << "  results:" << '\n';
  std::cout << "    duration: " << durat << " microseconds" << '\n';
}

void CacheInfo::printReport(std::string &method) {
  printHeader(method);
  for (size_t i = zero; i < sizeOfBuf.size(); i++) {
    printResult(sizeOfBuf[i], investigate(method, sizeOfBuf[i]), i);
  }
}

void CacheInfo::printReports() {
  std::vector<std::string> methods = std::vector<std::string>();
  methods.emplace_back("straight");
  methods.emplace_back("reverse");
  methods.emplace_back("random");
  for (std::string s : methods) {
    printReport(s);
  }
}
