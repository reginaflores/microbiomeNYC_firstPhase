//
//  Sphere.hpp
//  flare_map_2
//
//  Created by Regina Flores on 11/25/15.
//
//

#ifndef Sphere_hpp
#define Sphere_hpp

#include "ofMain.h"

class Sphere{
    
public:

    
    void setup();
    void draw();

    
    
    int num;
    float dist;
    float noisey;
    
    vector<ofPoint> points;
    
    vector<float> theta;
    vector<float> phi;
    vector<int> radius;
    
    vector<float> changeTheta;
    vector<float> changePhi;

    ofPoint center;
    
};



#endif /* Sphere_hpp */
