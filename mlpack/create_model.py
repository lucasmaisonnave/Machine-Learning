from pandas import read_csv
import numpy as np
import os
import tensorflow
from keras import callbacks, optimizers
from keras.layers import (LSTM, BatchNormalization, Dense, Dropout, Flatten,
                          TimeDistributed)
from keras.layers.convolutional import Conv2D, MaxPooling2D
from keras.models import Sequential, load_model, model_from_json
from matplotlib import pyplot as plt
import sklearn
import sklearn.model_selection

#%%
print("Load Data")
FILE_NAME = "./data/DataSet_processed.csv"
BLANC = 1
NOIR = 0
epochs = 20
batch_size = 64
nb_classes = 7

df = read_csv(FILE_NAME, header = None)
df = df[df[772] == BLANC]

X = df.loc[:, :767]
X = np.array(X)
X = X.reshape(len(X), 8, 8, 12)
y_c1 = df[768]
y_c1 = np.array(y_c1)
y_l1 = df[769]
y_l1 = np.array(y_l1)
y_c2 = df[770]
y_c2 = np.array(y_c2)
y_l2 = df[771]
y_l2 = np.array(y_l2)


X_train, X_test, y_c1_train, y_c1_test = sklearn.model_selection.train_test_split(X, y_c1, test_size=0.3, random_state = 42)

#%%
print("Taille train set", len(X_train))
model1 = Sequential()
model1.add(Conv2D(filters=64, kernel_size=1, activation='relu', input_shape=(8, 8, 12)))
model1.add(MaxPooling2D())
model1.add(Conv2D(filters=32, kernel_size=1, activation='relu'))
model1.add(MaxPooling2D())
model1.add(Conv2D(filters=10, kernel_size=1, activation='relu'))
model1.add(Flatten())
model1.add(BatchNormalization())
model1.add(Dense(nb_classes,activation = 'softmax'))

model2 = Sequential()
model2.add(Dense(256,activation = 'relu', input_shape = (8*8*12,)))
model2.add(Dense(128,activation = 'relu', kernel_regularizer="l2"))
model2.add(Dropout(0.2))
model2.add(Dense(256,activation = 'relu', kernel_regularizer="l2"))
model2.add(Dropout(0.2))
model2.add(Dense(128,activation = 'relu', kernel_regularizer="l2"))
model2.add(Dropout(0.2))
model2.add(Dense(32,activation = 'relu', kernel_regularizer="l2"))
model2.add(Dropout(0.2))
model2.add(Dense(nb_classes,activation = 'softmax'))




model = model1
model.summary()
model.compile(optimizer='Nadam', loss='mse', metrics=["accuracy"])
#%%

print('Training Network...')
history = model.fit(X_train, y_c1_train, batch_size = batch_size, epochs = epochs, verbose = 2, validation_split=0.2)

test_scores = model.evaluate(X_test, y_c1_test, verbose = 2)
print("Test accuracy : ", test_scores[0])
print("Test loss : ", test_scores[1])

plt.plot(history.history['accuracy'])











