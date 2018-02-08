import pandas as pd
import numpy as np
import os
import glob
import operator

def genretonum(str):
    if str=='classical':
        return 0
    if str == 'country':
        return 1
    if str == 'edm_dance':
        return 2
    if str == 'jazz':
        return 3
    if str == 'kids':
        return 4
    if str == 'latin':
        return 5
    if str == 'metal':
        return 6
    if str == 'pop':
        return 7
    if str == 'rnb':
        return 8
    if str == 'rock':
        return 9

def numtogenre(i):
    if i==0:
        return 'classical'
    if i==1:
        return 'country'
    if i==2:
        return 'edm_dance'
    if i==3:
        return 'jazz'
    if i==4:
        return 'kids'
    if i==5:
        return 'latin'
    if i==6:
        return 'metal'
    if i==7:
        return 'pop'
    if i==8:
        return 'rnb'
    if i==9:
        return 'rock'

################################
###Prepping Data################
################################

####creating answer key
labelsfile_ = 'C:\Mgenre\song_data\labels.csv' # use your path
answerKeyDf = pd.read_csv(labelsfile_,index_col=0, header=0)

#####creating music genre Training segment dataframe
path =r'C:\Mgenre\song_data\training_small' # use your path
allFiles = glob.glob(path + "/*")
trn_df = pd.DataFrame()   #[row_indxr, clm_indxr]
trn_genre=pd.DataFrame()  #contains song ID for each vector position
                            #trn_genre[i] holdscorrect genre for trn row i
for file_ in allFiles:
    tmpdflong = pd.read_csv(file_,index_col=None, header=None)
    rowcnt = len(tmpdflong)
    ####condensing each song into 60 rows
    tmpdf = pd.DataFrame(np.zeros((60, 12)))
    for i in range(0, 60):
        brow = int((i/60)*rowcnt)
        trow = int(((i+1)/60)*rowcnt)
        tmp=tmpdflong.iloc[brow:trow-1]
        for j in range(0,12):
            tmpdf.iat[i,j] = tmp[j].mean()
    #concacting tmp list to total training dataframe
    trn_df= pd.concat([trn_df, tmpdf])
    
    #Updating list of trn rows genre assignment
    songgenre = genretonum(answerKeyDf.at[os.path.basename(file_),'category'])
    tmpsonglist = [songgenre]*len(tmpdf)
    tmpgenre=pd.DataFrame(data=tmpsonglist, index=None, columns=None)
    trn_genre= pd.concat([trn_genre, tmpgenre])  #row, song ID
    
reducedFile = "60threduced.csv"
print(reducedFile)
writefile = open(reducedFile, 'w')
trn_df.to_csv(reducedFile,header=None, index=None)
writefile.close()

reducedlabels = "60thlabels.csv"
writefile = open(reducedlabels, 'w')
trn_genre.to_csv(reducedlabels,header=None, index=None)
writefile.close()

###blocked out code can be used when doing a lot of testing to save time on
###reconstructing the reduced data set each time
###will have to update the file paths training and training labels
###also comment out everthing in the Prepping Data Section
'''
################################
###Beginning of Testing#########
################################
####creating answer key
labelsfile_ = r'C:\Mgenre\song_data\kreduced\60thGenres.csv' # use your path
answerKeyDf = pd.read_csv(labelsfile_,index_col=None, header=None)

#####creating music genre Training segment dataframe
trnfile =r'C:\Mgenre\song_data\kreduced\60thReducedRows.csv' # use your path
trn_df = pd.read_csv(trnfile, index_col=None, header=None)

'''


#####Run Testing 
path =r'C:\Mgenre\song_data\test_small' # use your path
allFiles = glob.glob(path + "/*")
cnt_file=0
knnassgn=[] #list of tuples (test_song id, classification)
trn_df = trn_df.values
for file_ in allFiles:
    print(("completed file number ",cnt_file))
    cnt_file+=1
    tstsongid = os.path.basename(file_)
    tstsong_df = pd.read_csv(file_,index_col=None, header=None)
    tstsong_df=tstsong_df.values
    cumdist = [0]*(len(trn_df))     #list that holds cumulative distance to each row in training set
    for i in range(0, len(trn_df)):
        for j in range(0, len(tstsong_df)):
            cumdist[i]+=np.linalg.norm(trn_df[i,:]-tstsong_df[j,:])
    #####calc cumulative dist for each row in training set, to each row in tst song
    k=550
    kclosest = []   #holds tuple of (nbr row pos, distance), sorted from smallest dist to longest
    
    #loop to build initial k training rows analyzed
    for i in range(0, k):
        kclosest.append((i,cumdist[i]))
    kclosest.sort(key=operator.itemgetter(1))
    #loop to for rest of training rows, adds neighbor, sorts kclosest, and removes furthest
    for i in range(k,len(trn_df)):
        #if distance less than greatest dist in kNN, find proper place in kNN
        if(cumdist[i]<kclosest[k-1][1]):
            kclosest.append((i,cumdist[i]))
            kclosest.sort(key=operator.itemgetter(1))
            #remove furthest
            kclosest.pop()
    
    ####Computing majority of kNN
    genrecnt=[0]*10     #list to tabulate how many of each genre there are in kclosest
    for i in range(0, len(kclosest)):
        songrow=int(kclosest[i][0])
        songgenre=int(trn_genre.iat[songrow,0])
        genrecnt[songgenre]+=1
    
    #this line computes maximum(val) and max-pos(idx) in genrecnt 
    val, idx = max((val, idx) for (idx, val) in enumerate(genrecnt))
    knnassgn.append((tstsongid, numtogenre(idx)))
     
 #writing file ID and assignment to test labels file   
NeigborDistFile = "knn_tstlabels.csv"
writefile = open(NeigborDistFile, 'w')
for tple in knnassgn:
    wrtstr=str(tple[0])+","+(tple[1])+"\n"
    writefile.write(wrtstr)
writefile.close()

