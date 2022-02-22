#pragma once

#include <cctype>
#include <list>
#include <string>
#include <unordered_set>
#include <vector>

// typedef unsigned int uint;

namespace automata {
class orientable_automaton {
public:
  orientable_automaton();

  uint getNr_of_states(void);
  uint getNr_of_symbols(void);
  uint getNr_of_starting_states(void);
  uint getNr_of_final_states(void);
  const std::vector<std::vector<uint>> &getTransition_function(void);
  const std::vector<std::vector<std::list<uint>>> &
  getInverse_transition_function(void);
  const std::vector<uint> &getStarting_states(void);
  const std::vector<uint> &getFinal_states(void);

  const std::vector<uint> getAccesible_states(void);
  const std::vector<uint> getProductive_states(void);
  const std::vector<uint> getUseful_states(void);
  const std::list<uint> getSync_sequence(void);

protected:
  uint nr_of_states;
  uint nr_of_symbols;
  uint nr_of_starting_states;
  uint nr_of_final_states;

  std::vector<std::vector<uint>> transition_function;
  std::vector<std::vector<std::list<uint>>> inverse_transition_function;

  const std::pair<uint, uint> next_helpful_state(std::pair<uint, uint> state, uint symbol);

  std::vector<uint> starting_states;
  std::vector<uint> final_states;
};
} // namespace automata