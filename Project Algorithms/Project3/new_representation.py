import sys

import keras
import pandas as pd
import numpy as np

from keras import layers,losses

from keras.models import load_model




############## Read the input file #######################

data = pd.read_csv(sys.argv[2], delimiter = ',', usecols = range(1,129) )
# ignore timestamp => column 0

############## Load model ########################
old_model = load_model('WindDenseNN.h5')

weights = old_model.layers[0].get_weights()

################## Create our new model ################################
model = keras.Sequential()

model.add(layers.Dense(64, input_shape=(128,), activation='softmax' ) )
#model.add(layers.Dense(64, input_shape=(128,), activation='relu' ) )

model.layers[0].set_weights(weights)    # insert the weights of our pre-trained model into our new one

################### Predict result ##########################################
result_array = model.predict(data)
result = pd.DataFrame(result_array) #turn it into data frame (used later)


################################# Create new_representation.csv ######################################

df1 = pd.read_csv(sys.argv[2], delimiter = ',', usecols = [0])
#the 1st column ==> timestamps

df2 = pd.concat([df1,result], axis=1)
df2.to_csv("new_representation.csv", header=False,index_label=False,index=False)

print("Finished\n")

