import matplotlib
import matplotlib.pyplot as plt
import tradealgo.algos_stats as algos_stats
import alpaca_trade_api as tradeapi
import tradealgo.weight_generation as wg
import time
import numpy as np
import pandas as pd
import os.path
import math


#second based simulator

def record_stats(file_name, data): #has to be in csv data format already
		if os.path.isfile(file_name) != True:
			f = open(file_name, 'w')
			f.write(data)
			f.flush()
			f.close

class trading_simulator:

	def __init__(self, buyingpower = 30000, algoname = "default"):
		self.safety = True
		self.buyingpower = buyingpower #total cash in account
		self.portfolio_value = buyingpower #portfolio value
		self.stocks = {}
		self.algoname = algoname
		self.order_flag = "hold"

	def pipline(self, function, ticker, start_date, end_date, data, resolution, safety = True, record = False): #dates have to be in panda timestamp form
		date = start_date
		filename = "history/" + self.algoname + ":" + self.pipline.__name__ + ":" + function.__name__ + ":" + str(start_date) + "-" + str(end_date) + ".txt"
 
		if(date.hour <= 9 and date.minute <= 30):
			date += pd.Timedelta('9 hours') + pd.Timedelta('30 minutes')
			if safety:
				pass

		while(date.year != end_date.year or date.month != end_date.month or date.day != end_date.day or date.hour != 16+4): #unix time now
			if(date.isoweekday() < 6):
				# try:
				algo_vars, data_vars = function(ticker, date, data)
				# except:
				# 	print("Failed on this date: " + str(date))
			date = self.time_simulator(date, resolution)

	def time_simulator(self, time, time_delta):
		time = time + pd.Timedelta('1 min')
		return time

	def plot_trading(self, history):
		plt.figure(0)
		N = len(history)
		x = np.linspace(0.0, 1.0, N)
		plt.plot(x, history)
		print(len(history))

	def buy(self, ticker, current_price, num_stocks = .25):
		num_stocks = math.floor((self.portfolio_value*num_stocks)/current_price)
		if(self.buyingpower - current_price*num_stocks < 0):
			pass
		else:
			self.buyingpower = self.buyingpower - num_stocks*current_price
			self.stocks[ticker] = self.stocks[ticker] + num_stocks
			self.order_flag = "buy"
		self.portfolio_value = self.buyingpower + self.stocks[ticker]*current_price

	def sell(self, ticker, current_price, num_stocks = .25):
		num_stocks = math.floor((self.portfolio_value*num_stocks)/current_price)
		if(self.stocks[ticker] != 0):
			if(self.stocks[ticker] < num_stocks):
				self.sell_all(ticker, current_price)
			else:
				self.buyingpower = self.buyingpower + current_price*num_stocks
				self.stocks[ticker] -= num_stocks
				self.order_flag = "sell"
		self.portfolio_value = self.buyingpower + self.stocks[ticker]*current_price

	def sell_all(self, ticker, current_price):
		if(self.stocks[ticker] != 0):
			self.buyingpower = self.buyingpower + current_price*self.stocks[ticker]
			self.stocks[ticker] = 0
			self.order_flag = "sell"
		self.portfolio_value = self.buyingpower + self.stocks[ticker]*current_price

	def box_trading(self, ticker, data, low, high, lag = 0):
		self.stocks[ticker] = 0
		count = 0
		history = []
		critical_price = 0
		risk = False
		print(self.portfolio_value)
		print(self.buyingpower)
		for i in range(len(data)):
			last_n_seconds = []
			time_pd = pd.to_datetime(data[i]['unix_time'], unit='ms')
			if (time_pd.hour >= 9 or time_pd.minute >= 45) and (time_pd.hour != 19 or time_pd.minute <=45):
				if data[i-210]['price'] < data[i]['price'] and data[i-30]['price'] < data[i]['price']:
					self.sell_all(ticker, data[i+lag]['price'])
					if not risk:
						critical_price = data[i]['price']
					risk = True
					#print("buy")
				if ((1+high)*critical_price) < (data[i]['price']):
					critical_price = data[i]['price']
					#print("hold")
				if (1-low)*critical_price >= data[i]['price']:
					risk = False
					self.buy(ticker, data[i+lag]['price'])
					#print("sell")

				print(str(self.portfolio_value) +" c.p. high " + str((1+high)*critical_price) + " c.p. low " + str((1-low)*critical_price))
				print(str(data[i]['price']) + " 60 seconds ago : " + str(data[i-60]['price']) + " 10 seconds ago : "  + str(data[i-10]['price']))
				print("----------------------------------------")
				self.portfolio_value = self.buyingpower + self.stocks[ticker]*data[i+lag]['price']
				history.append(self.portfolio_value)
			else:
				self.sell_all(ticker, data[i]['price'])
		print(self.portfolio_value)
		self.plot_trading(history)

	def momentum_function(self, ticker, data, n_seconds = 10, lag = 0):
		self.stocks[ticker] = 0
		count = 0
		history = []
		for i in range(len(data)):
			last_n_seconds = []
			time_pd = pd.to_datetime(data[i]['unix_time'], unit='ms')
			if (time_pd.hour >= 9 or time_pd.minute >= 45) and (time_pd.hour != 19 or time_pd.minute <=45):
				for j in range(1,n_seconds+1,1):
					last_n_seconds.append(data[i-j]['price'])
					last_n_seconds.reverse()
				x = np.linspace(1, n_seconds, n_seconds)
				regression = np.polyfit(x, last_n_seconds, 1)
				#if (abs(data[i]['price'] - data[i-1]['price']) < 0.3 ):
				if regression[0] > 0:
					self.buy(ticker, data[i+lag]['price'])
				else:
					self.sell_all(ticker, data[i+lag]['price'])
				#print(str(self.portfolio_value) + " " + str(data[i]['price']) + " " + self.order_flag + " "+str(pd.to_datetime(data[i]['unix_time'], unit='ms')))
				history.append(self.portfolio_value)
			else:
				self.sell_all(ticker, data[i+lag]['price'])
			count += 1
		#print("n_seconds: " + str(n_seconds) + " Balance: " + str(self.portfolio_value) + " " + str(self.momentum_function.__name__))
		plt.figure(0)
		N = len(history)
		x = np.linspace(0.0, 1.0, N)
		plt.plot(x, history)
		print(len(history))

	def recursive_momentum_function(self, ticker, data, n_seconds = 10, lag = 0):
		self.stocks[ticker] = 0
		count = 0
		history = []
		for i in range(len(data)):
			decision = 0
			last_n_seconds = []
			time_pd = pd.to_datetime(data[i]['unix_time'], unit='ms')
			if (time_pd.hour >= 9 or time_pd.minute >= 45) and (time_pd.hour != 19 or time_pd.minute <=45):
				for k in range(2, n_seconds+1,1):
					last_n_seconds = []
					for j in range(1, k+1, 1):
						last_n_seconds.append(data[i-j]['price'])
						last_n_seconds.reverse()
					x = np.linspace(1, k, len(last_n_seconds))
					regression = np.polyfit(x, last_n_seconds, 1)
					if regression[0] > 0:
						decision += 1
					else:
						decision -= 1
				if decision > 0:
					self.buy(ticker, data[i+lag]['price'])
				else:
					self.sell_all(ticker, data[i+lag]['price'])
				#print(str(self.portfolio_value) + " " + str(data[i]['price']) + " " + self.order_flag + " "+str(pd.to_datetime(data[i]['unix_time'], unit='ms')))
				history.append(self.portfolio_value)
			else:
				self.sell_all(ticker, data[i+lag]['price'])
			count += 1
		#print("n_seconds: " + str(n_seconds) + " Balance: " + str(self.portfolio_value) + " " + str(self.momentum_function.__name__))
		plt.figure(0)
		N = len(history)
		x = np.linspace(0.0, 1.0, N)
		plt.plot(x, history)
		print(len(history))


	def reverse_momentum_function(self, ticker, data, n_seconds = 10, lag = 0):
		self.stocks[ticker] = 0
		count = 0
		history = []
		for i in range(len(data)):
			last_n_seconds = []
			time_pd = pd.to_datetime(data[i]['unix_time'], unit='ms')
			if (time_pd.hour >= 9 or time_pd.minute >= 45) and (time_pd.hour != 19 or time_pd.minute <=45):
				for j in range(1,n_seconds+1,1):
					last_n_seconds.append(data[i-j]['price'])
				x = np.linspace(1, n_seconds, n_seconds)
				regression = np.polyfit(x, last_n_seconds, 1)
				#if (abs(data[i]['price'] - data[i-1]['price']) < 0.3 ):
				if regression[0] > 0:
					self.buy(ticker, data[i+lag]['price'])
				else:
					self.sell_all(ticker, data[i+lag]['price'])
				#print(str(self.portfolio_value) + " " + str(data[i]['price']) + " " + self.order_flag + " "+str(pd.to_datetime(data[i]['unix_time'], unit='ms')))
				history.append(self.portfolio_value)
			else:
				self.sell_all(ticker, data[i+lag]['price'])
			count += 1
		print("n_seconds: " + str(n_seconds) + " Balance: " + str(self.portfolio_value) +  " " + str(self.reverse_momentum_function.__name__))
		plt.figure(0)
		N = len(history)
		x = np.linspace(0.0, 1.0, N)
		plt.plot(x, history)
		print(len(history))

	def recursive_momentum_function_record(self, ticker, name, data, n_seconds = 10):
		self.stocks[ticker] = 0
		count = 0
		history = []
		filename = "ML_data_" + name + "_" + self.recursive_momentum_function_record.__name__ + "_" + ticker + ".txt"
		f = open(filename, "w")
		f = open(filename, "a")
		for i in range(len(data)-1):
			decision = 0
			last_n_seconds = []
			recording_coefficients = []
			time_pd = pd.to_datetime(data[i]['unix_time'], unit='ms')
			if (time_pd.hour >= 9 or time_pd.minute >= 45) and (time_pd.hour != 19 or time_pd.minute <=45):
				for k in range(2, n_seconds+1,1):
					last_n_seconds = []
					for j in range(1, k+1, 1):
						last_n_seconds.append(data[i-j]['price'])
					last_n_seconds.reverse()
					#print(last_n_seconds)
					x = np.linspace(1, k, len(last_n_seconds))
					#print(x)
					regression = np.polyfit(x, last_n_seconds, 1)
					recording_coefficients.append(regression[0])
					#print(str(regression[0]) + " " + str(regression[1]))
				current_difference = data[i]['price'] - data[i-1]['price']
			label = 0
			if data[i+1]['price'] > data[i]['price']:
				label = 1
			if len(recording_coefficients) > 1:
				data_string = str(current_difference) + " " + " ".join(str(e) for e in recording_coefficients) + " " + str(label) + "\n"
				print(data_string)
				f.write(data_string)



	def recursive_momentum_function_trained(self, ticker, data, training_data, n_seconds = 10, epochs = 100, lag = 0):
		self.stocks[ticker] = 0
		count = 0
		history = []
		model = wg.learn_weights(training_data, epochs = epochs)
		for i in range(len(data)):
			decision = 0
			last_n_seconds = []
			recording_coefficients = []
			time_pd = pd.to_datetime(data[i]['unix_time'], unit='ms')
			if (time_pd.hour >= 9 or time_pd.minute >= 45) and (time_pd.hour != 19 or time_pd.minute <=45):
				for k in range(2, n_seconds+1,1):
					last_n_seconds = []
					for j in range(1, k+1, 1):
						last_n_seconds.append(data[i-j]['price'])
					last_n_seconds.reverse()
					#print(last_n_seconds)
					x = np.linspace(1, k, len(last_n_seconds))
					#print(x)
					regression = np.polyfit(x, last_n_seconds, 1)
					recording_coefficients.append(regression[0])
					#print(str(regression[0]) + " " + str(regression[1]))
				current_difference = data[i]['price'] - data[i-1]['price']
				recording_coefficients.insert(0, current_difference)
				#print(model.predict(np.array([recording_coefficients]))[0][0])
				if model.predict(np.array([recording_coefficients]))[0][0] < 0.45:
					self.buy(ticker, data[i+lag]['price'])
					count += 1
				else:
					self.sell_all(ticker, data[i+lag]['price'])
				print(str(self.portfolio_value) + " " + str(count) + " " +str(model.predict(np.array([recording_coefficients]))[0][0]))
				history.append(self.portfolio_value)
		plt.figure(0)
		N = len(history)
		x = np.linspace(0.0, 1.0, N)
		plt.plot(x, history)
		#record as (diff curr-past),(coefficients), (volume), (label)


	def momentum_function_quick_sell(self, ticker, data, n_seconds = 10, quick_sell_seconds = 5, lag = 0):
		self.stocks[ticker] = 0
		count = 0
		history = []
		for i in range(len(data)):
			last_n_seconds = []
			quick_sell_list = []
			time_pd = pd.to_datetime(data[i]['unix_time'], unit='ms')
			if (time_pd.hour >= 9 or time_pd.minute >= 45) and (time_pd.hour != 19 or time_pd.minute <=45):
				
				for j in range(1,n_seconds+1,1): #look back behind you
					last_n_seconds.append(data[i-j]['price'])
					last_n_seconds.reverse()
				x = np.linspace(1, n_seconds, n_seconds)
				regression = np.polyfit(x, last_n_seconds, 1)
				#if (abs(data[i]['price'] - data[i-1]['price']) < 0.3 ):
				
				quick_sell_list = last_n_seconds[0:quick_sell_seconds]
				x = np.linspace(1, quick_sell_seconds, len(quick_sell_list))
				regression_quick = np.polyfit(x, quick_sell_list, 1)

				if regression[0] < 0 or regression_quick[0] < 0:
					self.sell_all(ticker, data[i+lag]['price'])
				else:	
					self.buy(ticker, data[i+lag]['price'])
				#print(str(self.portfolio_value) + " " + str(data[i]['price']) + " " + self.order_flag + " "+str(pd.to_datetime(data[i]['unix_time'], unit='ms')))
				history.append(self.portfolio_value)
			else:
				self.sell_all(ticker, data[i+lag]['price'])
			count += 1
		print("n_seconds: " + str(n_seconds) + " Balance: " + str(self.portfolio_value))
		plt.figure(0)
		N = len(history)
		x = np.linspace(0.0, 1.0, N)
		plt.plot(x, history)

	# def momentum_function(self, ticker, data, n_seconds = 10, lag = 0):
	# self.stocks[ticker] = 0
	# count = 0
	# for i in range(len(data)):
	# 	last_n_seconds = []
	# 	time_pd = pd.to_datetime(data[i]['unix_time'], unit='ms')
	# 	if time_pd.hour >= 13 and time_pd.minute >= 40:
	# 		for j in range(1,n_seconds+1,1):
	# 			last_n_seconds.append(data[i-j]['price'])
	# 		x = np.linspace(1, n_seconds, n_seconds)
	# 		regression = np.polyfit(x, last_n_seconds, 1)
	# 		if regression[0] > 0:
	# 			self.buy(ticker, data[i+lag]['price'])
	# 		else:
	# 			self.sell_all(ticker, data[i+lag]['price'])
	# 		print(str(self.ledger) + " " + self.order_flag + " "+str(data[i]['unix_time']))
	# 	count += 1




