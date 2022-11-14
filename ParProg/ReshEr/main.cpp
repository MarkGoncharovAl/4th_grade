#include <iostream>
#include <future>
#include <vector>

constexpr int N = 1000;

bool compare (const std::vector<bool>& lhs , const std::vector<bool>& rhs) {
  if (lhs.size() != rhs.size())
    return false;
  for (int i = 0; i < lhs.size(); ++i) {
    if (lhs[i] != rhs[i])
      return false;
  }
  return true;
}

void Seq (std::vector<bool> &data) {
  for ()
}

int main() {
  std::vector<bool> data(N, true);

}