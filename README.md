# CMSC 142: Longest Path

## Brute Force Approach
1. DFS from every possible starting vertex
2. Keep track of current path and the longest path so far
3. For every DFS, check if the current path's length exceeds the longest path so far
4. If so, replace the longest path with the current path

## Non-Brute Force Approach
1. Uses a heuristic approach to reduce the search space
2. Keeps only a limited number of candidate paths (BEAM_WIDTH)
3. Expands the most promising paths first
4. Stores discarded paths in a stack for backtracking
5. Faster than DFS but does not always guarantee the optimal solution

6. References:
   Zhou, R., & Hansen, E. A. (2005). Beam-stack search: Integrating backtracking with beam search. In Proceedings of the Fifteenth International Conference on Automated Planning and Scheduling (ICAPS 2005) (pp. 90–98). AAAI Press. https://www.aaai.org/Library/ICAPS/2005/icaps05-010.php
