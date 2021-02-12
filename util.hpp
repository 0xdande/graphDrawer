#ifndef UTIL_HPP
#define UTIL_HPP
#include <QDebug>

namespace util {

void Get(std::ifstream &file, int &&x) { file.ignore(x); }

template <typename T>
void Get(std::ifstream &file, T &read) {
  file >> read;
}

template <typename... T>
void ReadFileData(std::ifstream &file, T &&... els) {
  int dummy_arr[1 + sizeof...(T)] = {(Get(file, std::forward<T>(els)), 0)...};
}

std::vector<int> BytesToInts(std::string bytes) {
  std::vector<int> ints;
  if (bytes.size()) {
    std::stringstream ss(bytes);

    for (int i; ss >> i;) {
      ints.push_back(i);
      if (ss.peek() == ',') ss.ignore();
    }
  }
  return ints;
}
}  // namespace util
#endif  // UTIL_HPP
