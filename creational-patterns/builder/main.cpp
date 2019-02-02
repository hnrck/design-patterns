#include <ios>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::ios_base;
using std::make_shared;
using std::make_unique;
using std::move;
using std::ostream;
using std::pair;
using std::shared_ptr;
using std::static_pointer_cast;
using std::string;
using std::stringstream;
using std::unique_ptr;
using std::vector;

using UpStuff = unique_ptr<class Stuff>;
using SpBuilder = shared_ptr<class Builder>;

enum class ConfType {
  X,
  Y,
  Z,
};

struct ConfAttribute {
  ConfType type_;
  string val_;
};

class Stuff final {
private:
  stringstream ss_{ios_base::in | ios_base::out};

public:
  explicit Stuff(const string &s) { ss_ << this << " " << s << ": "; }
  ~Stuff() = default;
  Stuff(const Stuff &) = delete;
  Stuff(Stuff &&) = delete;
  void operator=(const Stuff &) = delete;
  void operator=(Stuff &&) = delete;
  void setX(const string &xs1, const string &xs2) { ss_ << "X( " << xs1 << ": " << xs2 << " ) "; }
  void setY(const string &ys1, const string &ys2) { ss_ << "Y( " << ys1 << ": " << ys2 << " ) "; }
  void setZ(const string &zs1, const string &zs2) { ss_ << "Z( " << zs1 << ": " << zs2 << " ) "; }
  string str() const {
    return ss_.str();
  }
};

class Builder {
protected:
  UpStuff up_stuff_;

public:
  Builder() = default;
  virtual ~Builder() = default;
  Builder(const Builder &) = delete;
  Builder(Builder &&) = delete;
  void operator=(const Builder &) = delete;
  void operator=(Builder &&) = delete;
  virtual void confX(const string &) = 0;
  virtual void confY(const string &) = 0;
  virtual void confZ(const string &) = 0;
  UpStuff get() { return (move(up_stuff_)); }
};

class ABuilder final : public Builder {
public:
  ABuilder() { up_stuff_ = make_unique<Stuff>("Stuff A"); }
  void confX(const string &xs) override { up_stuff_->setX("A", xs); };
  void confY(const string &ys) override { up_stuff_->setY("A", ys); };
  void confZ(const string &zs) override { up_stuff_->setZ("A", zs); };
};

class BBuilder final : public Builder {
public:
  BBuilder() { up_stuff_ = make_unique<Stuff>("Stuff B"); }
  void confX(const string &xs) override { up_stuff_->setX("B", xs); };
  void confY(const string &ys) override { up_stuff_->setY("B", ys); };
  void confZ(const string &zs) override { up_stuff_->setZ("B", zs); };
};

class Reader final {
private:
  SpBuilder sp_builder_;

public:
  void set_builder(SpBuilder spb) { sp_builder_ = move(spb); }
  void construct(vector<ConfAttribute> attrs) {
    for (const auto &attr : attrs) {
      switch (attr.type_) {
      case ConfType::X:
        sp_builder_->confX(attr.val_);
        break;
      case ConfType::Y:
        sp_builder_->confY(attr.val_);
        break;
      case ConfType::Z:
        sp_builder_->confZ(attr.val_);
        break;
      }
    }
  }
};

int main() {
  auto v_up_s = vector<UpStuff>();

  {
    auto v_bc = vector<pair<const SpBuilder, vector<ConfAttribute>>>{
        pair<const SpBuilder, vector<ConfAttribute>>(
            static_pointer_cast<Builder>(make_shared<ABuilder>()),
            vector<ConfAttribute>()),
        pair<const SpBuilder, vector<ConfAttribute>>(
            static_pointer_cast<Builder>(make_shared<BBuilder>()),
            vector<ConfAttribute>()),
    };
    auto &bc_a = v_bc[0];
    auto &bc_b = v_bc[1];

    auto r = Reader();

    cout << "setting A conf" << endl;
    bc_a.second.push_back({ConfType::X, "Xa"});
    bc_a.second.push_back({ConfType::Y, "Ya"});
    bc_a.second.push_back({ConfType::Z, "Za"});

    cout << "setting B conf" << endl;
    bc_b.second.push_back({ConfType::X, "Xb"});
    bc_b.second.push_back({ConfType::Z, "Zb1"});
    bc_b.second.push_back({ConfType::Z, "Zb2"});

    cout << "building confs" << endl;
    for (const auto &bc : v_bc) {
      const auto &sp_b = bc.first;
      const auto &conf = bc.second;

      r.set_builder(sp_b);
      r.construct(conf);

      v_up_s.push_back(sp_b->get());
    }
    cout << endl;
  }

  cout << "stuffs:" << endl;
  for (const auto &up_s : v_up_s) {
    cout << up_s->str() << endl;
  }

  return 0;
}
