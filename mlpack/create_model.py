from pandas import read_csv
import numpy as np
import os
import tensorflow as tf
import keras
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

y = []
ref = X[0] #Premier coup utile pour séparer les différentes parties dans le dataset
nb_mov = 1
for i in range(1, len(X)):
    if(X[i] != ref):
        nb_mov++
    else:
        nb_mov = 1
        y += [nb_mov for j in range(nb_mov)]



y_c1 = df[768]
y_c1 = np.array(y_c1)
y_l1 = df[769]
y_l1 = np.array(y_l1)
y_c2 = df[770]
y_c2 = np.array(y_c2)
y_l2 = df[771]
y_l2 = np.array(y_l2)


X_train, X_test, y_c1_train, y_c1_test = sklearn.model_selection.train_test_split(X, y_c1, test_size=0.3, random_state = 42)
y_c1_train = tf.one_hot(y_c1_train, nb_classes)
y_c1_test = tf.one_hot(y_c1_test, nb_classes)
#%%
print("Taille train set", len(X_train))
model1 = Sequential()

model1.add(Conv2D(64, 3, strides = (2, 1), activation='relu', input_shape=(8, 8, 12)))
model1.add(MaxPooling2D(pool_size=(2, 2), strides=None, padding="valid", data_format=None,))
model1.add(Conv2D(32, 3, activation='relu'))
model1.add(Flatten(data_format = None))
model1.add(Dense(nb_classes, name = 'c1'))

model = model1
model.summary()
model.compile(optimizer='Adam', loss='mse', metrics=["accuracy"])
#%%

print('Training Network...')
earlystop = keras.callbacks.EarlyStopping(monitor='loss', min_delta=0, patience=250, verbose=0, mode='auto', baseline=None, restore_best_weights=True)
history = model.fit(X_train, y_c1_train, batch_size = batch_size, epochs = epochs, verbose = 2, validation_split=0.2, callbacks = [earlystop])

test_scores = model.evaluate(X_test, y_c1_test, verbose = 2)
print("Test accuracy : ", test_scores[1])
print("Test loss : ", test_scores[0])

plt.plot(history.history['accuracy'])











