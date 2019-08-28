# 3D Playground

3D Playground consists of a library component *3DEngine* which holds components to build a graphical 3D application and an executable *Graphics Playground* which creates a window, loads a scene and connects mouse and keyboard inputs. As the name suggests, this is a playground to experment with 3D rendering concepts.

The engine relies modern OpenGL 4 approach with GLSL shaders to leverage features of modern graphics cards in order to achieve high realism while maintaining interactive frame rates.

## Features

* Loading of complex scenes and different model / image formats
* Very basic animation capabilities
* Multiple camera models
* Bounding volume hierarchy
* Mouse & keyboard interactivity
* Platform independent
* Supports Wavefront Obj files
* Leverages C++ 17 

### Rendering features
* Illumination from various light types (directional, spot, point, SH)
* Shadow mapping with PCF
* Multi-texturing
* Environment mapping
* Spherical Harmonics lighting

## Impressions

![Car9](https://user-images.githubusercontent.com/54601848/63808614-aa9c0980-c920-11e9-9471-32a5d2644bf6.png)

![Dragon](https://user-images.githubusercontent.com/54601848/63808650-bd164300-c920-11e9-8a4b-53430ff71c24.png)

![heli](https://user-images.githubusercontent.com/54601848/63808756-f5b61c80-c920-11e9-8495-e8752f49c64a.png)

## Prerequisites

This project builds out of the box with [Microsoft Visual Studio Community Edition](https://visualstudio.microsoft.com). However, it is possible to build it with different compilers and for different platforms, since the code is platform-independent. See section 'Improvements'.

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