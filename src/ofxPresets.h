
#pragma once

#include "ofMain.h"
#include "ofParameter.h"
#include "ofxGui.h"


class ofxPresets {
    
public:
    
    ofxPresets();
    
    void setup(string name, int numPresets, float x = 10, float y = 10);
    void setParameters(ofParameterGroup * group);
    
    void save(); // persist presets to the hard drive
    void load(); // load presets from the hard drive
    
    void draw();
    
    // change active index without clicking on gui
    void setPreset(int index);
    void setNextPreset();
    void setPrevPreset();
    
    int getActiveIndex() { return activeIndex; }
    
    // wraps ofxBaseGui functionallity:
    
    void setPosition(ofPoint pos);
    void setPosition(float x, float y);
    void setName(string name);
    
    string getName();
    ofPoint getPosition();
    
    ofxPanel* getPanel() { return &gui; }
    
    int getNumPresets(){ return numPresets; }
    
    
private:
    
    string name;
    int activeIndex;
    int numPresets;
    
    ofParameterGroup* parameters;  // this group holds all parameters to create presets
    
    vector<ofXml*> presets;  // each xml stores the preset
    
    //gui
    ofParameterGroup toggles;
    ofxPanel gui;
    void onTogglePressed(bool & value);  // listener for changes in gui
    
    ofxButton saveButton;
    void onSavePresetPressed();
    
    
    void updateGuiColor();
};