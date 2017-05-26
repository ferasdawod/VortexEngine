# 3D Graphics and Game Engine

## Description
Fast and open source 3D Graphics and Game Engine written in C++ and DirectX 11 currenly a work in proggress, some features are complete while others are still WIP and others are highly unstable

## Features
- Actors and Componenets based architecture
- Prefabs System (for components and actors re-usability)
- High Level Rendering Manager
- Complete Material System
- 3d Models Import System using the great [Assimp library](https://github.com/assimp/assimp)
- UI using the awesome [ImGui library](https://github.com/ocornut/imgui)
- Interactive level editor using the own engine rendering pipeline
- XML Serialization and DeSerialization using the [TinyXml library](https://github.com/leethomason/tinyxml2)
- Performance and profiling support using custom built profiling manager
- Configurable engine paramaters using external config files
- Events Managment and dispatching system for communication between the engine's systems, the componenets and the actors
- Resources Managment and on-demand loading using custom built Resource Cache and Handles
- Easy Packages Managment using Microsoft's [vcpkg](https://github.com/Microsoft/vcpkg)
- Rendering Features:
  - Diffuse textures
  - Normalmap textures
  - Specular maps textures
  - Realtime shadows using shadow mapping technology
  - Directional lights
  - Point lights
  - Spot lights
  - Occlusion Culling (for limiting the number of rendered objects)
  - High level managment for graphics objects and their lifespan (Buffers, Shaders, etc...)
  - Offline Render Targets Support
  - Multi-Viewport Support

## Installation
- start with cloning this repo
```git
git clone https://github.com/Feras94/GraphicsEngine.git GraphicsEngine
```
- clone and setup vcpkg ([See how to clone and compile the tool](https://github.com/Microsoft/vcpkg#quick-start))

- setup global package installation for vcpjg (requires admin account)
```
vcpkg integrate install
```

- install the project dependencies through vcpkg
```
vcpkg install assimp
vcpkg install directxtk
```

- open the included visual studio solution and start coding something great

## Usage
the included project comes with a demo project showing how to reference the engine code and also how to setup a basic scene
**currenlty the Assets folder needed to launch the demo application is missing from git and will be pushed as soon as possible**
More usage instruction will be posted soon!

## Contributing
Feel free to clone and fork the project and send pull requests or submit issues and we'll work on them as soon as possible!
Any contributions are more than welcome!

## Credits
This project is the result of the hard work and devotion of:
- [Feras Dawod](https://github.com/Feras94)
- Rasha Malandi