import pandas as pd
import json

csv = pd.read_csv('ALL_HIVE_DATA_preliminary_hive_analysis.csv')
samples = (u'Honey', u'Bee', u'Bee Debris Solid', u'Bee Debris Liquid',
       u'Propolis in Solvant', u'Iaise Hive 1', u'Ralph\'s Hand', u'Beeswax ',
       u'Outside Hive', u'Inside Hive', u'Scrapper', u'Propolis no solvent ')

taxonomy = {
    'k': 'kingdom',
    'p': 'phylum',
    'c': 'class',
    'o': 'order',
    'f': 'family',
    'g': 'genus',
    's': 'species',
    't': 'subspecies',
}

output = [] # dict()

def parse_location(row):
    loc = row['Location']
    # if loc not in output:
    #     output[loc] = {}
    return loc

# { k1: v1, k2: v2 }
def parse_row(row):
    # find the json with data for this location
    location = parse_location(row)
    location_data = {'samples':[],'location':location}
    location_idx = -1
    for i in xrange(len(output)):
        if output[i]['location'] == location:
            location_data = output[i]
            location_idx = i
    if location_idx < 0:
        output.append(location_data)
        location_idx = len(output)
        for sample in samples:
            location_data['samples'].append({
                'sample': sample.strip(),
                'children': [],
            })
    # print "location data:", location_data
    # if not output[location]:
    #     for sample in samples:
    #         sample = sample.strip()
    #         if sample not in output[location]:
    #             output[location][sample] = {}

    hierarchy = row['ID'].split('|')
    sample_idx = 0 #samples are always in the same order
    for sample in samples:
        if row[sample] > 0:
            sample_data = location_data['samples'][sample_idx]

            # d is the root - will change as you go further in the pipe structure
            # d = output[location][sample.strip()]
            # h_idx = 0
            d = sample_data['children']
            for h in hierarchy:
                k, v = h.split('__') #k=key and v=value. the notation k,v is assigning a two element array to k and v
                t = taxonomy[k]
                child_idx = -1
                for j in xrange(len(d)):
                    if d[j]['name'] == v:
                        child_idx = j
                        child_data = d[j]

                if child_idx < 0:
                    child_data = {
                        'name': v,
                        'type': t,
                        'children': [],
                    }
                    d.append(child_data)
                if h == hierarchy[-1]:
                    child_data['percentage'] = row[sample]
                d = child_data['children']

                # h_idx += 1
        sample_idx += 1





# for (i = 0, i < length,, i++)
#    A[i]
# pythonic 

csv.apply(parse_row, axis=1) 
#######################
#apply function takes the parse_location function and apply to every row
#axis = 1 means go along rows
# for row in csv.iterrows():
#   loc = row['Location']
#   ...
#######################

json.dump(output, open('meta_file_nameKey.json', 'w'), indent=2, separators=(',', ': '))
#w means write mode

# { location: {Honey: ??? }}
