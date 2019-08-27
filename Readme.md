# 3D Playground

## Features
* Modern OpenGL rendering
* Leverages C++ 11

## Prerequisites

This project builds out of the box with [Microsoft Visual Studio Community Edition](https://visualstudio.microsoft.com). However it is possible to build it with different tools and for different platforms, since the code is platform-independent. See section 'Improvements'.

### Dependencies 

This project uses [vcpkg](https://github.com/Microsoft/vcpkg) to manage dependencies. Install it and install the Visual Studio integration with 
```vcpkg integrate install```

These libraries are used in the project:
* [GLFW3](https://www.glfw.org) for windows and input
* [glbinding](https://glbinding.org) for typseafe OpenGL bindings
* [TinyXML2](https://github.com/leethomason/tinyxml2) for XML parsing
* [glm](https://glm.g-truc.net) for OpenGL mathematics
* [DevIL](https://github.com/DentonW/DevIL) to load images

Install the dependencies with 
```vcpkg install glload:x64-windows tinyxml2:x64-windows glm:x64-windows devil:x64-windows glfw3:x64-windows```


## Running the application

Set the working dir to the root of the data folder and run:
```Graphics Playground.exe" [scene.xml]```

## License
This project is licensed under the [GNU Lesser General Public License version 3](https://opensource.org/licenses/LGPL-3.0). See LICENSE file in the project root

## Possible Improvements

* Use CMake to be independent from specific build tools
### Known issues:
* You might have to copy lcms2.dll from your vcpkg/packages/lcms/bin folder to the x64/Debug folder
* Rendering artifacts on Intel HD Graphics 4600