import pandas as pd
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

output = []

def parse_row(row):
    hierarchy = row['ID'].split('|')

    if len(hierarchy) == 7:
        location = row['Location']
        # print "location_data:", location_data
        # if not output[location]:
            # only create empty samples if there is nothing in
            # output[location] already

        for sample in samples:
            if row[sample] > 0:
                # find the sample where this data should go
                sample_name = sample.strip()

                species = hierarchy[-1].split('__')[1]
                
                data = {
                    'location': location,
                    'species': species,
                    'sample': sample_name,
                    'percentage': row[sample]
                }
                output.append(data)




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

json.dump(output, open('meta_file_species.json', 'w'), sort_keys=True, indent=2, separators=(',', ': '))
#w means write mode

# { location: {Honey: ??? }}
