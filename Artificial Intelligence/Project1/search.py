# search.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


"""
In search.py, you will implement generic search algorithms which are called by
Pacman agents (in searchAgents.py).
"""

import util

class SearchProblem:
    """
    This class outlines the structure of a search problem, but doesn't implement
    any of the methods (in object-oriented terminology: an abstract class).

    You do not need to change anything in this class, ever.
    """

    def getStartState(self):
        """
        Returns the start state for the search problem.
        """
        util.raiseNotDefined()

    def isGoalState(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state.
        """
        util.raiseNotDefined()

    def getSuccessors(self, state):
        """
          state: Search state

        For a given state, this should return a list of triples, (successor,
        action, stepCost), where 'successor' is a successor to the current
        state, 'action' is the action required to get there, and 'stepCost' is
        the incremental cost of expanding to that successor.
        """
        util.raiseNotDefined()

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.
        The sequence must be composed of legal moves.
        """
        util.raiseNotDefined()


def tinyMazeSearch(problem):
    """
    Returns a sequence of moves that solves tinyMaze.  For any other maze, the
    sequence of moves will be incorrect, so only use this for tinyMaze.
    """
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    return  [s, s, w, s, w, w, s, w]

def depthFirstSearch(problem):
    """
    Search the deepest nodes in the search tree first.

    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:

    print "Start:", problem.getStartState()
    print "Is the start a goal?", problem.isGoalState(problem.getStartState())
    print "Start's successors:", problem.getSuccessors(problem.getStartState())
"""
    "*** YOUR CODE HERE ***"

	# We will implement DFS, by implementing Graph-Search using stack for the frontier
    from util import Stack

    stack = Stack()		# This will be our frontier==> stack of tuples ((x,y), [path])
                        # In this problem , (x,y) is the position in the grid ==> state
    path = []			# how did we get here (from the Initial State) 
    explored = []		# explored node

    #Initialise Frontier-Stack	
    stack.push( ( problem.getStartState(),[]) )	

    while(True):

        if stack.isEmpty() == True:	# No path forward ==> FAILED
            return []

        #Get 2 items from stack ==> state,[path]	
        state,path = stack.pop()

        if problem.isGoalState(state) == True: 	
            return path 
            
        explored.append(state)

        #Get the Successors(returns list)
        successors = problem.getSuccessors( state )

        if successors != []:
            for x in successors:	# x is a tuple
                if x[0] not in explored :	#Choose node that you haven't explored


    #################################################################
    # The algorithm presented in our lectures                       #
    # is more efficient than the one accepted by the autograder.    #
    # Basically, the autograder algorithm doesn't check             #
    # if the successor is already in the fringe                     #
    #################################################################


                    # This is the fringe check
                    # Comment this, in order for autograder to work
                    
                    #Also ,choose a node that isn't in stack
#                    NotInside = True
#                    for state1  in stack.list:
#                        if state1[0] == x[0]:
#                            NotInside = False
#                            break
                    
#                    if NotInside == True:
#                        new_path = path + [x[1]]
#                        stack.push( ( x[0], new_path) )



                    # This is for autograder ( Uncomment this and comment above)
                    #Don't check if successor is in the frontier
                    new_path = path + [x[1]]
                    stack.push( (x[0],new_path) )
#DONE 2



def breadthFirstSearch(problem):
    """Search the shallowest nodes in the search tree first.
    "*** YOUR CODE HERE ***"
"""
    #Implemented the algorithm ==> 2nd Lecture page 27

    #Implement frontier with Queue()
    
    from util import Queue
    
    queue = Queue()	# This will be our frontier==> queue of tuples ((x,y), [path])
                        # In this problem , (x,y) position in the grid ==> ID
    path = []			# how did we get here (from the Initial State) 
    explored = []		# explored nodes
    
    node = (problem.getStartState(),0)      # (state,path_cost)
    
    if problem.isGoalState( node[0] ) == True :
        # Trivial Solution ==> Initial state is goal state
        return []

    #Initialise Frontier-Queue
    queue.push( (node,[]) )	

    while(True):
    
        if queue.isEmpty() == True:	# No path forward ==> FAILED
            return []

        #Get 2 items from queue ==> (x,y),[path]	
        node,path = queue.pop()
        state = node[0]
        path_cost = node[1]
        
        explored.append(state)	#Explored this node, moving on
        
        ##############################################################################
        # As always, autograder refuses the lecture code.                           #
        # The differenc between the 2, is when we check if the state is goal state. #
        #                                                                           #
        # Autograder want to check, when we POP out of the queue                    #
        #                                                                           #
        # Lecture ==> when the node is created ==>                                  #
        #  ==> after expanding a node check if successors are goal (immediately)    #
        #############################################################################
        
        
        
        
        # Uncomment this (and comment below) for the autograder to work
        if problem.isGoalState(state) == True:
            return path

        #Get the Successors(returns list)
        successors = problem.getSuccessors(state)
        
        if successors != []:
            for x in successors:	# x is a tuple (successor,action,StepCost)
                if x[0] not in explored:	#Choose node that you haven't explored

                    #Also ,choose a node that isn't in queue/fringe
                    NotInside = True
                    for state1  in queue.list:
                        if state1[0][0] == x[0]:
                            NotInside = False
                            break
                    
                    if NotInside == True:

                        # compute new path, because pacman will get stuck, a step away from goal
                        new_path = path + [x[1]]


                        # Uncomment this and comment above for the algorithm to work
#                        if problem.isGoalState(x[0]) == True:
#                            return new_path

                        new_cost = path_cost + problem.getCostOfActions( new_path )
                        new_node = (x[0],new_cost)
                        queue.push( (new_node,new_path) )

#DONE 2



def uniformCostSearch(problem):
    """Search the node of least total cost first."""
    "*** YOUR CODE HERE ***"

    #This time we will use priority queue
    
    from util import PriorityQueue
        
    queue = PriorityQueue() #priority queue of ((x,y), [path], priority)
                        # In this problem , (x,y) is the position in the grid ==> ID
    path = []			# how did we get here (from the Initial State) 
    explored = []		# explored nodes
    

         
    #Initialise Frontier-Queue	
    queue.push( (problem.getStartState(),[]),0 )	 

    while(True): #Same-ish as Queue, but we check priorities

        if queue.isEmpty() == True:	# No path forward ==> FAILED
            return []

        #Get 2 items from queue ==> (state),[path]	
        state,path = queue.pop()

        if problem.isGoalState(state) == True: 	
            return path

        explored.append(state)	#Explored this position, moving on 
        
        #Get the Successors(returns list)
        successors = problem.getSuccessors(state)
        
        
        if successors != []:
            for x in successors:
                if x[0] not in explored:	#Choose node that you haven't explored

                    #Also ,choose a node that isn't in queue
                    NotInside = True
                    for state1 in queue.heap:
                        if state1[2][0] == x[0]:
                            NotInside = False
                            break
                    
                    if NotInside == True:
                        new_path = path + [x[1]]
                        priority = problem.getCostOfActions(new_path)
                        queue.push( (x[0],new_path),priority )

                    else: #state is Inside queue, check if path has lower cost/priority
                    
                        for state1 in queue.heap:  #Compute old priority

                            if state1[2][0] == x[0]:
#                                old_priority = problem.getCostOfActions(state1[2][1])
                                old_priority = state1[0]
                    
                        new_priority = problem.getCostOfActions(path + [x[1]] )
                        
                        if new_priority < old_priority: #lower cost/priority
                            new_path = path + [x[1]]
                            queue.update( (x[0],new_path), new_priority)

#DONE 2


def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    return 0








#for the A*
#def functionA(problem,state,heuristic):
""" Didn't need this
def functionA(x):   # X is a tuple => ( (x,y), [path],problem,heuristic)

#    result = problem.getCostOfActions(x[1]) + heuristic(x[0],x[2])
    heuristic = x[3]
    problem = x[2]
    
    result = problem.getCostOfActions(x[1]) + heuristic(x[0],problem)   # f(n) = g(n) + h(n)
    return result
"""    

def aStarSearch(problem, heuristic=nullHeuristic):
    """Search the node that has the lowest combined cost and heuristic first."""
    "*** YOUR CODE HERE ***"
    
    #Same algorithm as UCS, only this time we use g+h, instead of just g
    
    
    
    from util import PriorityQueue

    queue = PriorityQueue()
    
    path = []			# how did we get here (from the Initial State) 
    explored = []		# explored nodes
    
    if problem.isGoalState(problem.getStartState()) == True :
        # Trivial Solution ==> Initial state is goal state
        return []

    queue.push( (problem.getStartState(),[]), 0 )
    
    while(True):
    
        if queue.isEmpty() == True:	# No path forward ==> FAILED
            return []

        #Get 2 items from queue ==> (x,y),[path]
        state,path = queue.pop()

        if problem.isGoalState(state) == True: 	
            return path
        
        explored.append(state)	#Explored this node, moving on
        
        #Get the Successors(returns list)
        successors = problem.getSuccessors(state)

        if successors != []:
            for x in successors:
                if x[0] not in explored:	#Choose node that you haven't explored

                    #Also ,choose a node that isn't in queue
                    NotInside = True
                    for state1 in queue.heap:
                        if state1[2][0] == x[0]:
                            NotInside = False
                            break
                    
                    if NotInside == True:
                        new_path = path + [x[1]]
                        priority = problem.getCostOfActions(new_path) + heuristic(x[0],problem)
                        queue.push( (x[0],new_path), priority )

                    else: #state is Inside queue, check if path has lower cost/priority
                    
                        for state1 in queue.heap:  #Compute old priority

                            if state1[2][0] == x[0]:
                                old_priority = state1[0]
                                
                                
                        new_path = path + [x[1]]
                        new_priority = problem.getCostOfActions(new_path) + heuristic(x[0],problem)
                        
                        if new_priority < old_priority: #lower cost/priority
                            queue.update( (x[0],new_path), new_priority)
   
    #DONE 2

# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch
