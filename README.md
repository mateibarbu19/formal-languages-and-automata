# Homework

**Note: The homework checker is the intellectual property of the Formal
Languages and Automata team and may have a more restrictive license.**

## Finding accessible states

I used a BFS with the queue is initialized with the start states. The search stops
when there are no visitable states left.

## Finding productive states

At first, when I read from the input file, I built the inverse transition
relation. As an observation, the inverse of the function of a deterministic
automaton is a relation corresponding to a non-deterministic automaton.

I started a BFS from the final states using the inverse transition relation.

## Finding useful states

I intersected the set of accessible states with those of the productive ones.

## Synchronizing sequences

I applied the simplified version of Eppstein's algorithm, according to algorithm
2 in Sven Sandberg's chapter "Homing and Synchronizing Sequences".

The basic idea is to start from a set of states. We choose two states at random
to which we find a sequence of walks. Then we apply that sequence to all states
in the initial set, and we update the set by removing duplicates. We repeat the
process as long as the set has a cardinality greater than 1.

If we choose the initial set of states, the set of all states, the algorithm is
complete.

For a restricted synchronization, we must find a sequence from the synchronized
state, to a final state.
