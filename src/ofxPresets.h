
#pragma once

#include "ofMain.h"
#include "ofParameter.h"
#include "ofxGui.h"
#include "ofxXmlPoco.h"


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

    vector<ofxXmlPoco*> presets;  // each xml stores the preset
    
    //gui
    ofParameterGroup toggles;
    ofxPanel gui;
    void onTogglePressed(bool & value);  // listener for changes in gui
    
    ofxButton saveButton;
    void onSavePresetPressed();
    
    
    void updateGuiColor();

	// linear interpolation between presets
	ofxXmlPoco* presetSrc;
	ofxXmlPoco* presetDst;
	void setValues (ofAbstractParameter & parameter, ofxXmlPoco & src, ofxXmlPoco & dst, float pct);
	void startTransition();
	void endTransition();
	void updateTransition(ofEventArgs& arg);

	int prevActiveIndex;

	float pct;
};
