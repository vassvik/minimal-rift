// Minimal and straight forward OpenGL OVR Example
//
// For more info, see https://developer.oculus.com/documentation/pcsdk/latest/concepts/book-dg/

//
#include <stdio.h>

// 
#include <glad/glad.c>
#include <GLFW/glfw3.h>

//
#include <OVR/OVR_CAPI_GL.h> // includes OVR_CAPI.h

// Use discrete GPU by default.
extern "C" {
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

// error callbacks
void print_last_rift_error() {
    ovrErrorInfo error_info; ;
    ovr_GetLastErrorInfo(&error_info);
    printf("OVR Error %d: '%s'\n", error_info.Result, error_info.ErrorString);
}

void glfw_error_callback(int error, const char *desc) {
	printf("GLFW Error %d: '%s'\n", error, desc);
}

// debug helpers
void print_hmd_desc(ovrHmdDesc *hmd_desc) {
	printf("Type                  = %d\n", hmd_desc->Type);
	printf("ProductName           = %s\n", hmd_desc->ProductName);
	printf("Manufacturer          = %s\n", hmd_desc->Manufacturer);
	printf("VendorId              = %d\n", hmd_desc->VendorId);
	printf("ProductId             = %d\n", hmd_desc->ProductId);
	printf("SerialNumber          = %s\n", hmd_desc->SerialNumber);
	printf("FirmwareMajor         = %d\n", hmd_desc->FirmwareMajor);
	printf("FirmwareMinor         = %d\n", hmd_desc->FirmwareMinor);
	printf("AvailableHmdCaps      = %d\n", hmd_desc->AvailableHmdCaps);
	printf("DefaultHmdCaps        = %d\n", hmd_desc->DefaultHmdCaps);
	printf("AvailableTrackingCaps = %d\n", hmd_desc->AvailableTrackingCaps);
	printf("DefaultTrackingCaps   = %d\n", hmd_desc->DefaultTrackingCaps);
	for (int i = 0; i < ovrEye_Count; i++) {
		printf("DefaultEyeFov[%d]      = {%f, %f, %f, %f}\n", i, hmd_desc->DefaultEyeFov[i].UpTan, hmd_desc->DefaultEyeFov[i].DownTan, hmd_desc->DefaultEyeFov[i].LeftTan, hmd_desc->DefaultEyeFov[i].RightTan);
		printf("MaxEyeFov[%d]          = {%f, %f, %f, %f}\n", i, hmd_desc->MaxEyeFov[i].UpTan, hmd_desc->MaxEyeFov[i].DownTan, hmd_desc->MaxEyeFov[i].LeftTan, hmd_desc->MaxEyeFov[i].RightTan);
	}
	printf("Resolution            = {%d, %d}\n", hmd_desc->Resolution.w, hmd_desc->Resolution.h);
	printf("DisplayRefreshRate    = %f\n", hmd_desc->DisplayRefreshRate);
}

int main() {
	// See https://developer.oculus.com/documentation/pcsdk/latest/concepts/dg-sensor/#dg_sensor

	//
	if (!OVR_SUCCESS(ovr_Initialize(nullptr))) {
        print_last_rift_error();
        goto CLEANUP0;
    }
    printf("Initialized OVR\n");

    //
    ovrSession session;
    ovrGraphicsLuid luid;
    if (!OVR_SUCCESS(ovr_Create(&session, &luid))) {
        print_last_rift_error();
        goto CLEANUP1;
    }
    printf("Created VR session\n");

    //
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
    	goto CLEANUP2;
    }
    printf("Initialized GLFW\n");

    const char *title = "Minimal OpenGL OVR";
    int resx = 1280, resy = 720;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(resx, resy, title, nullptr, nullptr);
    if (!window) {
    	goto CLEANUP3;
    }
    printf("Created GLFW window\n");

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    //
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("glad error: Could not load OpenGL functions\n");
        goto CLEANUP3;
    }
    printf("Loaded OpenGL functions\n");

    // See // https://developer.oculus.com/documentation/pcsdk/latest/concepts/dg-render/#dg-render-initialize
    ovrHmdDesc hmd_desc = ovr_GetHmdDesc(session);
	if (0) print_hmd_desc(&hmd_desc);

    // Create texture swap chains
    ovrSizei texture_eye_sizes[ovrEye_Count];
    ovrTextureSwapChain texture_swap_chains[ovrEye_Count];
    for (int eye = 0; eye < ovrEye_Count; eye++) {
    	// 
        texture_eye_sizes[eye] = ovr_GetFovTextureSize(session, ovrEyeType(eye), hmd_desc.DefaultEyeFov[eye], 1.0);

        //
        ovrTextureSwapChainDesc desc = {
        	ovrTexture_2D,                  // Type
        	OVR_FORMAT_R8G8B8A8_UNORM_SRGB, // Format
        	1,                              // ArraySize
        	texture_eye_sizes[eye].w,       // Width
        	texture_eye_sizes[eye].h,       // Height
        	1,                              // MipLevels
        	1,                              // SampleCount
        	ovrFalse,                       // StaticImage
        	ovrTextureMisc_None,            // MiscFlags
        	ovrTextureBind_None             // BindFlags
        };
        if (!OVR_SUCCESS(ovr_CreateTextureSwapChainGL(session, &desc, &texture_swap_chains[eye]))) {
            print_last_rift_error();
            goto CLEANUP4;
        }

        int length;
        ovr_GetTextureSwapChainLength(session, texture_swap_chains[eye], &length);
        for (int i = 0; i < length; i++) {
            unsigned int chain_tex_id;
            if (!OVR_SUCCESS(ovr_GetTextureSwapChainBufferGL(session, texture_swap_chains[eye], i, &chain_tex_id))) {
                print_last_rift_error();
                goto CLEANUP4;
            }
            glBindTexture(GL_TEXTURE_2D, chain_tex_id);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
    }
    printf("Created texture swap chains\n");

    // We want to mirror to a window on the screen
    ovrMirrorTexture mirror_texture_ovr;
    ovrMirrorTextureDesc mirror_desc = {
	    OVR_FORMAT_R8G8B8A8_UNORM_SRGB, // Format
		resx,                           // Width
		resy,                           // Height
		ovrTextureMisc_None,            // MiscFlags
		0                               // MirrorOptions
    }; 
    if (!OVR_SUCCESS(ovr_CreateMirrorTextureGL(session, &mirror_desc, &mirror_texture_ovr))) { 
        print_last_rift_error();
        goto CLEANUP4;
    }

    unsigned int mirror_texture_gl;
    if (!OVR_SUCCESS(ovr_GetMirrorTextureBufferGL(session, mirror_texture_ovr, &mirror_texture_gl))) {
        print_last_rift_error();
        goto CLEANUP5;
    }
    printf("Created mirror texture\n");

    // Setup framebufers
    unsigned int mirror_fbo;
    glGenFramebuffers(1, &mirror_fbo);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mirror_fbo);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirror_texture_gl, 0);
    glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    printf("Created mirror framebuffer\n");

    unsigned int per_eye_fbo[ovrEye_Count];
    glGenFramebuffers(ovrEye_Count, &per_eye_fbo[0]);
    printf("Created per-eye framebuffers\n");

    // Set up "layers"
    // See: https://developer.oculus.com/documentation/pcsdk/latest/concepts/dg-render/#dg-render-layers
    ovrLayerEyeFov layer = {
        {   // Header
            ovrLayerType_EyeFov, 
            ovrLayerFlag_TextureOriginAtBottomLeft
        }, 
        {   // ColorTexture[ovrEye_Count]
            texture_swap_chains[0], 
            texture_swap_chains[1]
        },
        {   // Viewport[ovrEye_Count]
            {0, 0, texture_eye_sizes[0].w, texture_eye_sizes[0].h}, 
            {0, 0, texture_eye_sizes[1].w, texture_eye_sizes[1].h}
        },
        {   // Fov[ovrEye_Count]
            hmd_desc.MaxEyeFov[0], 
            hmd_desc.MaxEyeFov[1]
        }
        // RenderPose and SensorSampleTime set in the render loop
    };

    // Render descriptions, constant as long as the FOV doesn't change. 
    // See https://developer.oculus.com/documentation/pcsdk/latest/concepts/dg-render/#dg-render-frame
    ovrEyeRenderDesc eye_render_desc[ovrEye_Count] = {
        ovr_GetRenderDesc(session, ovrEye_Left, hmd_desc.DefaultEyeFov[0]), 
        ovr_GetRenderDesc(session, ovrEye_Right, hmd_desc.DefaultEyeFov[1])
    };

    // The eye offsets are used to construct view matrices for each eye.
    ovrPosef hmd_to_eye_pose[ovrEye_Count] = { eye_render_desc[0].HmdToEyePose, eye_render_desc[1].HmdToEyePose };
    printf("Set up default layer and relative pose of each eye\n");

    // We want sRGB to match the swapchain format 
    glEnable(GL_FRAMEBUFFER_SRGB);

    // 
    printf("Starting main loop\n"); 
    fflush(stdout);

    unsigned long long int frame_index = 0;
    while (!glfwWindowShouldClose(window)) {
    	// non-VR input
    	glfwPollEvents();
    	
    	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    		glfwSetWindowShouldClose(window, GLFW_TRUE);
    	}

    	// get the latest hmd (and possibly controller) data
        // See: https://developer.oculus.com/documentation/pcsdk/latest/concepts/dg-sensor/#dg-sensor-head-tracking
        ovr_GetEyePoses(session, frame_index, ovrTrue, &hmd_to_eye_pose[0], &layer.RenderPose[0], &layer.SensorSampleTime);
        ovrTrackingState ts = ovr_GetTrackingState(session, 0, 1);

        // render once for each eye
        for (int eye = 0; eye < ovrEye_Count; eye++) {
            // grab the next entry in the swap chain and bind it
            int current_index;
            ovr_GetTextureSwapChainCurrentIndex(session, texture_swap_chains[eye], &current_index);

            unsigned int current_texture_id;
            ovr_GetTextureSwapChainBufferGL(session, texture_swap_chains[eye], current_index, &current_texture_id);

            glBindFramebuffer(GL_FRAMEBUFFER, per_eye_fbo[eye]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, current_texture_id, 0);
            
            // Render normally here
            glViewport(0, 0, texture_eye_sizes[eye].w, texture_eye_sizes[eye].h);
            glClearColor(eye, 0.0, 1.0 - eye, 1.0); // left = blue, right = red
            glClear(GL_COLOR_BUFFER_BIT);

            // 
            ovr_CommitTextureSwapChain(session, texture_swap_chains[eye]);
        }

        // submit the frame
        ovrLayerHeader *layer_header = &layer.Header;
        if (!OVR_SUCCESS(ovr_SubmitFrame(session, frame_index, nullptr, &layer_header, 1))) {
            print_last_rift_error();
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

    	// blit to mirror framebuffer (to show a copy on a normal window)
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mirror_fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, resy, resx, 0,   0, 0, resx, resy,   GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    	// 
    	glfwSwapBuffers(window);
    	frame_index++;
    }


    // Gotos to simplify cleanup on error instead of making wrappers or have large indentation levels...
CLEANUP5:
	ovr_DestroyMirrorTexture(session, mirror_texture_ovr);

CLEANUP4:
    for (int i = 0; i < ovrEye_Count; i++) {
    	if (texture_swap_chains[i]) ovr_DestroyTextureSwapChain(session, texture_swap_chains[i]);
    }
    
CLEANUP3:
    glfwTerminate();

CLEANUP2:
    ovr_Destroy(session);

CLEANUP1:
    ovr_Shutdown();

CLEANUP0:

    //
    return 0;
}