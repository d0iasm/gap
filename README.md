# gap
A metaheuristic algorithm for the generalized assignment problem (GAP)


## Algorithms

### gap_SLS.c : Simple Local Search
1. Create initial feasible solution by random swap.
2. Move to a feasible neighbor solution by first admissible move strategy.
3. Finish improvement if calculate time is over time limit or same value is continuing 100 times.

### gap_MLS.c : Multi Start Local Search
1. Create initial feasible solution by random swap.
2. Move to a feasible neighbor solution by first admissible move strategy.
3. Finish improvement if calculate time is over time limit.
4. Go back to 1. and repeat 1~3 then store best score until execution time is over.


## Results
https://docs.google.com/spreadsheets/d/1vndw0acG6aL370qhovJF6bpHiANgEf6WrEJNM_DleQI

## Check
- ハンガリアンアルゴリズム

## References
- http://www.co.mi.i.nagoya-u.ac.jp/~yagiura/saitekika-tokuron/index.html

