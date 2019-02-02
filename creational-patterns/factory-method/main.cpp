#include <iostream>
#include <memory>
#include <vector>

using std::cout;
using std::endl;
using std::unique_ptr;
using std::vector;

class A {
protected:
  int id_;
  static int total_;
  A() { id_ = total_++; }

public:
  virtual ~A() = default;
  A(const A &) = delete;
  void operator=(const A &) = delete;
  A(A &&) = delete;
  void operator=(A &&) = delete;
  virtual void get_id() const = 0;
};

int A::total_ = 0;

using UpA = unique_ptr<A>;

class A1 final : public A {
private:
  friend class Factory;
  A1() = default;

public:
  ~A1() override = default;
  A1(const A1 &) = delete;
  void operator=(const A1 &) = delete;
  A1(A1 &&) = delete;
  void operator=(A1 &&) = delete;
  void get_id() const override { std::cout << this << " A1: " << id_ << std::endl; }
};

class A2 final : public A {
private:
  friend class Factory;
  A2() = default;

public:
  ~A2() override = default;
  A2(const A2 &) = delete;
  void operator=(const A2 &) = delete;
  A2(A2 &&) = delete;
  void operator=(A2 &&) = delete;
  void get_id() const override { std::cout << this << " A2: " << id_ << std::endl; }
};

class Factory final {
public:
  template <class T> static UpA create();
};

template <> UpA Factory::create<A1>() { return (unique_ptr<A>(new A1())); }

template <> UpA Factory::create<A2>() { return (unique_ptr<A>(new A2())); }

int main() {
  auto v_up_a = vector<UpA>();

  cout << "creating A1" << endl;
  v_up_a.push_back(Factory::create<A1>());

  cout << "creating A2" << endl;
  v_up_a.push_back(Factory::create<A2>());

  cout << endl;

  cout << "As:" << endl;
  for (const auto &up_a : v_up_a) {
    up_a->get_id();
  }

  return 0;
}
