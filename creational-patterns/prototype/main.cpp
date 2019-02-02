#include <iostream>
#include <memory>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::make_unique;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::vector;

class A final {
private:
  string a1_;
  string a2_;
  string a3_;
  static unique_ptr<A> up_clone_;

  A(const A &) = default;

public:
  A() = default;
  ~A() = default;
  void operator=(const A &) = delete;
  A(A &&) = delete;
  void operator=(A &&) = delete;

  static void set_clone(unique_ptr<A> up_a) { up_clone_ = move(up_a); }

  static unique_ptr<A> clone() { return (unique_ptr<A>(new A(*up_clone_))); }

  void set_a1(const string &a1) { a1_ = a1; }
  void set_a2(const string &a2) { a2_ = a2; }
  void set_a3(const string &a3) { a3_ = a3; }

  void print() const { cout << this << " a1: " << a1_ << ", a2: " << a2_ << ", a3: " << a3_ << endl; }
};

using UpA = unique_ptr<A>;

UpA A::up_clone_;

int main() {
  {
    cout << "creating future prototype" << endl;
    auto up_proto = make_unique<A>();

    cout << "setting future prototype" << endl;
    up_proto->set_a1("toto");
    up_proto->set_a2("tata");
    up_proto->set_a3("titi");

    cout << "future prototype:" << endl;
    up_proto->print();
    cout << endl;

    A::set_clone(move(up_proto));
  }

  auto v_up_a = vector<UpA>();

  cout << "cloning prototype" << endl;
  v_up_a.push_back(A::clone());
  v_up_a.push_back(A::clone());
  v_up_a.push_back(A::clone());
  v_up_a.push_back(A::clone());

  cout << "modifying clones" << endl;
  for (auto &up_a : v_up_a) {
    static auto i = 0U;
    up_a->set_a3("t" + to_string(i++) + "ti");
  }
  cout << endl;

  cout << "prototype:" << endl;
  A::clone()->print();
  cout << endl;

  cout << "clones:" << endl;
  for (auto &up_a : v_up_a) {
    up_a->print();
  }

  return 0;
}
