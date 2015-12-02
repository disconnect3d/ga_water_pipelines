## Genetic Algorithms course project

The idea was to get the shortest water channels network that would provide water to as many towns as possible on a square map. Towns were represented by circles and water channels were lines between two walls.

The individual was represented by array of 10 integers. Each integer different than 0 was water channel.

Two genetic operators were used: mutation which added or removed water channel and one-point crossover which swapped first five integers between two individuals. I also tested if swapping random integers would be better but it turned out to be much slower – probably because of branch prediction fail, although I did not profile it. 

Implemented selection had two versions – first one, when population was not so good (generations count < 3000) made that 10% of new population were completely new individuals. The second one when generations count was higher than 3000 made that 10% of the new population were the best individuals.

## other informations

The code is written in c++11. No external libraries were used.

The description and comments are mainly in polish since that was an university project (my bad..).

The code uses [placement new](https://en.wikipedia.org/wiki/Placement_syntax) in some places just to get better performance (should have used a std::vector, but oh well, wanted to do something weird and funny).

Python was used as external scripts to debug the solutions (generate map, plot the solution, plot the relation between genethic parameters like probability of mutation/crossover and fitness function).
