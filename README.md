# realEngine
Game engine that i realistically could create by myself(almost). I'll try to do that it will be capable of creating real-time 3D rendering applications.

## What this thing can do?
On this time, well...  
It's initial building stage, so you don't have big possibilities.  
You can: 
+ write logs
+ call event system

## How to build it?
Of course by using precious [PreMake](https://github.com/premake/premake-core)  
`git pull` this project, then `cd ./RealEngine`  
And just run `premake <build type>`  
And then you can run this thing

## Structure
This project has 2 "modules" in it.  
1) `RealEngine` - the engine himself, works like dynamic library
2) `Sandbox` - project that will use the engine, more like Editor

## Libraries
For now this engine is using:
+ [spdlog](https://github.com/gabime/spdlog)