# 3D Graphics and Game Engine

[![Language](https://img.shields.io/badge/Language-C++-blue.svg?style=flat-square)]()
[![DirectX11](https://img.shields.io/badge/DirectX-11-red.svg?style=flat-square)]()

[![Windows-10](https://img.shields.io/badge/windows-10-blue.svg?style=flat-square)]()
[![Windows-8.1](https://img.shields.io/badge/windows-8.1-orange.svg?style=flat-square)]()
[![Windows-8](https://img.shields.io/badge/windows-8-green.svg?style=flat-square)]()
[![Windows-7](https://img.shields.io/badge/windows-7-yellow.svg?style=flat-square)]()


## Description
Fast and open source 3D Graphics and Game Engine written in C++ and DirectX 11 currently a work in progress, some features are complete while others are still WIP and others are highly unstable

## Features
- Actors and Components based architecture
- Prefabs System (for components and actors re-usability)
- High Level Rendering Manager
- Complete Material System
- 3d Models Import System using the great [Assimp library](https://github.com/assimp/assimp)
- UI using the awesome [ImGui library](https://github.com/ocornut/imgui)
- Interactive level editor using the own engine rendering pipeline
- XML Serialization and DeSerialization using the [TinyXml library](https://github.com/leethomason/tinyxml2)
- Performance and profiling support using custom built profiling manager
- Configurable engine parameters using external configuration files
- Events Management and dispatching system for communication between the engine's systems, the components and the actors
- Resources Management and on-demand loading using custom built Resource Cache and Handles
- Easy Packages Management using Microsoft's [vcpkg](https://github.com/Microsoft/vcpkg) Package Manager
- Rendering Features:
  - Diffuse textures
  - Normal map textures
  - Specular maps textures
  - Real time shadows using shadow mapping technology
  - Directional lights
  - Point lights
  - Spot lights
  - Occlusion Culling (for limiting the number of rendered objects)
  - High level management for graphics objects and their lifespan (Buffers, Shaders, etc...)
  - Offline Render Targets Support
  - Multi-Viewport Support

## Installation
- start with cloning this repo
```
git clone https://github.com/Feras94/GraphicsEngine.git GraphicsEngine
```
- clone and setup vcpkg ([See how to clone and compile the tool](https://github.com/Microsoft/vcpkg#quick-start))

- setup global package installation for vcpkg (requires admin account)
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

you can find example asset files in the bin folder (bin\Assets), More usage instruction will be posted soon!

## Contributing
Feel free to clone and fork the project and send pull requests or submit issues and we'll work on them as soon as possible!
Any contributions are more than welcome!

## Credits
This project is the result of the hard work and devotion of:
- [Feras Dawod](https://github.com/Feras94)
- Rasha Malandi