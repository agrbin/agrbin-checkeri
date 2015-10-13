#include <iostream>
#include <cassert>
#include <cstdlib>
#include <string>

using namespace std;

// Usage:
//  VERDICT(0) << "bok " << 5 << " a treba " << 6;
// This will output
//  0\n
//  bok 5 a treba 6\n
// And exit with status 0.
class ExitStream {
 public:
  ExitStream(double score) : status_(0) {cout << score << endl;}
  ExitStream(string dummy, int status) : status_(status) {}

  template<class T>
  ExitStream& operator<< (const T& obj) {
    cout << obj;
    return *this;
  }

  ~ExitStream() { cout << endl; exit(status_); }
 private:
  int status_;
};
#define VERDICT(score) ExitStream(score)
#define EXIT(status) ExitStream("", status)


typedef void (*TestBody)();
class Tester {
 public:
  Tester(TestBody body) {body();}
};
#define TEST(ime, body) void TEST_##ime() body; Tester test_##ime(TEST_##ime);

int gcd(int a, int b) { return a % b == 0 ? b : gcd(b, a % b); }

TEST(gcd, {
  assert(gcd(12, 18) == 6 && gcd(12, 13) == 1);
  assert(gcd(18, 12) == 6);
});

// t: embryo.cc:34: void TEST_dummy2(): Assertion `gcd(12, 18) == 6 && gcd(12,
// 13) == 2' failed.
// TEST(dummy2, { assert(gcd(12, 18) == 6 && gcd(12, 13) == 2); });

int main() {
  VERDICT(0) << "bok " << 5 << " a treba " << 6;
  return 0;
}
