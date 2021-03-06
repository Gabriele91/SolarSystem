#include <stdafx.h>
#include <ETime.h>
#ifdef PLATFORM_UNIX
    #include <sys/time.h>
#endif
//cpu Ticks
extern SolarSystem::bit64 SolarSystem::GetTimeTicks(){
	SolarSystem::bit64 val;
	#if defined(_MSC_VER)
		QueryPerformanceCounter( (LARGE_INTEGER *)&val );
	#else
		timeval timeVal;

		gettimeofday( &timeVal, NULL );

		val = (SolarSystem::bit64)timeVal.tv_sec * (1000*1000) + (SolarSystem::bit64)timeVal.tv_usec;
	#endif
	return val;
}
//time
extern double SolarSystem::GetTime(){
	static double	coe;

#if defined(_MSC_VER)
	static SolarSystem::bit64 freq;

	if ( freq == 0 )
	{
		QueryPerformanceFrequency( (LARGE_INTEGER *)&freq );
		coe = 1000.0 / freq;
	}

#else
	coe = 1.0 / 1000.0;

#endif

	return GetTimeTicks() * coe;
}
