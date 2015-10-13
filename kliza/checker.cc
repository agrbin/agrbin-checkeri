#include <cassert>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

// Usage:
//  VERDICT(0) << "got " << 5 << " expected " << 6;
// This will output
//  0\n
//  bok 5 a treba 6\n
// And exit with status 0.
//
//  EXIT(1) << "bye bye";
// Will output
//  bye bye\n
// And exit with status 1.
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

string ploca;
int sluzbeno_min_broj_poteza;
int kandidat_min_broj_poteza;
vector<int> kandidat_rekonstrukcija;
vector<int> sluzbeno_rekonstrukcija;

void procitaj_sluzbeni_input(ifstream &in) {
  for (int i = 0; i < 9; ++i) {
    string element;
    if (!(in >> element)) {
      VERDICT(0) << "Couldn't read " << i << "th element from official input.";
    }
    if (element.size() != 1) {
      VERDICT(0) << "Unexpected word in official input: " << element;
    }
    char znak = element[0];
    if (!(znak == 'X' || (znak >= '0' && znak <= '9'))) {
      VERDICT(0) << "Unexpected char in official input: " << element;
    }
    ploca.push_back(znak);
  }
  string sorted_ploca = ploca;
  sort(sorted_ploca.begin(), sorted_ploca.end());
  if (sorted_ploca != "12345678X") {
    VERDICT(0) << "Unexpected official input table.";
  }
}

void procitaj_sluzbeni_output(ifstream &in) {
  if (!(in >> sluzbeno_min_broj_poteza)) {
    VERDICT(0) << "Couldn't read official minimum number of moves.";
  }
  for (int i = 0; i < sluzbeno_min_broj_poteza; ++i) {
    int potez;
    if (!(in >> potez)) {
      VERDICT(0) << "Couldn't read "
        << i << "th move from official input. Expected "
        << sluzbeno_min_broj_poteza << " moves.";
    }
    sluzbeno_rekonstrukcija.push_back(potez);
  }
  int tmp;
  if (in >> tmp) {
    VERDICT(0) << "Official input contains too much data.";
  }
}

void procitaj_kandidat_output(ifstream &in) {
  if (!(in >> kandidat_min_broj_poteza)) {
    VERDICT(0) << "Couldn't read minimal number of moves.";
  }
  if (kandidat_min_broj_poteza != sluzbeno_min_broj_poteza) {
    VERDICT(0.5) << "Wrong minimal number of moves. Got: "
      << kandidat_min_broj_poteza << " Expected "
      << sluzbeno_min_broj_poteza;
  }
  for (int i = 0; i < kandidat_min_broj_poteza; ++i) {
    int potez;
    if (!(in >> potez)) {
      VERDICT(0.5) << "Couldn't read "
        << i << "th move from official input. Expected: "
        << sluzbeno_min_broj_poteza << " moves.";
    }
    kandidat_rekonstrukcija.push_back(potez);
  }
  int tmp;
  if (in >> tmp) {
    VERDICT(0.5) << "User input contains too much data.";
  }
}

int mabs(int x) {return x < 0 ? -x : x;}

TEST(mabs, {
  assert(mabs(-5) == 5);
  assert(mabs(5) == 5);
  assert(mabs(0) == 0);
});

bool jeli_susjedno(int i1, int i2) {
  int r1 = i1 / 3, s1 = i1 % 3;
  int r2 = i2 / 3, s2 = i2 % 3;
  return mabs(r1 - r2) + mabs(s2 - s1) == 1;
}

TEST(jeli_susjedno, {
    // 0 1 2
    // 3 4 5
    // 6 7 8
  assert(jeli_susjedno(0, 1));
  assert(!jeli_susjedno(0, 2));
  assert(jeli_susjedno(0, 3));
  assert(!jeli_susjedno(0, 4));
  assert(jeli_susjedno(4, 1));
  assert(jeli_susjedno(4, 5));
  assert(jeli_susjedno(4, 3));
  assert(jeli_susjedno(4, 7));
  assert(!jeli_susjedno(4, 8));
});

// kome je "official output" ili "user output".
void provjeri_rekonstrukciju(string kome, string ploca,
    const vector<int>& rekonstruckija) {
  vector<int> gdje_je(10);
  int gdje_je_x = -1;
  for (size_t i = 0; i < ploca.size(); ++i) {
    if (ploca[i] != 'X') {
      gdje_je[ploca[i] -'0'] = i;
    } else {
      gdje_je_x = i;
    }
  }
  assert(gdje_je_x != -1);
  for (size_t i = 0; i < rekonstruckija.size(); ++i) {
    int trenutni_potez = rekonstruckija[i];
    if (!jeli_susjedno(gdje_je[trenutni_potez], gdje_je_x)) {
      VERDICT(0.5) << i << "th move is invalid for " << kome << ".";
    }
    swap(ploca[gdje_je[trenutni_potez]], ploca[gdje_je_x]);
    swap(gdje_je[trenutni_potez], gdje_je_x);
  }
  if (ploca != "12345678X") {
    VERDICT(0.5) << "Final board position is " << ploca
                 << " for " << kome << ".";
  }
}

int main(int argc, char *argv[]) {
  assert(argc == 4);

  ifstream finput(argv[1]);
  ifstream fsluzb(argv[2]);
  ifstream fnatj(argv[3]);

  if (finput.fail() || fsluzb.fail() || fnatj.fail()) {
    EXIT(1) << "Internal: one of the input streams failed.";
  }

  string rj, slurj;
  procitaj_sluzbeni_input(finput);
  procitaj_sluzbeni_output(fsluzb);
  procitaj_kandidat_output(fnatj);

  provjeri_rekonstrukciju("official output", ploca, sluzbeno_rekonstrukcija);
  provjeri_rekonstrukciju("user output", ploca, kandidat_rekonstrukcija);
  VERDICT(1) << "Correct solution!";

  return 0;
}

