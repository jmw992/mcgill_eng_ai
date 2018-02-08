import sys, os
import numpy as np
import matplotlib.pyplot as plt
import pandas

def wait():
    programPause = raw_input("Press the <ENTER> key to continue...")



#this function will check if qbert is on the screen and will return the 
#important colors
def qPrsnt_lvlclrs(scrnarr):
  (values,counts) = np.unique(scrnarr,return_counts=True)
  pyramidNullColor=0
  pyramidNullPos=0

  qbertonscrn=False

  #for loop finds null pyramid color and checks if qbert on scrn
  for i in range(len(counts)):
    if(counts[i]<8000 and counts[i]>4000):
      pyramidNullColor = values[i]
    elif(values[i]==52 and counts[i]>17):
      qbertonscrn = True

  ##defines lvl colors which will be used to identify important
  # features about pyramid blocks
  if(pyramidNullColor==184):
    #flipped clr, untoched color, null clr
    lvlclrs = [26, 148, 184]
    #flipped clr, untoched color, null clr
    return qbertonscrn, lvlclrs
  elif(pyramidNullColor==38):
    #flipped clr, untoched color, null clr
    lvlclrs = [148, 26, 38]
    return qbertonscrn, lvlclrs
  else:
    #flipped clr, untoched color, null clr
    lvlclrs = [0,0,0]
    return qbertonscrn, lvlclrs

def scrn_to_ftrs(scrnarr, lvlclrs):
  '''
  this function will take the screen color list
  and it will return 1d feature vec
  will average the raw color values present in grid
  0 and 184 are raw representation of background color
  #https://stackoverflow.com/questions/6252280/find-the-most-frequent-number-in-a-numpy-vector
  
  '''
  qbrawcolor = 52
  prpSnakecolor = 102
  greencolor = 196

  flippedcolor = lvlclrs[0]
  unchangedclr = lvlclrs[1]
  pyramidNullClr = lvlclrs[2]

  unqlist =np.zeros(1,)
  qblist = [] # will hold pixels of qbert location
  prpSnake = [] # will hold pixels of purple snake
  greenMeanie = [] #will hold pixel location of the green meanie

  ftrvec=[]

  for i in range(210):
    for j in range(160):
      pixelcolor = scrnarr[i*160+ j]  
      #first ignore all bacground colors 
      if pixelcolor==0:
        continue
      elif pixelcolor==pyramidNullClr:
        continue
      elif pixelcolor ==unchangedclr:
        continue
      elif pixelcolor ==flippedcolor:
        continue
      #start add monster colors
      elif pixelcolor== qbrawcolor:
        qblist.append((i,j))
      elif pixelcolor == prpSnakecolor:
        prpSnake.append((i,j))
      elif pixelcolor == greencolor:
        greenMeanie.append((i,j))

  #######section of for loops is  goint go calculate
  ######centroids of moving characters
  ###### will convert their positions into 10x10 cells
  qbcentroid=0
  xsum=0
  ysum=0
  for i in range(len(qblist)):
    xsum+=qblist[i][0]
    ysum+=qblist[i][1]
  xmean = int(xsum/len(qblist)/21)  #want these converted to basis of 10x10 grid
  ymean = int(ysum/len(qblist)/16)  #want these converted to basis of 10x10 grid
  ftrvec.append(xmean)
  ftrvec.append(ymean)
  ##getting purple snake position into feature vector
  if(len(prpSnake)==0):
    ftrvec.append(-99)
    ftrvec.append(-99) #-99 will be flag for it being offscreen
  else:
    for i in range(len(prpSnake)):
      xsum+=prpSnake[i][0]
      ysum+=prpSnake[i][1]
    xmean = int(xsum/len(prpSnake)/21)  #want these converted to basis of 10x10 grid
    ymean = int(ysum/len(prpSnake)/16)  #want these converted to basis of 10x10 grid
    ftrvec.append(xmean)
    ftrvec.append(ymean)
  ##getting green guy position into feature vector
  if(len(greenMeanie)==0):
    ftrvec.append(-99)
    ftrvec.append(-99)   #-99 will be flag for it being offscreen 
  else:  
    for i in range(len(greenMeanie)):
      xsum+=greenMeanie[i][0]
      ysum+=greenMeanie[i][1]
    xmean = int(xsum/len(greenMeanie)/21)   #want these converted to basis of 10x10 grid
    ymean = int(ysum/len(greenMeanie)/16)   #want these converted to basis of 10x10 grid
    ftrvec.append(xmean)
    ftrvec.append(ymean)

  ##Next check to see if left and right platforms are present
  #left platform
  if(scrnarr[138*160+15]!=0):
    ftrvec.append(1)
  else:
    ftrvec.append(0)
  #right platform
  if(scrnarr[138*160+142]!=0):
    ftrvec.append(1)
  else:
    ftrvec.append(0)

  ###Next will check for features of whether blocks have been flipped or not
  ##checks to points on each block just incase one point is covered by character
  blocklocations = pandas.read_csv("blockpositions.csv", header=None, names=None)
  # print(blocklocations)
  for row in range(len(blocklocations)):
    j = blocklocations.iat[row,0]
    i = blocklocations.iat[row,1]
    jj = (blocklocations.iat[row,2])
    ii = blocklocations.iat[row,3]

    if (scrnarr[i*160+j]==flippedcolor):
      ftrvec.append(1)
    elif (scrnarr[ii*160+jj]==flippedcolor):
      ftrvec.append(1)
    else:
      ftrvec.append(0)
  
  return ftrvec

def print_ftrs(featureVec):
  #print out 3 characters and two platform x,y positions
  for i in range(0, 7, 2):
    print(featureVec[i], featureVec[i+1])
  i=7
  for x in range(1):
    i+=1
    print featureVec[i],
  print '\n',
  for x in range(2):
    i+=1
    print featureVec[i],
  print '\n',
  for x in range(3):
    i+=1
    print featureVec[i],
  print '\n',
  for x in range(4):
    i+=1
    print featureVec[i],
  print '\n',
  for x in range(5):
    i+=1
    print featureVec[i],
  print '\n',
  for x in range(6):
    i+=1
    print featureVec[i],
  print '\n',

#function will take an input tuple of qbert positions representing displayed positions
#   when checking ram for qbert, will check if these correspond with states associated
#     with jumping inside the pyramid, not falling or regenerating
def qbertValidPos(qbertpos, cntsincevalid):

  ###Thanks to Sam Otis for pulling this info RAM
  ###it is list of valid positions QBERT can have in ram, when not falling and or invisible
  validPositions= [[69, 77], [92, 77], [120, 65], [97, 65], [147, 53], [124, 53],
           [152, 41], [175, 41], [180, 29], [203, 29], [231, 16], [231, 41],
           [175, 65], [180, 53], [203, 53], [147, 77], [120, 93], [152, 65],
           [231, 65], [97, 93], [180, 77], [231, 93], [180, 105],
           [147, 105], [203, 77], [175, 77], [175, 117], [231, 117], [203, 129],
           [203, 105], [180, 129], [231, 141], [152, 117], [124, 77], [124, 105],
           [152, 93], [47,89], [175,93], [92, 77], [17,77]]
           # [175, 93]
  # print("sf valid pos",qbertpos[0], qbertpos[1])

  for i in range(len(validPositions)):
    # print("lst v pos", validPositions[i][0], validPositions[i][1])
    if(qbertpos[0]== validPositions[i][0] \
     and qbertpos[1]== validPositions[i][1]):
        # print("Valid Positions returned true")
        if(cntsincevalid==30):
          return True
        #this handles the instance of the qbert falling through
        #   and passing by valid position
        elif not (cntsincevalid>30 and cntsincevalid<150):
          return True
  return False

# used in debugging
# def getramdif(oldram, newram):
#   for i in range (len(newram)):
#     if oldram[i]!= newram[i]:
#       if not(i==112 or i==116 or i==121 or i==122):
#         tmp = str(i)+","+str(oldram[i])+","+str(newram[i])
#         print(tmp)