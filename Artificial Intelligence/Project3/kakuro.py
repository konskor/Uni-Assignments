import csp
import time



####################### Our tests from AIMA github #################################################


# difficulty 0
kakuro1 = [['*', '*', '*', [6, ''], [3, '']],
           ['*', [4, ''], [3, 3], '_', '_'],
           [['', 10], '_', '_', '_', '_'],
           [['', 3], '_', '_', '*', '*']]

# difficulty 0
kakuro2 = [
    ['*', [10, ''], [13, ''], '*'],
    [['', 3], '_', '_', [13, '']],
    [['', 12], '_', '_', '_'],
    [['', 21], '_', '_', '_']]

# difficulty 1
kakuro3 = [
    ['*', [17, ''], [28, ''], '*', [42, ''], [22, '']],
    [['', 9], '_', '_', [31, 14], '_', '_'],
    [['', 20], '_', '_', '_', '_', '_'],
    ['*', ['', 30], '_', '_', '_', '_'],
    ['*', [22, 24], '_', '_', '_', '*'],
    [['', 25], '_', '_', '_', '_', [11, '']],
    [['', 20], '_', '_', '_', '_', '_'],
    [['', 14], '_', '_', ['', 17], '_', '_']]

# difficulty 2
kakuro4 = [
    ['*', '*', '*', '*', '*', [4, ''], [24, ''], [11, ''], '*', '*', '*', [11, ''], [17, ''], '*', '*'],
    ['*', '*', '*', [17, ''], [11, 12], '_', '_', '_', '*', '*', [24, 10], '_', '_', [11, ''], '*'],
    ['*', [4, ''], [16, 26], '_', '_', '_', '_', '_', '*', ['', 20], '_', '_', '_', '_', [16, '']],
    [['', 20], '_', '_', '_', '_', [24, 13], '_', '_', [16, ''], ['', 12], '_', '_', [23, 10], '_', '_'],
    [['', 10], '_', '_', [24, 12], '_', '_', [16, 5], '_', '_', [16, 30], '_', '_', '_', '_', '_'],
    ['*', '*', [3, 26], '_', '_', '_', '_', ['', 12], '_', '_', [4, ''], [16, 14], '_', '_', '*'],
    ['*', ['', 8], '_', '_', ['', 15], '_', '_', [34, 26], '_', '_', '_', '_', '_', '*', '*'],
    ['*', ['', 11], '_', '_', [3, ''], [17, ''], ['', 14], '_', '_', ['', 8], '_', '_', [7, ''], [17, ''], '*'],
    ['*', '*', '*', [23, 10], '_', '_', [3, 9], '_', '_', [4, ''], [23, ''], ['', 13], '_', '_', '*'],
    ['*', '*', [10, 26], '_', '_', '_', '_', '_', ['', 7], '_', '_', [30, 9], '_', '_', '*'],
    ['*', [17, 11], '_', '_', [11, ''], [24, 8], '_', '_', [11, 21], '_', '_', '_', '_', [16, ''], [17, '']],
    [['', 29], '_', '_', '_', '_', '_', ['', 7], '_', '_', [23, 14], '_', '_', [3, 17], '_', '_'],
    [['', 10], '_', '_', [3, 10], '_', '_', '*', ['', 8], '_', '_', [4, 25], '_', '_', '_', '_'],
    ['*', ['', 16], '_', '_', '_', '_', '*', ['', 23], '_', '_', '_', '_', '_', '*', '*'],
    ['*', '*', ['', 6], '_', '_', '*', '*', ['', 15], '_', '_', '_', '*', '*', '*', '*']]









############################################# Useful Functions ###################################################


def CreateVariable(i,j):

    variable = "X." + str(i) + "." + str(j)
    return variable


def getCoordinates(variable):

    # Returns a list of coordinates [x,y]

    substring = variable.split(".")

    coordinates = []
    coordinates.append(int(substring[1]) )
    coordinates.append(int(substring[2]) )
    
    return coordinates



def Create_Line(i,j,t):

    # t stands for type ( horizontal or vertical)   => string

    line = t + "." + str(i) + "." + str(j)  # h.1.2 : horizontal line with the first white cell in (1,2)
    return line



def Domain_values(str_domain, num_variables, current_sum, sum_constraint):

    # Recursively produce values that are acceptable for our encapsulated variable ==> "line"
    # Final product will be a list of these values , for example: ["12","21"]

    # Acceptable values are those that satisfy both (all_diff) and (sum_constraint)

    if( len(str_domain) == num_variables):      #base case

        if( current_sum != sum_constraint):
            return False

        return True

    result_list = []

    for i in range(1,10):

        if( (str(i) in str_domain) or (current_sum + i > sum_constraint) ) : continue

        new_sum = current_sum + i
        new_str_domain = str_domain + str(i)

        result = Domain_values(new_str_domain, num_variables, new_sum, sum_constraint)

        if result is False: continue
        elif result is True:
            result_list.append(new_str_domain)
        else:
            result_list = result_list + result


    return result_list
            
            
        






class MyKakuro(csp.CSP):


    def __init__(self, puzzle):
    
        # Kakuro class contains:
        # 1) puzzle :   our puzzle
        # 2) rows:      how many rows we have
        # 3) columns:   how many columns we have

        # 4) catalogue:  a dictionary where  {key : value} = {line : [sum_constraint,variables]}
        #                An easy way to access the white cells of our line as well as the sum constraint
        #                catalogue will contain the [sum_c,variables] of each line


        # line: Consecutive variables, in the same row or column (with no black cells in between), create what I call a "Line". Each line has a sum_constraint that we have to satisfy.
        # ===> [sum_constraint,variables] ==> [sum_constraint, [var1,var2,var3,....] ]

        # line is our encapsulated variables ==> used in search ==> oficial Variables of CSP
        # the white cells are the "un-official" variables of our problem

        # The domain of our encapsulated variable is basically the "acceptable" values which satisfy the 2 constraints: all_diff and  sum_constraint
        # The domain of each "line" is produced by our recursive function ==> Domain_values()

        # we chose "line" as variable , because this is a CSP-Nary problem, and I had to "transform" the problem into its binary form
        # http://ktiml.mff.cuni.cz/~bartak/constraints/binary.html


        self.puzzle = puzzle

        variables = []
        domains = {}

        catalogue1 = {} # this dictionary will help us, find the sum_constraint and the white cells of each line



        for i,row in enumerate(puzzle):
        
            for j,column in enumerate(row):
                continue
                # do nothing

        self.rows = i+1
        self.columns = j+1        

        for i in range(0,self.rows):
        
            for j in range(0,self.columns):

                #Searching only for "constraint" cells
                if puzzle[i][j] == '*' or puzzle[i][j] == '_':
                    continue

                constraint = puzzle[i][j]

                if constraint[0] != '':     # look down

                    variables1 = []

                    for k in range(i+1,self.rows):
                        
                        if puzzle[k][j] != '_':      #encountered a black cell
                            break
                        
                        variable2 = CreateVariable(k,j)
                        variables1.append(variable2)
                        

                    # Variables list is filled 

                    line = Create_Line(i,j,"v")
                    variables.append(line)
                    
                    temp = [ constraint[0], variables1 ]
                    catalogue1[line] = temp


                    #Creating the domain of our variable                    
                    num_variables = len(variables1)

                    domains[line] = Domain_values("", num_variables, 0, int(constraint[0]) )

                   
                if constraint[1] != '':     #look right

                    variables1 = []

                    for k in range(j+1,self.columns):
                        
                        if puzzle[i][k] != '_':      #encountered a black cell
                            break
                        
                        variable2 = CreateVariable(i,k)
                        variables1.append(variable2)
                        

                    # Variables list is filled

                    line = Create_Line(i,j,"h")
                    variables.append(line)
                    
                    temp = [ constraint[1], variables1 ]
                    catalogue1[line] = temp


                    #Creating the domain of our variable                    
                    num_variables = len(variables1)

                    domains[line] = Domain_values("", num_variables, 0, int(constraint[1]) )

        self.catalogue = catalogue1

        self.rows = i+1
        self.columns = j+1
                    
        #Created the variables and domains ==> fixing neighbors


        neighbors = {}
        for v in variables:     #for each line

            neighbors[v] = []

            line_vars = catalogue1[v][1]    # the white cells of our line (v)
            line_type = v[0]

            for v1 in line_vars:    #for each white cell in l

                v1_c = getCoordinates(v1)
            
                if(line_type == "h"):   # v is a horizontal line ==> search upwards

                    for i in range( v1_c[0], -1, -1):

                        if(puzzle[i][v1_c[1]] != "_"):

                            temp_line = Create_Line(i,v1_c[1],"v")
                            neighbors[v].append(temp_line)
                            break

                if(line_type == "v"):   # v is a vertical line ==> search left
                    
                    for j in range( v1_c[1], -1, -1):

                        if puzzle[v1_c[0]][j] != "_":

                            temp_line = Create_Line(v1_c[0],j,"h")
                            neighbors[v].append(temp_line)
                            break

        self.catalogue = catalogue1
        super().__init__(variables, domains, neighbors, self.constraints)


    def constraints(self,A,a,B,b):

        # A and B are neighbouring variables ==> lines that intersect
        # a and b are their values ==> each digit corresponds to the value of the white cell (in our line)


        # Find the white cell (the "intersection" cell ==> item)
        # Check if it has the same value in both lines

        line_type = A[0]

        item = None

        c_A = getCoordinates(A)
        c_B = getCoordinates(B)
        

        if( line_type == "h"):  # B is vertical
            item = CreateVariable( c_A[0], c_B[1])  #the white cell which might have conflicting values            
        if( line_type == "v"):
            item = CreateVariable( c_B[0], c_A[1])

        index_a = self.catalogue[A][1].index(item)
        index_b = self.catalogue[B][1].index(item)
            
        str_a = str(a)
        str_b = str(b)

        if( int(str_a[index_a]) != int(str_b[index_b])):
            return False

        return True



    # This one is from AIMA code
    def display(self, assignment=None):
    
        for i, line in enumerate(self.puzzle):
        
            puzzle = ""
            for j, element in enumerate(line):
                if element == '*':
                    puzzle += "[*]\t"
                elif element == '_':
                    var = CreateVariable(i,j)       #Add somethign here later for lines
                    if assignment is not None:
                        
                        counter = 0
                        for k in range(i-1,-1,-1):

                            if( self.puzzle[k][j] != '_'):
                                break
                            counter = counter + 1

                        line1 = Create_Line(k,j,"v")
                        value_line = str(assignment[line1])
                        
                        value = value_line[counter]
                        puzzle += "[" + value + "]\t"

                    else:
                        puzzle += "[_]\t"
                else:
                    puzzle += str(element[0]) + "\\" + str(element[1]) + "\t"
            print(puzzle)



############################### MAIN ########################################


############## Kakuro1 #################

print("Kakuro1")

test1_fc = MyKakuro(kakuro1)

start_fc = time.time()
#result1_fc = csp.backtracking_search(test1_fc, inference=csp.forward_checking)   # Using Fc
result1_fc = csp.backtracking_search(test1_fc, csp.mrv, csp.lcv, csp.forward_checking) 
duration_fc = time.time() - start_fc


test1_mac = MyKakuro(kakuro1)

start_mac = time.time()
#result1_mac = csp.backtracking_search(test1_mac, inference=csp.mac) #Using Mac/AC3
result1_mac = csp.backtracking_search(test1_mac, csp.mrv, csp.lcv, csp.mac) # Using MAC/AC3
duration_mac = time.time() - start_mac

print("Using FC+MRV+LCV, it took %f seconds and %d assignemnts" % (duration_fc, test1_fc.nassigns) )
print("Using MAC+MRV+LCV, it took %f seconds and %d assignemnts" % (duration_mac, test1_mac.nassigns) )



######################## Kakuro2 ###########################
print("Kakuro2")

test2_fc = MyKakuro(kakuro2)

start_fc = time.time()
#result2_fc = csp.backtracking_search(test2_fc, inference=csp.forward_checking)   # Using Fc
result2_fc = csp.backtracking_search(test2_fc, csp.mrv, csp.lcv, csp.forward_checking) 
duration_fc = time.time() - start_fc


test2_mac = MyKakuro(kakuro2)

start_mac = time.time()
#result2_mac = csp.backtracking_search(test2_mac, inference=csp.mac) # Using MAC/AC3
result2_mac = csp.backtracking_search(test2_mac, csp.mrv, csp.lcv, csp.mac) # Using MAC/AC3
duration_mac = time.time() - start_mac

print("Using FC+MRV+LCV, it took %f seconds and %d assignemnts" % (duration_fc, test2_fc.nassigns) )
print("Using MAC+MRV+LCV, it took %f seconds and %d assignemnts" % (duration_mac, test2_mac.nassigns) )


################################# Kakuro3 #########################

print("Kakuro3")

test3_fc = MyKakuro(kakuro3)


start_fc = time.time()
#result3_fc = csp.backtracking_search(test3_fc, inference=csp.forward_checking)   # Using Fc
result3_fc = csp.backtracking_search(test3_fc, csp.mrv, csp.lcv, csp.forward_checking)   # Using Fc
duration_fc = time.time() - start_fc


test3_mac = MyKakuro(kakuro3)

start_mac = time.time()
#result3_mac = csp.backtracking_search(test3_mac, inference=csp.mac) # Using MAC/AC3
result3_mac = csp.backtracking_search(test3_mac, csp.mrv, csp.lcv, csp.mac)   # Using Fc
duration_mac = time.time() - start_mac

print("Using FC+MRV+LCV, it took %f seconds and %d assignemnts" % (duration_fc, test3_fc.nassigns) )
print("Using MAC+MRV+LCV, it took %f seconds and %d assignemnts" % (duration_mac, test3_mac.nassigns) )


################################# Kakuro4 #########################

print("Kakuro4")

test4_fc = MyKakuro(kakuro4)

start_fc = time.time()
#result4_fc = csp.backtracking_search(test4_fc, inference=csp.forward_checking)   # Using Fc
result4_fc = csp.backtracking_search(test4_fc, csp.mrv, csp.lcv, csp.forward_checking)   # Using Fc
duration_fc = time.time() - start_fc



test4_mac = MyKakuro(kakuro4)

start_mac = time.time()
#result4_mac = csp.backtracking_search(test4_mac, inference=csp.mac) # Using MAC/AC3
result4_mac = csp.backtracking_search(test4_mac, csp.mrv, csp.lcv, csp.mac) # Using MAC/AC3
duration_mac = time.time() - start_mac

print("Using FC+MRV+LCV, it took %f seconds and %d assignemnts" % (duration_fc, test4_fc.nassigns) )
print("Using MAC+MRV+LCV, it took %f seconds and %d assignemnts" % (duration_mac, test4_mac.nassigns) )

print("\n")

print("The resulting grids are:\n")

print("Test1")
test1_fc.display(result1_fc)

print("\n")

test1_mac.display(result1_mac)

print("\n")

print("Test2")

test2_fc.display(result2_fc)

print("\n")
test2_mac.display(result2_mac)


print("\n")

print("Test3")

test3_fc.display(result3_fc)

print("\n")

test3_mac.display(result3_mac)


print("\n")

print("Test4")

test4_fc.display(result4_fc)

print("\n\n")

test4_mac.display(result4_mac)


print("\n")

print("Finished\n")
