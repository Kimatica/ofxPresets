//
//  ofxPresets.cpp
//  kinectMapper
//
//  Created by Nestor Rubio Garcia on 13/02/2015.
//
//

#include "ofxPresets.h"

//--------------------------------------------------------------
ofxPresets::ofxPresets(){}

//--------------------------------------------------------------
void ofxPresets::setup(string name_, int numPresets_, float x_, float y_){
    name = name_;
    numPresets = numPresets_;
    
    // allocate memory for the presets
    presets.clear();
    for(int i = 0; i < numPresets; i++){
        presets.push_back( new ofXml() );
    }
    
    // default active index
    activeIndex = 0;
    
    // build toggle group
    toggles.setName(name);
    for(int i = 0; i < numPresets; i++){
        ofParameter<bool> toggle;
        
        toggle.addListener(this, &ofxPresets::onTogglePressed);
        
        toggles.add(toggle.set("preset " + ofToString(i), false));
    }
    
    // this doesn't work
    // the event is only triggered when the parameters are changed directly
    // eg: toggles.getBool("preset 2") = true;
    // changing the parameter triggers both the button listener and the parameter listener
    // if we use the mouse to press the button, then, the parameter listener is not triggered
    //    ofAddListener(toggles.parameterChangedE, this,  &ofxPresets::onParameterChange);
    
    // build gui
    gui.clear();
    gui.setHeaderBackgroundColor(ofColor::chocolate);
    gui.setBorderColor(ofColor::chocolate);
    gui.setup(name, name+"-settings.xml", x_, y_);
    gui.add(toggles);
    
    // add the "save" button
    saveButton.setup("<-- save preset");
    saveButton.addListener(this, &ofxPresets::onSavePresetPressed);
    gui.add(&saveButton);
}

//--------------------------------------------------------------
void ofxPresets::setParameters(ofParameterGroup* group){
    parameters = group;
}

//--------------------------------------------------------------
void ofxPresets::onTogglePressed(bool & value){
    if(value != true){
        return;
    }
    
    int prevActiveIndex = activeIndex;
    
    // hacky way to find which toggle has changed
    // we need to do this because the group listener
    // does not report the event when a toggle changes :(
    string groupName = gui.getControlNames()[0];
    ofxGuiGroup guiGroup = gui.getGroup(groupName);
    for (int i = 0; i < numPresets; i++) {
        guiGroup.getControl(i)->setBackgroundColor(ofColor::black);
        if (toggles.getBool(i) == true) {
            toggles.getBool(i) = false;
            activeIndex = i;
            guiGroup.getControl(i)->setBackgroundColor(ofColor::saddleBrown);
        }
    }
    
    //cout << "toggle " + toggles.getBool(activeIndex).getName() + " is active" << endl;
    
    if(activeIndex != prevActiveIndex){
        // we need to do this to refresh the drawing due to a bug in ofxGui
        gui.setPosition(gui.getPosition());
        
        // load parameters from the current active preset
        presets[activeIndex]->deserialize( *parameters );
    }
}


//--------------------------------------------------------------
void ofxPresets::updateGuiColor() {
    string groupName = gui.getControlNames()[0];
    ofxGuiGroup guiGroup = gui.getGroup(groupName);
    for (int i = 0; i < numPresets; i++) {
        guiGroup.getControl(i)->setBackgroundColor(ofColor::black);
        if (i == activeIndex) {
            guiGroup.getControl(i)->setBackgroundColor(ofColor::saddleBrown);
        }
    }
    
}


//--------------------------------------------------------------
void ofxPresets::setPreset(int index){
    //if(index >= numPresets  ||  index == activeIndex){
    if(index >= numPresets){
        return;
    }
    
    int prevActiveIndex = activeIndex;
    activeIndex = index;
    
    // apply preset
    presets[activeIndex]->deserialize(*parameters);
    // update gui
    updateGuiColor();
}


void ofxPresets::setNextPreset() {
    int newIndex = activeIndex + 1;
    if (newIndex >= numPresets) {
        newIndex = 0;
    }
    setPreset(newIndex);
}


void ofxPresets::setPrevPreset() {
    int newIndex = activeIndex - 1;
    if (newIndex < 0) {
        newIndex = numPresets - 1;
    }
    setPreset(newIndex);
}


//--------------------------------------------------------------
void ofxPresets::onSavePresetPressed(){
    presets[activeIndex]->clear();
    presets[activeIndex]->serialize(*parameters);
}


//--------------------------------------------------------------
void ofxPresets::save(){
    // save the settings stored in the active preset
    presets[activeIndex]->clear();
    presets[activeIndex]->serialize(*parameters);
    
    // save the xml settings
    string root = "settings/presets-" + name + "/";
    for(int i = 0; i < numPresets; i++){
        string fileName = "preset-" + ofToString(i) + ".xml";
        presets[i]->save(root + fileName);
    }
}

//--------------------------------------------------------------
void ofxPresets::load(){
    // load the xml settings
    string root = "settings/presets-" + name + "/";
    for(int i = 0; i < numPresets; i++){
        string fileName = "preset-" + ofToString(i) + ".xml";
        presets[i]->load(root + fileName);
    }
    
    // apply the settings stored in the active preset
    presets[activeIndex]->deserialize(*parameters);
    
    //    gui.getControl(activeIndex)->getParameter().cast<bool>().set( true ); // don't work
    toggles.getBool(activeIndex) = true;
}

//--------------------------------------------------------------
void ofxPresets::draw(){
    gui.draw();
}

//--------------------------------------------------------------
void ofxPresets::setPosition(ofPoint pos){
    gui.setPosition(pos);
}

//--------------------------------------------------------------
void ofxPresets::setPosition(float x, float y){
    gui.setPosition(x,y);
}

//--------------------------------------------------------------
ofPoint ofxPresets::getPosition(){
    return gui.getPosition();
}

//--------------------------------------------------------------
void ofxPresets::setName(string name_){
    name = name_;
    gui.setName(name_);
}

//--------------------------------------------------------------
string ofxPresets::getName(){
    return gui.getName();
}
