#include <cassert>
#include <map>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

const char WRONG_ENERGY[] = "The final energy is wrong.";
const char NOT_FIRST[] = "The starting lotus must be the first lotus.";
const char WRONG_NUMBER_OF_MOVES[] = "The number of moves is wrong.";
const char WRONG_MOVE_GEOMETRY[] = "Invalid move: not on the same axis.";
const char WRONG_MOVE_ENERGY[] = "Invalid move: not enough energy.";
const char WRONG_MOVE_NO_LOTUS[] = "Invalid move: no lotus on coordinates.";
const char WRONG_ENERGY_SUM[] = "Final energy after moves is not correct.";

const char NOT_ENOUGH_DATA[] = "Not enough data written to output.";
const char TOO_MUCH_DATA[] = "Too much data written to output.";
const char CORRECT_SOLUTION[] = "Correct solution!";

const int maxn = 300000;
typedef pair<int, int> pii;

struct Node {
  int x, y, m;
};

size_t N, K;
Node nodes[maxn];

map<pii, Node*> lookup;

int sluzbeno_energija, sluzbeno_poteza;

void verdict(int score, const char* text) {
  printf("%d\n%s\n", score, text);
  exit(0);
}

void procitaj_sluzbeni_input(ifstream &in) {
  assert(in >> N >> K);
  for (size_t i = 0; i < N; ++i) {
    assert(in >> nodes[i].x >> nodes[i].y >> nodes[i].m);
    lookup[make_pair(nodes[i].x, nodes[i].y)] = nodes + i;
  }
}

void procitaj_sluzbeni_output(ifstream &in) {
  assert(in >> sluzbeno_energija >> sluzbeno_poteza);
}

int mabs(int x) {return x < 0 ? -x : x;}

int check_valid_move(const pii& last, const pii& current, int energy) {
  int dx = mabs(current.first - last.first);
  int dy = mabs(current.second - last.second);
  if (std::min(dx, dy) != 0) {
    verdict(0, WRONG_MOVE_GEOMETRY);
  }
  if (lookup[current] == NULL) {
    verdict(0, WRONG_MOVE_NO_LOTUS);
  }
  energy -= K;
  if (energy < 0) {
    verdict(0, WRONG_MOVE_ENERGY);
  }
  energy += lookup[current]->m;
  return energy;
}

void procitaj_kandidat_output(ifstream &in) {
  int kandidat_energija, kandidat_poteza;
  if (!(in >> kandidat_energija >> kandidat_poteza)) {
    verdict(0, NOT_ENOUGH_DATA);
  }
  if (kandidat_energija != sluzbeno_energija) {
    verdict(0, WRONG_ENERGY);
  }
  if (kandidat_poteza != sluzbeno_poteza) {
    verdict(0, WRONG_NUMBER_OF_MOVES);
  }
  pii last(nodes[0].x, nodes[0].y);
  // first move is special it will take K energy and it will
  // give energy from the first lotus. So after first move we will have
  // expected amount of energy (K - K + first_lotus_energy).
  int energy = K;
  for (int i = 0; i < kandidat_poteza; ++i) {
    pii current;
    if (!(in >> current.first >> current.second)) {
      verdict(0, NOT_ENOUGH_DATA);
    }
    if (i == 0 && lookup[current] != nodes) {
      verdict(0, NOT_FIRST);
    }
    energy = check_valid_move(last, current, energy);
    last = current;
  }
  if (energy != kandidat_energija) {
    verdict(0, WRONG_ENERGY_SUM);
  }
  if ((in >> kandidat_energija)) {
    verdict(0, TOO_MUCH_DATA);
  }
}

int main(int argc, char *argv[]) {
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

