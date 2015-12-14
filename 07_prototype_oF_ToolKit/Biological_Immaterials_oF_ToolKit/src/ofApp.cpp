#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ubuntuSmall.loadFont("Ubuntu-LI.ttf", 10);
    red = 233; blue = 233; green = 233, alpha = 60;
    scale1 = 5;
    scale2 = 1;
    scale3 = 1;
    
    //open file - check to make sure it is readable
    std::string file = "meta_file_species_family.json";
    bool success = result.open(file);
    if (!success) {
        exit();
    }
    setupGui();
    setupPositionsEdges();
    setupMesh();
    setupMesh2();

    setupBacteria();
    
    
}

//--------------------------------------------------------------
void ofApp::setupBacteria() {
    // create spheres all at 0,0
    for (const ofPoint& p : pos) {
        Sphere sphere;
        bacteria.push_back(sphere);
    }
    
    // get centers and shift each bacteria to its center
    // scale it to the window
    float scale = 5.0;
    for (auto node : nodeMap) {
        string location = node.first;
        ofPoint centerOfMass = centers[location];

        for (int i : node.second) {
            ofPoint newPos = scale*(pos[i] - centerOfMass); //+ screenCenter;
            newPos.x *= 1.5;
            bacteria[i].center = newPos;
            bacteria[i].setup();
        }
    }
}

//--------------------------------------------------------------
void ofApp::setupMesh(){
    //Making Mesh and Shader
    ofEnableDepthTest();
    ofDisableArbTex();
    ofLoadImage(textureMesh1, "nyc.jpg");
    shaderMesh1.load("shader.vert", "shader.frag");
    
    int scale = 5;
    int w = 1000/scale;
    int h = 500/scale;
    for (int y = 0; y < h; y++){
        for (int x = 0; x<w; x++){
            
            //  Per pixel we set the position, normal and texCoord
            float offsetX = 0;
            float offsetY = (x%2==1)?0.5:0.0;
//            float offsetY;
//            if (x % 2 == 1) {
//                offsetY = 0.5;
//            } else {
//                offsetY = 0.0;
//            }
            mesh1.addVertex(ofPoint((x+offsetX)*scale,(y+offsetY)*scale,0));
            mesh1.addNormal(ofPoint(1,0,0));
            mesh1.addTexCoord(ofVec2f((float)(x+offsetX)/(float)w,(float)(y+offsetY)/(float)h));
        }
    }
    
    //  Finally we set the indexes... We tell openGL how the vertex are connected in triangles (a,b,c)
    for (int y = 0; y<h-1; y++){
        for (int x=0; x<w-1; x++){
            if(x%2==0){
                mesh1.addIndex(x+y*w);				// a
                mesh1.addIndex((x+1)+y*w);			// b
                mesh1.addIndex(x+(y+1)*w);			// d
                
                mesh1.addIndex((x+1)+y*w);			// b
                mesh1.addIndex((x+1)+(y+1)*w);		// c
                mesh1.addIndex(x+(y+1)*w);			// d
            } else {
                mesh1.addIndex((x+1)+y*w);			// b
                mesh1.addIndex(x+y*w);				// a
                mesh1.addIndex((x+1)+(y+1)*w);		// c
                
                mesh1.addIndex(x+y*w);				// a
                mesh1.addIndex(x+(y+1)*w);			// d
                mesh1.addIndex((x+1)+(y+1)*w);		// c
            }
        }
    }
}
//--------------------------------------------------------------
void ofApp:: setupGui(){

    gui = new ofxUICanvas();
    gui->addLabel("Biological Immaterials", OFX_UI_FONT_MEDIUM);
    gui->addLabel("Taxonomical Data");
    gui->addLabel("Press f to Fullscreen", OFX_UI_FONT_SMALL);
    gui->addLabel("Press x to Hide GUI", OFX_UI_FONT_SMALL);

    gui->addSpacer();
    
    gui->addLabel("Interactive");
    gui->addToggle("Show Mesh1", showMesh1);
    gui->addToggle("Show Mesh2", showMesh2);

    gui->addToggle("Wire Frame", showWire);
    gui->addSpacer();
    
    gui->addLabel("Correlation");
    gui->addToggle("Show Flare Map", showFlare);
    gui->addLabel("Connecting Color", OFX_UI_FONT_SMALL);
    gui->addSlider("RED", 0.0, 255.0, &red)->setTriggerType(OFX_UI_TRIGGER_ALL);
    gui->addSlider("GREEN", 0.0, 255.0, &green)->setTriggerType(OFX_UI_TRIGGER_BEGIN|OFX_UI_TRIGGER_CHANGE|OFX_UI_TRIGGER_END);
    gui->addSlider("BLUE", 0.0, 255.0, &blue)->setTriggerType(OFX_UI_TRIGGER_BEGIN|OFX_UI_TRIGGER_CHANGE);
    gui->addSlider("ALPHA", 0.0, 255.0, &alpha)->setTriggerType(OFX_UI_TRIGGER_BEGIN|OFX_UI_TRIGGER_CHANGE);


    gui->addSpacer();
    gui->addLabel("Adundance");
    gui->setWidgetFontSize(OFX_UI_FONT_SMALL);
    gui->addToggle("Show Random", showRandom);
    gui->addToggle("Show Spiral", showSpiral);
    gui->addLabel("Scale Spiral Circle Sizes", OFX_UI_FONT_SMALL);
    gui->addSlider("Scale 1", 1, 5, &scale1);
    gui->addSlider("Scale 2", 1, 20, &scale2);
    gui->addSlider("Scale 3", 1, 20, &scale3);
    
    names.push_back("ASTORIA HIVE");
    names.push_back("CROWN HEIGHTS - LANGSTROTH");
    names.push_back("CROWN HEIGHTS -TOP BAR");
    names.push_back("FORT GREENE");
    ddl = gui->addDropDownList("Select Brooklyn Hive Location", names);
    ddl->setAllowMultiple(false);
    currentLocation = "";
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent, this, &ofApp::guiEvent);
    gui->addSpacer();

}

//--------------------------------------------------------------

void ofApp::setupPositionsEdges(){
    
    pos.clear();
    nodeMap.clear();
    scaledRadius.clear();
    
    //this is a randon position for the cluster clouds
    double x1 = ofRandom(50, ofGetWidth()-50);
    double x2 = ofRandom(50, ofGetWidth()-50);
    double x3 = ofRandom(50, ofGetWidth()-50);
    double x4 = ofRandom(50, ofGetWidth()-50);
    double y1 = ofRandom(50, ofGetHeight()-50);
    double y2 = ofRandom(50, ofGetHeight()-50);
    double y3 = ofRandom(50, ofGetHeight()-50);
    double y4 = ofRandom(50, ofGetHeight()-50);
    
    centers["FORT GREENE"] = ofPoint(x1, y1);
    centers["CROWN HEIGHTS - LANGSTROTH"] = ofPoint(x2, y2);
    centers["CROWN HEIGHTS -TOP BAR"] = ofPoint(x3, y3);
    centers["ASTORIA HIVE"] = ofPoint(x4, y4);
    
    for(int i = 0; i< result.size(); i++){
        auto item = result[i];
        
        int dx = ofRandom(ofGetWidth() / 8) - ofGetWidth() / 16;
        int dy = ofRandom(ofGetHeight() / 8) - ofGetHeight() / 16;
        pos.push_back(ofPoint(dx, dy) + centers[item["location"].asString()]);
        
        //scale the radius so that they are not so big
        double r = log(item["percentage"].asDouble() + 1)*2;
        scaledRadius.push_back(r);
        
        vel.push_back((ofPoint(0, 0)));
        
        
        //Making a map of locations to nodes in that location
        string loc = item["location"].asString();
        if (nodeMap.find(loc) == nodeMap.end()) {
            vector<int> v;
            nodeMap[loc] = v;
        }
        nodeMap[loc].push_back(i);
        
        string family = item["family"].asString();
        if (familyColors.find(family) == familyColors.end()) {
            familyColors[family] = ofColor(ofRandom(255),ofRandom(255),ofRandom(255));
        }
        
        string location = item["location"].asString();
        if (familyLocations.find(location) == familyLocations.end()) {
            set<string> myset;
            familyLocations[location] = myset;
        }
        familyLocations[location].insert(family);
    }
    ///////////////////////////////////////
    
    colormap["FORT GREENE"] = ofColor(255);
    colormap["CROWN HEIGHTS - LANGSTROTH"] = ofColor(255);
    colormap["CROWN HEIGHTS -TOP BAR"] = ofColor(255);
    colormap["ASTORIA HIVE"] = ofColor(255);
    
    
    calculateEdges(true);
    //Bezier Edges
    bx1 = ofRandom(ofGetWidth());
    bx2 = ofRandom(ofGetWidth());
    by1 = ofRandom(ofGetHeight());
    by2 = ofRandom(ofGetHeight());
    
}

//--------------------------------------------------------------

void ofApp::calculateEdges(bool first) {
    // (p1, p2) --> yes there is an edge
    if (first || moved) {
        edges.clear();
        for (int i = 0; i < result.size(); ++i) {
            for (int j = 0; j < i; ++j) {
                if (i != j) {
                    auto item1 = result[i];
                    auto item2 = result[j];
                    if (item1["location"] != item2["location"]) {
                        if (item1["species"] == item2["species"]) {
                            edges.push_back( pair<ofPoint, ofPoint>(pos[i], pos[j]) );
                        }
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::calculatePositions(){
    
    //Note: Using Flocking Algorithm
    
    //PART 1 - Calculate Center of Mass
    string location = result[thingIGrabbed]["location"].asString();
    ofPoint center;
    int numberOfPoints = 0;
    for (int i : nodeMap[location]) {
        center += pos[i];
        numberOfPoints++;
    }
    center = center / numberOfPoints;
    ofPoint cohesion_velocity = (center - pos[thingIGrabbed])/1.0f;
    
    
    //Part 2 - Calculate Distance between each node and itself
    ofPoint displacement;
    for (int i : nodeMap[location]) {
        if (i != thingIGrabbed) {
            float dist = ofDist(pos[i].x, pos[i].y, mouseX, mouseY);
            
            if(dist < 10.0){
                displacement = displacement - (pos[i] - ofPoint(mouseX, mouseY));
            }
        }
    }
    
    ofPoint separation_velocity;
    separation_velocity = displacement;
    
    
    //Part 3 - Add Velocity
    ofPoint alignment_velocity;
    if (previousGrabbedPosition.x != 0 || previousGrabbedPosition.y != 0) {
        // v = (x2 - x1)/t ==> but we assign time = 1
        vel[thingIGrabbed] = ofPoint(mouseX, mouseY) - previousGrabbedPosition;
        
        ofPoint avgVel;
        for (int i : nodeMap[location]) {
            avgVel += vel[i];
        }
        alignment_velocity = avgVel / (numberOfPoints + 1) / 1.0f;
    }
    previousGrabbedPosition = ofPoint(mouseX, mouseY);
    
    ofPoint swarmV = -cohesion_velocity + separation_velocity + alignment_velocity;
    
    
    for (int i : nodeMap[location]) {
        pos[i] += swarmV;
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------

int ofApp::idxOfHover(){
    
    
    for (int i = 0; i < pos.size(); ++i) {
        if (ofDist(ofGetMouseX(), ofGetMouseY(), pos[i].x, pos[i].y) < scaledRadius[i]) {
            return i;
        }
    }
    return -1;
    
    
}
//--------------------------------------------------------------

void ofApp::drawNodes(int i) {
    auto item = result[i];
    double radius = item["percentage"].asDouble();
    ofColor color = colormap[item["location"].asString()];
    ofSetColor(color);
    if (haveIGrabbedAThing && thingIGrabbed == i) {
        ofCircle(mouseX, mouseY, scaledRadius[i]);
        pos[i] = ofPoint(mouseX, mouseY);
        moved = true;
        calculatePositions();
    } else {
        ofCircle(pos[i], scaledRadius[i]);
    }
}
//-------------------------------------------------------------- 

void ofApp::drawRandom(){
   
        vector<int> nodes = nodeMap[currentLocation];
        ofPoint centerOfMass = centers[currentLocation];
        
        ofPoint screenCenter = ofPoint(ofGetWidth()/2, ofGetHeight()/2);
        float scale = 5.0;
        
        int hoverIdx = -1;
        ofPoint hoverPos;
        for (int i : nodes) {
            ofPoint newPos = scale*(pos[i] - centerOfMass) + screenCenter;
            ofSetColor(255);
            ofCircle(newPos, scaledRadius[i]);
            
            if (ofDist(newPos.x, newPos.y, mouseX, mouseY) < max(1.0,scaledRadius[i])) {
                hoverIdx = i;
                hoverPos = newPos;
            }
        }
        if (hoverIdx >= 0) {
            string s = result[hoverIdx]["species"].asString();
            ofSetColor(255);
            ubuntuSmall.drawString(s, hoverPos.x, hoverPos.y);
        }

}

//--------------------------------------------------------------
void ofApp::drawSpiral(){

        ofPushStyle();
        vector<Species> species;
        vector<int> nodes = nodeMap[currentLocation];
        for (int i : nodes) {
            Species s(
                result[i]["species"].asString(),
                scale1*log(scaledRadius[i]*scale2 + scale3),
                familyColors[result[i]["family"].asString()]
            );
            species.push_back(s);
        }
        sort(species.begin(), species.end());
    
        string hoverName = "";
        ofPoint hoverPos;
        ofPoint c(ofGetWidth()/2, ofGetHeight()/2);
        int t = 0;
        for (const Species & s : species) {
            ofPoint p(3*t*cos(3*sqrt(t)) + c.x, 3*t*sin(3*sqrt(t)) + c.y);
            ofSetColor(s.color);
            ofNoFill();
            ofCircle(p, s.radius);
            t++;
            
            if (ofDist(p.x, p.y, mouseX, mouseY) < max(1.0f,s.radius)) {
                hoverName = s.name;
                hoverPos = p;
            }
        }
        if (hoverName.length() > 0) {
            ofSetColor(255);
            ubuntuSmall.drawString(hoverName, hoverPos.x+8, hoverPos.y);
        }
        
        //Legend:
        ofSetColor(255);
        ofNoFill();
        ofSetLineWidth(1.5);
        ofRect(ofGetWidth()-200, ofGetHeight()-500, 180, 480);
        
        int heightOffset = 490;
        int lineHeight = 15;
        ubuntuSmall.drawString("Family", ofGetWidth()-170, ofGetHeight() - heightOffset + 3);
        heightOffset -= lineHeight;
        for (const string & family : familyLocations[currentLocation]) {
            ofSetColor(familyColors[family]);
            ofCircle(ofPoint(ofGetWidth()-180, ofGetHeight() - heightOffset), 5);
            ubuntuSmall.drawString(family.substr(0, family.find('_')), ofGetWidth()-170, ofGetHeight() - heightOffset + 3);
            heightOffset -= lineHeight;
        }
        ofPopStyle();

}
//--------------------------------------------------------------

void ofApp::drawFlareMap(){

    ofSetColor(255);
    for (int i = 0; i < result.size(); i++) {
        drawNodes(i);
    }
    
    ofSetColor(red, green, blue, alpha);
    ofSetLineWidth(0.5);
    for (auto edge : edges) {
        //curved lines
        ofPolyline b;
        b.addVertex(edge.first);
        b.bezierTo(ofPoint(bx1,by1), ofPoint(bx2,by2), edge.second);
        b.draw();
    }

    //Draw label when hover
    int hoverIndex = idxOfHover();
    if(hoverIndex >= 0){
        auto item = result[hoverIndex];
        const ofPoint& p = pos[hoverIndex];
        string s = item["species"].asString() + " " + item["location"].asString();
        ofSetColor(255);
        ubuntuSmall.drawString(s, p.x, p.y);
    }
        
    calculateEdges();
}
//--------------------------------------------------------------
void ofApp::drawMesh1(){

    cam.begin();
    ofPushMatrix();
    
    //  Center the mesh;
    ofTranslate(-500, -250);
    
    shaderMesh1.begin();
    shaderMesh1.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
    shaderMesh1.setUniform1f("u_time", ofGetElapsedTimef());
    shaderMesh1.setUniformTexture("u_tex0", textureMesh1, 0);
    shaderMesh1.setUniform2f("u_tex0Resolution", textureMesh1.getWidth(), texture.getHeight());
    
    if(showWire){
        mesh1.drawWireframe();
    }else if(!showWire){
        mesh1.draw();
    }
    
    ofPopMatrix();
    shaderMesh1.end();
    
    for(Sphere & b : bacteria){
        b.draw();
    }
    
    cam.end();

}
//--------------------------------------------------------------

void ofApp::setupMesh2(){
    
    
    std::string file = "functional_byCol.json";
    bool success = result2.open(file);
    
    auto item = result2[0];
    auto pathways = item["abundance"];
    
    
    float r, sz;
    for(int i =0; i< pathways.size(); i++){
        sz = pathways[i]["amount"].asFloat();
        r = log(sz + 1)/2;
        sphereRadius.push_back(r);
        
    }
    
    ofBackground(0,0,0);
    ofEnableDepthTest();
    
    ofDisableArbTex();
    ofLoadImage(texture, "terrain-texture.jpg");
    shader.load("shader.vert", "shader.frag");
  
    int scale = 5;
    int w = 1000/scale;
    int h = 500/scale;
    for (int y = 0; y < h; y++){
        for (int x = 0; x<w; x++){
            
            //  Per pixel we set the position, normal and texCoord
            float offsetX = 0;
            float offsetY = (x%2==1)?0.5:0.0;
            mesh.addVertex(ofPoint((x+offsetX)*scale,(y+offsetY)*scale,0));
            mesh.addNormal(ofPoint(1,0,0));
            mesh.addTexCoord(ofVec2f((float)(x+offsetX)/(float)w,(float)(y+offsetY)/(float)h));
        }
    }
    
    //  Finally we set the indexes... We tell openGL how the vertex are connected in triangles (a,b,c)
    //
    for (int y = 0; y<h-1; y++){
        for (int x=0; x<w-1; x++){
            if(x%2==0){
                mesh.addIndex(x+y*w);				// a
                mesh.addIndex((x+1)+y*w);			// b
                mesh.addIndex(x+(y+1)*w);			// d
                
                mesh.addIndex((x+1)+y*w);			// b
                mesh.addIndex((x+1)+(y+1)*w);		// c
                mesh.addIndex(x+(y+1)*w);			// d
            } else {
                mesh.addIndex((x+1)+y*w);			// b
                mesh.addIndex(x+y*w);				// a
                mesh.addIndex((x+1)+(y+1)*w);		// c
                
                mesh.addIndex(x+y*w);				// a
                mesh.addIndex(x+(y+1)*w);			// d
                mesh.addIndex((x+1)+(y+1)*w);		// c
            }
        }
    }
    
    
    // load the texure
    ofDisableArbTex();
    ofLoadImage(textureGlow, "dot.png");
    
    // set the camera distance
    camDist  = 505;
    cam.setDistance(camDist);
    
    // randomly add a point on a sphere
    //    int   num = 500;
    float radius = 100;
    for(int i = 0; i<pathways.size(); i++ ) {
        
        float theta1 = ofRandom(0, TWO_PI);
        float theta2 = ofRandom(0, TWO_PI);
        
        // TODO: this is what makes the spherical lines
        //       change to be on the mesh
        ofVec3f p;
        p.x = ofRandom(1000) - 500; //cos( theta1 ) * cos( theta2 );
        p.y = ofRandom(500) - 250; //sin( theta1 );
        p.z = ofRandom(-50) - 50; //cos( theta1 ) * sin( theta2 );
        
        //        sz = pathways[i]["amount"].asFloat();
        //        r = log(sz + 1)/2;
        //        p *= r;
        
        addPoint(p.x, p.y, p.z);
        
    }
    
    // upload the data to the vbo
    int total = (int)points.size();
    vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
    vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
    
    
        // load the shader
    #ifdef TARGET_OPENGLES
        shaderGlow.load("glow_shader");
    #else
        shaderGlow.load("glow_shader");
    #endif


}
//--------------------------------------------------------------

void ofApp::drawMesh2(){
    
    
    glDepthMask(GL_FALSE);
    
    ofSetColor(255, 100, 90);
    
    // this makes everything look glowy :)
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofEnablePointSprites();
    
    // bind the shader and camera
    // everything inside this function
    // will be effected by the shader/camera
    shaderGlow.begin();
    cam.begin();
    
    // bind the texture so that when all the points
    // are drawn they are replace with our dot image
    textureGlow.bind();
    vbo.draw(GL_POINTS, 0, (int)points.size());
    textureGlow.unbind();
    
    cam.end();
    shaderGlow.end();
    
    ofDisablePointSprites();
    ofDisableBlendMode();
    
    // check to see if the points are
    // sizing to the right size
    ofEnableAlphaBlending();
    cam.begin();
    for (unsigned int i=0; i<points.size(); i++) {
        ofSetColor(255, 80);
        ofVec3f mid = points[i];
        mid.normalize();
        mid *= 300;
    }
    cam.end();
    
    glDepthMask(GL_TRUE);
    
    
    
    cam.begin();
    ofPushMatrix();
    
    //  Center the mesh;
    //
    ofTranslate(-500, -250);
    
    shader.begin();
    shader.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
    shader.setUniform1f("u_time", ofGetElapsedTimef());
    shader.setUniformTexture("u_tex0", texture, 0);
    shader.setUniform2f("u_tex0Resolution", texture.getWidth(), texture.getHeight());
    
    if(showWire){
        mesh.drawWireframe();
    }else if(!showWire){
        mesh.draw();
    }
    
    ofPopMatrix();
    shader.end();
    cam.end();


}
//--------------------------------------------------------------

void ofApp::addPoint(float x, float y, float z) {
    ofVec3f p(x, y, z);
    points.push_back(p);
    
    // we are passing the size in as a normal x position
    // TODO: use data to determine the radius
    float size = ofRandom(5, 50);
    sizes.push_back(ofVec3f(size));
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (showFlare) {
        drawFlareMap();
    }
    if(showRandom){
//    if(!showFlare && !showMesh && !showSpiral && currentLocation != ""){
        drawRandom();
    }

    if(showSpiral){
//    if (!showFlare && !showMesh && !showRandom && currentLocation != "") {
        drawSpiral();
    }
    
    if(showMesh1){
        drawMesh1();
    }
    
    if (showMesh2){
        drawMesh2();
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 'f'){
        ofToggleFullscreen();
    }
    
    if(key =='x'){
        gui->toggleVisible();

    }
}

//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();

    if(name == "Select Brooklyn Hive Location") {
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();

        for(int i = 0; i < selected.size(); i++)
        {
     
            if (selected.size() > 0) {
                if (selected[0]) {
                    currentLocation = selected[0]->getName();
                }
            } else {
                currentLocation = "";
            }
        }
    } else if(name == "Show Mesh1"){
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
        showMesh1 = toggle->getValue();
        
    }else if(name == "Show Mesh2"){
        
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
        showMesh2 = toggle->getValue();
    
    }else if(name == "Show Flare Map"){
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
        showFlare = toggle->getValue();
        if(showFlare)setupPositionsEdges();
    }else if (name == "Wire Frame"){
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
        showWire = toggle->getValue();
    
    }else if (name == "Show Random"){
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
        showRandom = toggle->getValue();
        
    }else if (name== "Show Spiral"){
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
        showSpiral = toggle->getValue();
    
    }
    

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    if(!haveIGrabbedAThing){
        int hoverIndex = idxOfHover();
        if (hoverIndex >= 0) {
            thingIGrabbed = hoverIndex;
            haveIGrabbedAThing = true;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    //when you release your mouse, set haveIgrabbed a thing back to false, so that you don't keep moving the thing
    haveIGrabbedAThing = false;
    thingIGrabbed = -1;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
