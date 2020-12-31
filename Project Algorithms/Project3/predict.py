import sys

import keras
import pandas as pd
import numpy as np

from keras import layers,losses

from keras.models import load_model




def Manhattan_Dist(vector1,vector2):

    result = 0

    for i,x in enumerate(vector1):

        result+=  abs( float(vector1[i]) - float(vector2[i]) )

    return result


############## Read the input file #######################

data = pd.read_csv(sys.argv[2], delimiter = ',', usecols = range(1,129) )
# ignore timestamp => column 0

############## Load model ########################
model = load_model('WindDenseNN.h5')

############## Predict result ####################
result_array = model.predict(data)
result = pd.DataFrame(result_array) #turn it into data frame (used later)

######################### Read the actual.csv ############################

result1 = pd.read_csv("actual.csv", delimiter = ',', usecols = range(1,8) )
#ignore timestamp => column 0

result_array1 = result1.to_numpy(copy=True)

################### Compute ERRORS #####################################################

mse = 0
mae = 0
mape = 0

mse_list = []
mae_list = []
mape_list = []

for i,v in enumerate(result_array):  #for each vector

    dist = Manhattan_Dist(v, result_array1[i]) 

    temp =  dist ** 2 
    mse_list.append(temp)
    
    mae_list.append(dist)
    
    # MAPE
    temp = dist / ( result_array1[i].sum() )	# actual_array
    mape_list.append(temp)
    
    # the sum inside MAPE is basically:
    #
    #   Manhattan_Dist(true_v - false_v) / Manhattan_Dist( true_v - np.zeros( (1,7) ) )
    #
    #   np.zeros() creates a ZERO vector ==> zero coordinates ==> dimension = 7
    #   the denominator is basically the sum of all coordinates of true_vector


mse = sum(mse_list) / len(mse_list)
mae = sum(mae_list) / len(mae_list)

mape = ( sum(mape_list) / len(mape_list) ) * 100    # this is a percentage

# Create final DataFrame and print into predicted.csv
out_f = "predicted.csv"

df = pd.DataFrame([["MAE:",mae,"MAPE:",mape,"MSE:",mse]])
df.to_csv(out_f, header=False, index_label=False,index=False)


df1 = pd.read_csv(sys.argv[2], delimiter = ',', usecols = [0])
#the 1st column ==> timestamps

df2 = pd.concat([df1,result], axis=1)
df2.to_csv(out_f, mode='a', header=False,index_label=False,index=False)

print("Finished\n")





