# Real
Game engine that i realistically could create by myself(almost). I'll try to do that it will be capable of creating real-time 3D rendering applications.

## What this thing can do?
On this time, well...  
It's developing, so you don't have big possibilities(for now, i hope).  
You can: 
+ Read logs!
+ Look at demo window of imgui!
+ check what buttons you are pressing!
+ Resizing window!
+ Look at Cubes and ranbow triangle

## How to build it?
Of course by using precious [PreMake](https://github.com/premake/premake-core)  
`git clone --recursive` this project, then `cd RealEngine`  
And just run `premake <build type>`  
And then you will get project to run.  
Launch `Sandbox` module and it will build everything for you

## Build options
You can build it in 3 modes:
+ `Debug` - for development purposes
+ `Release` - optimization things all on and all debug things are off
+ `Dist` - (or distribution) For now, it's the same as `Release` mode

## Structure
This project has 2 "modules" in it.  
1) `RealEngine` - the engine himself. Static library
2) `Sandbox` - project that will use the engine, more like Editor

## Libraries
For now this engine is using:
+ [spdlog](https://github.com/gabime/spdlog) (great log library)
+ [GLFW](https://github.com/Zelourses/glfw) (my fork)
+ [Glad](https://glad.dav1d.de/) (great thing)
+ [ImGUI](https://github.com/Zelourses/imgui) (my fork)
+ [glm](https://github.com/g-truc/glm) (Great math lib)