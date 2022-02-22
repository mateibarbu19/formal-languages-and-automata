#include "orientable_dfa.hpp"
#include <algorithm>
#include <cstdio>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>

using namespace automata;
using namespace std;

orientable_automaton::orientable_automaton() {
  scanf("%u", &nr_of_states);
  scanf("%u", &nr_of_symbols);
  scanf("%u", &nr_of_starting_states);
  scanf("%u", &nr_of_final_states);

  transition_function.resize(nr_of_states, vector<uint>(nr_of_symbols, 0));
  inverse_transition_function.resize(nr_of_states,
                                     vector<list<uint>>(nr_of_symbols));
  starting_states.resize(nr_of_starting_states, 0);
  final_states.resize(nr_of_final_states, 0);

  for (uint i = 0; i < nr_of_states; i++) {
    for (uint j = 0; j < nr_of_symbols; j++) {
      uint next_state;
      scanf("%u", &next_state);
      transition_function[i][j] = next_state;
      inverse_transition_function[next_state][j].push_back(i);
    }
  }

  for (uint i = 0; i < nr_of_starting_states; i++) {
    scanf("%d", &starting_states[i]);
  }

  for (uint i = 0; i < nr_of_final_states; i++) {
    scanf("%d", &final_states[i]);
  }
}

uint orientable_automaton::getNr_of_states(void) { return nr_of_states; }

uint orientable_automaton::getNr_of_symbols(void) { return nr_of_symbols; }

const vector<vector<uint>> &orientable_automaton::getTransition_function(void) {
  return transition_function;
};

const vector<vector<list<uint>>> &
orientable_automaton::getInverse_transition_function(void) {
  return inverse_transition_function;
}

const vector<uint> &orientable_automaton::getStarting_states(void) {
  return starting_states;
}

const vector<uint> &orientable_automaton::getFinal_states(void) {
  return final_states;
}

const vector<uint> orientable_automaton::getAccesible_states(void) {
  unordered_set<uint> accesible_states(starting_states.begin(),
                                       starting_states.end());
  queue<uint> q;
  for (unsigned starting_state : starting_states) {
    q.push(starting_state);
  }

  while (!q.empty()) {
    uint state = q.front();
    q.pop();

    for (uint next_state : transition_function[state]) {
      if (!accesible_states.contains(next_state)) {
        q.push(next_state);
        accesible_states.insert(next_state);
      }
    }
  }

  vector<uint> result(accesible_states.begin(), accesible_states.end());
  std::sort(result.begin(), result.end());

  return result;
}

const vector<uint> orientable_automaton::getProductive_states(void) {
  unordered_set<uint> productive_states(final_states.begin(),
                                        final_states.end());
  queue<uint> q;
  for (unsigned final_state : final_states) {
    q.push(final_state);
  }

  while (!q.empty()) {
    uint state = q.front();
    q.pop();
    for (auto prev_states : inverse_transition_function[state]) {
      for (auto prev_state : prev_states) {
        if (!productive_states.contains(prev_state)) {
          q.push(prev_state);
          productive_states.insert(prev_state);
        }
      }
    }
  }

  vector<uint> result(productive_states.begin(), productive_states.end());
  std::sort(result.begin(), result.end());

  return result;
}

const vector<uint> orientable_automaton::getUseful_states(void) {
  vector<uint> acc = getAccesible_states();
  vector<uint> prod = getProductive_states();
  vector<uint> result;

  set_intersection(acc.begin(), acc.end(), prod.begin(), prod.end(),
                   std::back_inserter(result));

  return result;
}

/*
 * A generic function to find an element inside a container
 * https://codereview.stackexchange.com/a/59999
 */
template<class C, class T>
auto contains(const C& v, const T& x) -> decltype(end(v), true) {
  return end(v) != std::find(begin(v), end(v), x);
}

const pair<uint, uint> orientable_automaton::next_helpful_state(pair<uint, uint> state, uint symbol) {
  return minmax(transition_function[state.first][symbol],
                     transition_function[state.second][symbol]);
}

const list<uint> orientable_automaton::getSync_sequence(void) {
  list<uint> sequence;
  list<uint> history;
  list <uint> unmerged_states;

  // Initialize the list off all unmerged states
  for (uint state = 0; state < nr_of_states; state++) {
    unmerged_states.push_back(state);
  }

  // Mergem them until just one remains
  while (unmerged_states.size() != 1) {
    uint fst = unmerged_states.front();
    unmerged_states.pop_front();
    uint snd = unmerged_states.front();
    unmerged_states.pop_front();
    pair<uint, uint> init_state = minmax(fst, snd);

    stack<pair<uint, uint>> states;
    states.push(init_state);

    set<pair<uint, uint>> seen;
    seen.insert(init_state);

    history.push_back(-1);  

    // Do a DFS until we reach a pair of the same states
    while (!states.empty() && states.top().first != states.top().second) {
      auto state = states.top();

      uint symbol = history.back() + 1;
      history.pop_back();

      while (symbol < nr_of_symbols && seen.contains(next_helpful_state(state, symbol))) {
        symbol++;
      }

      if (symbol != nr_of_symbols) {
        auto next_state = next_helpful_state(state, symbol);
        states.push(next_state);
        seen.insert(next_state);
  
        history.push_back(symbol);
        history.push_back(-1);
      } else {
        states.pop();
      }
    }

    if (states.empty()) {
      // visited all the states and found no merging sequence, too bad
      return list<uint>();
    }

    history.pop_back();
    for (uint &s : unmerged_states) {
      for (uint h : history) {
        s = transition_function[s][h];
      }
    }
    unmerged_states.push_front(states.top().first);
    unmerged_states.sort();
    unmerged_states.unique();

    sequence.splice(sequence.end(), history);
  }

  // find a sequence to a final state

  if (nr_of_final_states) {
    stack<uint> s;
    s.push(unmerged_states.front());

    unordered_set<uint> seen(nr_of_states);
    seen.insert(unmerged_states.front());

    history.push_back(-1);

    while (!s.empty() && !contains(final_states, s.top())) {
      uint state = s.top();

      uint symbol = history.back() + 1;
      history.pop_back();

      while (symbol < nr_of_symbols && seen.contains(transition_function[state][symbol])) {
        symbol++;
      }

      if (symbol != nr_of_symbols) {
        uint next_state = transition_function[state][symbol];
        s.push(next_state);
        seen.insert(next_state);

        history.push_back(symbol);
        history.push_back(-1);
      } else {
        s.pop();
      }

    }

    if (s.empty()) {
      // reached a synchronized state which is not final
      return list<uint>();
    }

    history.pop_back();
    sequence.splice(sequence.end(), history);
  }

  return sequence;
}
