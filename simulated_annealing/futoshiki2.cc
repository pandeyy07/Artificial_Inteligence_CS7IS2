/**
 * \file      	   futoshiki.cc
 * \brief          The following program is part of TCD's CS172 course in Artifical Intelligence project. It uses
 *                 simulated annealing, a metahueristic method, to solve the Japanese boardgame Futoshiki. Futoshiki
 *                 is a logic puzzle that can be seen as a variant of Sudoku with more constraints. The objective is
 *                 to place the numbers (ranging from 0 to the the number associated with the square grid size) such
 *                 that each tow and column contains only one of each digit. There are also inequality constraints that
 *                 are initially specified  between some of the squares such that one square must be greater than or
 *                 less than its neighbour.These constraints must be satisfied in order to 'complete' the puzzle.
 * \author         P. Prunty
 * \version        1.0
 * \date           2020-02-22
 */

#include <iostream>
#include <random>
#include <ctime>
#include <unistd.h>
#include "futoshiki2.h"

#define BILLION 1E9

int main() {

    /* Create an object of class Futoshiki with default square grid size = 5. */
    Futoshiki A;
    
    /* Create a vector of Mersenne Twister state size. */
    std::vector<unsigned int> random_data(std::mt19937_64::state_size);
    std::cout << "Creating vector with Mersenne Twister state-size ["
    << std::mt19937_64::state_size << "]." << '\n';
    
    std::random_device rand;                          /* Non-deterministic random number generator which picks up noise from network, i.e from the
                                                         keyboard or from traffic coming through the network. We use this to seed the other rng. */
    std::generate(std::begin(random_data),
    std::end(random_data), std::ref(rand));           /* Pull state_size numbers from the random device. */
    std::seed_seq seeds(std::begin(random_data),
    std::end(random_data));                           /* Generate a sequence of integers between 0 and 2^{32} from sequence above. */
    std::mt19937_64 seeded_engine(seeds);             /* Mersenne Twister quasi-uniform rng with large period, seeded using the random device. */
    std::uniform_int_distribution<int> dist {0,4};    /* Uniform distribution over [0,4], will be used to select a random element, data[i][j], where
                                                         i,j e {0,4}, of the board to alter. */
	
    std::vector<int> v = {0, 1, 2, 3, 4};
	std::shuffle(v.begin(), v.end(), rand);		      /* Shuffle vector elements above in order to select a random element of the board, data[i][j],
                                                         where i,j are an element of the shuffled vector, say i = v[1], where v[i] after shuffling is
                                                         3, and j = v[4], where v[4] after shuffling is 0. Then we read the element data[i][j] =
                                                         data[3][0] and change its value to generate a new configuration (state) for the board, where
                                                         data[3][0] = some new value between [0,5].*/
	std::uniform_real_distribution<> uniform(0, 1);   /* Double precision uniform rng between 0 and 1 used in the simulated annealing process for
                                                         accepting bad moves with a certain probability. */

    struct timespec requestStart, requestEnd;         /* Calculate time taken by a request*/
    

    A.initial_configuration();			 	    //< Initialize starting configuration.
    A.print_board();                            //< Print the current board state onto the screen for the user to see.
	double old_energy = A.compute_energy();     //< Compute the energy of the initial configuration.
    
	std::cout << "\nEnergy of initial (random) configuration is " << old_energy << '\n';
    
    /* Create a new object B of class Futoshiki. */
   	std::cout << "Creating copy of assignment." << std::endl;
	Futoshiki B;
    
     /* ----------------------------------- Simulated Annealing Process -----------------------------------------*/
    
    std::cout << "\nBeginning Simulated Annealing Process." << '\n';
    std::cout << "Beginning timer." << '\n';
       clock_gettime(CLOCK_REALTIME, &requestStart);
    
    std::cout << '\n' <<  "  Iteration           Temperature             Energy \n";
    std::cout << std::setw(60) << std::setfill('-') << '\n' << std::setfill(' ');
 	
        //double T{100000}, cool_rate{0.0003};
        double T{100}, cool_rate{0.000002};     //< Temperature and cooling rate for the system.
		int count{0};                           //< Will follow the number of iterations until solution is found.
        double new_energy, delta_energy;        /*< The energy of the new (altered) configuration and the difference between
                                                    the energy in the new configuration compared to the old configuration. */

        /* While the temperature is not zero, continue to search for a solution to the puzzle. */
        while ( T >= 0.25 ) {
                   
			int &m = v[dist(seeded_engine)];       //< Shuffle elements in matrix to be changed and pass m,n,i, and j to
    		int &n = v[dist(seeded_engine)];	      //  alter_configuration function. Logic of this is explained @ line 42.
            
            /* Assignment operator is called here. */
            B = A;                              //< Copy data in A and update (paste) it to B.
            
            /* Alter configuration of B which is inhereted from A. */
			B.alter_configuration(seeded_engine, m, n);
			
			new_energy = B.compute_energy();    //< Get the energy of the new configuration.
	
            /* Find the difference in energy from the old configuration compared to the new one. */
            delta_energy = new_energy - old_energy;
            
			//std::cout << "Delta energy = " << delta_energy << "\n";

            /* If the new configuration is better than our current solution, accept it unconditionally. */
            if ( delta_energy > 0) {
                old_energy = new_energy;
                A = B;                          //< Copy data in B and update (paste) it to A.
				}
            
            /* Else accept the worse state with some probability. */
            /* In other words, if the change in energy is small, and the temperature is high -> accept w/
               high prob. Consequently, if the temperature is negative, meaning it has cooled  -> accept
               w/ low prob. */
            else if ( exp(delta_energy/T) > uniform(seeded_engine) ) {
                old_energy = new_energy;
                A = B;
               }

            ++count;                           //< Increment the number of iterations.
            T *= (1-cool_rate);                //< Cool the system after each iteration.

            /* Print results after every X iterations and after the first iteration.s */
            if ( count % 100000 == 0 || count == 1) {
            std::cout << std::setw(3) << count << "\t\t\t" <<
            std::setprecision(8) <<  T << "\t\t" << std::setprecision(8) << old_energy << "\n";
			}

	   }
	
	std::cout << std::setw(60) << std::setfill('-') << '\n' << std::setfill(' ');
    std::cout << "\n";
    clock_gettime(CLOCK_REALTIME, &requestEnd);

	if ( A.verify_board() == 0 ) {
	std::cout << "Solution is found." << "\n\n";
	A.print_board();
	std::cout << "\n\nEnergy = " << old_energy << '\n';
	}
	else {
	std::cout << "Not a solution. Try running the program again." << '\n';
	std::cout << '\n';
	A.print_board();
	std::cout << "Energy = " << old_energy << "\n";
	}



    double accum = ( requestEnd.tv_sec - requestStart.tv_sec )
    + ( requestEnd.tv_nsec - requestStart.tv_nsec )
    / BILLION;
    
    printf("CPU time used (per clock_gettime()) for simulated annealing algorithm: %lf seconds\n", accum);
    
    return 0;
    
}



