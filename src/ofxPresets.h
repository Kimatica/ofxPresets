#pragma once
#include "ofMain.h"
#include "ofParameter.h"
#include "ofxGui.h"

class ofxPresets {
    
public:
    
    void setup(string name, int numPresets, float x = 10, float y = 10);
    
    void setParameters(ofParameterGroup* group) { parameters = group; }
    
    void save(); //< persist presets to hard drive
    void load(); //< load presets from hard drive
    
    void applyPreset(int index);
    void applyNextPreset();
    void applyPrevPreset();
    
    // -- panel
    
    void draw() { gui.draw(); }
    
    void setPosition(ofPoint pos) { gui.setPosition(pos); }
    void setPosition(float x, float y) { gui.setPosition(x,y); }
    
    string getName() { return gui.getName(); }
    ofPoint getPosition() { return gui.getPosition(); }
    ofxPanel* getPanel() { return &gui; }
    
private:
    
    string name;
    int numPresets;
    int activeIndex;
    
    ofParameterGroup* parameters;
    vector<ofXml*> presets;
    
    void savePreset();
    
    // panel
    
    ofxPanel gui;
    ofParameterGroup toggles;
    ofxButton saveButton;
    
    void onToggleChange(ofAbstractParameter& parameter);
};
