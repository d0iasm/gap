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
- Hungarian algorithm
- Late acceptance hill climbing
- Stochastic optimization
- Negascout 

## References
- http://www.co.mi.i.nagoya-u.ac.jp/~yagiura/saitekika-tokuron/index.html
- https://github.com/rodolfolabiapari/Generalized-Assignment-Problem
- http://dse.ssi.ist.hokudai.ac.jp/~onosato/EDUCATION/sec13.pdf
- http://bin.t.u-tokyo.ac.jp/metaheuristics2013/document/1.pdf
- https://www.jstage.jst.go.jp/article/ieejeiss1987/121/6/121_6_986/_pdf
- https://www.researchgate.net/profile/Mauricio_Resende2/publication/225237245_Greedy_Randomized_Adaptive_Search_Procedures/links/0fcfd50aa7e1d94ded000000/Greedy-Randomized-Adaptive-Search-Procedures.pdf
- 
