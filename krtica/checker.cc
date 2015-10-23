#include <cassert>
#include <cstring>
#include <queue>
#include <cstdlib>
#include <set>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

const char WRONG_DISTANCE[] = "The shortest distance is wrong.";
// after this test is passed we must give 0.7

const char HALLWAY_INVALID[] = "Invalid hallway in reconstruction (node).";
const char CLOSING_CLOSED_HALLWAY[] = "Invalid hallway in reconstruction (c).";
const char OPENING_OPENED_HALLWAY[] = "Invalid hallway in reconstruction (o).";
const char WRONG_RECONSTRUCTION[] = "Wrong reconstruction.";
const char WRONG_RECONSTRUCTION_TREE[] = "Wrong reconstruction. Not a tree.";

const char NOT_ENOUGH_DATA[] = "Not enough data written to output.";
const char NOT_ENOUGH_DATA_REC[] = "Not enough data in reconstruction.";
const char TOO_MUCH_DATA[] = "Too much data in reconstruction.";
const char CORRECT_SOLUTION[] = "Correct solution!";

const int maxn = 300000;

int N;
// if graph[a] contains b, then graph[b] contains a.
vector<int> graph[maxn];
int sluzbeno_rjesenje;

void verdict(float score, const char* text) {
  printf("%.1f\n%s\n", score, text);
  exit(0);
}

bool valid_node(int x) { return x >= 0 && x < N; }

bool ima_edge(int a, int b) {
  assert(valid_node(a) && valid_node(b));
  for (size_t i = 0; i < graph[a].size(); ++i) {
    if (graph[a][i] == b) {
      return true;
    }
  }
  return false;
}

void dodaj_edge(int a, int b) {
  assert(valid_node(a) && valid_node(b));
  graph[a].push_back(b);
  graph[b].push_back(a);
}

void izbrisi_element(vector<int>& v, int x) {
  vector<int> novi;
  for (size_t i = 0; i < v.size(); ++i) {
    if (v[i] != x) {
      novi.push_back(v[i]);
    }
  }
  assert(novi.size() + 1 == v.size());
  v = novi;
}

void izbrisi_edge(int a, int b) {
  assert(valid_node(a) && valid_node(b));
  izbrisi_element(graph[a], b);
  izbrisi_element(graph[b], a);
}

void procitaj_sluzbeni_input(ifstream &in) {
  assert(in >> N);
  for (int i = 0; i < N - 1; ++i) {
    int a, b;
    assert(in >> a >> b);
    --a; --b;
    assert(valid_node(a) && valid_node(b));
    dodaj_edge(a, b);
  }
}

void procitaj_sluzbeni_output(ifstream &in) {
  assert(in >> sluzbeno_rjesenje);
}

int bio[maxn];

bool najdalji_cvor(int start, int *distance, int *sol) {
  memset(bio, -1, sizeof bio);
  int visited = 0;
  queue<int> q;
  q.push(start);
  bio[start] = 0;
  ++visited;
  while (q.size()) {
    const int current = q.front();
    q.pop();
    *sol = current;
    *distance = bio[current];
    for (size_t i = 0; i < graph[current].size(); ++i) {
      const int next = graph[current][i];
      if (bio[next] == -1) {
        q.push(next);
        bio[next] = bio[current] + 1;
        ++visited;
      }
    }
  }
  return visited == N;
}

void check_najduzi_put() {
  int cvor = 0, sol = -1;
  if (!najdalji_cvor(cvor, &sol, &cvor)) {
    verdict(0.7, WRONG_RECONSTRUCTION_TREE);
  }
  najdalji_cvor(cvor, &sol, &cvor);
  najdalji_cvor(cvor, &sol, &cvor);
  if (sluzbeno_rjesenje != sol) {
    verdict(0.7, WRONG_RECONSTRUCTION);
  }
}

void procitaj_kandidat_output(ifstream &in) {
  int kandidat_rjesenje;
  if (!(in >> kandidat_rjesenje)) {
    verdict(0, NOT_ENOUGH_DATA);
  }
  if (kandidat_rjesenje != sluzbeno_rjesenje) {
    verdict(0, WRONG_DISTANCE);
  }
  int close_a, close_b;
  int open_a, open_b;
  if (!(in >> close_a >> close_b >> open_a >> open_b)) {
    verdict(0.7, NOT_ENOUGH_DATA_REC);
  }
  if (in >> close_a) {
    verdict(0.7, TOO_MUCH_DATA);
  }
  close_a--; close_b--; open_a--; open_b--;
  if (!valid_node(close_a) || !valid_node(close_b) || !valid_node(open_a) ||
      !valid_node(open_b)) {
    verdict(0.7, HALLWAY_INVALID);
  }
  if ((close_a == open_a && close_b == open_b) ||
      (close_a == open_b && close_b == open_a)) {
    verdict(0.7, HALLWAY_INVALID);
  }
  if (!ima_edge(close_a, close_b)) {
    verdict(0.7, CLOSING_CLOSED_HALLWAY);
  }
  if (ima_edge(open_a, open_b)) {
    verdict(0.7, OPENING_OPENED_HALLWAY);
  }
  izbrisi_edge(close_a, close_b);
  dodaj_edge(open_a, open_b);
  check_najduzi_put();
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


