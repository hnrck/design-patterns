#include <iostream>
#include <memory>
#include <vector>

using std::cout;
using std::endl;
using std::make_unique;
using std::unique_ptr;
using std::vector;

using UpA = unique_ptr<class A>;
using UpFactory = unique_ptr<class Factory>;

class A {
protected:
  int id_;
  static int total_;

public:
  A() { id_ = total_++; }
  virtual ~A() = default;
  A(const A &) = delete;
  void operator=(const A &) = delete;
  A(A &&) = delete;
  void operator=(A &&) = delete;
  virtual void get_id() = 0;
};

int A::total_ = 0;

class A11 final : public A {
public:
  void get_id() override { cout << this << " A11 id: " << id_ << endl; }
};

class A12 final : public A {
public:
  void get_id() override { cout << this << " A12 id: " << id_ << endl; }
};

class A21 final : public A {
public:
  void get_id() override { cout << this << " A21 id: " << id_ << endl; }
};

class A22 final : public A {
public:
  void get_id() override { cout << this << " A22 id: " << id_ << endl; }
};

class Factory {
public:
  Factory() = default;
  virtual ~Factory() = default;
  Factory(const Factory &) = delete;
  void operator=(const Factory &) = delete;
  Factory(Factory &&) = delete;
  void operator=(Factory &&) = delete;
  virtual UpA make_1() = 0;
  virtual UpA make_2() = 0;
};

class Factory1 final : public Factory {
public:
  UpA make_1() override { return (make_unique<A11>()); }

  UpA make_2() override { return (make_unique<A12>()); }
};

class Factory2 final : public Factory {
public:
  UpA make_1() override { return (make_unique<A21>()); }

  UpA make_2() override { return (make_unique<A22>()); }
};

int main() {
  auto v_up_a = vector<UpA>();
  auto v_up_f = vector<UpFactory>();

  v_up_f.push_back(make_unique<Factory1>());
  v_up_f.push_back(make_unique<Factory2>());

  for (const auto &up_f : v_up_f) {
    v_up_a.push_back(up_f->make_1());
    v_up_a.push_back(up_f->make_2());
  }

  for (const auto &up_a : v_up_a) {
    up_a->get_id();
  }

  return 0;
}
