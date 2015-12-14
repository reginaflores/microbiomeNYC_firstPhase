//
//  Species.hpp
//  flare_map_2
//
//  Created by Regina Flores on 11/27/15.
//
//

#ifndef Species_hpp
#define Species_hpp

#include <stdio.h>
#include "ofMain.h"


class Species {
public:
    string name;
    float radius;
    ofColor color;
    
    Species(string n, float r, ofColor c) : name(n), radius(r), color(c) {};
    
    bool operator<(const Species& other) const {
        return (radius < other.radius);
    }

};

//bool compareSpecies2(const Species &s1, const Species & s2) {
//    return s1.radius < s2.radius;
//};

#endif /* Species_hpp */

