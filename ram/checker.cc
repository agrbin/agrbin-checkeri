#include <cassert>
#include <set>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

const char WRONG_SOLUTION[] = "Wrong solution.";
const char NOT_ENOUGH_DATA[] = "Not enough data written to output.";
const char TOO_MUCH_DATA[] = "Too much data written to output.";
const char CORRECT_SOLUTION[] = "Correct solution!";

void verdict(int score, const char* text) {
  printf("%d\n%s\n", score, text);
  exit(0);
}

int main(int argc, char *argv[]) {
  assert(argc == 4);

  ifstream finput(argv[1]);
  ifstream fsluzb(argv[2]);
  ifstream fnatj(argv[3]);

  assert(!(finput.fail() || fsluzb.fail() || fnatj.fail()));

  int a, t;
  while (fsluzb >> t) {
    if (!(fnatj >> a)) {
      verdict(0, NOT_ENOUGH_DATA);
    }
    // Neka je A vas odgovor, a T tocan odgovor.
    if (t != 0) {
      // Vas odgovor smatramo pribliznim ako vrijedi T <= A < 2T.
      if (!(t <= a && a < 2 * t)) {
        verdict(0, WRONG_SOLUTION);
      }
    } else {
      // Iznimno, ako je tocan odgovor T jednak nuli, tada i vas odgovor A mora
      // biti jednak nuli.
      if (a != 0) {
        verdict(0, WRONG_SOLUTION);
      }
    }
  }

  if ((fnatj >> a)) {
    verdict(0, TOO_MUCH_DATA);
  } else {
    verdict(1, CORRECT_SOLUTION);
  }

  return 0;
}

