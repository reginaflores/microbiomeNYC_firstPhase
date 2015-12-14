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

output = []
# [
#     {
#         location: ASTORIA,
#         sample: ...
#     }
# ]

def parse_location(row):
    loc = row['Location']

    for location_data in output:
        if loc == location_data['location']:
            return location_data

    # not found, create one
    location_data = {
        'location': loc,
        'sample': [],
    }
    output.append(location_data)
    return location_data


# { k1: v1, k2: v2 }
def parse_row(row):
    location_data = parse_location(row)
    # print "location_data:", location_data
    # if not output[location]:
        # only create empty samples if there is nothing in
        # output[location] already

    if not location_data['sample']: # it's empty
        for sample in samples:
            sample = sample.strip()

            # only insert new sample data if it doesn't exist
            sample_data = {
                'sample_name': sample,
                'data': [],
            }
            location_data['sample'].append(sample_data)

    hierarchy = row['ID'].split('|')
    for sample in samples:
        if row[sample] > 0 and len(hierarchy) == 1:
            # find the sample where this data should go
            sample_name = sample.strip()

            kingdom = hierarchy[0].split('__')[1]
            for sample_data in location_data['sample']:
                if sample_name == sample_data['sample_name']:
                    # this is the array we should append to
                    kingdom_data = {
                        'kingdom': kingdom,
                        'percentage': row[sample],
                    }
                    sample_data['data'].append(kingdom_data)

    #         # d is the root - will change as you go further in the pipe structure
    #         d = output[location][sample.strip()]
    #         for h in hierarchy:
    #             k, v = h.split('__') #k=key and v=value. the notation k,v is assigning a two element array to k and v
    #             t = taxonomy[k]
    #             if v not in d:
    #                 d[v] = {
    #                     'type': t,
    #                     'children': {},
    #                 }
    #             if h == hierarchy[-1]:
    #                 d[v]['percentage'] = row[sample]
    #             d = d[v]['children']





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

json.dump(output, open('meta_file_with_IDs.json', 'w'), sort_keys=True, indent=2, separators=(',', ': '))
#w means write mode

# { location: {Honey: ??? }}
