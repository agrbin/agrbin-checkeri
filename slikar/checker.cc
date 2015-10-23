#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const char NOT_ENOUGH_DATA[] = "Not enough elements.";
const char TOO_MUCH_DATA[] = "Too much elements.";
const char WRONG_DIMENSIONS[] = "Output image has wrong dimensions.";
const char WRONG_DISTANCE[] = "Wrong minimal distance.";
const char WRONG_RECONSTRUCTION[] = "Wrong reconstruction (distance).";
const char IMPOSSIBLE_RECONSTRUCTION[] = "Wrong reconstruction (impossible).";
const char CORRECT_SOLUTION[] = "Correct solution!";

const int dx[4] = {0, 1, 0, 1};
const int dy[4] = {0, 0, 1, 1};

void verdict(int score, const char *msg) {
  printf("%d\n%s\n", score, msg);
  exit(0);
}

int N;
int sluzbeno_distance;
vector<string> sluzbeno;
vector<string> kandidat;

// jeli subkvadrat 'koji' cijeli u boji boja.
bool jeli_boje(int x, int y, int n, int koji, char boja) {
  n /= 2;
  int mx = x + dx[koji] * n;
  int my = y + dy[koji] * n;
  for (int i = mx; i < mx + n; ++i) {
    for (int j = my; j < my + n; ++j) {
      if (kandidat[i][j] != boja) {
        return false;
      }
    }
  }
  return true;
}

void daj_druga_dva(int prvi, int drugi, int *treci, int *cetvrti) {
  assert(prvi != drugi);
  int busy[4] = {0};
  busy[prvi] = busy[drugi] = 1;
  for (int i = 0; i < 4; ++i) {
    if (!busy[i]) {
      busy[*treci = i] = 1;
      break;
    }
  }
  for (int i = 0; i < 4; ++i) {
    if (!busy[i]) {
      *cetvrti = i;
      return;
    }
  }
  assert(false);
}

int check_possible(int x, int y, int n);

int check_possible_manji(int x, int y, int n, int koji) {
  return check_possible(x + dx[koji] * (n / 2),
      y + dy[koji] * (n / 2), n / 2);
}

int check_possible(int x, int y, int n) {
  if (n == 1) {
    return true;
  }
  assert(n % 2 == 0);
  for (int bijeli = 0; bijeli < 4; ++bijeli) {
    for (int crni = 0; crni < 4; ++crni) {
      if (bijeli == crni) {
        continue;
      }
      int mix1, mix2;
      daj_druga_dva(bijeli, crni, &mix1, &mix2);
      if (jeli_boje(x, y, n, bijeli, '1') &&
          jeli_boje(x, y, n, crni, '0') &&
          check_possible_manji(x, y, n, mix1) &&
          check_possible_manji(x, y, n, mix2)) {
        return true;
      }
    }
  }
  return false;
}

void procitaj_sluzbeni_input(ifstream &in) {
  assert(in >> N);
  for (int i = 0; i < N; ++i) {
    string line;
    assert(in >> line);
    assert((int) line.size() == N);
    sluzbeno.push_back(line);
  }
}

void procitaj_sluzbeni_output(ifstream &in) {
  assert(in >> sluzbeno_distance);
}

int get_distance(const string& lhs, const string& rhs) {
  assert(lhs.size() == rhs.size());
  assert((int) lhs.size() == N);
  int sol = 0;
  for (int i = 0; i < N; ++i) {
    sol += (lhs[i] != rhs[i]);
  }
  return sol;
}

int get_distance() {
  assert((int) sluzbeno.size() == N);
  assert((int) kandidat.size() == N);
  int sol = 0;
  for (int i = 0; i < N; ++i) {
    sol += get_distance(sluzbeno[i], kandidat[i]);
  }
  return sol;
}

void procitaj_kandidat_output(ifstream &in) {
  int kandidat_distance;
  if (!(in >> kandidat_distance)) {
    verdict(0, NOT_ENOUGH_DATA);
  }
  if (kandidat_distance != sluzbeno_distance) {
    verdict(0, WRONG_DISTANCE);
  }
  for (int i = 0; i < N; ++i) {
    string line;
    if (!(in >> line)) {
      verdict(0, NOT_ENOUGH_DATA);
    }
    if ((int) line.size() != N) {
      verdict(0, WRONG_DIMENSIONS);
    }
    kandidat.push_back(line);
  }
  {
    string tmp;
    if ((in >> tmp)) {
      verdict(0, TOO_MUCH_DATA);
    }
  }
  if (get_distance() != sluzbeno_distance) {
    verdict(0, WRONG_RECONSTRUCTION);
  }
  if (!check_possible(0, 0, N)) {
    verdict(0, IMPOSSIBLE_RECONSTRUCTION);
  }
}

int main(int argc, char* argv[]) {
  assert(argc == 4);
  ifstream finput(argv[1]);
  ifstream fsluzb(argv[2]);
  ifstream fnatj(argv[3]);
  assert(!(finput.fail() || fsluzb.fail() || fnatj.fail()));
  procitaj_sluzbeni_input(finput);
  procitaj_sluzbeni_output(fsluzb);
  procitaj_kandidat_output(fnatj);
  verdict(1, CORRECT_SOLUTION);
  return 0;
}
