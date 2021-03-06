#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.138                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = 
CFLAGS =  -Wall
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = 

INC_DEBUG_LINUX =  $(INC) -I./ -Iinclude -Iinclude/linux -ISolarSystem
CFLAGS_DEBUG_LINUX =  $(CFLAGS) -g -std=gnu++0x -fpermissive -D_DEBUG -DORDERED_TABLE
RESINC_DEBUG_LINUX =  $(RESINC)
RCFLAGS_DEBUG_LINUX =  $(RCFLAGS)
LIBDIR_DEBUG_LINUX =  $(LIBDIR)
LIB_DEBUG_LINUX = $(LIB)
LDFLAGS_DEBUG_LINUX =  $(LDFLAGS) -lX11 -lXxf86vm -lGL -lGLU -lopenal
OBJDIR_DEBUG_LINUX = obj/Debug
DEP_DEBUG_LINUX = 
OUT_DEBUG_LINUX = bin/Debug/sphere

INC_RELEASE_LINUX =  $(INC) -I./ -Iinclude -Iinclude/linux -ISolarSystem
CFLAGS_RELEASE_LINUX =  $(CFLAGS) -O2 -std=gnu++0x -fpermissive -DORDERED_TABLE
RESINC_RELEASE_LINUX =  $(RESINC)
RCFLAGS_RELEASE_LINUX =  $(RCFLAGS)
LIBDIR_RELEASE_LINUX =  $(LIBDIR)
LIB_RELEASE_LINUX = $(LIB)
LDFLAGS_RELEASE_LINUX =  $(LDFLAGS) -s -lX11 -lXxf86vm -lGL -lGLU -lopenal
OBJDIR_RELEASE_LINUX = obj/Release
DEP_RELEASE_LINUX = 
OUT_RELEASE_LINUX = bin/Release/sphere

OBJ_DEBUG_LINUX = $(OBJDIR_DEBUG_LINUX)/src/EString.o $(OBJDIR_DEBUG_LINUX)/src/Object.o $(OBJDIR_DEBUG_LINUX)/src/Math2D.o $(OBJDIR_DEBUG_LINUX)/src/MainInstance.o $(OBJDIR_DEBUG_LINUX)/src/Input.o $(OBJDIR_DEBUG_LINUX)/src/Image/lodepng.o $(OBJDIR_DEBUG_LINUX)/src/ETime.o $(OBJDIR_DEBUG_LINUX)/src/Screen.o $(OBJDIR_DEBUG_LINUX)/src/Debug.o $(OBJDIR_DEBUG_LINUX)/src/Camera.o $(OBJDIR_DEBUG_LINUX)/src/Application.o $(OBJDIR_DEBUG_LINUX)/src/linux/OpenGLLinux.o $(OBJDIR_DEBUG_LINUX)/src/linux/LinuxScreen.o $(OBJDIR_DEBUG_LINUX)/src/linux/LinuxInput.o $(OBJDIR_DEBUG_LINUX)/src/linux/LinuxApp.o $(OBJDIR_DEBUG_LINUX)/src/Utility.o $(OBJDIR_DEBUG_LINUX)/src/Timer.o $(OBJDIR_DEBUG_LINUX)/src/Texture.o $(OBJDIR_DEBUG_LINUX)/src/Table.o $(OBJDIR_DEBUG_LINUX)/src/Shader.o $(OBJDIR_DEBUG_LINUX)/SolarSystem/SolarSky.o $(OBJDIR_DEBUG_LINUX)/SolarSystem/SolarRender.o $(OBJDIR_DEBUG_LINUX)/SolarSystem/SolarFly.o $(OBJDIR_DEBUG_LINUX)/SolarSystem/PlanetsManager.o $(OBJDIR_DEBUG_LINUX)/main.o $(OBJDIR_DEBUG_LINUX)/SolarSystem/Planet.o

OBJ_RELEASE_LINUX = $(OBJDIR_RELEASE_LINUX)/src/EString.o $(OBJDIR_RELEASE_LINUX)/src/Object.o $(OBJDIR_RELEASE_LINUX)/src/Math2D.o $(OBJDIR_RELEASE_LINUX)/src/MainInstance.o $(OBJDIR_RELEASE_LINUX)/src/Input.o $(OBJDIR_RELEASE_LINUX)/src/Image/lodepng.o $(OBJDIR_RELEASE_LINUX)/src/ETime.o $(OBJDIR_RELEASE_LINUX)/src/Screen.o $(OBJDIR_RELEASE_LINUX)/src/Debug.o $(OBJDIR_RELEASE_LINUX)/src/Camera.o $(OBJDIR_RELEASE_LINUX)/src/Application.o $(OBJDIR_RELEASE_LINUX)/src/linux/OpenGLLinux.o $(OBJDIR_RELEASE_LINUX)/src/linux/LinuxScreen.o $(OBJDIR_RELEASE_LINUX)/src/linux/LinuxInput.o $(OBJDIR_RELEASE_LINUX)/src/linux/LinuxApp.o $(OBJDIR_RELEASE_LINUX)/src/Utility.o $(OBJDIR_RELEASE_LINUX)/src/Timer.o $(OBJDIR_RELEASE_LINUX)/src/Texture.o $(OBJDIR_RELEASE_LINUX)/src/Table.o $(OBJDIR_RELEASE_LINUX)/src/Shader.o $(OBJDIR_RELEASE_LINUX)/SolarSystem/SolarSky.o $(OBJDIR_RELEASE_LINUX)/SolarSystem/SolarRender.o $(OBJDIR_RELEASE_LINUX)/SolarSystem/SolarFly.o $(OBJDIR_RELEASE_LINUX)/SolarSystem/PlanetsManager.o $(OBJDIR_RELEASE_LINUX)/main.o $(OBJDIR_RELEASE_LINUX)/SolarSystem/Planet.o

all: debug_linux release_linux

clean: clean_debug_linux clean_release_linux

before_debug_linux: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG_LINUX)/src || mkdir -p $(OBJDIR_DEBUG_LINUX)/src
	test -d $(OBJDIR_DEBUG_LINUX)/src/Image || mkdir -p $(OBJDIR_DEBUG_LINUX)/src/Image
	test -d $(OBJDIR_DEBUG_LINUX)/src/linux || mkdir -p $(OBJDIR_DEBUG_LINUX)/src/linux
	test -d $(OBJDIR_DEBUG_LINUX)/SolarSystem || mkdir -p $(OBJDIR_DEBUG_LINUX)/SolarSystem
	test -d $(OBJDIR_DEBUG_LINUX) || mkdir -p $(OBJDIR_DEBUG_LINUX)

after_debug_linux: 

debug_linux: before_debug_linux out_debug_linux after_debug_linux

out_debug_linux: before_debug_linux $(OBJ_DEBUG_LINUX) $(DEP_DEBUG_LINUX)
	$(LD) $(LIBDIR_DEBUG_LINUX) -o $(OUT_DEBUG_LINUX) $(OBJ_DEBUG_LINUX)  $(LDFLAGS_DEBUG_LINUX) $(LIB_DEBUG_LINUX)

$(OBJDIR_DEBUG_LINUX)/src/EString.o: src/EString.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/EString.cpp -o $(OBJDIR_DEBUG_LINUX)/src/EString.o

$(OBJDIR_DEBUG_LINUX)/src/Object.o: src/Object.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/Object.cpp -o $(OBJDIR_DEBUG_LINUX)/src/Object.o

$(OBJDIR_DEBUG_LINUX)/src/Math2D.o: src/Math2D.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/Math2D.cpp -o $(OBJDIR_DEBUG_LINUX)/src/Math2D.o

$(OBJDIR_DEBUG_LINUX)/src/MainInstance.o: src/MainInstance.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/MainInstance.cpp -o $(OBJDIR_DEBUG_LINUX)/src/MainInstance.o

$(OBJDIR_DEBUG_LINUX)/src/Input.o: src/Input.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/Input.cpp -o $(OBJDIR_DEBUG_LINUX)/src/Input.o

$(OBJDIR_DEBUG_LINUX)/src/Image/lodepng.o: src/Image/lodepng.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/Image/lodepng.cpp -o $(OBJDIR_DEBUG_LINUX)/src/Image/lodepng.o

$(OBJDIR_DEBUG_LINUX)/src/ETime.o: src/ETime.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/ETime.cpp -o $(OBJDIR_DEBUG_LINUX)/src/ETime.o

$(OBJDIR_DEBUG_LINUX)/src/Screen.o: src/Screen.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/Screen.cpp -o $(OBJDIR_DEBUG_LINUX)/src/Screen.o

$(OBJDIR_DEBUG_LINUX)/src/Debug.o: src/Debug.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/Debug.cpp -o $(OBJDIR_DEBUG_LINUX)/src/Debug.o

$(OBJDIR_DEBUG_LINUX)/src/Camera.o: src/Camera.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/Camera.cpp -o $(OBJDIR_DEBUG_LINUX)/src/Camera.o

$(OBJDIR_DEBUG_LINUX)/src/Application.o: src/Application.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/Application.cpp -o $(OBJDIR_DEBUG_LINUX)/src/Application.o

$(OBJDIR_DEBUG_LINUX)/src/linux/OpenGLLinux.o: src/linux/OpenGLLinux.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/linux/OpenGLLinux.cpp -o $(OBJDIR_DEBUG_LINUX)/src/linux/OpenGLLinux.o

$(OBJDIR_DEBUG_LINUX)/src/linux/LinuxScreen.o: src/linux/LinuxScreen.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/linux/LinuxScreen.cpp -o $(OBJDIR_DEBUG_LINUX)/src/linux/LinuxScreen.o

$(OBJDIR_DEBUG_LINUX)/src/linux/LinuxInput.o: src/linux/LinuxInput.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/linux/LinuxInput.cpp -o $(OBJDIR_DEBUG_LINUX)/src/linux/LinuxInput.o

$(OBJDIR_DEBUG_LINUX)/src/linux/LinuxApp.o: src/linux/LinuxApp.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/linux/LinuxApp.cpp -o $(OBJDIR_DEBUG_LINUX)/src/linux/LinuxApp.o

$(OBJDIR_DEBUG_LINUX)/src/Utility.o: src/Utility.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/Utility.cpp -o $(OBJDIR_DEBUG_LINUX)/src/Utility.o

$(OBJDIR_DEBUG_LINUX)/src/Timer.o: src/Timer.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/Timer.cpp -o $(OBJDIR_DEBUG_LINUX)/src/Timer.o

$(OBJDIR_DEBUG_LINUX)/src/Texture.o: src/Texture.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/Texture.cpp -o $(OBJDIR_DEBUG_LINUX)/src/Texture.o

$(OBJDIR_DEBUG_LINUX)/src/Table.o: src/Table.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/Table.cpp -o $(OBJDIR_DEBUG_LINUX)/src/Table.o

$(OBJDIR_DEBUG_LINUX)/src/Shader.o: src/Shader.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c src/Shader.cpp -o $(OBJDIR_DEBUG_LINUX)/src/Shader.o

$(OBJDIR_DEBUG_LINUX)/SolarSystem/SolarSky.o: SolarSystem/SolarSky.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c SolarSystem/SolarSky.cpp -o $(OBJDIR_DEBUG_LINUX)/SolarSystem/SolarSky.o

$(OBJDIR_DEBUG_LINUX)/SolarSystem/SolarRender.o: SolarSystem/SolarRender.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c SolarSystem/SolarRender.cpp -o $(OBJDIR_DEBUG_LINUX)/SolarSystem/SolarRender.o

$(OBJDIR_DEBUG_LINUX)/SolarSystem/SolarFly.o: SolarSystem/SolarFly.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c SolarSystem/SolarFly.cpp -o $(OBJDIR_DEBUG_LINUX)/SolarSystem/SolarFly.o

$(OBJDIR_DEBUG_LINUX)/SolarSystem/PlanetsManager.o: SolarSystem/PlanetsManager.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c SolarSystem/PlanetsManager.cpp -o $(OBJDIR_DEBUG_LINUX)/SolarSystem/PlanetsManager.o

$(OBJDIR_DEBUG_LINUX)/main.o: main.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c main.cpp -o $(OBJDIR_DEBUG_LINUX)/main.o

$(OBJDIR_DEBUG_LINUX)/SolarSystem/Planet.o: SolarSystem/Planet.cpp
	$(CXX) $(CFLAGS_DEBUG_LINUX) $(INC_DEBUG_LINUX) -c SolarSystem/Planet.cpp -o $(OBJDIR_DEBUG_LINUX)/SolarSystem/Planet.o

clean_debug_linux: 
	rm -f $(OBJ_DEBUG_LINUX) $(OUT_DEBUG_LINUX)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG_LINUX)/src
	rm -rf $(OBJDIR_DEBUG_LINUX)/src/Image
	rm -rf $(OBJDIR_DEBUG_LINUX)/src/linux
	rm -rf $(OBJDIR_DEBUG_LINUX)/SolarSystem
	rm -rf $(OBJDIR_DEBUG_LINUX)

before_release_linux: 
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE_LINUX)/src || mkdir -p $(OBJDIR_RELEASE_LINUX)/src
	test -d $(OBJDIR_RELEASE_LINUX)/src/Image || mkdir -p $(OBJDIR_RELEASE_LINUX)/src/Image
	test -d $(OBJDIR_RELEASE_LINUX)/src/linux || mkdir -p $(OBJDIR_RELEASE_LINUX)/src/linux
	test -d $(OBJDIR_RELEASE_LINUX)/SolarSystem || mkdir -p $(OBJDIR_RELEASE_LINUX)/SolarSystem
	test -d $(OBJDIR_RELEASE_LINUX) || mkdir -p $(OBJDIR_RELEASE_LINUX)

after_release_linux: 

release_linux: before_release_linux out_release_linux after_release_linux

out_release_linux: before_release_linux $(OBJ_RELEASE_LINUX) $(DEP_RELEASE_LINUX)
	$(LD) $(LIBDIR_RELEASE_LINUX) -o $(OUT_RELEASE_LINUX) $(OBJ_RELEASE_LINUX)  $(LDFLAGS_RELEASE_LINUX) $(LIB_RELEASE_LINUX)

$(OBJDIR_RELEASE_LINUX)/src/EString.o: src/EString.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/EString.cpp -o $(OBJDIR_RELEASE_LINUX)/src/EString.o

$(OBJDIR_RELEASE_LINUX)/src/Object.o: src/Object.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/Object.cpp -o $(OBJDIR_RELEASE_LINUX)/src/Object.o

$(OBJDIR_RELEASE_LINUX)/src/Math2D.o: src/Math2D.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/Math2D.cpp -o $(OBJDIR_RELEASE_LINUX)/src/Math2D.o

$(OBJDIR_RELEASE_LINUX)/src/MainInstance.o: src/MainInstance.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/MainInstance.cpp -o $(OBJDIR_RELEASE_LINUX)/src/MainInstance.o

$(OBJDIR_RELEASE_LINUX)/src/Input.o: src/Input.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/Input.cpp -o $(OBJDIR_RELEASE_LINUX)/src/Input.o

$(OBJDIR_RELEASE_LINUX)/src/Image/lodepng.o: src/Image/lodepng.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/Image/lodepng.cpp -o $(OBJDIR_RELEASE_LINUX)/src/Image/lodepng.o

$(OBJDIR_RELEASE_LINUX)/src/ETime.o: src/ETime.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/ETime.cpp -o $(OBJDIR_RELEASE_LINUX)/src/ETime.o

$(OBJDIR_RELEASE_LINUX)/src/Screen.o: src/Screen.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/Screen.cpp -o $(OBJDIR_RELEASE_LINUX)/src/Screen.o

$(OBJDIR_RELEASE_LINUX)/src/Debug.o: src/Debug.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/Debug.cpp -o $(OBJDIR_RELEASE_LINUX)/src/Debug.o

$(OBJDIR_RELEASE_LINUX)/src/Camera.o: src/Camera.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/Camera.cpp -o $(OBJDIR_RELEASE_LINUX)/src/Camera.o

$(OBJDIR_RELEASE_LINUX)/src/Application.o: src/Application.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/Application.cpp -o $(OBJDIR_RELEASE_LINUX)/src/Application.o

$(OBJDIR_RELEASE_LINUX)/src/linux/OpenGLLinux.o: src/linux/OpenGLLinux.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/linux/OpenGLLinux.cpp -o $(OBJDIR_RELEASE_LINUX)/src/linux/OpenGLLinux.o

$(OBJDIR_RELEASE_LINUX)/src/linux/LinuxScreen.o: src/linux/LinuxScreen.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/linux/LinuxScreen.cpp -o $(OBJDIR_RELEASE_LINUX)/src/linux/LinuxScreen.o

$(OBJDIR_RELEASE_LINUX)/src/linux/LinuxInput.o: src/linux/LinuxInput.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/linux/LinuxInput.cpp -o $(OBJDIR_RELEASE_LINUX)/src/linux/LinuxInput.o

$(OBJDIR_RELEASE_LINUX)/src/linux/LinuxApp.o: src/linux/LinuxApp.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/linux/LinuxApp.cpp -o $(OBJDIR_RELEASE_LINUX)/src/linux/LinuxApp.o

$(OBJDIR_RELEASE_LINUX)/src/Utility.o: src/Utility.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/Utility.cpp -o $(OBJDIR_RELEASE_LINUX)/src/Utility.o

$(OBJDIR_RELEASE_LINUX)/src/Timer.o: src/Timer.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/Timer.cpp -o $(OBJDIR_RELEASE_LINUX)/src/Timer.o

$(OBJDIR_RELEASE_LINUX)/src/Texture.o: src/Texture.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/Texture.cpp -o $(OBJDIR_RELEASE_LINUX)/src/Texture.o

$(OBJDIR_RELEASE_LINUX)/src/Table.o: src/Table.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/Table.cpp -o $(OBJDIR_RELEASE_LINUX)/src/Table.o

$(OBJDIR_RELEASE_LINUX)/src/Shader.o: src/Shader.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c src/Shader.cpp -o $(OBJDIR_RELEASE_LINUX)/src/Shader.o

$(OBJDIR_RELEASE_LINUX)/SolarSystem/SolarSky.o: SolarSystem/SolarSky.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c SolarSystem/SolarSky.cpp -o $(OBJDIR_RELEASE_LINUX)/SolarSystem/SolarSky.o

$(OBJDIR_RELEASE_LINUX)/SolarSystem/SolarRender.o: SolarSystem/SolarRender.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c SolarSystem/SolarRender.cpp -o $(OBJDIR_RELEASE_LINUX)/SolarSystem/SolarRender.o

$(OBJDIR_RELEASE_LINUX)/SolarSystem/SolarFly.o: SolarSystem/SolarFly.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c SolarSystem/SolarFly.cpp -o $(OBJDIR_RELEASE_LINUX)/SolarSystem/SolarFly.o

$(OBJDIR_RELEASE_LINUX)/SolarSystem/PlanetsManager.o: SolarSystem/PlanetsManager.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c SolarSystem/PlanetsManager.cpp -o $(OBJDIR_RELEASE_LINUX)/SolarSystem/PlanetsManager.o

$(OBJDIR_RELEASE_LINUX)/main.o: main.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c main.cpp -o $(OBJDIR_RELEASE_LINUX)/main.o

$(OBJDIR_RELEASE_LINUX)/SolarSystem/Planet.o: SolarSystem/Planet.cpp
	$(CXX) $(CFLAGS_RELEASE_LINUX) $(INC_RELEASE_LINUX) -c SolarSystem/Planet.cpp -o $(OBJDIR_RELEASE_LINUX)/SolarSystem/Planet.o

clean_release_linux: 
	rm -f $(OBJ_RELEASE_LINUX) $(OUT_RELEASE_LINUX)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE_LINUX)/src
	rm -rf $(OBJDIR_RELEASE_LINUX)/src/Image
	rm -rf $(OBJDIR_RELEASE_LINUX)/src/linux
	rm -rf $(OBJDIR_RELEASE_LINUX)/SolarSystem
	rm -rf $(OBJDIR_RELEASE_LINUX)

.PHONY: before_debug_linux after_debug_linux clean_debug_linux before_release_linux after_release_linux clean_release_linux

