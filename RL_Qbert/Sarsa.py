import sys, os
import numpy as np
import random
import math
import random
from datetime import datetime
import pandas

def wait():
    programPause = raw_input("Press the <ENTER> key to continue...")

class SarsaLookup:
	#The number of actions considered by the learner */
	numActions=2  #int

	#The usual set of SARSA/epsilon-greedy parameters */
	#Learning rate */
	alpha = 0.1
	#Discount factor */
	gamma = 0.999
	#Probability of a random action */
	epsilon = 0.05
	#Reward to be used when the first time visiting a non visited state action pair
	optimisticR = 26
	#A set of variables used to perform the SARSA update */
	lastAction=0    #int
	lastActState = (-1,)	#tuple containing action,feature1, feature2.....
	#qtable will be a nested dictionary with action - qbert Pos tuple as first
	# level.  2nd level is full action-state tuple as key, with Qval stored
	qtable = dict()

	#ntable will be a nested dictionary used in SelectAction function, will 
	ntable = dict()

	#this 2d list holds information on when qbert is in what position, what are the feature
	# numbers which would be the blocks qbert can jump on to.  sub lists are described as so
	# [qbertrow, qbert colm, pos in feature vec of Neigbhor 1, "" Neigbhor 2,...]
	blockNeighbors = [[1,4,10,11],
	[2,4,9,12,13], [2,5,9,13,14],
	[3,3,10,15,16],[3,4,10,11,16,17], [3,5,10,11,16,17], [3,6,11,17,18],
	[5,2,12,19,20], [5,4,12,13,20,21], [5,5,13,14,21,22], [5,7,14,22,23],
	[6,1,15,24,25], [6,3,15,16,25,26], [6,4,16,17,26,27], [6,6,16,18,27,28], [6,8,18,28,29],
	[8,1,19], [8,2,19,20], [8,4,20,21], [8,5,21,22], [8,7,22,23], [8,8,23]]

	def __init__(self, numlegal):
		self.numActions=numlegal
		self.alpha = 0.1
		self.gamma = 0.999
		self.optimisticR = 26
		self.lastAction=0
		self.lastActState=(-1,)
		self.qtable = dict()
		self.ntable = dict()

	def makeTuple(self, action, state):
		actStateTuple = (action, )
		for i in state:
			actStateTuple = actStateTuple + (i,)
		return actStateTuple

	###############Description#####################################
	#this function will look for an approximate match with the lookup feature
	# if it doesn't find something qualifying as an approximate state, it will
	#  enter the optimistic Reward.  Will also enter looked up state into the 
	#   qlookup table if it doesn't exist in the lookup table.
	###############Output#####################################
	# this function will output the selected action, as well as
	# the state-action tuple
	###############Input#####################################
	# this function takes in a tuple lookup key described by
	#(action, feature vec)
	#the feature vector used is the described in CondScreenFunc.py
	def qpredict (self, lkey):
		#first check to see if this act and Qbert Position have been
		# encountered, if not just return optimistic value
		actPosKey = (lkey[0], lkey[1], lkey[2])
		if(self.qtable.get(actPosKey)==None):
			return self.optimisticR, lkey

		#next see if this same exact state has been encountered, if so
		# return in it
		inqtable = self.qtable.get(actPosKey,{}).get(lkey)
		if(inqtable!=None):
			return inqtable, lkey

		#make variables from key values for easier code reading
		qRow=lkey[1]
		qClm=lkey[2]
		prpRow=lkey[3]
		prpClm=lkey[4]
		grnRow=lkey[5]
		grnClm=lkey[6]
		platLeft=lkey[7]
		platRight=lkey[8]

		#boolean to see if Monster guy is in position where he could potentially jump on qbert
		# if so, we'll make sure to check states in Q(s,a) for monster position
		prpdist = abs(qRow-prpRow)+abs(qClm - prpClm)
		grndist = abs(qRow-grnRow)+abs(qClm - grnClm)
		if(prpdist<5):
			checkPrp=True
		else:
			checkPrp=False
		if(grndist<5):
			checkGrn=True
		else:
			checkGrn=False

		##if on the same row though, monster can't jump on qbert
		if (prpRow==qRow):
			checkPrp=False
		if (grnRow==qRow):
			checkGrn=False
		#checks to see if qbert is next to platform locations
		# if not, has little impact on qbert decision, won't consider distance calc
		checkRPlat = (qRow==6) and (qClm==8)
		checkLPlat = (qRow==6) and (qClm==1)

		neigborList=[]
		for lst in self.blockNeighbors:
			if(lst[0]==qRow and lst[1]==qClm):
				for i in range(2, len(lst)):
					neigborList.append(lst[i])

		closestState = float("inf")
		closestKey = lkey
		closestQval = 0.001

		for key in self.qtable[actPosKey]:

			stateDist = float("inf")
			#make sure action state have same action
			if (lkey[0] != key[0] or qRow!=key[1] or qClm!=key[2]):
				continue

			else:
				stateDist = 0
				#if monster is close we want an exact match to begin learning from,
				#but we will take info from a state that is close
				if (checkPrp and(prpRow!=key[3] and prpClm!=key[4]) ):
					stateDist += 100
				if (checkGrn and (grnRow!=key[5] and grnClm!=key[6]) ):
					stateDist += 100
					
				#if qbert is next to right platform and  there is difference between states
				# platform existance, do not treat them as the same. If not tally difference
				if(checkLPlat):
					if(platLeft!=key[7]):
						stateDist = float("inf")
						continue
				else:
					stateDist += abs(lkey[7]-key[7])
				#now do same for right platform
				if(checkRPlat):
					if(key[8]!=platRight):
						stateDist = float("inf")
						continue
				else:
					stateDist = abs(platRight-key[8])

				#Now tally up differences between flipped blocks
				# give extra weight to distances that are neighbors of qbert position
				for i in range (9, len(key)):
					if (i in neigborList):
						stateDist += abs(key[i]-lkey[i])*10
					else:
						stateDist += abs(key[i]-lkey[i])
				#if the total distance between key and new state is zero,
				# you have found an exact match keep it
				
				#if no diff between lookup state and Dictionary state use that key and lookup
				#values
				if(stateDist==0):
					return self.qtable.get(key, self.optimisticR), key
				#if not check to see if it is the closest state
				elif (stateDist<closestState):
					closestState = stateDist
					closestKey = key
					closestQval = self.qtable[actPosKey][key]

				
		#if the distances are close treat as the same state,
		if(closestState<10):
			return closestQval, closestKey

		#if the distances between state are far but have matching Qbert pos,
		# and action can use closest action-state for prediction val
		elif(closestState<float("inf")):
			return closestQval, lkey
		#if no match exists lookup totally new 
		else: 
			return self.optimisticR, lkey


	###############Description#####################################
	#this function will update the sarsa lookup table with appropriate Q value
	###############Output#####################################
	# None
	###############Input#####################################
	# this function takes in a tuple lookup key described by
	#(action, feature vec)
	#and reward from observing game state
	def sarsaUpdate(self, lkey , reward):
		#First handle state of brand new qbert act and pos
		#create new sub dictionary and enter the key value
		actPosKey = (lkey[0], lkey[1], lkey[2])
		lastAPKey=(self.lastActState[0], self.lastActState[1], self.lastActState[2])
		if(self.qtable.get(actPosKey)==None):
			self.qtable[actPosKey]={}
			self.qtable[actPosKey][lkey]=self.optimisticR
		if(self.qtable.get(lastAPKey)==None):
			self.qtable[lastAPKey]={}
		
		#dictionary command which will get the value associated with keyvalue
		#	prntActState, if it doesn't find it will return optimisticR
		present_qsa=self.qtable.get(actPosKey).get(lkey, self.optimisticR)

		
		past_qsa=self.qtable.get(lastAPKey,{}).get(self.lastActState, self.optimisticR)

		#heart of learning, updating table value with Sarsa update equation
		self.qtable[lastAPKey][self.lastActState] = \
		past_qsa + self.alpha* (reward+self.gamma*present_qsa-past_qsa )

	###############Description#####################################
	#argmax equivalent, will check potential actions and select state
	# with maximum expected value, implements N[s,a] frequency check
	# and random action selection with pruning for negative states
	###############Output#####################################
	# action, action-feature tuple which can be used in key for lookup Q(s,a), N(s,a) tables
	###############Input#####################################
	# this function takes in a tuple lookup key described by
	# feature vec described in ScreenFunc.py
	def selectA(self, state):
		# print "select A",
		bestValue = float("inf")
		bestValue = -1*bestValue
		worstValue = float("inf")

		bestAction = -1
		bestKey = (0,)
		ties = []       # new array list<integer>

		#when random action selection, only choose from
		# items with positive q value
		positiveList=[]	
		keyList=[]		#used in case all Qvals are negative

		#tracks which values have been skipped due to frequency, 
		#will draw from these incase all other actions are negative,
		# and skipped item has positive expected outcome
		nSkipList=[]	

		stateTuple = ()
		#first turn feature set into state
		for i in state:
			stateTuple = stateTuple + (i,)

		for a in range(self.numActions):
			Qval=0.1

			#add in act to create act state tuple
			actStateTuple= (a,)+stateTuple

			Qval, aKey = self.qpredict(actStateTuple)
			keyList.append(aKey)

			# If it has positive reward add to list of pos
			# reward states, which are selected from in event
			# of random selection
			if Qval>0:
				positiveList.append(aKey)

			#check if action state has been visited too much, if so
			# skip from consideration, and reset count to zero
			if(self.ntable.get(aKey)>2):
				self.ntable[aKey]=0
				nSkipList.append((aKey, Qval))
				continue

			#checks to find the maximum
			if (Qval > bestValue):
				bestValue = Qval
				bestAction = a
				bestKey = aKey

			elif (Qval == bestValue):
				ties.append(aKey)

			if (Qval < worstValue):
				worstValue = Qval

			#Tie-breaker
			if (len(ties) > 1):
				r = int(random.random()*len(ties))
				bestKey = ties[r]
				bestAction = bestKey[0]

		#if going to select random action, only select action
		# from positive action - state combinations,
		# this should greatly reduce random death jumps and encourage
		# better exploration
		
		if (random.random() < self.epsilon):
			if(len(positiveList)<=0):
				r = int(random.random() * len(keyList))
				self.ntable[keyList[r]]=1 + self.ntable.get(keyList[r],0)
				return keyList[r][0], keyList[r]
			else:
				r = int(random.random() * len(positiveList))
				self.ntable[positiveList[r]]=1 + self.ntable.get(positiveList[r],0)
				return positiveList[r][0], positiveList[r]

		#if there are only negative things left should check to see if
		# N skipped value is positive, may be only positive choice such 
		# as the bottom right / left pyramids
		if(bestValue<0 and len(nSkipList)>0):
			for tple in nSkipList:
				if (tple[1]>bestValue and tple[1]>0):
					bestValue=tple[1]
					bestKey = tple[0]
					bestAction = tple[0][0]

		self.ntable[bestKey]=1 + self.ntable.get(bestKey,0)
		return bestAction, bestKey

	###############Description#####################################
	#this function will output a csv version of qtable
	###############Output#####################################
	#will output a csv file with qtable dict entries row will follow format
	#key1, key2, Q(s,a)val
	###############Input#####################################
	# will take in string filename to give name to qbert file

	def qtab_output(self, qtabFile):
		count=0
		clms = range(34)
		outDF = pandas.DataFrame(columns=clms)
		for key1 in self.qtable:
			firstkeylist=[]

			for i in range(len(key1)):
				firstkeylist.append(key1[i])
			for key2 in self.qtable[key1]:
				scndkeylist=[]
				for i in range(len(key2)):
					scndkeylist.append(key2[i])

				merged = firstkeylist+scndkeylist
				qval = self.qtable[key1][key2]
				merged.append(qval)

				npmerged = np.array(merged, dtype=float)
				tmpdf=pandas.DataFrame(np.atleast_2d(npmerged))

				frames = [outDF, tmpdf]

				outDF = pandas.concat(frames)

		outDF.to_csv(qtabFile, header=False)

	###############Description#####################################
	#read the csv output of the function above, and update the rlagent
	###############Output#####################################
	#None
	###############Input#####################################
	# will take in string filename to read from
	def qtab_input(self, inputFile):
		inputDF = pandas.read_csv(inputFile, header=None)
		tmpdict = dict()

		for index, row in inputDF.iterrows():
			key1=tuple()
			for i in range(1,4):
				tmptple = (int(row[i]),)
				key1 = key1+tmptple

			key2=tuple()
			for i in range(4, 34):
				tmptple = (int(row[i]),)
				key2 = key2+tmptple

			qval=row[34]

			if(tmpdict.get(key1)==None):
				tmpdict[key1]={}

			tmpdict[key1][key2]=qval


		self.qtable = tmpdict