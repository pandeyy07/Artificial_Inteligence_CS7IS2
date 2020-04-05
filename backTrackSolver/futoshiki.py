import time

MOVES = 0
#converts puzzle objects into text
def convertToText(p):
    textPuzzle = ['|', '|']        
    puzzle = p[0]
    constraints = p[1]
    
    for num1, num2 in enumerate(zip(puzzle, constraints)):
        #is cell last in row or not if not add space at end
        if( ((num1 + 1) % SIZE) == 0):
            insidePosition = False
            space = ''
        else:
            insidePosition = True
            space = ' '
           
        #convert numbers to their symbols    
        if(num2[1] == 1):
            constraints = ('<', '')
        if(num2[1] == 2):
            constraints = ('>', '')
        if(num2[1] == 4):
            constraints = ('', '^')
        if(num2[1] == 8):
            constraints = ('', 'v')
        if(num2[1] == 0):
            constraints = ('', '')
                                
        #assigns output of side arrows and numbers 
        textPuzzle[-2] += f"{num2[0] or '-'}{constraints[0] or space}"
            
        #assigns output of up down arrows
        textPuzzle[-1] += f"{constraints[1] or '.' or ' '}{space}"

        #assigns output of start and end lines
        numCells = SIZE * SIZE - 1
        if(num1 != numCells and insidePosition == False):
            textPuzzle = textPuzzle + ['|', '|']

            
    textPuzzle = '| \n'.join(textPuzzle[:-1])
    return f"{textPuzzle}|\n"



#converts puzzle text into objects   
def readInPuzzle(filename):
    #convert symbols to numbers
    file = open(filename, 'r')
    file = file.read().strip()
    file = file.replace('.', '0')
    file = file.replace(' ', '0')
    file = file.replace('-', '0')
    file = file.replace('<', '1')
    file = file.replace('>', '2')
    file = file.replace('^', '4')
    file = file.replace('v', '8')
    file = file.replace('|', '')
    file = file.split('\n')
        
    
    #collects pre defined values
    definedValues = []
    for num in file[::2]:
        for value in num[::2]:
            definedValues.append(int(value))

    #collects constraints 
    constraintValues = []
    for num in file[1::2]:
        for value in num[::2]:
            constraintValues.append(int(value))
                
    constraintValues += [0] * SIZE

    for i, num in enumerate(file[::2]):
        for j, value in enumerate(num[1::2]):
            constraintValues[i * SIZE + j] += int(value)

    return (definedValues, constraintValues, SIZE)




def substitute(p, nextEmptyCell, nextValue, num):
    puzzle = p[0]
    constraints = p[1]
    
    if(num == nextEmptyCell):
        return nextValue, constraints[num]
    else:
        return puzzle[num], constraints[num]
    

def compare(p, nextEmptyCell, nextValue, row, col):
        #convert to 1d
        index = (row[0] * SIZE + row[1]), (col[0] * SIZE + col[1])

        #check if inside puzzle
        if index[1] >= SIZE ** 2:
            return True

        #create swap index, swap if index[1] < index[0]
        if(index[1] > index[0]):
            s = 1
        if(index[1] < index[0]):
            s = -1

        #sub old value for new value
        substitutes = tuple(substitute(p, nextEmptyCell, nextValue, num) for num in index[::s])

        #check constraints 
        for num in range(2):
            numMod = num % 2
            #check up, down constraints
            if row[1] == col[1]:
                num = num + 2

            #check first cell  
            if(2 ** num & substitutes[0][1]):
                #if second cell not empty continue check
                if(substitutes[1][0] != 0):
                    #check if values match the constraint - if not return false 
                    if(not substitutes[numMod][0] < substitutes[not numMod][0]):
                        return False
           
        return True


def backtrack(p):
    global MOVES
    MOVES = MOVES + 1
    
    puzzle = p[0]
    constraints = p[1]    

    try:
        #Find next empty cell to solve
        nextEmptyCell = puzzle.index(0)

    except ValueError:
        #if no more empty cells puzzle is solved
        return True
    
    #check if cell can equal num
    for num in range(SIZE):
        nextValue = num + 1
        isAllowed = True

        #make 2D
        row = nextEmptyCell // SIZE
        col = nextEmptyCell % SIZE
    
        #Get all values of row and column of cell
        fullRow = puzzle[row * SIZE:row * SIZE + SIZE]
        fullColumn = puzzle[0 * SIZE + col:SIZE * SIZE + col:SIZE]

        #check for duplicates in row and column
        if nextValue in fullRow:
            isAllowed = False
        if nextValue in fullColumn:
            isAllowed = False

        #check constraints
        surroundingCells = ((0, 1), (1, 0), (0, -1), (-1, 0))
        for cell in surroundingCells:
            if not compare(p, nextEmptyCell, nextValue, (row, col), (row + cell[0], col + cell[1])):
                isAllowed = False

        
        #if allowed assign value and continue
        if(isAllowed == True):
            puzzle[nextEmptyCell] = nextValue

            if backtrack(p):
                return True

            #if backtrack(p) false backtrack to previous and reassign 
            else:
                puzzle[nextEmptyCell] = 0

    
    return False


    

if __name__ == '__main__':
    
    #four.txt or five.txt or six.txt or seven.txt or eight.txt or nine.txt
    
    userInput = input('Puzzle file: ')

    global SIZE
    if(userInput == 'four.txt'):
        SIZE = 4
    if(userInput == 'five.txt'):
        SIZE = 5
    if(userInput == 'six.txt'):
        SIZE = 6
    if(userInput == 'seven.txt'):
        SIZE = 7
    if(userInput == 'eight.txt'):
        SIZE = 8
    if(userInput == 'nine.txt'):
        SIZE = 9
    
    p = readInPuzzle(userInput)
    

    string = convertToText(p)
    print('Unsolved Futoshiki Puzzle:')
    print(string)

    startTime = time.time()
    if backtrack(p) == True:
        string = convertToText(p)
        print('Solved Futoshiki Puzzle: ')
        print(string)

    else:
        print('No solution')

    print('Number of moves: ', MOVES)
    print('Seconds taken: ', (time.time() - startTime))

    input()






             
