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

locations = dict() # location -> node index
species = dict() # species -> locations it's in

def parse_location(row):
    loc = row['Location']
    if loc not in locations:
        locations[loc] = len(locations)
    # if loc not in output:
    #     output[loc] = {}
    return loc

# { k1: v1, k2: v2 }
def parse_row(row):
    if all(row[sample] == 0 for sample in samples):
        return

    # find the json with data for this location
    location = parse_location(row)

    # location_idx = -1
    # for i in xrange(len(output['children'])):
    #     if output['children'][i]['name'] == location:
    #         location_data = output['children'][i]
    #         location_idx = i
    # if location_idx < 0:
    #     location_data = {
    #         'name': location,
    #         'children': [],
    #     }
    #     output['children'].append(location_data)
    #     location_idx = len(output)
    #     for sample in samples:
    #         location_data['children'].append({
    #             'name': sample.strip(),
    #             'children': [],
    #         })


    hierarchy = row['ID'].split('|')
    k, v = hierarchy[-1].split('__')
    if k == 's':
        if v not in species:
            species[v] = set()
        species[v].add(location)

    # sample_idx = 0 #samples are always in the same order
    # for sample in samples:
    #     if row[sample] > 0:
    #         sample_data = location_data['children'][sample_idx]

    #         # d is the root - will change as you go further in the pipe structure
    #         # d = output[location][sample.strip()]
    #         # h_idx = 0
    #         d = sample_data['children']
    #         for h in hierarchy:
    #             k, v = h.split('__') #k=key and v=value. the notation k,v is assigning a two element array to k and v
    #             t = taxonomy[k]
    #             child_idx = -1
    #             for j in xrange(len(d)):
    #                 if d[j]['name'] == v:
    #                     child_idx = j
    #                     child_data = d[j]

    #             if child_idx < 0:
    #                 if v.endswith('_unclassified'):
    #                     v = v[:-len('_unclassified')]
    #                 child_data = {
    #                     'name': v,
    #                     'children': [],
    #                     'size': row[sample],
    #                 }
    #                 d.append(child_data)
    #             if h == hierarchy[-1] and k == 's':
    #                 # child_data['percentage'] = row[sample]
    #                 # child_data['size'] = row[sample]
    #                 pass
    #             else:
    #                 # child_data['children'] = []
    #                 d = child_data['children']

    #             # h_idx += 1
    #     sample_idx += 1





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
    'nodes': [],
    'links': [],
}
for loc, i in locations.iteritems():
    output['nodes'].append({
        'name': loc,
        'group': 1,
    })
node_index_iter = len(locations) # node id of node currently being added
for s, locs in species.iteritems():
    output['nodes'].append({
        'name': s,
        'group': 3 if len(locs) == 1 else 2,
    })
    for loc in locs:
        target = locations[loc]
        output['links'].append({
            'source': node_index_iter,
            'target': target,
            'value': 1,
        })
    node_index_iter += 1


uniques = {} # location -> species
for s, locs in species.iteritems():
    if len(locs) == 1:
        loc = list(locs)[0]
        if loc not in uniques:
            uniques[loc] = []
        uniques[loc].append(s)
json.dump(uniques, open('meta_file_species_uniques.json', 'w'), indent=2, separators=(',', ': '))

json.dump(output, open('meta_file_species_force.json', 'w'), indent=2, separators=(',', ': '))
#w means write mode





# { location: {Honey: ??? }}
