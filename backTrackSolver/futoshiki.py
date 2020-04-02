SIZE = 5;

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


def substitute(p, nextValue, nextEmptyCell, num):
    puzzle = p[0]
    constraints = p[1]

    if(num == nextEmptyCell):
        return nextValue, constraints[num]
    else:
        return puzzle[num], constraints[num]
    

def compare(p, nextValue, nextEmptyCell, row, col):
    puzzle = p[0]
    constraints = p[1]
    
    #convert to 1d
    index = (row[0] * SIZE + row[1]), col[0] * SIZE + col[1]

    #check if inside puzzl
    if index[1] >= SIZE ** 2:
        return True
    
    #create swap index, swap if index[1] < index[0]
    if(index[1] > index[0]):
        s = 1
    if(index[1] < index[0]):
        s = -1

    #sub old value for new value
    substitutes = tuple(substitute(p, nextValue, nextEmptyCell, num) for num in index[::s])
    
    #check constraints 
    for num in range(2):
        #check up, down constraints
        if row[1] == col[1]:
            num = num + 2

        #check first cell            
        if(2 ** num == substitutes[0][1]):
            #if second cell not empty continue check
            if(substitutes[1][0] != 0):
                #check if values match the constraint - if not return false 
                if(not substitutes[num % 2][0] < substitutes[not num % 2][0]):
                    return False

    return True


    

def backtrack(p):
    puzzle = p[0]

    try:
        #Find next empty cell to solve
        nextEmptyCell = puzzle.index(0)
    
    except ValueError:
        #if non puzzle solved
        return True


    #check if cell can equal num
    for num in range(SIZE):
        nextValue = num + 1;
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
            if compare(p, nextValue, nextEmptyCell, (row, col), (row + cell[0], col + cell[1])) == False:
                isAllowed = False
                
        #if assign value and continue
        if isAllowed == True:
            puzzle[nextEmptyCell] = num + 1

            if backtrack(p):
                return True

            else:
                puzzle[nextEmptyCell] = 0
    
    return False


    

if __name__ == '__main__':
    p = readInPuzzle('easy.txt')
    #p = readInPuzzle('hard.txt')

    string = convertToText(p)
    print('Unsolved Futoshiki Puzzle:')
    print(string)

    if backtrack(p) == True:
        string = convertToText(p)
        print('Solved Futoshiki Puzzle: ')
        print(string)

    else:
        print('No solution')





             
