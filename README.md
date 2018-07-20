# Minimal and straightforward OpenGL VR example using the Oculus SDK

We use the Oculus Rift SDK to create a minimal example of rendering a simple image to a VR headset from which it is easy to build more complex programs.

The end result will be a simple image with different colors per eye, which looks like the following:

![Screenshot](https://i.imgur.com/Q9i0yyA.png)


## Build instructions
To build with VS2017 simply call `build.bat` from a development console (x64 Native Tools Command Prompt for VS 2017), or run the appropriate `vcvarsall.bat` file manually before building. 


## Motivation

The OVR SDK contains several examples for how to use it, but the smallest example is far from minimal and quite complex to follow along and not to mention bogged down with "architecture" that's reused in the other samples. 

The file `OculusRoomTiny/OculusRoomTiny (GL)/main.cpp`, which handles logic, is 437 lines, and includes `OculusRoomTiny_Advanced/Common/Win32_GLAppUtil.h` which is 834 lines long! 

This project was made to make it easier for others to get started, striving towards making the absolute minimal example that is still useful. This means it doesn't actually render any scene, as that's (for the most parT) exactly the same as a regular renderer (which I assume most readers are already familiar with). 


## Extensions

The total line count is just short of 400 lines, consisting mostly of boilerplate, which means the extension to more complex programs is straight forward - simply render as normal inside the per-eye loop and react to the hmd and controller state. 

The program is written in a straightforward sequential manner, which I hope is instructive. There are a few comments scattered around to give some context and links for further reading.

For extending the program, consider looking at `ovr_GetEyePoses` for pose information (position and orientation) for the HMD, `ovr_GetTrackingState` for pose information for the controllers, and `ovr_GetInputState` to get controller input state (buttons, sticks). The orientations are regular quaternions. See `ovrMatrix4f_Projection` to get the perspective projection matrices per eye, and consider using the quaternions directly instead of a view matrix. 

A simple vertex shader might look something like this: 
```glsl
#version 430 core

// vertex attribs
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

// passthrough
out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

// uniforms
// p = position, q = quaternion
uniform vec3 p_hmd;
uniform vec4 q_hmd;

uniform vec4 q_model;
uniform vec3 p_model;

uniform mat4 P; // perspective projection matrix, per-eye

// quaternion helpers
vec3 qrot(vec3 p, vec4 q) { 
    // http://www.geeks3d.com/20141201/how-to-rotate-a-vertex-by-a-quaternion-in-glsl/
    return p + 2.0 * cross(q.xyz, cross(q.xyz, p) + q.w * p);
}

vec4 qconj(vec4 q) { 
    return vec4(-q.xyz, q.w); 
}

void main() {
    // model space
    vec3 v = vertexPosition;

    // model to world space (e.g. controller models), 
    // first apply rotation in model-space then translate
    v = qrot(v, q_model) + p_model;

    // passthrough to fragment shader, in world space
    TexCoords = vertexUV;
    Normal = qrot(vertexNormal, q_model);
    WorldPos = v;

    // world space to camera space, 
    // first move the camera to the origin then rotate
    v = qrot(v - p_hmd, qconj(q_hmd));

    // output, view space to clip space
    gl_Position = P*vec4(v, 1.0);
}
```


## Notes

To download the latest OVR SDK and samples, go [here](https://developer.oculus.com/downloads/package/oculus-sdk-for-windows/). 

For more info on how to use the SDK, see [here](https://developer.oculus.com/documentation/pcsdk/latest/concepts/book-gsg/), or simply read the SDK header files (they're quite nice!)


## Dependencies

The dependencies of this project are [GLFW](https://github.com/elmindreda/glfw) version 3.3.0, [glad](https://github.com/Dav1dde/glad) 4.3 core loader from the [web interface](https://github.com/Dav1dde/glad), and the [The Oculus Software Development Kit](https://developer.oculus.com/downloads/package/oculus-sdk-for-windows/) version 1.26. 

All dependencies are bundled for simplicity. 

All license notices are supplied in `THIRD_PARTY_NOTICES.txt`. 

