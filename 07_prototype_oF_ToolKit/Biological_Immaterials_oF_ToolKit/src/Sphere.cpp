//
//  Sphere.cpp
//  flare_map_2
//
//  Created by Regina Flores on 11/25/15.
//
//

#include "Sphere.hpp"



void Sphere::setup(){
    
    num = 100;
    
    
    for(int i =0; i < num; i++){
        ofPoint p;
        points.push_back(p);
        
        float t;
        theta.push_back(t);
        
        float ph;
        phi.push_back(ph);
        
        int r;
        radius.push_back(r);
        
        float chPh;
        changePhi.push_back(chPh);
        
        float cT;
        changeTheta.push_back(cT);
        
    }
    
    //put the sphere in the setup function so all the points would not be moving
    for(int i = 0; i < num; i++){
        
        theta[i] = ofRandom(0,2*PI);
        phi[i] = ofRandom(0,PI);
        radius[i] = ofRandom(2,10);
        
//        points[i] = ofPoint(radius[i]*cos(theta[i])*sin(phi[i]), radius[i]*sin(theta[i])*sin(phi[i]), radius[i]*cos(phi[i]));
        float x = center.x + radius[i]*cos(theta[i])*sin(phi[i]);
        float y = center.y + radius[i]*sin(theta[i])*sin(phi[i]);
        float z = radius[i]*cos(phi[i]);
        points[i] = ofPoint(x,y,z);
        
        changeTheta[i] = ofRandom(0,0.01);
        changePhi[i] = ofRandom(0,0.01);
        
    }

}

void Sphere::draw() {
    
    for(int i = 0; i < num; i++){
        float x = center.x + radius[i]*cos(theta[i])*sin(phi[i]);
        float y = center.y + radius[i]*sin(theta[i])*sin(phi[i]);
        float z = radius[i]*cos(phi[i]);
//        cout << "(x,y,z): (" << x << "," << y << "," << z << ")" << endl;
        points[i] = ofPoint(x, y, z);
        
        noisey = sin(0.01*ofGetFrameNum());
        //noisey = ofNoise(ofGetFrameNum());
        radius[i] = ofNoise(theta[i]*noisey, phi[i]*noisey)*10;
        
        
        theta[i] += changeTheta[i];
        phi[i] += changePhi[i];
        
    }

    for(int i = 0; i< num; i++){
        for(int j = i+1; j < num; j++){
//            dist = pow(points[i].x - points[j].x, 2) + pow(points[i].y - points[j].y, 2) + pow(points[i].z - points[j].z, 2);
            // dist = sqrt(dist);
            
//            if (dist < 2*ofGetMouseX()) {
            if (ofRandom(100) < 1) {
                ofSetColor(255, 50);
                ofSetLineWidth(1.5);
                ofLine(points[i].x, points[i].y, points[i].z, points[j].x, points[j].y, points[j].z);
            }
            
        }
    }

}



