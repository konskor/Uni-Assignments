# multiAgents.py
# --------------
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


from util import manhattanDistance
from game import Directions
import random, util

from game import Agent

class ReflexAgent(Agent):
    """
      A reflex agent chooses an action at each choice point by examining
      its alternatives via a state evaluation function.

      The code below is provided as a guide.  You are welcome to change
      it in any way you see fit, so long as you don't touch our method
      headers.
    """


    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {North, South, West, East, Stop}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]
        chosenIndex = random.choice(bestIndices) # Pick randomly among the best

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getFood()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]

        "*** YOUR CODE HERE ***"
        
        from util import manhattanDistance
        
        FList = newFood.asList()                # Food List
        GList = successorGameState.getGhostPositions()  # ghost positions as list
        
        FDist = []      # List of distances from all the food
        GDist = []      # List of distances from all the ghosts
        
        # Fill FDist
        for x in FList:
            fd = manhattanDistance(newPos,x)
            FDist.append(fd)
        
        # Fill GDist
        for x in GList:
            gd = manhattanDistance(newPos,x)
            GDist.append(gd)
        
        
        # Now we start the evaluation procedure
        evaluation = 0  # our evaluation value
        
        for dist in FDist:
        
            if dist == 0:
                evaluation = evaluation + 2
            else:
                evaluation = evaluation + (1.0/dist)
                #use the reciprocal of distance to food ==> example : 
                #example: dist = 5 ==> evaluation = evaluation + 1/5
        
        for dist in GDist:
        
            if dist == 0:   # Encountered ghost ==> Lose game ==> BAD
                evaluation = evaluation - 2
                # maybe 1-evaluation (just a thought)
            else:
                evaluation = evaluation - (1.0/dist)
        
        
        return successorGameState.getScore() + evaluation


def scoreEvaluationFunction(currentGameState):
    """
      This default evaluation function just returns the score of the state.
      The score is the same one displayed in the Pacman GUI.

      This evaluation function is meant for use with adversarial search agents
      (not reflex agents).
    """
    return currentGameState.getScore()

class MultiAgentSearchAgent(Agent):
    """
      This class provides some common elements to all of your
      multi-agent searchers.  Any methods defined here will be available
      to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

      You *do not* need to make any changes here, but you can if you want to
      add functionality to all your adversarial search agents.  Please do not
      remove anything, however.

      Note: this is an abstract class: one that should not be instantiated.  It's
      only partially specified, and designed to be extended.  Agent (game.py)
      is another abstract class.
    """

    def __init__(self, evalFn = 'scoreEvaluationFunction', depth = '2'):
        self.index = 0 # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)

class MinimaxAgent(MultiAgentSearchAgent):
    """
      Your minimax agent (question 2)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action from the current gameState using self.depth
          and self.evaluationFunction.

          Here are some method calls that might be useful when implementing minimax.

          gameState.getLegalActions(agentIndex):
            Returns a list of legal actions for an agent
            agentIndex=0 means Pacman, ghosts are >= 1

          gameState.generateSuccessor(agentIndex, action):
            Returns the successor game state after an agent takes an action

          gameState.getNumAgents():
            Returns the total number of agents in the game
        """
        "*** YOUR CODE HERE ***"
        
        #In our problem, we have MAX-Player ==> Pacman (agent 0)
        #
        # and MIN-Players ==> Ghosts ( agent 1 up to gamestate.getNumAgents() )
        
        def MiniMax_Decision(gameState):
        
            actions = gameState.getLegalActions(self.index)
            
            v = -float("inf")
            result = None
            
            for x in actions:
            
                successor = gameState.generateSuccessor(self.index,x)
                y = Min_Value(successor,self.index+1,0)
                
                if y > v:
                    v = y
                    result = x
        
            return result
        
        
        def Max_Value(gameState,depth):

            # Check termination conditions
            if depth == self.depth: # reached max Depth ==> reached Leaf node
                return self.evaluationFunction(gameState)
        
        
            actions = gameState.getLegalActions(self.index)
            # Pacman can't do anything ==> return
            if actions == []:  # No legal actions available
                return self.evaluationFunction(gameState)
                
        
            v = -float("inf")
            for x in actions:
            
                successor = gameState.generateSuccessor(self.index,x)
                
                v1 = Min_Value(successor,self.index+1,depth)
                if v1 > v:
                    v = v1
            return v
        
        
        def Min_Value(gameState,agent,depth):
        
            # Check termination conditions
            actions = gameState.getLegalActions(agent)

            # Agent can't do anything ==> return
            if actions == []:  # No legal actions available
                return self.evaluationFunction(gameState)

            # Who is going to be the next agent
            next_agent = agent + 1
            if agent == gameState.getNumAgents() -1:
                # The last ghost is currently responding
                # Next agent will be Packman
                next_agent = self.index
                
                # Also, we are moving to a new_depth = depth+1
                new_depth = depth + 1

            v = float("inf")
            for x in actions:
            
                successor = gameState.generateSuccessor(agent,x)
                
                if next_agent == 0: # pacman's turn ==> Max_value()
                    v1 = Max_Value(successor,new_depth)
                else:
                    v1 = Min_Value(successor,next_agent,depth)

                if v1 < v:
                    v = v1
            return v


        #Return of get Action
        return MiniMax_Decision(gameState)

    

class AlphaBetaAgent(MultiAgentSearchAgent):
    """
      Your minimax agent with alpha-beta pruning (question 3)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action using self.depth and self.evaluationFunction
        """
        "*** YOUR CODE HERE ***"
        
        
                #In our problem, we have MAX-Player ==> Pacman (agent 0)
        #
        # and MIN-Players ==> Ghosts ( agent 1 up to gamestate.getNumAgents() )


        def Alfa_Beta_Search(gameState):
        
            a = -float("inf")
            b = float("inf")
        
            actions = gameState.getLegalActions(self.index)
            
            v = -float("inf")
            result = None
            
            for x in actions:
            
                successor = gameState.generateSuccessor(self.index,x)
                v = Min_Value(successor,self.index+1,0,a,b)
                
                if v > a:   # update a
                    a = v
                    result = x
        
            return result



        
        def Max_Value(gameState,depth,a,b):

            # Check termination conditions
            if depth == self.depth: # reached max Depth ==> reached Leaf node
                return self.evaluationFunction(gameState)
        
        
            actions = gameState.getLegalActions(self.index)
            # Pacman can't do anything ==> return
            if actions == []:  # No legal actions available
                return self.evaluationFunction(gameState)
                
        
            v = -float("inf")
            for x in actions:
            
                successor = gameState.generateSuccessor(self.index,x)
                
                v1 = Min_Value(successor,self.index+1,depth,a,b)
                if v1 > v:
                    v = v1
                # Now v has max value 
                    
                    
                # Pruning ( v is bigger than b)
                if v > b:   # no need to search anymore
                    return v
                    
                #Else, check if we need to update a 
                if v > a:
                    a = v
                    
                    
            return v
        
        
        def Min_Value(gameState,agent,depth,a,b):
        
            # Check termination conditions
            actions = gameState.getLegalActions(agent)

            # Agent can't do anything ==> return
            if actions == []:  # No legal actions available
                return self.evaluationFunction(gameState)

            # Who is going to be the next agent
        
            next_agent = agent + 1
            if agent == gameState.getNumAgents() -1:
                # The last ghost is currently responding
                # Next agent will be Packman
                next_agent = self.index
                
                # Also, we are moving to a new_depth = depth+1
                new_depth = depth + 1

            v = float("inf")
            for x in actions:
            
                successor = gameState.generateSuccessor(agent,x)
                
                if next_agent == 0: # pacman's turn ==> Max_value()
                    v1 = Max_Value(successor,new_depth,a,b)
                else:
                    v1 = Min_Value(successor,next_agent,depth,a,b)

                if v1 < v:
                    v = v1
                # v has minimum value
                if v < a:   # Stop searching ( v is smaller than a)
                    return v
                    
                # Check if we need to update  b
                if v < b:
                    b = v
                    
                    
            return v


        #Return of get Action
        return Alfa_Beta_Search(gameState)


class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    def getAction(self, gameState):
        """
          Returns the expectimax action using self.depth and self.evaluationFunction

          All ghosts should be modeled as choosing uniformly at random from their
          legal moves.
        """
        "*** YOUR CODE HERE ***"
        
        # With Expectimax, the ghosts no longer pick the optimal choice
        # Now they choose at random (chance = 1/ #actions) uniformly
        
        # So we will take Minimax algorithm 
        # and modify the behavior of MIN-players
        
        def ExpectiMax_Decision(gameState):
        
            actions = gameState.getLegalActions(self.index)
            
            v = -float("inf")
            result = None
            
            for x in actions:
            
                successor = gameState.generateSuccessor(self.index,x)
                y = Min_Value(successor,self.index+1,0)
                
                if y > v:
                    v = y
                    result = x
        
            return result
        
        
        def Max_Value(gameState,depth):

            # Check termination conditions
            if depth == self.depth: # reached max Depth ==> reached Leaf node
                return self.evaluationFunction(gameState)
        
        
            actions = gameState.getLegalActions(self.index)
            # Pacman can't do anything ==> return
            if actions == []:  # No legal actions available
                return self.evaluationFunction(gameState)
                
        
            v = -float("inf")
            for x in actions:
            
                successor = gameState.generateSuccessor(self.index,x)
                
                v1 = Min_Value(successor,self.index+1,depth)
                if v1 > v:
                    v = v1
            return v
        
        
        def Min_Value(gameState,agent,depth):
        
            # Check termination conditions
            actions = gameState.getLegalActions(agent)

            # Agent can't do anything ==> return
            if actions == []:  # No legal actions available
                return self.evaluationFunction(gameState)

            # Who is going to be the next agent
            next_agent = agent + 1
            if agent == gameState.getNumAgents() -1:
                # The last ghost is currently responding
                # Next agent will be Packman
                next_agent = self.index
                
                # Also, we are moving to a new_depth = depth+1
                new_depth = depth + 1

            result = 0  #our sum 
            for x in actions:
            
                successor = gameState.generateSuccessor(agent,x)
                
                if next_agent == 0: # pacman's turn ==> Max_value()
                    result = result + Max_Value(successor,new_depth)
                else:
                    result = result + Min_Value(successor,next_agent,depth)
                    
            result1 = float(result)/float(len(actions))
            
            return result1


        #Return of get Action
        return ExpectiMax_Decision(gameState)

def betterEvaluationFunction(currentGameState):
    """
      Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
      evaluation function (question 5).

      DESCRIPTION: <write something here so we know what you did>
    """
    "*** YOUR CODE HERE ***"
    
    """DESCRIPTION: Almost same with previous evaluation function
                    However:
                    1)We include as features , the capsules and scared ghosts
                    2) We evaluate states , not actions ==> curent state, not successor
                    
                    Notes:
                    1)We shall use the reciprocal of values ( not 5 , but 1/5)
                    2) Eating scared ghosts is more beneficial than just eating food,
                    so we will have to arrange the weight accordingly (maybe 2x)
                    
    """
    
    
    
    
    # Useful information you can extract from a GameState (pacman.py)
    Food = currentGameState.getFood()
    GhostStates = currentGameState.getGhostStates()
    ScaredTimes = [ghostState.scaredTimer for ghostState in GhostStates]

    #This one is new
    Capsules = currentGameState.getCapsules()
    current_position = currentGameState.getPacmanPosition()

    from util import manhattanDistance
        
    FList = Food.asList()                # Food List
    FDist = []      # List of distances from all the food
    
    CDist = []     # List of distances from all the capsules
    
    # Changed
    GBrave = []     # list of positions of brave ghosts
    GScared = []    # lis of positions of scared ghosts
    
    # Fill the two lists
    for x in GhostStates:
        if x.scaredTimer > 0:
            GScared.append(x.getPosition())
        else:
            GBrave.append(x.getPosition())
    
    #INCOMPLETE ==> fill the 2 below (and more)
    GBDist = []      # List of distances from all the Brave ghosts
    GSDist = []     # List of distances from all the scared ghosts


    # Fill FDist
    for x in FList:
       fd = manhattanDistance(current_position,x)
       FDist.append(fd)
        
    # Fill GDist
    for x in GScared:
       gd = manhattanDistance(current_position,x)
       GSDist.append(gd)
       
    for x in GBrave:
        gd = manhattanDistance(current_position,x)
        GBDist.append(gd)

    # Fill CDist
    for x in Capsules:
       cd = manhattanDistance(current_position,x)
       CDist.append(cd)
        
        
    # Now we start the evaluation procedure
    evaluation = 0  # our evaluation value
        
        
    #Check for food
    for dist in FDist:  # check the food
        
       if dist == 0:
           evaluation = evaluation + 2      #changed this from one to 2
       else:
           evaluation = evaluation + (1.0/dist)
           #use the reciprocal of distance to food ==> example : 
           #example: dist = 5 ==> evaluation = evaluation + 1/5


    # Check for brave ghosts (not scared)
    for dist in GBDist:

        if dist == 0:   # Encountered ghost ==> Lose game ==> BAD
            evaluation = evaluation - 2
        else:
            evaluation = evaluation - (1.0/dist)
            
    # Check for scared ghosts 
    for dist in GSDist:

        if dist == 0:   # Encountered scared ghost ==> eat it ==> Bonus points good
            evaluation = evaluation + 4
        else:
            evaluation = evaluation + 2 * (1.0/dist)
            
    # Check for capsules
    for dist in CDist:

        if dist == 0:   # Encountered ghost ==> Lose game ==> BAD
            evaluation = evaluation + 3
        else:
            evaluation = evaluation + 1.5 * (1.0/dist)
    
    
    
    
    return currentGameState.getScore() + evaluation



# Abbreviation
better = betterEvaluationFunction

