#include "ofxPresets.h"

void ofxPresets::setup(string name, int numPresets, float x, float y)
{
    this->name = "presets_" + name;
    this->numPresets = numPresets;
    activeIndex = -1;
    
    presets.clear();
    for (int i = 0; i < numPresets; i++) {
        presets.push_back( new ofXml() );
    }
    
    // build gui
    
    toggles.clear();
    toggles.setName(this->name);
    for (int i = 0; i < numPresets; i++) {
        ofParameter<bool> toggle;
        toggles.add(toggle.set(ofToString(i), false));
    }
    ofAddListener(toggles.parameterChangedE(), this, &ofxPresets::onToggleChange);
    
    gui.clear();
    gui.setHeaderBackgroundColor(ofColor::chocolate);
    gui.setBorderColor(ofColor::chocolate);
    gui.setup(this->name, this->name+"_settings.xml", x, y);
    gui.add(toggles);
    
    saveButton.set("<-- save preset");
    saveButton.addListener(this, &ofxPresets::saveCurrent);
    gui.add(saveButton);
    
    clearButton.set("<-- clear preset");
    clearButton.addListener(this, &ofxPresets::clearCurrent);
    gui.add(clearButton);

    saveToDiskButton.set("<-- save to disk");
    saveToDiskButton.addListener(this, &ofxPresets::saveToDisk);
    gui.add(saveToDiskButton);

    loadFromDiskButton.set("<-- load from disk");
    loadFromDiskButton.addListener(this, &ofxPresets::loadFromDisk);
    gui.add(loadFromDiskButton);
}


// TODO: change name to onToggleChange
// this callback is triggered on parameter change (click or osc)
void ofxPresets::onToggleChange(ofAbstractParameter& p)
{
    auto& clicked = p.cast<bool>();
    bool value = clicked.get();
    
    if (value == false) return;
    
    for (int i = 0; i < toggles.size(); i++) {
        auto& toggle = toggles.getBool(i);
        bool isClicked = (toggle.getName() == clicked.getName());
        
        // update gui
        toggle.disableEvents();
        toggle.set(isClicked);
        toggle.enableEvents();
        
        // set preset
        if (isClicked && value == true) {
            set(i);
        }
    }
    
    // hack to refresh ofxGui panel
    gui.setPosition(gui.getPosition());
}

void ofxPresets::set(int index)
{
    if (index >= numPresets) return;
    //if (index == activeIndex) return;
    
    activeIndex = index;
    ofDeserialize(*presets[activeIndex], *parameters);
}

void ofxPresets::setNext()
{
    int newIndex = activeIndex + 1;
    if (newIndex >= numPresets) {
        newIndex = 0;
    }
    set(newIndex);
}

void ofxPresets::setPrev()
{
    int newIndex = activeIndex - 1;
    if (newIndex < 0) {
        newIndex = numPresets - 1;
    }
    set(newIndex);
}

void ofxPresets::saveCurrent()
{
    presets[activeIndex]->clear();
    ofSerialize(*presets[activeIndex], *parameters);
    updateGuiColor();
}

void ofxPresets::clearCurrent()
{
    presets[activeIndex]->clear();
    updateGuiColor();
}

void ofxPresets::saveToDisk()
{
    //saveCurrent();
    
    //string root = "settings/" + name + "/";
    string root = name + "/";
    ofDirectory dir(root);
    if(!dir.exists()) dir.create(); // pass true to create recursive directories
    
    for(int i = 0; i < numPresets; i++){
        string fileName = "preset_" + ofToString(i) + ".xml";
        presets[i]->save(dir.path() + fileName);
    }
}

void ofxPresets::loadFromDisk()
{
    //string root = "settings/" + name + "/";
    string root = name + "/";
    for(int i = 0; i < numPresets; i++){
        string fileName = "preset_" + ofToString(i) + ".xml";
        presets[i]->load(root + fileName);
    }
    
    set(0);
    toggles.getBool(0) = true;
    updateGuiColor();
}


void ofxPresets::updateGuiColor()
{
    string groupName = gui.getControlNames()[0];
    ofxGuiGroup& guiGroup = gui.getGroup(groupName);
    for (int i = 0; i < numPresets; i++) {
        ofColor color = isEmpty(i) ? ofColor(0) : ofColor(60);
        guiGroup.getControl(i)->setBackgroundColor(color);
    }
    gui.setPosition(gui.getPosition());
}


bool ofxPresets::isEmpty(int index)
{
    return !presets[index]->getFirstChild();
}
