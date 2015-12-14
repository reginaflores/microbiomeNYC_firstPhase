import json
with open('meta_file.json') as data_file:    
    data = json.load(data_file)

output = {}

for location, sample in data.iteritems():
	print location
	output[location] = {}
	#for each type=kindom
	#print kingdom name and percentage
	#for (i)	==> location
	#	for j ==> sample
	for sample_name, kingdoms in sample.iteritems():
		if kingdoms:
			print "\t", sample_name
			output[location][sample_name] = {}

			for kingdom_name, kingdom_data in kingdoms.iteritems():
				print "\t\t", kingdom_name, kingdom_data['percentage']
				output[location][sample_name][kingdom_name] = kingdom_data['percentage']


json.dump(output, open('top_line.json', 'w'), indent=2, separators=(',', ': '))
