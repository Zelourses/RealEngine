# realEngine
Game engine that i realistically could create by myself(almost). I'll try to do that it will be capable of creating real-time 3D rendering applications.

## What this thing can do?
On this time, well...  
It's developing, so you don't have big possibilities(for now, i hope).  
You can: 
+ write logs
+ call event system
+ look at one-color non-closing window

## How to build it?
Of course by using precious [PreMake](https://github.com/premake/premake-core)  
`git clone --recursive` this project, then `cd RealEngine`  
And just run `premake <build type>`  
And then you will get project to run.  
Launch `Sandbox` module and it will build everything for you

## Structure
This project has 2 "modules" in it.  
1) `RealEngine` - the engine himself, works like dynamic library
2) `Sandbox` - project that will use the engine, more like Editor

## Libraries
For now this engine is using:
+ [spdlog](https://github.com/gabime/spdlog)
+ My forked [GLFW](https://github.com/Zelourses/glfw) with `premake5.lua` inside