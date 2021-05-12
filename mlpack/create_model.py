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

winner = np.array(df[773])
X = df.loc[:, :767]
X = np.array(X)
ref = X[0] #Premier coup utile pour séparer les différentes parties dans le dataset
y = []

nb_mov = 1
for i in range(1, len(X)):
    if(not np.array_equal(X[i], ref)):
        nb_mov += 1
        if(i == (len(X) - 1)):
            w = winner[i-1]
            L = [w*(j+1)/nb_mov for j in range(nb_mov)]
            y = np.concatenate((y, L), axis = 0)
    else:
        w = winner[i-1]
        L = [w*(j+1)/nb_mov for j in range(nb_mov)]
        y = np.concatenate((y, L), axis = 0)
        nb_mov = 1

X = X.reshape(len(X), 8, 8, 12)


y_c1 = df[768]
y_c1 = np.array(y_c1)
y_l1 = df[769]
y_l1 = np.array(y_l1)
y_c2 = df[770]
y_c2 = np.array(y_c2)
y_l2 = df[771]
y_l2 = np.array(y_l2)


X_train, X_test, y_train, y_test = sklearn.model_selection.train_test_split(X, y, test_size=0.3, random_state = 42)

#%%
print("Taille train set", len(X_train))
model = Sequential()
model.add(Conv2D(filters=64, kernel_size=1, activation='relu', input_shape=(8,8,12)))
model.add(MaxPooling2D())
model.add(Conv2D(filters=24, kernel_size=1, activation='relu'))
model.add(MaxPooling2D())
model.add(Conv2D(filters=10, kernel_size=1, activation='relu'))
model.add(Flatten())
model.add(BatchNormalization())
model.add(Dense(40,activation = 'relu'))
model.add(Dense(1,activation = 'tanh'))

model.summary()
model.compile(optimizer='Adam', loss='mse', metrics=["mse"])
#%%

print('Training Network...')
earlystop = keras.callbacks.EarlyStopping(monitor='loss', min_delta=0, patience=250, verbose=0, mode='auto', baseline=None, restore_best_weights=True)
history = model.fit(X_train, y_train, batch_size = batch_size, epochs = epochs, verbose = 2, validation_split=0.2, callbacks = [earlystop])

test_scores = model.evaluate(X_test, y_test, verbose = 2)
print("Test loss : ", test_scores[0])

plt.plot(history.history['loss'], label = "train", c = "blue")
plt.plot(history.history['val_loss'],label = "validation", c = "red")
plt.xlabel("epochs")
plt.ylabel("loss")
plt.legend()
plt.show()

model.save("./models/model_h1", save_format='tf')











