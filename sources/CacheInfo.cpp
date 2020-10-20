//
// Copyright [2020] <Timoshenko Yulia>
//

#include "CacheInfo.hpp"

std::vector<int> cacheInfo::generateCacheInfo() {
  std::vector<int> sizeOfBuf = std::vector<int>();
  for (int i = 64; i <= 4096; i *= 2) {
    sizeOfBuf.push_back(i * 4);
  }
  sizeOfBuf.push_back(2048 * 3 * 4);
  return sizeOfBuf;
}

int cacheInfo::straightExp(int Size) {
  int k;
  int *arr = new int[Size];
  for (int i = 0; i < Size; i += 16)  // прогрев
    k = arr[i];
  std::chrono::microseconds base =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch());
  for (int t = 0; t < 1000; t++) {
    for (int i = 0; i < Size; i += 16)  // чтение
      k = arr[i];
  }
  std::chrono::microseconds delta =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch()) -
      base;
  arr[1] = k;
  return delta.count();
}

int cacheInfo::reverseExp(int Size) {
  int k;
  int *arr = new int[Size];
  for (int i = Size - 1; i >= 0; i -= 16)  // прогрев
    k = arr[i];
  std::chrono::microseconds base =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch());
  for (int t = 0; t < 1000; t++) {
    for (int i = Size - 1; i >= 0; i -= 16)  // чтение
      k = arr[i];
  }
  std::chrono::microseconds delta =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch()) -
      base;
  arr[1] = k;
  return delta.count();
}

int cacheInfo::randomExp(int size) {
  int k;
  std::vector<int> indOrder = std::vector<int>();
  for (int i = 0; i < size; i += 16) {
    indOrder.push_back(i);
  }
  auto random = std::default_random_engine{};
  std::shuffle(std::begin(indOrder), std::end(indOrder), random);

  int *arr = new int[size];
  for (size_t i = 0; i < indOrder.size(); i++)  // прогрев
    k = arr[indOrder[i]];
  std::chrono::microseconds base =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch());
  for (int t = 0; t < 1000; t++) {
    for (size_t i = 0; i < indOrder.size(); i++) k = arr[indOrder[i]];
  }
  std::chrono::microseconds delta =
      std::chrono::duration_cast<std::chrono::microseconds>(
          std::chrono::system_clock::now().time_since_epoch()) -
      base;
  arr[1] = k;
  return delta.count();
}

int cacheInfo::investigate(std::string &method, int size) {
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
void cacheInfo::printHeader(std::string &method) {
  std::cout << "investigation:" << '\n';
  std::cout << "  travel_variant: " << method << '\n';
  std::cout << "  experiments:" << '\n';
}

void cacheInfo::printResult(int size, int durat, int number) {
  std::cout << "  - experiment:" << '\n';
  std::cout << "  number:" << number + 1 << '\n';
  std::cout << "  input_data:" << '\n';
  if (size > 4095)
    std::cout << "    buffer_size: " << size / 4096 << "mb" << '\n';
  else
    std::cout << "    buffer_size: " << size / 4 << "kb" << '\n';
  std::cout << "  results:" << '\n';
  std::cout << "    duration: " << durat << " microseconds" << '\n';
}

void cacheInfo::printReport(std::string &method, std::vector<int> &cacheInfo) {
  printHeader(method);
  for (size_t i = 0; i < cacheInfo.size(); i++) {
    printResult(cacheInfo[i], investigate(method, cacheInfo[i]), i);
  }
}

void cacheInfo::printReports() {
  std::vector<int> caches = generateCacheInfo();
  std::vector<std::string> methods = std::vector<std::string>();
  methods.emplace_back("straight");
  methods.emplace_back("reverse");
  methods.emplace_back("random");
  for (std::string s : methods) {
    printReport(s, caches);
  }
}
