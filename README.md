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
+ [spdlog](https://github.com/gabime/spdlog) - great log library
+ [GLFW](https://github.com/Zelourses/glfw) - my fork of great window creation
+ [Glad](https://glad.dav1d.de/) - great library for openGL
+ [ImGUI](https://github.com/Zelourses/imgui) - my fork of great UI lib
+ [glm](https://github.com/g-truc/glm) - great math lib
+ [entt](https://github.com/skypjack/entt/) - great lib for working with ECS
+ [yaml-cpp](https://github.com/Zelourses/yaml-cpp) - my fork of very good library for work with yaml files (why not TOML? see [why](#choosing-serialization-format))

### Choosing serialization format

When I first encounter the problem of serialization, I knew that there is a very interesting format named TOML, JSON, YAML and XML. For my project I wanted something not so verbose as XML and even JSON, because there is a lot of brackets for more comfortable parsing, not for humans. Also JSON doesn't support comments inside files, that makes it not as good as it looks at first glance. So, after that I had only YAML and TOML. They are pretty similar languages, but syntax in TOML looked a little bit better than in YAML (IMO). I tried to found some toml-parsing libraries and one of the best was [toml++](https://github.com/marzer/tomlplusplus). It was not so verbose as toml11 (seriously, why it's literally like C api? Why not like functions?), and it supported C++17 with some C++20 features. But after some work on serializer I encountered that this library doesn't preserve original addition order (more [here](https://github.com/marzer/tomlplusplus/issues/62)). That means that all things that will be added here lay there "somehow", and order of comments will be also almost random. So, I had only 2 ways to go: write my own toml parser or choose YAML. Because of that, now I am using yaml-cpp because it's very easy to use by syntax of state machine that you switch by one line of code.

