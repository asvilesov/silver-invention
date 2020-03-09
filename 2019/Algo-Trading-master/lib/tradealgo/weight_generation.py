import tensorflow as tf
import numpy as np 
from tensorflow import keras
import pandas as pd
import matplotlib.pyplot as plt


def learn_weights(file, epochs = 100):
	f = open(file, 'r')
	first_line = f.readline()
	list_label = []
	list_data = []
	while first_line != "":
		data = first_line.split()
		data = list(map(float, data))
		label = data.pop()
		list_label.append(label)
		list_data.append(data)
		first_line = f.readline()

	print(len(list_data[0]))
	print(len(list_label))

	df_data = pd.DataFrame(list_data)
	encode = keras.utils.to_categorical(list_label)
	train_stats = df_data.describe()
	train_stats = train_stats.T
	print(train_stats)

	def norm(x):
	  return (x - train_stats['mean']) / train_stats['std']

	normed_train_data = norm(df_data)

	print(encode)

	model = keras.Sequential([
	    keras.layers.Dense(40, activation='relu', input_shape=[len(list_data[0])]),
	    keras.layers.Dense(30, activation='relu'),
	    keras.layers.Dense(20, activation='tanh'),
	    keras.layers.Dense(10, activation='tanh'),
	    keras.layers.Dense(2, activation=tf.nn.sigmoid)
	])

	optimizer = tf.keras.optimizers.RMSprop(0.001)

	# model.compile(loss='mean_squared_error',
	# 				optimizer=optimizer,
	# 				metrics=['mean_absolute_error', 'mean_squared_error'])
	model.compile(optimizer='adam',
	              loss='sparse_categorical_crossentropy',
	              metrics=['accuracy'])

	model.fit(normed_train_data, list_label, epochs = epochs)

	test_loss, test_acc = model.evaluate(normed_train_data, list_label)

	print('Test accuracy:', test_acc)

	return model




if __name__ == '__main__':

	f = open("ML_data_recursive_momentum_function_record_AAPL.txt", 'r')

	first_line = f.readline()
	list_label = []
	list_data = []

	while first_line != "":
		data = first_line.split()
		data = list(map(float, data))
		label = data.pop()
		list_label.append(label)
		list_data.append(data)
		first_line = f.readline()







	print(len(list_data[0]))
	print(len(list_label))


	df_data = pd.DataFrame(list_data)
	# df_labels = pd.DataFrame(list_label)
	# df_labels['positive'] = (df_labels == 0)*1.0
	# df_labels['negative'] = (df_labels == 1)*1.0
	encode = keras.utils.to_categorical(list_label)

	train_stats = df_data.describe()
	train_stats = train_stats.T
	print(train_stats)


	def norm(x):
	  return (x - train_stats['mean']) / train_stats['std']


	normed_train_data = norm(df_data)

	print(normed_train_data)
	print(encode)

	model = keras.Sequential([
	    keras.layers.Dense(64, activation=tf.nn.relu, input_shape=[len(list_data[0])]),
	    keras.layers.Dense(64, activation=tf.nn.relu),
	    keras.layers.Dense(2, activation=tf.nn.softmax)
	])


	optimizer = tf.keras.optimizers.RMSprop(0.001)

	# model.compile(loss='mean_squared_error',
	# 				optimizer=optimizer,
	# 				metrics=['mean_absolute_error', 'mean_squared_error'])
	model.compile(optimizer='adam',
	              loss='sparse_categorical_crossentropy',
	              metrics=['accuracy'])

	model.fit(normed_train_data, list_label, epochs = 10)

	test_loss, test_acc = model.evaluate(normed_train_data, list_label)

	print('Test accuracy:', test_acc)

	print(model.predict(np.array([[-0.513664,    1.603767,   0.783579,   0.405229,   0.421173,   0.031025,  -0.186649,   0.038496,   0.405949,   0.691756]])))




	#history = model.fit(normed_train_data, encode, epochs = 10, validation_split = 0.2, verbose = 0)

	def plot_history(history):
	  hist = pd.DataFrame(history.history)
	  hist['epoch'] = history.epoch

	  plt.figure()
	  plt.xlabel('Epoch')
	  plt.ylabel('Mean Abs Error [MPG]')
	  plt.plot(hist['epoch'], hist['mean_absolute_error'],
	           label='Train Error')
	  plt.plot(hist['epoch'], hist['val_mean_absolute_error'],
	           label = 'Val Error')
	  plt.ylim([0,5])
	  plt.legend()

	  plt.figure()
	  plt.xlabel('Epoch')
	  plt.ylabel('Mean Square Error [$MPG^2$]')
	  plt.plot(hist['epoch'], hist['mean_squared_error'],
	           label='Train Error')
	  plt.plot(hist['epoch'], hist['val_mean_squared_error'],
	           label = 'Val Error')
	  plt.ylim([0,20])
	  plt.legend()
	  plt.show()

	#plot_history(history)


	#print(pd.read_csv("ML_data_recursive_momentum_function_record_AAPL.txt"))






