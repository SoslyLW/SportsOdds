# NHL Playoff Odds Calculator
A Pythagorean win expectation-based Monte Carlo simulator to determine the odds of NHL teams making the playoffs. Inspired by [sportsclubstats.com](sportsclubstats.com).

Using [Pythagorean win expectation](https://en.wikipedia.org/wiki/Pythagorean_expectation) and the [log5](https://en.wikipedia.org/wiki/Log5) mathematical formula, this 
program simulates individual seasons of the 2022-2023 NHL season and keeps track of how many times teams make the playoffs. Instead of determining every possible 
combination, a technique called the [Monte Carlo method](https://en.wikipedia.org/wiki/Monte_Carlo_method) is used to extrapolate random results to meaningful 
conclusions. The more trials are run, the more accurate the results.

**Pre-ran simulations from *2023-01-04* can be found in the *Results* folder if you do not want to run an intesive 100,000+ trial simulation yourself. The number of trials for each run is indicated in the filename.**

The simulation is capable of running ~20.9 seasons/sec and has a default trial size of 1000. This can be modified in the *League.h* file.

Data is acquired from [Sports Reference](https://www.hockey-reference.com/leagues/NHL_2023_games.html).
