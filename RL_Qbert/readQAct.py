import sys, os
import numpy as np
import random
from ale_python_interface import ALEInterface
import ScreenFunc as sf     #scrn_to_features, ftrs_#print
import Sarsa as srs
import pandas

#used for debugging
def wait():
    programPause = raw_input("Press the <ENTER> key to continue...")


#########################################################3
############ Loading and Prepping Varables
#########################################################3

if len(sys.argv) < 2:
	sys.exit()

ale = ALEInterface()

# Get & Set the desired settings
##########################################
random.seed(99)
ale.setInt(b'random_seed', 99)
ale.setInt(b'frame_skip', 0)


# Set USE_SDL to true to display the screen. ALE must be compilied
# with SDL enabled for this to work. On OSX, pygame init is used to
# proxy-call SDL_main.
USE_SDL = True
if USE_SDL:
	if sys.platform == 'darwin':
		import pygame
		pygame.init()
		ale.setBool('sound', True) # Sound doesn't work on OSX
	elif sys.platform.startswith('linux'):
		ale.setBool('sound', True)
		ale.setBool('display_screen', True)

# Load the ROM file
rom_file = str.encode(sys.argv[1])
ale.loadROM(rom_file)

# Get the list of legal actions
legal_actions = range(4)

##print("ftrs type", type(ftrs))
#sf.ftrs_#print(ftrs, 10, 10)
#print(len(legal_actions))
rewardlist = []
mvcountlist = []
growthQtable =[]

#########################################################3
############ Loop for multiple learning sessions
#########################################################3
for z in range(0, 1):
	(screen_width,screen_height) = ale.getScreenDims()
	rawscrn = np.zeros(screen_width*screen_height,dtype=np.uint8)
	ale.getScreen(rawscrn)

	ram_size = ale.getRAMSize()
	scrnramnew = np.zeros((ram_size),dtype=np.uint8)
	ale.getRAM(scrnramnew)

	qbertpos=[]
	rlagent = srs.SarsaLookup(4)

	######################################################
	inputfile = "QtabEp499"
	######################################################

	rlagent.qtab_input(inputfile)

	actstolearn = 0

	framecnt=0

	fcntSinceValid=1000

	#########################################################3
	############ Loop for Each Learning Session
	#########################################################3
	for episode in range(500):
		#sf.ftrs_#print(ftrs, 10, 10)
		total_reward=0
		reward = 0
		pasttotal=0
		mvcount=0

		catalystAction = 0
		catalystReward = 0

		fcntSinceValid=1000

		#########################################################
		############ Loop for Each Game
		#########################################################
		while not ale.game_over():
			###Getting Qbert location info and screen features
			ale.getRAM(scrnramnew)
			oldqbert = qbertpos
			qbertpos = (scrnramnew[33], scrnramnew[43])
			framecnt+=1

			ale.getScreen(rawscrn)

			fcntSinceValid+=1	#counts number of frames since last valid frame

			#if statement to checks to see if qbert has changed position. 
			# It has to ignore false positions that show up in ram that are not 
			#	associated with qbert decision movements
			validpos = False
			if(sf.qbertValidPos(qbertpos, fcntSinceValid)):
				validpos = True

			#check screen to see if qbert is there and pull out
			# relevant colors for each level
			qonscreen, lvlclrs = sf.qPrsnt_lvlclrs(rawscrn)

			##########################################
			#####If a valid move to learn from is made
			##########################################
			if ((scrnramnew[0]==0)  \
				and (scrnramnew[len(scrnramnew)-1]==181) \
				and qonscreen and validpos\
				and framecnt > 29):

				fpsFtrs = sf.scrn_to_ftrs(rawscrn, lvlclrs)

				a=0
				aKey= (0,)
				a, aKey = rlagent.selectA(fpsFtrs)

				#debugging stuff
				actPosKey = (aKey[0], aKey[1], aKey[2])
				# print(a, rlagent.qtable.get(actPosKey,{}).get(aKey))
			
				reward = ale.act(a+2)
				total_reward+=reward
				catalystReward= total_reward - pasttotal

				#records the total reward at the current state to compare with
				# when looking at the next state
				pasttotal = total_reward


				#situation of first move no update
				if(mvcount==0):
					rlagent.lastActState=aKey
					catalystReward=0
				#this penalizes the reward if qbert jumps off the map
				#  the selction of the current state which could be positive can
				#  feed into the past state, which can make jumping off the edge
				#  or onto monster seem good. This properly penalizes this notion
				elif(fcntSinceValid>30):
					# #print("penalizing?", catalystReward, fpsFtrs[0], fpsFtrs[1])
					catalystReward=catalystReward-100
					rlagent.sarsaUpdate(aKey, catalystReward)
					#After learning, update features and actions for this move
					#learning for this move will occur after reward is found
					rlagent.lastActState=aKey
					catalystReward=0
				else:
					#first do the learning of the previous catalystAction
					# #print("updating?", catalystReward, fpsFtrs[0], fpsFtrs[1])
					rlagent.sarsaUpdate(aKey, catalystReward)
					#After learning update features and actions for this move
					#	learning will be done after reward is found
					rlagent.lastActState=aKey
					catalystReward=0

				#update move and count since last valid frame 
				fcntSinceValid= 0
				mvcount +=1

			##########################################
			#####If an intermediate frame
			##########################################
			else:

				#this is the do nothing move, move shouldn't matter, but just in case
				a = 0
				reward = ale.act(a)

				total_reward+=reward
				catalystReward= total_reward - pasttotal

		ale.reset_game()

		##########################################
		#####Recording info before begin next game
		##########################################
		qtablesize=0
		for key in rlagent.qtable:
			qtablesize+=len(rlagent.qtable[key])

		rewardlist.append(total_reward)
		mvcountlist.append(mvcount)
		growthQtable.append(qtablesize)

		print('RandSeed %d Episode %d score %d moves %d size %d' \
			% (z, episode, total_reward, mvcount, qtablesize))

		if((episode+1)%25==0):
			outputfile = "Ap4qtableEpisode"+str(episode)
			rlagent.qtab_output(outputfile)

			qtabSer = pandas.Series(growthQtable)
			rewSer = pandas.Series(rewardlist)


			qtabSer.to_csv("qtabgrowth10ap4.csv")
			rewSer.to_csv("rewardlist10ap4.csv")	