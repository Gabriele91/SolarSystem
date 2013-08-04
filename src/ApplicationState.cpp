//
//  ApplicationState.cpp
//  SolarSystem
//
//  Created by Gabriele Di Bari on 04/08/13.
//
//
#include <stdafx.h>
#include <Application.h>
#include <ApplicationState.h>

using namespace SolarSystem;

Screen* ApplicationState::getScreen(){
	return Application::instance()->getScreen();
}

Audio* ApplicationState::getAudio(){
	return Application::instance()->getAudio();
}

Input* ApplicationState::getInput(){
	return Application::instance()->getInput();
}