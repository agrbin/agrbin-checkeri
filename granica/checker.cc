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
const char WRONG_SOLUTION[] = "Wrong solution.";
const char CORRECT_SOLUTION[] = "Correct solution!";

void verdict(int score, const char *msg) {
  printf("%d\n%s\n", score, msg);
  exit(0);
}

vector<string> sluzbeno;
vector<string> kandidat;

void procitaj_sluzbeni_output(ifstream &in) {
  for (string t; in >> t; sluzbeno.push_back(t));
}

void procitaj_kandidat_output(ifstream &in) {
  for (string t; in >> t; kandidat.push_back(t));
}

void usporedi() {
  if (kandidat.size() < sluzbeno.size()) {
    verdict(0, NOT_ENOUGH_DATA);
  }
  if (kandidat.size() > sluzbeno.size()) {
    verdict(0, TOO_MUCH_DATA);
  }
  std::sort(kandidat.begin(), kandidat.end());
  std::sort(sluzbeno.begin(), sluzbeno.end());
  if (kandidat != sluzbeno) {
    verdict(0, WRONG_SOLUTION);
  }
  verdict(1, CORRECT_SOLUTION);
}

int main(int argc, char* argv[]) {
  assert(argc == 4);
  ifstream finput(argv[1]);
  ifstream fsluzb(argv[2]);
  ifstream fnatj(argv[3]);
  assert(!(finput.fail() || fsluzb.fail() || fnatj.fail()));
  procitaj_sluzbeni_output(fsluzb);
  procitaj_kandidat_output(fnatj);
  usporedi();
  return 0;
}

