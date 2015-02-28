#include <stdafx.h>
#include "SolarPad.h"
#include "Gamepad.h"

using namespace SolarSystem;

void GamepadManager::_callbackOnAttach(uint id)
{
    auto& it=hendlers.find(id);
    if(it!=hendlers.end()) it->second->onAttach();
}
void GamepadManager::_callbackOnRemove(uint id)
{
    auto& it=hendlers.find(id);
    if(it!=hendlers.end()) it->second->onRemove();
}
void GamepadManager::_callbackOnButtonDown(uint id,uint button,double timestamp)
{
    auto& it=hendlers.find(id);
    if(it!=hendlers.end()) it->second->onButtonDown(button,timestamp);
}
void GamepadManager::_callbackOnButtonUp(uint id,uint button,double timestamp)
{
    auto& it=hendlers.find(id);
    if(it!=hendlers.end()) it->second->onButtonUp(button,timestamp);
}
void GamepadManager::_callbackAxisMove(uint id,
                                       uint axisID,
                                       float value, 
                                       float lastValue, 
                                       double timestamp)
{
    auto& it=hendlers.find(id);
    if(it!=hendlers.end()) it->second->onAxisMove(axisID,value,lastValue,timestamp);
}

uint GamepadManager::deviceCount()
{
    return Gamepad_numDevices();
}
GamepadManager::GamepadDeviceInfo* GamepadManager::getInfo(uint id)
{
    if(id<Gamepad_numDevices())
        return (GamepadManager::GamepadDeviceInfo*)Gamepad_deviceAtIndex(id);
    else
        return nullptr;
}
void GamepadManager::init()
{
    Gamepad_init();
    //add events
    Gamepad_deviceAttachFunc(
    [](struct Gamepad_device * device,  void * context)
    {
       ((GamepadManager*)context)->_callbackOnAttach(device->deviceID);
    }, 
    (void*)this);

    Gamepad_deviceRemoveFunc(
    [](struct Gamepad_device * device,  void * context)
    {
       ((GamepadManager*)context)->_callbackOnRemove(device->deviceID);
    }, 
    (void*)this);
    
    Gamepad_buttonDownFunc(
    [](struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context)
    {
       ((GamepadManager*)context)->_callbackOnButtonDown(device->deviceID, buttonID, timestamp);
    }, 
    (void*)this);

    Gamepad_buttonUpFunc(
    [](struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context)
    {
       ((GamepadManager*)context)->_callbackOnButtonUp(device->deviceID, buttonID, timestamp);
    }, 
    (void*)this);

    Gamepad_axisMoveFunc(
    [](struct Gamepad_device * device,unsigned int axisID, float value, float lastValue, double timestamp, void * context)
    {
       ((GamepadManager*)context)->_callbackAxisMove(device->deviceID, axisID, value, lastValue, timestamp);
    }, 
    (void*)this);
}
void GamepadManager::update()
{
    Gamepad_processEvents();
}
void GamepadManager::end()
{
    Gamepad_shutdown();
}