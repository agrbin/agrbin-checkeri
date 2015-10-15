#include <cassert>
#include <set>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

const char INTERSECTING_ROUTES[] = "The routes are not disjoint.";
const char NOT_HAMILTON[] = "The route is not a hamiltonian path.";
const char NOT_VALID_NODE[] = "The route contains an invalid node.";
const char NOT_ENOUGH_DATA[] = "Not enough data written to output.";
const char TOO_MUCH_DATA[] = "Too much data written to output.";
const char CORRECT_SOLUTION[] = "Correct solution!";

const size_t maxn = 2000;

size_t N;
vector< vector<int> > routes;

void verdict(int score, const char* text) {
  printf("%d\n%s\n", score, text);
  exit(0);
}

void procitaj_sluzbeni_input(ifstream &in) {
  assert(in >> N);
  assert(N < maxn);
}

void procitaj_kandidat_output(ifstream &in) {
  size_t t;
  for (size_t i = 0; i < (N - 1) / 2; ++i) {
    routes.push_back(vector<int>());
    for (size_t j = 0; j < N; ++j) {
      if (!(in >> t)) {
        verdict(0, NOT_ENOUGH_DATA);
      }
      if (!(t > 0 && t <= N)) {
        verdict(0, NOT_VALID_NODE);
      }
      routes.back().push_back(t);
    }
  }
  if ((in >> t)) {
    verdict(0, TOO_MUCH_DATA);
  }
}

bool bio[maxn][maxn];

void provjeri_rekonstrukciju() {
  for (size_t i = 0; i < routes.size(); ++i) {
    const vector<int>& route = routes[i];
    if (set<int>(route.begin(), route.end()).size() != N) {
      verdict(0, NOT_HAMILTON);
    }
    for (size_t j = 0; j < N; ++j) {
      bool& bio_edge = bio[route[j]][route[(j + 1) % N]];
      if (bio_edge) {
        verdict(0, INTERSECTING_ROUTES);
      }
      bio_edge = 1;
    }
  }
}

int main(int argc, char *argv[]) {
  assert(argc == 4);

  ifstream finput(argv[1]);
  ifstream fsluzb(argv[2]);
  ifstream fnatj(argv[3]);

  assert(!(finput.fail() || fsluzb.fail() || fnatj.fail()));

  procitaj_sluzbeni_input(finput);
  procitaj_kandidat_output(fnatj);
  provjeri_rekonstrukciju();
  verdict(1, CORRECT_SOLUTION);

  return 0;
}

