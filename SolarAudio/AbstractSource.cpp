#include <stdafx.h>
#include <AbstractSource .h>
using namespace SolarSystem;

const String& AbstractSource::getName(){
	return this->name;
}
void AbstractSource::_SetNameFromPathName(const Utility::Path & pathfilename){	
	this->name=pathfilename.getFilebasename();
}