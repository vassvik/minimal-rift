# Minimal and straight forward OpenGL VR example using the Oculus SDK

We use the Oculus Rift SDK to create a minimal example of rendering an image to a VR headset from which it is simple to build more complex programs.

The end result will be a simple image with different colors per eye, which looks like the following:

![Screenshot](https://i.imgur.com/Q9i0yyA.png)


## Build instructions
To build with VS2017 simply call `build.bat` from a development console (x64 Native Tools Command Prompt for VS 2017), or run the appropriate `vcvarsall.bat` file manually before building. 


## Dependencies
All dependencies are bundled for simplicity. All license notices are supplied in `THIRD_PARTY_NOTICES.txt`. 

	- [GLFW](https://github.com/elmindreda/glfw) (version 3.3.0)
	- [glad](https://github.com/Dav1dde/glad) (4.3 core loader from the web interface, https://github.com/Dav1dde/glad)
	- [The Oculus Software Development Kit](https://developer.oculus.com/downloads/package/oculus-sdk-for-windows/) (version 1.26)


## Notes

To download the latest OVR SDK, go [here](https://developer.oculus.com/downloads/package/oculus-sdk-for-windows/).

For more info on how to use the SDK, see [here](https://developer.oculus.com/documentation/pcsdk/latest/concepts/book-gsg/), or simply read the header files (they're quite nice!)



