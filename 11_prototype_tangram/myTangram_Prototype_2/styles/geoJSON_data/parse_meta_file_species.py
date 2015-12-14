import pandas as pd
import numpy as np
import json

csv = pd.read_csv('ALL_HIVE_DATA_preliminary_hive_analysis_colsFixed.csv')
samples = (u'Honey', u'Bee', u'Bee Debris Solid', u'Bee Debris Liquid',
       u'Propolis in Solvant', u'Ralph\'s Hand', u'Beeswax ',
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



max_lat = 40.7725
min_lat = 40.7424
max_lng = -73.8567
min_lng = -73.9469
dlat = max_lat - min_lat
dlng = max_lng - min_lng


def generate_coordinates(amount):
    lat = np.random.uniform(min_lat, max_lat)
    lng = np.random.uniform(min_lng, max_lng)

    top = [ lat + dlat/100, lng ]
    left = [lat, lng - dlat/200 ]
    right = [lat, lng + dlat/200 ]

    return [top, left, right, top]


features = []

def parse_row(row):
    hierarchy = row['ID'].split('|')

    if len(hierarchy) == 7:
        location = row['Location']
        if location != "ASTORIA HIVE":
            return

        # print "location_data:", location_data
        # if not output[location]:
            # only create empty samples if there is nothing in
            # output[location] already

        amount = np.mean([row[sample] for sample in samples])


        species = hierarchy[-1].split('__')[1]
        
        geometry = {
            'type': 'Polygon',
            'coordinates': generate_coordinates(amount)
        }
        data = {
            'location': location,
            'species': species,
            'percentage': amount,
            'kind': 'bacteria',
        }
        feature = {
            'geometry': geometry,
            'type': "Feature",
            'properties': data,
        }

        features.append(feature)


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
output = {
    'type': "FeatureCollection",
    'features': features,
}
json.dump(output, open('meta_file_species.json', 'w'), sort_keys=True, indent=2, separators=(',', ': '))
#w means write mode

# { location: {Honey: ??? }}
