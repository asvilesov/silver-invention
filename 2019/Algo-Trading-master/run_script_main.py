import alpaca_trade_api as tradeapi
import time
import numpy as np
import pandas as pd
from datetime import datetime

import tradealgo.algos as ta
from tradealgo import misc

#Feeding in Username, API Passcode
fh = open("../private.txt", "r")
temp = fh.read().split('\n')
api = tradeapi.REST(
    key_id= temp[0],
    secret_key= temp[1],
    base_url='https://paper-api.alpaca.markets'
)

# print(api.get_clock())
# print()
print(time.clock())
hi = pd.Timestamp(year=2017, month=1, day=1, hour=12, minute = 3).isoformat()
print(hi)
print(time.localtime())



to_email = 'avilesov@usc.edu' 
subject = 'Reboot'  
body = 'Starting Process Algo-Trade'
filename = 'work.txt'

print(api.get_account())
# while True:
# 	check_time = time.localtime()
# 	if(check_time.tm_min % 5 == 0):
# 		try:
# 			misc.emailing_package(to_email, subject, body, filename)
# 		except: 
# 			print("Email not sent.")
# 	time.sleep(50)






base = ta.trading_algo(api)

crit_price = 200 #neccessary for initating box_trading

while True:
	clock = api.get_clock()
	check_time = time.localtime()
	unix_time = time.time()
	utc_time = datetime.fromtimestamp(unix_time)

	if clock.is_open and (utc_time.hour > 6 or utc_time.minute > 45):
		print("")
		crit_price = base.box_trading('TSLA', 0.0005, 0.0005, crit_price)

	else:
		time.sleep(120)
		hours = check_time.tm_hour - 8
		print("Market Closed " + str(clock.timestamp))
		if(hours < 0):
			hours = hours +24
		if(hours == 23 and check_time.tm_min == 28): #market just closed
			to_email = 'avilesov@usc.edu'
			subject = base.algo_name
			body = "Variable Info of " + base.algo_name + " algorithm"
			filename = str(time.localtime().tm_year) + "-" + str(time.localtime().tm_mon) + "-" + str(time.localtime().tm_mday) + ".txt"
			filename = base.algo_name + "-" + str(time.localtime().tm_year) + "-" + str(time.localtime().tm_mon) + "-" + str(time.localtime().tm_mday) + ".txt"
			misc.emailing_package(to_email, subject, body, filename)

	time.sleep(.5)

	

	# avg = 0
# volume_list = []
# bars = api.get_barset('AAPL', '5Min', limit=1000)
# for i in bars["AAPL"]:
# 	volume_list.append(i.v)
# 	avg = avg + i.v
# avg = avg /1000/5
# print(avg)
# print(np.percentile(volume_list, 95)/5)
# print(api.get_barset('AAPL', '1Min', limit=5))


# api.submit_order(
#                 symbol='AAPL',
#                 qty=2156,
#                 side='buy',
#                 type='market',
#                 time_in_force='day',
#             )

# api.submit_order(
#                 symbol='TSLA',
#                 qty=903,
#                 side='buy',
#                 type='market',
#                 time_in_force='day',
#             )
# api.submit_order(
#                 symbol='FB',
#                 qty=1056,
#                 side='buy',
#                 type='market',
#                 time_in_force='day',
#             )
