#pragma once
#include "ofMain.h"
#include "ofParameter.h"
#include "ofxGui.h"

class ofxPresets {
    
public:
    
    void setup(string name, int numPresets, float x = 10, float y = 10);
    
    void setParameters(ofParameterGroup* group) { parameters = group; }
    
    void set(int index);
    void setNext();
    void setPrev();
    
    void saveCurrent();
    void clearCurrent();
    
    void moveCurrentUp();
    void moveCurrentDown();
    
    void saveToDisk();
    void loadFromDisk(); //< load presets from hard drive
    
    // panel
    
    void draw() { gui.draw(); }
    void setPosition(float x, float y) { gui.setPosition(x,y); }
    ofPoint getPosition() { return gui.getPosition(); }
    ofxPanel& getPanel() { return gui; }
    ofParameterGroup& getTogglesGroup() { return toggles; } // osc control
    
private:
    
    string name;
    int numPresets;
    int activeIndex;
    
    ofParameterGroup* parameters;
    vector<ofXml*> presets;
    
    bool isEmpty(int index);
    
    // panel
    
    ofxPanel gui;
    ofParameterGroup toggles;
    ofParameter<void> saveButton;
    ofParameter<void> clearButton;

    void onToggleClick(ofAbstractParameter& parameter);
    void updateGuiColor();
};
