# NHL Playoff Odds Calculator
A Pythagorean win expectation-based Monte Carlo simulator to determine the odds of NHL teams making the playoffs. Inspired by [sportsclubstats.com](sportsclubstats.com).

Using [Pythagorean win expectation](https://en.wikipedia.org/wiki/Pythagorean_expectation) and the [log5](https://en.wikipedia.org/wiki/Log5) mathematical formula, this 
program simulates individual seasons of the 2022-2023 NHL season and keeps track of how many times teams make the playoffs. Instead of determining every possible 
combination, a technique called the [Monte Carlo method](https://en.wikipedia.org/wiki/Monte_Carlo_method) is used to extrapolate random results to meaningful 
conclusions. The more trials are run, the more accurate the results.

**Pre-ran simulations from *2023-01-04* can be found in the *Results* folder if you do not want to run an intesive 100,000+ trial simulation yourself. The number of trials for each run is indicated in the filename.**

Performance varies by CPU *and has been vastly improved since 2023-10-13*. I was able to run at ~263 seasons/sec on my laptop with an i7-1260P. Generally, selecting simulation sizes < 20,000 should run fairly quickly.

Trial size can be changed upon starting the program and defaults to 1,000 seasons if no input is received.

Data is acquired from [Sports Reference](https://www.hockey-reference.com/leagues/NHL_2023_games.html).
