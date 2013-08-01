#include <stdafx.h>
#include <Config.h>
#include <stdlib.h>
#include <string.h>
#include <Application.h>
#include <Debug.h>
#include <EString.h>
#include <Utility.h>
#include <Types.h>
////////////////////////////
using namespace SolarSystem;
// buffer is always in big endian!
static unsigned short readByte16(const unsigned char buffer[2]) {
#if PCCE_BIG_ENDIAN
 	return (buffer[0] << 8) + buffer[1];
 	#else
 	return (buffer[1] << 8) + buffer[0];
#endif
}
static unsigned long readByte32(const unsigned char buffer[4]) {
 	#if PCCE_BIG_ENDIAN
 	return (buffer[0] << 24) + (buffer[1] << 16) + (buffer[2] << 8) + buffer[3];
 	#else
 	return (buffer[3] << 24) + (buffer[2] << 16) + (buffer[1] << 8) + buffer[0];
 	#endif
}
//load wav
ALuint OpenALReadWav(const Utility::Path& path,int & outformat,float & leng){
	//get raw file
	void *data=NULL; size_t len=0;
	Application::instance()->loadData(path,data,len);

	#define str4cmp(s1,s2) (strncmp(s1,s2,4)==0)

	ASPACKED(struct WavHeader{
		char chunkID[4];
		uint chunkSize;
		char format[4];
	});
	
	ASPACKED(struct WavRIFFHeader{
		char subchunk1ID[4];
		uint subchunk1Size;
		ushort audioFormat;
		ushort numChannels;
		uint sampleRate;
		uint byteRate;// SampleRate * NumChannels * BitsPerSample/8
		ushort blockAlign;//  NumChannels * BitsPerSample/8
		ushort bitsPerSample;
	});

	ASPACKED(struct RawHeader{
		char subchunk2ID[4];
		uint subchunk2Size;
		char data[1];
	});
	
	WavHeader *wavHeader;
	WavRIFFHeader *wavRIFFHeader;
	RawHeader *rawData;
	//point to file readed:
	char *cData=(char*)data;
	wavHeader=(WavHeader*)cData;
	wavRIFFHeader=(WavRIFFHeader*)(cData+sizeof(WavHeader));
	rawData=(RawHeader*)(cData+sizeof(WavHeader)+8+wavRIFFHeader->subchunk1Size);
	//is a wave, riff, fmt ,pcm file.
	bool isriff=str4cmp(wavHeader->chunkID,"RIFF");
	bool iswave=str4cmp(wavHeader->format,"WAVE");
	bool isfmt=str4cmp(wavRIFFHeader->subchunk1ID,"fmt ");
	bool ispcm=wavRIFFHeader->audioFormat==1;//pcm==1
	DEBUG_ASSERT_MSG(isriff,"OpenALReadWav: file must to be riff header");
	DEBUG_ASSERT_MSG(iswave,"OpenALReadWav: file must to be wave format");
	DEBUG_ASSERT_MSG(isfmt,"OpenALReadWav: file must to be ftm sound data's format");
	DEBUG_ASSERT_MSG(ispcm,"OpenALReadWav: file must to be PCM = 1 (i.e. Linear quantization)");
	//openAL	
	//get openAL format
 	ALenum format;
	if (wavRIFFHeader->numChannels == 1)
        format = (wavRIFFHeader->bitsPerSample == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
 	else
        format = (wavRIFFHeader->bitsPerSample == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
	//id buffer
 	ALuint buffer = AL_NONE;
	alGenBuffers(1, &buffer);
 	DEBUG_ASSERT_MSG(alGetError() == AL_NO_ERROR, "OpenALReadWav: Could not generate buffer");
 	DEBUG_ASSERT_MSG(AL_NONE != buffer, "OpenALReadWav: Could not generate buffer");
	//load raw data
	alBufferData(buffer, format, rawData->data, rawData->subchunk2Size, wavRIFFHeader->sampleRate);
 	DEBUG_ASSERT_MSG(alGetError() == AL_NO_ERROR, "LoadWav: Could not load buffer data");
	//return len sound	
	leng=((float)rawData->subchunk2Size/(float)wavRIFFHeader->byteRate);
	//dealloc
	free(data);
	//return buffer id
	return buffer;
}
