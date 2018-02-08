import pandas as pd
import numpy as np
from numpy.linalg import inv
import os
import glob
from sklearn.preprocessing import normalize

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
    
def initialize_df_list(size):
    dflist = []
    for i in range(0, size):
        emptyDF = pd.DataFrame()
        dflist.append(emptyDF)
    return dflist

print("debugger test")

####creating answer key, used for training
labelsfile_ = 'C:\Mgenre\song_data\labels.csv' # use your path
answerKeyDf = pd.read_csv(labelsfile_,index_col=0, header=0)

#####creating music genre Training segment dataframe
path =r'C:\Mgenre\song_data\training_small' # use your path
allFiles = glob.glob(path + "/*")


#####Seperating data into genre subsets
np_train_lst = initialize_df_list(10)      #list of numpy arrays [genre][song_row, attribute]
for file_ in allFiles:
    tmpdf = pd.read_csv(file_,index_col=None, header=None)
    tmpnparr=tmpdf.as_matrix()
    catnum = genretonum(answerKeyDf.at[os.path.basename(file_),'category'])
    if(len(np_train_lst[catnum])>0):
        np_train_lst[catnum] = np.concatenate((np_train_lst[catnum], tmpnparr), axis=0)
    else:
        np_train_lst[catnum] = tmpnparr

#####Creating  mean & cov list for training
trnmeanlist=[]
for i in range(0,10):
    trnmeanlist.append(np.mean(np_train_lst[i], axis=0))
 
trncovlist = [] #[genre][vec_att_i][vec_att_j]
for i in range(0,10):
    trncovlist.append(np.cov(np_train_lst[i].T))
    
#####Run Testing 
path =r'C:\Mgenre\song_data\test' # use your path
allFiles = glob.glob(path + "/*")
accurate_id = 0
tstassgn=[]  #output vector for each song, will hold list of tuples (song_id, genre)
for file_ in allFiles:

    tstsongid = os.path.basename(file_)
    tstsong_df = pd.read_csv(file_,index_col=None, header=None) #[vec_att][row]
    tstsong_np = tstsong_df.as_matrix()

    tstmean=np.mean(tstsong_np, axis=0)  #[vec_att]
   
    
    ####UNLL
    unll_vec = [None]*10
    for i in range(0,10):
        #creating numpy genre mean vector
        dif_vec = trnmeanlist[i] - tstmean
        trans_dif = dif_vec.T
        inv_gen_cov = inv(trncovlist[i])
        
        step1 = np.dot(dif_vec,inv_gen_cov)
        step2 = np.dot(step1,trans_dif)
   
        unll_vec[i]=step2
        
    val, idx = min((val, idx) for (idx, val) in enumerate(unll_vec))
    tstassgn.append((tstsongid, idx))

####Outputing test file classification
NeigborDistFile = "ngauss_tstlabels.csv"
writefile = open(NeigborDistFile, 'w')
writefile.write("Id,Category\n")
for i in range(0, len(tstassgn)):
    wrtstr=str(tstassgn[i][0])+","+numtogenre(tstassgn[i][1])+"\n"
    writefile.write(wrtstr)

writefile.close()

