import matplotlib
import tradealgo.algos_stats as algos_stats
import alpaca_trade_api as tradeapi
import time
import numpy as np
import pandas as pd
import os.path
from scipy.fftpack import fft, dct
import matplotlib.pyplot as plt
import tradealgo.trading_simulator as ts



print("Testing Polygon API")

fh = open("../private.txt", "r")
temp = fh.read().split('\n')
api = tradeapi.REST(
    key_id= temp[0],
    secret_key= temp[1],
    base_url='https://paper-api.alpaca.markets'
)

def record_stats(file_name, data): #has to be in csv data format already
		if os.path.isfile(file_name) != True:
			f = open(file_name, 'w')
			f.write(data)
			f.flush()
			f.close

def conversion_lower_resolution(start, end, ticker, resolution = 'second'):
	unix_current = int(start.value /10**6)
	unix_end = int(end.value /10**6)
	lower_res_data = []
	while(unix_current < unix_end):
		data = api.polygon.historic_trades(ticker, str(start.year) + '-' + str(start.month) + '-' + str(start.day), limit = 10, offset = unix_current)
		count = 0
		price = 0
		size = 0
		previous_price = 0 #sometimes in a certain second there is no data so revert to past price
		new_dic = {}
		for i in data:
			if(int(i.timestamp.value /10**6) < unix_current + 1000):
				size += i.size
				price += i.price
				count += 1
			else: 
				break
		if(count == 0):
			price = previous_price
		else:
			price = price / count
			previous_price = price
		new_dic['unix_current'] = unix_current
		new_dic['price'] = price
		new_dic['size'] = size
		lower_res_data.append(new_dic)
		unix_current += 1000
		print(unix_current)
	return lower_res_data

def conversion_lower_resolutionV2(start, end, ticker, textfile_copy = False, resolution = 'second'): #900 times faster #this is in new york time
	unix_current = int(start.value /10**6)
	unix_end = int(end.value /10**6)
	lower_res_data = []
	count = 0
	price = 0
	size = 0
	data = api.polygon.historic_trades(ticker, str(start.year) + '-' + str(start.month) + '-' + str(start.day), limit = 10, offset = unix_current)
	previous_price = data[0].price #sometimes in a certain second there is no data so revert to past price
	new_dic = {}

	while(unix_current < unix_end):
		try:
			data = api.polygon.historic_trades(ticker, str(start.year) + '-' + str(start.month) + '-' + str(start.day), limit = 3000, offset = unix_current)
			for i in data:
				if ((i.timestamp.hour >= 9 and i.timestamp.minute > 30) or i.timestamp.hour > 9) and (i.timestamp.hour < 16): 
					if(int(i.timestamp.value /10**6) < unix_current + 1000):
						size += i.size
						price += i.price
						count += 1
					else: 
						if(count == 0 or abs(price/count - previous_price) > 0.3):
							price = previous_price
						else:
							price = price / count
							previous_price = price
						#record data
						new_dic['unix_time'] = unix_current
						new_dic['price'] = price
						new_dic['volume'] = size
						lower_res_data.append(new_dic)
						new_dic = {}
						size = i.size
						price = i.price
						count = 1
						unix_current += 1000
				else:
					print(start)
					start = start + pd.Timedelta('1 day')
					if(start.isoweekday() == 6):
						start = start + pd.Timedelta('2 day')

					unix_current = int(start.value /10**6)
					if(unix_current < unix_end):
						data = api.polygon.historic_trades(ticker, str(start.year) + '-' + str(start.month) + '-' + str(start.day), limit = 10, offset = unix_current)
						previous_price = data[0].price
					break
			print(str(unix_current) + " " + str(unix_end))
		except:
			print("Polygon data error")
			start = start + pd.Timedelta('1 day')
			if(start.isoweekday() == 6):
				start = start + pd.Timedelta('2 day')

	if textfile_copy:
		datafram = pd.DataFrame(lower_res_data)
		record_stats(start.isoformat() + "<>" + end.isoformat() + "-" + ticker, datafram.to_csv())

	return lower_res_data #returned as a dic

def fourier_transform(data, sample_spacing = 1): #data in list form please
	N = len(data)
	total_sum = sum(data)
	average = total_sum/N
	x = np.linspace(0.0, 1.0/sample_spacing, N)
	plt.figure(0)
	plt.plot(x, data)

	normalized_data = [i-average for i in data]

	N = len(normalized_data)
	y = np.array(normalized_data)
	yf = fft(y)
	xf = np.linspace(0.0, 1.0/(2.0*sample_spacing), N//2)

	log_data = np.log(2.0/N * np.abs(yf[0:N//2]))
	xf = np.delete(xf, 0)
	log_data = np.delete(log_data, 0)
	
	plt.figure(1)
	plt.plot(xf, log_data)
	plt.grid()
	plt.show()


start = pd.Timestamp(year=2019, month=7, day=15, hour=13, minute = 40)
end = pd.Timestamp(year=2019, month=7, day=15, hour=20, minute = 0)


# datalol = api.polygon.historic_trades('TSLA', str(start.year) + '-' + str(start.month) + '-' + str(start.day), limit = 10, offset = 0)
# print(datalol)
# print(len(datalol))
# suming = 0
# for i in datalol:
# 	suming+= i.price
# suming = suming/len(datalol)

#print([i('price') for i in datalol])

#print(start.isoformat())

data_low = conversion_lower_resolutionV2(start, end, 'AAPL', textfile_copy = True)

low_df = pd.DataFrame(data_low)
#print(lol)
#print(lol['price'].tolist())
#record_stats(start + "<>" + end + "-" + ticker, datafram.to_csv())
#fourier_transform(lol['price'].tolist())

sim = ts.trading_simulator()
sim.box_trading('AAPL', data_low, 0.001, 0.001, lag = 0)
# for i in range(1, 100, 1):
# 	sim.box_trading('AAPL', data_low, 0.001, i/10000, lag = 0)
# 	sim.portfolio_value = 30000
# 	sim.buying_power = 30000
# sim.recursive_momentum_function_record('AAPL', 'test_balanced', data_low, n_seconds = 10)
# sim.recursive_momentum_function_trained('AAPL', data_low, 'ML_data_test_balanced_recursive_momentum_function_record_AAPL.txt', n_seconds = 10, epochs = 10)

# print(sim.portfolio_value)
data = low_df['price'].tolist()
N = len(data)
total_sum = sum(data)
average = total_sum/N
x = np.linspace(0.0, 1.0, N)
plt.figure(1)
plt.plot(x, data)
plt.show()
#print(len(dicty))