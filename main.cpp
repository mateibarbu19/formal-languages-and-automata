#include "orientable_dfa.hpp"
#include <cstdio>
#include <string>

using namespace std;
using namespace automata;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: ./sync problem\n");
    return 1;
  }
  orientable_automaton a;

  string problem_name(argv[1]);
  if (problem_name == "accessible") {
    for (uint state : a.getAccesible_states()) {
      printf("%d\n", state);
    }
  } else if (problem_name == "productive") {
    for (uint state : a.getProductive_states()) {
      printf("%d\n", state);
    }
  } else if (problem_name == "useful") {
    for (uint state : a.getUseful_states()) {
      printf("%d\n", state);
    }
  } else if (problem_name == "synchronize") {
    for (uint symbol : a.getSync_sequence()) {
      printf("%d\n", symbol);
    }
  }

  return 0;
}