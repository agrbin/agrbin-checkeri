#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <algorithm>

using namespace std;

const char NOT_ENOUGH_DATA[] = "Not enough data.";
const char TOO_MUCH_DATA[] = "Too much data.";
const char WRONG_SOLUTION[] = "Wrong solution.";
const char CORRECT_SOLUTION[] = "Correct solution!";

void verdict(int score, const char *msg) {
  printf("%d\n%s\n", score, msg);
  exit(0);
}

int main(int argc, char *argv[]) {
  assert(argc == 4);
  ifstream finput(argv[1]);
  ifstream fsluzb(argv[2]);
  ifstream fnatj(argv[3]);
  assert(!(finput.fail() || fsluzb.fail() || fnatj.fail()));

  string sluzbeno;
  string kandidat;
  while (fsluzb >> sluzbeno) {
    if (!(fnatj >> kandidat)) {
      verdict(0, NOT_ENOUGH_DATA);
    }
    if (sluzbeno != kandidat) {
      verdict(0, WRONG_SOLUTION);
    }
  }

  if ((fnatj >> kandidat)) {
    verdict(0, TOO_MUCH_DATA);
  }
  verdict(1, CORRECT_SOLUTION);
  return 0;
}

