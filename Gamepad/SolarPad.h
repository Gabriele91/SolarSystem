#ifndef SOLARPAD_H
#define SOLARPAD_H

#include <Config.h>
#include <Math3D.h>
#include <Input.h>
#include <map>

namespace SolarSystem
{

class GamepadManager
{
public:
    

    struct GamepadDeviceInfo
    {
	    // device id
	    unsigned int deviceID;
	    // Human-readable device name
	    const char * description;
	    // USB vendor/product IDs as returned by the driver. Can be used to determine the particular model of device represented.
	    int vendorID;
	    int productID;
	    // Number of axis elements belonging to the device
	    unsigned int numAxes;
	    // Number of button elements belonging to the device
	    unsigned int numButtons;
	    // Array[numAxes] of values representing the current state of each axis, in the range [-1..1]
	    float * axisStates;
	    // Array[numButtons] of values representing the current state of each button
	    bool * buttonStates;

    private:
	    // Don't touch unless you know what you're doing and don't
	    void * privateData;
    };

    class Gamepad
    {
    public:
        virtual void onAttach(){};
        virtual void onRemove(){};
        virtual void onButtonDown(uint button, double timestamp){};
        virtual void onButtonUp(uint button,  double timestamp){};
        virtual void onAxisMove(uint axisID, float value, 
                                             float lastValue, 
                                             double timestamp){};
    };

private:

    std::map < uint, Gamepad* > hendlers;

    void _callbackOnAttach(uint id);
    void _callbackOnRemove(uint id);
    void _callbackOnButtonDown(uint id,uint button,double timestamp);
    void _callbackOnButtonUp(uint id,uint button,double timestamp);
    void _callbackAxisMove(uint id,
                           uint axisID,
                           float value, 
                           float lastValue, 
                           double timestamp);

public:

    uint deviceCount();
    GamepadDeviceInfo* getInfo(uint id);
    void addHandler(uint id,Gamepad* gamepad)
    {
        hendlers[id]=gamepad;
    }
    void removeHandler(uint id)
    {
        auto it=hendlers.find(id);
        if(it!=hendlers.end()) hendlers.erase(id);
    }
    
    void init();
    void update();
    void end();

};

};

#endif