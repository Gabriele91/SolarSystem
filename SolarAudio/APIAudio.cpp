#include <stdafx.h>
#include <APIAudio.h>
#include <EString.h>
#include <string.h>
#include <Debug.h>
//using name space
using namespace SolarSystem;
//data info
ALCdevice *device;
ALCcontext *context;

String GetALErrorString(ALenum err){
    switch(err)
    {
        case AL_NO_ERROR:
            return String("AL_NO_ERROR");
        break;

        case AL_INVALID_NAME:
            return String("AL_INVALID_NAME");
        break;

        case AL_INVALID_ENUM:
            return String("AL_INVALID_ENUM");
        break;

        case AL_INVALID_VALUE:
            return String("AL_INVALID_VALUE");
        break;

        case AL_INVALID_OPERATION:
            return String("AL_INVALID_OPERATION");
        break;

        case AL_OUT_OF_MEMORY:
            return String("AL_OUT_OF_MEMORY");
        break;
    };
    return String("AL_?????_ERROR");
}
String GetALCErrorString(ALCenum err){
    switch(err)
    {
	    case ALC_NO_ERROR:
            return String("ALC_NO_ERROR");
        break;

        case ALC_INVALID_ENUM:
            return String("ALC_INVALID_ENUM");
        break;

        case ALC_INVALID_VALUE:
            return String("ALC_INVALID_VALUE");
        break;

		case ALC_INVALID_CONTEXT:
            return String("ALC_INVALID_CONTEXT");
        break;

		case ALC_INVALID_DEVICE:
            return String("ALC_INVALID_DEVICE");
        break;

        case ALC_OUT_OF_MEMORY:
            return String("ALC_OUT_OF_MEMORY");
        break;
    };
    return String("AL_?????_ERROR");
}

void GetDevices(std::vector<String>& Devices){
    // Vidage de la liste
    Devices.clear();

    // Récupération des devices disponibles
    const ALCchar* DeviceList = alcGetString(NULL, ALC_DEVICE_SPECIFIER);

    if (DeviceList)
    {
        // Extraction des devices contenus dans la chaîne renvoyée
        while (strlen(DeviceList) > 0)
        {
            Devices.push_back(DeviceList);
            DeviceList += strlen(DeviceList) + 1;
        }
    }
}

void SolarSystem::APIAudio::init(){
	//
	std::vector<String> devices;
	GetDevices(devices);
	for(unsigned int i=0;i<devices.size();++i)
		DEBUG_MESSAGE( "audio device:"<<devices[i] );

    // Clear Error Code (so we can catch any new errors)
    alGetError();
	int error = AL_NO_ERROR;
	// Initialization
	alcMakeContextCurrent(NULL);
	// select the "preferred device"
    device = alcOpenDevice(alcGetString(NULL,ALC_DEFAULT_DEVICE_SPECIFIER));
	DEBUG_MESSAGE("alcOpenDevice = "<<device);
	DEBUG_ASSERT_MSG( device, "openAL can't open device");
	//
	if(!device)
		DEBUG_MESSAGE("openALc open device error:" << GetALCErrorString(alcGetError(device)) );
	else {

				if(!(context = alcCreateContext(device,NULL)))
					DEBUG_MESSAGE("openALc create context error:" << GetALCErrorString(alcGetError(device)) );

				if(!alcMakeContextCurrent(context))
					DEBUG_MESSAGE( "openALc make current error:" << GetALCErrorString(alcGetError(device)) );

				error = alGetError();
				DEBUG_ASSERT_MSG( error == AL_NO_ERROR , "openAL error:" << GetALErrorString(error) );
    }

 }

 void SolarSystem::APIAudio::exit(){
  //delete
  alcMakeContextCurrent(NULL);
  alcDestroyContext (context);
  //
  if (alcGetError (device) != ALC_NO_ERROR)
    {
      DEBUG_MESSAGE("OpenAL error destroy context: " << GetALErrorString(alcGetError (device)));
      return;
    }

  if (!alcCloseDevice (device))
    {
      DEBUG_MESSAGE("OpenAL error close device: " << GetALErrorString(alcGetError (device)));
    }
 }
