# Minimal and straight forward OpenGL VR code using the OVR SDK

We use the Oculus Rift SDK to create a minimal example of rendering an image to a VR headset. The end result will be a simple image with different colors per eye, which looks like the following:

[https://i.imgur.com/Q9i0yyA.png](Image)


## Build instructions
To build with VS2017 simply call `build.bat` from a development console (x64 Native Tools Command Prompt for VS 2017), or run the appropriate `vcvarsall.bat` file manually before building. 


## Dependencies
All dependencies are bundled for simplicity. All license notices are supplied in `THIRD_PARTY_NOTICES.txt`. 

	- GLFW 
	- glad
	- The Oculus Software Development Kit (OVR)
