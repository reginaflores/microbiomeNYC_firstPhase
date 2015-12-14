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

output = {} # dict()

def parse_location(row):
    loc = row['Location']
    if loc not in output:
        output[loc] = {}
    return loc

# { k1: v1, k2: v2 }
def parse_row(row):
    location = parse_location(row)
    if not output[location]:
        for sample in samples:
            sample = sample.strip()
            if sample not in output[location]:
                output[location][sample] = {}

    hierarchy = row['ID'].split('|')
    for sample in samples:
        if row[sample] > 0:
            # d is the root - will change as you go further in the pipe structure
            d = output[location][sample.strip()]
            for h in hierarchy:
                k, v = h.split('__') #k=key and v=value. the notation k,v is assigning a two element array to k and v
                t = taxonomy[k]
                if v not in d:
                    d[v] = {
                        'type': t,
                        'children': {},
                    }
                if h == hierarchy[-1]:
                    d[v]['percentage'] = row[sample]
                d = d[v]['children']





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

json.dump(output, open('meta_file.json', 'w'), indent=2, separators=(',', ': '))
#w means write mode

# { location: {Honey: ??? }}
