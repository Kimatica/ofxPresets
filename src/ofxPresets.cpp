#include "ofxPresets.h"

void ofxPresets::setup(string name, int numPresets, float x_, float y_)
{
    this->name = name;
    this->numPresets = numPresets;
    activeIndex = -1;
    
    presets.clear();
    for (int i = 0; i < numPresets; i++) {
        presets.push_back( new ofXml() );
    }
    
    toggles.clear();
    toggles.setName(name);
    for (int i = 0; i < numPresets; i++) {
        ofParameter<bool> toggle;
        toggles.add(toggle.set("preset " + ofToString(i), false));
    }
    ofAddListener(toggles.parameterChangedE(), this, &ofxPresets::onToggleChange);
    
    // build gui
    gui.clear();
    gui.setHeaderBackgroundColor(ofColor::chocolate);
    gui.setBorderColor(ofColor::chocolate);
    gui.setup(name, name+"-settings.xml", x_, y_);
    gui.add(toggles);
    
    // add the "save" button
    saveButton.setup("<-- save preset");
    saveButton.addListener(this, &ofxPresets::savePreset);
    gui.add(&saveButton);
}

void ofxPresets::onToggleChange(ofAbstractParameter& p)
{
    auto& clicked = p.cast<bool>();
    for (int i = 0; i < toggles.size(); i++) {
        auto& toggle = toggles.getBool(i);
        bool value = toggle.getName() == clicked.getName();
        toggle.disableEvents();
        toggle.set(value);
        toggle.enableEvents();
        if (value == true) {
            applyPreset(i);
        }
    }
    // hack to refresh ofxGui panel
    gui.setPosition(gui.getPosition());
}

void ofxPresets::applyPreset(int index)
{
    //if(index >= numPresets || index == activeIndex){
    if (index >= numPresets) {
        return;
    }
    activeIndex = index;
    ofDeserialize(*presets[activeIndex], *parameters);
}

void ofxPresets::applyNextPreset()
{
    int newIndex = activeIndex + 1;
    if (newIndex >= numPresets) {
        newIndex = 0;
    }
    applyPreset(newIndex);
}

void ofxPresets::applyPrevPreset()
{
    int newIndex = activeIndex - 1;
    if (newIndex < 0) {
        newIndex = numPresets - 1;
    }
    applyPreset(newIndex);
}

void ofxPresets::savePreset()
{
    presets[activeIndex]->clear();
    ofSerialize(*presets[activeIndex], *parameters);
}

void ofxPresets::save()
{
    // save the active preset
    savePreset();
    
    // persist all to hard drive
    string root = "settings/presets-" + name + "/";

    ofDirectory dir(root);
    if (!dir.exists())
        dir.create(true);

    for(int i = 0; i < numPresets; i++){
        string fileName = "preset-" + ofToString(i) + ".xml";
        bool ok = presets[i]->save(root + fileName);
    }
}

void ofxPresets::load()
{
    string root = "settings/presets-" + name + "/";
    for(int i = 0; i < numPresets; i++){
        string fileName = "preset-" + ofToString(i) + ".xml";
        presets[i]->load(root + fileName);
    }
    
    applyPreset(0);
    
    toggles.getBool(activeIndex) = true;
}

