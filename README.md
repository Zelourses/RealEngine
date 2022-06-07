# Real
Game engine that I realistically could create by myself(almost). I'll try to do that it will be capable of creating real-time 3D rendering applications.

## What this thing can do?
On this time, well...  
It's developing, so you don't have big possibilities(for now, I hope).  
You can: 
+ Read logs!
+ Look at demo window of imgui!
+ check what buttons you are pressing!
+ Resizing window!
+ Look at Cubes and ranbow triangle

## How to build it?
By using [PreMake](https://github.com/premake/premake-core)  
`git clone --recursive` this project, then `cd RealEngine`  
And just run `premake <build type>`  
And then you will get project to run.  
Launch `RealEditor` module and it will build everything for you

## Build options
You can build it in 3 modes:
+ `Debug` - for development purposes
+ `Release` - optimization things all on and all debug things are off
+ `Dist` - (or distribution) For now, it's the same as `Release` mode

## Structure
This project has 2 "modules" in it.  
1) `RealEngine` - the engine himself. Static library
2) `RealEditor` - editor, that will help to create app without recompiling C++ code
3) `Sandbox` - here all code that uses test features of engine

## Libraries
For now this engine is using:
+ [spdlog](https://github.com/gabime/spdlog) (great log library)
+ [GLFW](https://github.com/Zelourses/glfw) (my fork of great window creation)
+ [Glad](https://glad.dav1d.de/) (great library for openGL)
+ [ImGUI](https://github.com/Zelourses/imgui) (my fork of great ui lib)
+ [glm](https://github.com/g-truc/glm) (Great math lib)
+ [entt](https://github.com/skypjack/entt/) (Great lib for creating ECS)
