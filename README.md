# Minimal and straight forward OpenGL VR example using the Oculus SDK

We use the Oculus Rift SDK to create a minimal example of rendering an image to a VR headset from which it is simple to build more complex programs.

The end result will be a simple image with different colors per eye, which looks like the following:

![Screenshot](https://i.imgur.com/Q9i0yyA.png)


## Build instructions
To build with VS2017 simply call `build.bat` from a development console (x64 Native Tools Command Prompt for VS 2017), or run the appropriate `vcvarsall.bat` file manually before building. 


## Motivation

The OVR SDK contains several examples for how to use it, but the smallest example is far from minimal and quite complex to follow along and not to mention it being bogged down with "architecture" that's reused in all the samples. The file `OculusRoomTiny/OculusRoomTiny (GL)/main.cpp`, which handles logic, is 437 lines, and includes `OculusRoomTiny_Advanced/Common/Win32_GLAppUtil.h` which is 834 lines long! 

This project was made to make it easier for others to get started, striving towards making the absolute minimal example that is still useful. This means it doesn't actually render any scene, as that's exactly the same as a regular renderer (which I assume most readers already know how to do decently well). 


## Notes

To download the latest OVR SDK, go [here](https://developer.oculus.com/downloads/package/oculus-sdk-for-windows/). 

For more info on how to use the SDK, see [here](https://developer.oculus.com/documentation/pcsdk/latest/concepts/book-gsg/), or simply read the header files (they're quite nice!)

The extension to more complex programs is for the most part straight forward in the simplest cases. Simply add regular rendering code in the per-eye loop and react to the controller and hmd state. 

The program is written in a straight forward sequential manner. There are a few comments scattered around to give some context.


## Dependencies
All dependencies are bundled for simplicity. All license notices are supplied in `THIRD_PARTY_NOTICES.txt`. 

	- [GLFW](https://github.com/elmindreda/glfw) (version 3.3.0)
	- [glad](https://github.com/Dav1dde/glad) (4.3 core loader from the web interface, https://github.com/Dav1dde/glad)
	- [The Oculus Software Development Kit](https://developer.oculus.com/downloads/package/oculus-sdk-for-windows/) (version 1.26)

