#This parser parses across row to show the abundance of a pathway across the samples
#and location

# Filter things that are outside the 20th percentile
# Filter things less than 2 SD from the average of the row

#Filtering out:
# 1) small numbers (outliers on the left side of distribution) AND 
# 2) numbers that are far from the variance
import numpy as np
import pandas as pd
import json

csv = pd.read_csv('preliminary_sample_functional_analysis_test.csv')
columns = csv.columns

output = []

def parse_row(row):

	a = np.array([float(x.replace(',','')) for x in row[columns[1:]]])
	# new_array = []
	# for x in row[columns[1:]]: # loop through every cell except the first column
	# 	f = float(x.replace(',', '')) # cast number to float after removing comma
	# 	new_array.append(f)
	# a = np.array(new_array) # convert it to a numpy array, which has the .std() function

	# take the standard deviation here if you want it to be calculated across
	# the entire row
	std = a.std()
	mu = a.mean()

	#REMOVING LOW ABUNDANCE
	# remove the bottom 20% first:
	tup = zip(columns[1:,], a)

	# equivalent:
	# tup = []
	# for i in xrange(len(a)):
	# 	new_tuple = (columns[i+1], a[i])
	# 	tup.append(new_tuple)

	tup = sorted(tup, key=lambda x: x[1]) #to remove bottom 20% we sort ascending
	tup = tup[int(0.2*len(tup)):] #a1 = ['a', 'b', 'c'] then a1[1:] = ['b', 'c']
	tup = [x for x in tup if x[1] > 0] # remove any remaining 0s

	# equivalent:
	# tup2 = []
	# for x in in tup:
	# 	if x[1] > 0:
	# 		tup2.append(x)
	# tup = tup2

	#REMOVE ITEMS LOWER THAN 2 SD FROM MEAN
	threshold = mu - 2*std  #mu = mean
	tup = [x for x in tup if x[1] > threshold]

	# tup = [x for x in tup if x[1] > max(0, threshold)] # all in one step
	d = {}
	for k,v in tup:
		d[k.strip()] = round(v * 100) / 100 
	output.append({
		'path': row['Pathway'].decode('utf-8', 'ignore').encode('utf-8'),
		'abundance': d,
	})


csv.apply(parse_row, axis=1) 

json.dump(output, open('functional.json', 'w'), sort_keys=True, indent=2, separators=(',', ': '))

