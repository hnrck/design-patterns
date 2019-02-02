#include <iostream>
#include <memory>
#include <vector>

using std::cout;
using std::endl;
using std::shared_ptr;
using std::vector;

using SpA = shared_ptr<class A>;

class A final {
private:
  A() = default;

public:
  static SpA &get_instance() {
    static auto &&instance = SpA(new A());
    return (instance);
  }

  ~A() = default;
  A(const A &) = delete;

  void operator=(const A &) = delete;

  A(A &&) = default;

  A &operator=(A &&) = default;

  void call() const {
    static auto counter = 0;
    cout << this << " " << counter++ << endl;
  }
};

int main() {
  auto v_sp_a = vector<SpA>();

  v_sp_a.push_back(A::get_instance());
  v_sp_a.push_back(A::get_instance());

  for (const auto &sp_a : v_sp_a) {
    sp_a->call();
  }

  for (const auto &sp_a : v_sp_a) {
    sp_a->call();
  }

  return 0;
}
