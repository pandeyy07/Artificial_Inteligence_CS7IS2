# Futoshiki Simulated Annealing #

The following program solved the Japanese puzzle game Futoshiki using the metaheuristic algorithm, simulated annealing. Simulated annealing is a higher-level metaheuristic procedure which combines the stochastic process of a random walk with hill climbing. The folder already contains two executables, futoshiki and futoshiki2. The former executable solves the 'easy' difficulty futoshiki puzzle, which can viewed in the easy.png file, whilst the later executable solves the 'extreme' difficulty futoshiki puzzle. The executables are written such that they can be run multiple times with different outcomes. 

A hill-climbing algorithm that never makes “downhill” moves toward states with lower value (or higher cost) is guaranteed to be incomplete, because it can get stuck on a local maximum. In contrast, a purely random walk—that is, moving to a successor chosen uniformly at random from the set of successors—is complete but extremely inefficient. Therefore, it seems reasonable to try to combine hill climbing with a random walk in some way that yields both efficiency and quasi-completeness. Simulated annealing is such an algorithm.
Imagine the task of getting a ping-pong ball into the deepest crevice in a bumpy surface. If we just let the ball roll, it will come to rest at a local minimum. If we shake the surface, we can bounce the ball out of the local minimum. The trick is to shake just hard enough to bounce the ball out of local min-ima but not hard enough to dislodge it from the global minimum. The simulated-annealing solution is to start by shaking hard (i.e., at a high temperature) and then gradually reduce the intensity of the shaking (i.e., lower the temperature).
This notion of slow cooling implemented in the Simulated annealing algorithm is interpreted as a slow decrease in the probability of accepting worse solutions as the solution space is explored. Accepting worse solutions is a fundamental property of metaheuristics because it allows for a more extensive search for the global optimal solution. The following is a graphicalrepresentation of the probability that the simulated annealing algorithm accepts a downhill movement, as the temperature decreases (look at this graph from right to left).

![](Artificial_Inteligence_CS7IS/simulated_annealing/prob_param.png)



