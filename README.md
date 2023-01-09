# NHL Playoff Odds Calculator
A Pythagorean win expectation-based Monte Carlo simulator to determine the odds of NHL teams making the playoffs. Inspired by [sportsclubstats.com](sportsclubstats.com).

Using [Pythagorean win expectation](https://en.wikipedia.org/wiki/Pythagorean_expectation) and the [log5](https://en.wikipedia.org/wiki/Log5) mathematical formula, this 
program simulates individual seasons of the 2022-2023 NHL season and keeps track of how many times teams make the playoffs. Instead of determining every possible 
combination, a technique called the [Monte Carlo method](https://en.wikipedia.org/wiki/Monte_Carlo_method) is used to extrapolate random results to meaningful 
conclusions. The more trials are run, the more accurate the results.

The simulation is capable of running ~20.9 seasons/sec and has a default trial size of 1000. This can be modified in the *League.h* file.

Data is acquired from [Sports Reference](https://www.hockey-reference.com/leagues/NHL_2023_games.html).

Example files can be seen in the Results folder. Simulations were run with data accurate up to 2023-01-04 and the number of trials is included in the file name.
