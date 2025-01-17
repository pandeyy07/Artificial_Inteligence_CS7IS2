# Artificial_Inteligence_CS7IS2

Group Members: 
Aditya Vishnu
Alexandra Silva
Cian Conry
Patrick Prunty
Yash Pandey

Aim: Solve a Constraint Satisfaction problem using a variety of AI algorithms. 

The constraint satisfaction problem our group chose to solve is the Futoshiki logic puzzle. 

![Alt text](puzzleExample.png?raw=true)

The Algorithms we used to solve the puzzle iniclude Backtracking, forward checking, AC-3 and Simulated Annealing.The test games can found in the folder Futoshiki_testgames. 

To run Backtracking without forward checking go to BacktrackSolver/futoshiki.py. Run the code and input one of the four text files. (four.txt, five.txt, six.txt, seven.txt). 

To run Backtracking with forward checking go to ForwardChecking/BacktrackingImproved.ipynb and run the code.

To run AC-3 go to AC-3/AC3_AI.ipynb, make sure Futoshiki_matrix.txt file and the code are in same directory, then run the code.  

To compile and run Simulated Annealing algorithm, simply clone, or download the simulated_annealing directory onto your system and type 'make' inside of that directory on your terminal. After which there will be two executables. futoshiki and futoshiki2. The former executable, futoshiki, solves the 'easy' difficulty 5x5 futoshiki puzzle, which can viewed in the easy.png file, whilst the later executable, futoshiki2, solves the 'extreme' difficulty 5x5 futoshiki puzzle. In the case that you do not have the suitable commpiler to compile the code, two executables are ready placed in the folder. To run them, simply type ./futoshiki or ./futoshiki2 in the terminal. The programs are written in such a way that they will produce different outcomes each time that they are run.
