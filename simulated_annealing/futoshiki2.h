#ifndef FUTOSHIKI2_H_1DQ6ZZHW
#define FUTOSHIKI2_H_1DQ6ZZHW

#include <iostream>
#include <random>
#include <ctime>
#include <unistd.h>
#include <iomanip> 			// Output formatting
#include <algorithm>
#include <iterator>


/**
*	\type		Futoshiki
*   \brief     	This is the class for the Futoshiki board game. It dynamically allocates memory 
*				for the private variable, data, to store a 2-dimensional array 5x5 matrix to 
*				represent the board game. It also holds all function declarations for the program
*				inside of the class. The member functions are defined outside of the class, many 
*				of which take no parameters, but use the private variable, data, to manipulate the
*				game board or to verify constraints on the game board. 
*   \param     	none.
*/
class Futoshiki
{
    
    public:
        
        /* Default constructor for Futoshiki class. */
        Futoshiki () : size(5),
        /* Dynamically allocate memory for matrix. */
		data{new int*[size]}
		{
		for (auto i=0; i<size; i++) {
            data[i] = new int[size];
            }
        /* Error checking. */
       	if (data == NULL) {
		std::cerr << "Error allocating memory for matrix_fragmented." << "\n";
        	}
		std::cout << "Default constructor for Futoshiki game board constructing with size = " 
			<< size << ".\n";
        };
        
        /* Member function declarations. Definitions wrote outside of class. */
        void print_board();                              // Function to print game board.
        void initial_configuration();                    // Function to generate initial configuration.
        void alter_configuration(std::mt19937_64 rand,
        int n, int m);				                     // Funtion to alter the state of the configuration.
        double compute_energy();                         // Function to return energy of configuration.
		int check_inequalities();
        int verify_board();

        Futoshiki& operator=(const Futoshiki &rhs);      // Copy assignment operator.

        /* Default destructor. */
        ~Futoshiki() {
		/* Free heap memory. */
		for (auto i = 0; i<size; ++i) {
                delete[] data[i];
            }
        std::cout << "Destroying Futoshiki object of size = " << size << ".\n";
        };

        private:
       	int size; 				//< The square size of the board game.
    	int **data;             //< The data for the board game will be stored as pointer-to-pointers.
	
	};
                
/* A copy assignment operator is called when an already initialized object
is assigned a new value from another existing object. In other words, an
assignment operator is used to replace the data of a previously initialized
object with some other object's data. */
Futoshiki&::Futoshiki::operator=(const Futoshiki &rhs){

	/* Error check for dynamically allocated
	   memory. */
    if (this == &rhs) {
    std::cout << "Self copy!\n";
    return *this;
    }
                   
    if (data != rhs.data) {
        
        for (int i=0; i<5; ++i) {
            for (int j=0; j<5; ++j) {
                data[i][j] = rhs.data[i][j];
            }
        }
    }
    return *this;
}
                
                
/**
*  \brief       A function which initializes the current configuration of the board.
*				Any values that are initially specified at the beginning of the game
*				are initialized here, whereas all other values are initialized to the
*				value one.
*  \param       none 
*  \return      VOID
*
*/
void Futoshiki::initial_configuration() {
   	
	std::cout << "\nPrinting initial (random) board configuration.\n" << '\n';
    for (int i=0; i<size; ++i) {
        for (int j=0; j<size; ++j) {
        	data[i][j] = 1;
		} }

	// Elements given in initial configuration.
	data[1][1] = 4;
	data[0][3] = 4;
	data[4][4] = 3;
}

/**
*  \brief       A function which prints the current configuration of the game board 
* 				whenever this function is called.
*  \param       none . Uses the private variable data from the class. 
*  \return      VOID
*
*/            
void Futoshiki::print_board() {

    for (int i=0; i<5; ++i) {
    std::cout << "|\t";
        for (int j=0; j<5; j++) {
    
            if ( i == 0 && j == 1) {
                std::cout << "|" << data[i][j] << "|  <  ";
                }
            else if ( i == 0 && j == 2 ) {
                std::cout << "|" << data[i][j] << "|  <  ";
                }
            else if ( i == 3 && j == 2 ) {
                std::cout << "|" << data[i][j] << "|  >  ";
                }
            else if ( i == 4 && j == 1) {
                std::cout << "|" << data[i][j] << "|  <  ";
                }
            else if ( i == 4 && j == 2 ) {
                std::cout << "|" << data[i][j] << "|  >  ";
                }
            else if ( i == 2 && j == 4 ) {
                std::cout << "|" << data[i][j] << "|\t|" << "\n|"
                << "\t\t\t v\t\t\t";
                }
            else if ( i == 3 && j == 4 ) {
                std::cout << "|" << data[i][j] << "|\t|" << "\n|"
                << "\t\t\t\t\t\t";
            	}
			else if ( i == 1 && j == 4 ) {
                std::cout << "|" << data[i][j] << "|\t|" << "\n|"
                << "\t\t\t\t\t\t";
                }
            else if ( i == 0 && j == 4 ) {
                std::cout << "|" << data[i][j] << "|\t|" << "\n|"
                << "\t v\t\t\t\t\t";
                }
            else {
                std::cout << "|" << data[i][j] << "|\t";
            }
        }
    std::cout << "|\n";
    }
	std::cout << '\n';    
}

/**
*  \brief 		A function which takes two random integers, i and j, in the range [0,4] 
*				and uses them as indexers at each iteration to alter one of the elements
*				on the Futoshiki board, data[i][j] = some new random value in the range 
*				[1,5], and thus generate a new configuration of the board. Here, we encode
*				logic into the program. The inequality signs put constraints on what number
*				between 0 and 5 the the elements on either side of the inequality sign can
*				take on. Some of the logic is detailed inside the function. Much of which
*				is learned thru playing the game.
*  \param       rand . n . m . i . j . rand is the Mersenne Twister  seeded by the 
*				random device inside main. The other variables, n, m, i, and j are the
*				indexers for altering the data. These are taken from the std::shuffle
*				of the vector inside main and are random on each iteration.
*  \return      VOID
*
*/            
void Futoshiki::alter_configuration(std::mt19937_64 rand, int n, int m) {
  
    /* Different types of uniform distribution 
	   constraints for different elements on the
	   board. */
   	std::uniform_int_distribution<int> dist{1,5}; 
	std::uniform_int_distribution<int> dist1{2,5}; 
	std::uniform_int_distribution<int> dist2{3,5}; 
	std::uniform_int_distribution<int> dist3{4,5}; 
	std::uniform_int_distribution<int> dist4{2,4};
	std::uniform_int_distribution<int> dist5{2,3};
	std::uniform_int_distribution<int> dist6{3,4};
	std::uniform_int_distribution<int> dist7{1,4};
	std::uniform_int_distribution<int> dist8{1,2};

	// We use n, m, i and j to alter elements of the matrix 
	// to a new number and thus alter the configuration. 	
	// If data[0][0] > data[1][0] then 1 < data[0][0] <= 5.
    if ( (n == 0 && m == 0) ) {
	data[n][m] = dist1(rand);
	}
	// If data[0][0] < data[1][0] then let data[1][0] be a number less than data[0][0].
	else if ( (n == 1 && m == 0) ) {
	data[n][m] = rand() % data[0][0];
	}
	// If data[0][2] < 4 & data[0][2] > data[0][1] then  1 < data[0][2] < 4, i.e
	// data[0][2] is either 2 or 3.
	else if ( (n == 0 && m == 2) ) {
	data[n][m] = dist5(rand);
	}
	else if ( (n == 2 && m == 2) ) {
	data[n][m] = dist2(rand);
	}
	else if ( (n == 3 && m == 2) ) {
	data[n][m] = dist5(rand);
	}
	else if ( (n == 4 && m == 2) ) {
	data[n][m] = dist2(rand);
	}
	else if ( (n == 1 && m == 1) ) {
	data[n][m] = 4;
	}
	else if ( (n == 0 && m == 3) ) {
	data[n][m] = 4;
	}
	else if ( (n == 4 && m == 4) ) {
	data[n][m] = 3;
	}
    else if ( (n == 0 && m == 1) ) {
    data[n][m] = rand() % data[0][2];
    }
    else if ( (n == 4 && m == 1) ) {
    data[n][m] = rand() % data[4][3];
    }
    else if ( (n == 3 && m == 3) ) {
    data[n][m] = rand() % data[3][2];
    }
    else {
    data[n][m] = dist(rand);
    }
    
//
	if ( (m == 0 && n == 0) ) {
	data[m][n] = dist1(rand);
	}
	// If data[0][0] < data[1][0] then let data[1][0] be a number less than data[0][0].
	else if ( (m == 1 && n == 0) ) {
	data[m][n] = rand() % data[0][0];
	}
	// If data[0][2] < 4 & data[0][2] > data[0][1] then  1 < data[0][2] < 4, i.e
	// data[0][2] is either 2 or 3.
	else if ( (m == 0 && n == 2) ) {
	data[m][n] = dist5(rand);
	}
	else if ( (m == 2 && n == 2) ) {
	data[m][n] = dist2(rand);
	}
	else if ( (m == 3 && n == 2) ) {
	data[m][n] = dist5(rand);
	}
	else if ( (m == 4 && n == 2) ) {
	data[m][n] = dist2(rand);
	}
	else if ( (m == 1 && n == 1) ) {
	data[m][n] = 4;
	}
	else if ( (m == 0 && n == 3) ) {
	data[m][n] = 4;
	}
	else if ( (m == 4 && n == 4) ) {
	data[m][n] = 3;
	}
    else if ( (m == 0 && n == 1) ) {
    data[m][n] = rand() % data[0][2];
    }
    else if ( (m == 4 && n == 1) ) {
    data[m][n] = rand() % data[4][3];
    }
    else if ( (m == 3 && n == 3) ) {
    data[m][n] = rand() % data[3][2];
    }
    else {
    data[m][n] = dist(rand);
    }

}


/**
*  \brief       A function which computes the energy of the current board configuration 
*				using a rewards system. This reward system is detailed inside the function.
*				The simulated annealing process will only alter the current configuration if
*				the new configuration is better than the old or else we accept the bad 
*				configuration with some probability. Thus, reward increments are used to 
*				represent the energy of the system and reveal how close to the solution (the
*				maximum energy) the configuration is. 
*  \param       none . Uses the private variable data from the class to access current 
*				configuration's elements. 
*  \return      energy . The accumulated energy of the system. 
*
*/            
double Futoshiki::compute_energy() {

	double energy{0.0};		//< Initial energy of the system.
    
    /* Initialize rewards. */
	constexpr double reward0 = 5;		//< If the socre of the board is not far from the answer
	constexpr double reward1 = 8;	  	//<	If the score of the board is close to the answer
	constexpr double reward2 = 10;		//< If the configuration has satisfied a complete row or column.
    constexpr double reward3 = 7.5;   	//< If the configuration has satisfied an inequality.

	/* -------------------------------------
	*************  REWARD 2 ****************
	--------------------------------------*/

    int rowSum, colSum; 	//< Capture the sum of each row and each column
	int summa{0};			//< Count the number of rows and cols that satisfy constraints
	int rowCounter{0};		//< Sum over the product of sums in each row
	int colCounter{0};		//< Sum over the product of sums in each column
	int sum{0};				//< Sum each element on the board

    for (int i=0; i<5; ++i) {
    rowSum = 1;		//< Initialize rowSum to 1 and compute next iteration (rowSum).
    colSum = 1;		//< Initialize colSum to 1 and compute next iteration (colSum).
        for (int j=0; j<5; ++j) {
		// Find the sum of the product of elements in each row and each column. */
        rowSum *= data[i][j];
        colSum *= data[j][i];
		sum += data[i][j];
		}
	/* If either the row or columns product sum is equal to 120 = 5! then we know that
	   it satisies one of either the row or column constraint of the game puzzle. */
    	rowCounter += rowSum;
		colCounter += colSum;
	if ( rowSum == 120 ) {
    energy += reward2;
	++summa;		//< Increment the sum variable
    }
    if ( colSum == 120 ) {
    energy += reward2;
    ++summa;
		}
    }
	
	/* -------------------------------------
	************  REWARD 0 & 1 *************
	--------------------------------------*/
	
	
 	/* The solved futoshiki board will contain only one of each digit
 	   in each row and column. Thus, the summed product of each row, 
	   or column, will be equal to 5! = 120. There are 5 rows + 5 cols
	   on the board. Thus, the score of the solved configuration will
	   be 10 * 120 = 1,200. Let's determine how close the score of the
	   current configuration is to the score of the solved configuration
	   and reward it accordingly. Likewise, the sum of each element on 
	   a solved board will equal 75. Let's determine how close the 
	   score of the current configuration is to the score of the solved
	   configuration and reward it accordingly. */

	int boardCounter = rowCounter + colCounter;

	if ( (boardCounter > 1100 && boardCounter < 1300) && (sum > 40 && sum < 100 ) ) {
	energy += reward0;
	}
	if ( (boardCounter > 1120 && boardCounter < 1280) && (sum >= 60 && sum <= 90)) {
	energy += reward1;
	}
	
	/* -------------------------------------
	*************  REWARD 3 ****************
	--------------------------------------*/
	
    int i = Futoshiki::check_inequalities();	//< i represents the number of inequalities satisfied.
    energy += i * reward3;

	return energy;		//< Return the accumulated energy of the system. 

}

/**
*  \brief       A function which encodes the inequality constraints of the board, as the
*				human sees them. It increments the variable 'sum' whenever each of these 
*				inequality constraints are satisfied and returns the total number of 
*				inequalities satisfied in the current configuration of the board.
*  \param       none . Uses the private variable data from the class to access current 
*				configuration's elements. 
*  \return      sum . The accumulated number of inequalities satisfied in the current
*				configuration of the board. 
*
*/            
int Futoshiki::check_inequalities() {


    int sum{0};		//< Initialize sum (number of inequalities satisfied) to zero.

	/* Whenever an inequality on the board is satisfied, increment sum. */
    if ( data[0][3] > data[0][2] ) {
        ++sum;
        }

    if (data[0][2] > data[0][1] ) {
        ++sum;
        }

    if ( data[0][0] > data[1][0] ) {
        ++sum;
        }
                    
    if ( data[2][2] > data[3][2] ) {
        ++sum;
        }

    if ( data[3][2] > data[3][3] ) {
        ++sum;
        }
                        
    if ( data[4][2] > data[4][1] ) {
        ++sum;
        }
                    
    if ( data[4][2] > data[4][3] ) {
        ++sum;
        }
   
    return sum;

}
      
/**
 *  \brief       This function is used to check whether the program has found a solution 
 *				 or not. The program has the actual solution for the board encoded. The 
 *				 solution was found by the program, when the energy reached its max, and
 *				 was later encoded by me so that the program has a way of reliably 
 *				 determining if a solution is found.
 *  \param       none . Uses the private variable data from the class to access current
 *               configuration's elements.
 *  \return      1 or 0 . The program will return 0 if the check is succesful and 1 otherwise.
 *
 */
int Futoshiki::verify_board() {

    /* Here is what the solution looks like.*/
	if ( data[0][0] == 5 &&
		 data[1][0] == 2 &&
		 data[2][0] == 1 &&
		 data[3][0] == 3 &&
		 data[4][0] == 4 &&		 
		 data[0][1] == 2 &&
		 data[1][1] == 4 &&
		 data[2][1] == 3 &&
		 data[3][1] == 5 &&
		 data[4][1] == 1 &&
		 data[0][2] == 3 &&
		 data[1][2] == 1 &&
		 data[2][2] == 4 &&
		 data[3][2] == 2 &&
		 data[4][2] == 5 &&	
		 data[0][3] == 4 &&
		 data[1][3] == 3 &&
		 data[2][3] == 5 &&
		 data[3][3] == 1 &&
		 data[4][3] == 2 &&
		 data[0][4] == 1 &&
		 data[1][4] == 5 &&
		 data[2][4] == 2 &&
		 data[3][4] == 4 &&
		 data[4][4] == 3 
		) {
	return 0;
	}
	else {
	return 1;
	}
}

#endif  /* end of include guard: FUTOSHIKI2_H_1DQ6ZZHW */          

