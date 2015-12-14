#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxUI.h"
#include "Sphere.hpp"
#include "Species.hpp"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    int idxOfHover();
    void calculateEdges(bool first=false);
    void calculatePositions();
    void drawNodes(int i);
    void drawSpiral();
    void drawFlareMap();
    void setupGui();
    void setupPositionsEdges();
    void setupMesh();
    void setupBacteria();
    void drawMesh1();
    void drawMesh2();
    void addPoint(float x, float y, float z);
    void setupMesh2();

    void drawRandom();
    
    //Reading in a JSON file
    ofxJSONElement result;
    
    
    vector<ofPoint> pos;
    vector<ofPoint> vel;
    map<string, ofColor> colormap;
    map<string, ofColor> familyColors;
    map<string, set<string> > familyLocations;
    map<string, ofPoint> centers;
    map< string, vector<int> > nodeMap;
    vector<double> scaledRadius;
    string currentLocation;
    
    
    vector< pair<ofPoint, ofPoint> > edges;
    ofTrueTypeFont ubuntuSmall;
    
    ofxUICanvas *gui;
    ofxUIDropDownList *ddl;
    void guiEvent(ofxUIEventArgs &e);
    
    
    bool haveIGrabbedAThing = false;
    int thingIGrabbed = -1;
    bool moved = false;
    ofPoint previousGrabbedPosition;
    
    float bx1, bx2, by1, by2;
    
    ofShader    shaderMesh1;
    ofTexture   textureMesh1;
    ofVboMesh   mesh1;
    ofEasyCam cam;
    bool showMesh1 = false;
    bool showMesh2 = false;

    bool showWire = false;
    bool showFlare = false;
    bool showRandom = false;
    bool showSpiral = false;
    float scale1, scale2, scale3;
    float red, green, blue, alpha;
    vector<string> names;

    vector<Sphere> bacteria;
    
    ////////////////////////
    //mesh2
    ofxJSONElement result2;
    
    vector<ofPoint> pos2;
    vector<float> sphereRadius;
    
    
    ofShader    shader;
    ofTexture   texture;
    ofVboMesh   mesh;
    
    ofVbo vbo;
    ofShader shaderGlow;
    
    // vector to store all values
    vector <ofVec3f> points;
    vector <ofVec3f> sizes;
    
    float camDist;
    
    ofTexture textureGlow;

    
};
