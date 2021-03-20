#include <csignal>
#include <cstddef>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <atomic>
#include <stdlib.h>
#include <chrono>
#include <thread>

#include <Processing.NDI.Lib.h>

#ifdef _WIN32
	#include <windows.h>
	#define strncasecmp _strnicmp
	#ifdef _WIN64
		#pragma comment(lib, "Processing.NDI.Lib.x64.lib")
	#else
		#pragma comment(lib, "Processing.NDI.Lib.x86.lib")
	#endif
	#include <strings.h>
#endif

// PNG loader in a single file from http://lodev.org/lodepng/ 
#include "picopng.hpp"

static std::atomic<bool> exit_loop(false);
static void sigint_handler(int){exit_loop = true;}

int main(int argc, char* argv[])
{	
	// Catch interrupt so that we can shut down gracefully
    signal(SIGINT, sigint_handler);

	// Not required, but "correct" (see the SDK documentation.
	if (!NDIlib_initialize())
	{	// Cannot run NDI. Most likely because the CPU is not sufficient (see SDK documentation).
		// you can check this directly with a call to NDIlib_is_supported_CPU()
		printf("Cannot run NDI.\n");
		return 0;
	}	

	// Lets load the file from disk
	std::vector<unsigned char> png_data;
	loadFile(png_data, "image.png");
	if (png_data.empty()) {
		printf("PNG Data Empty.\n");
		return 0;
	}
	
	// Decode the PNG file
	std::vector<unsigned char> image_data;
	unsigned long xres = 0, yres = 0;
	if (decodePNG(image_data, xres, yres, &png_data[0], png_data.size(), true)) return 0;
		
	// Create an NDI source that is called "My_PNG" and is clocked to the video.
	NDIlib_send_create_t NDI_send_create_desc;
	NDI_send_create_desc.p_ndi_name = "My_PNG";

	// We create the NDI sender
	NDIlib_send_instance_t pNDI_send = NDIlib_send_create(&NDI_send_create_desc);
	if (!pNDI_send) return 0;

	// We are going to create a frame
	NDIlib_video_frame_v2_t NDI_video_frame;
	NDI_video_frame.xres = xres;
	NDI_video_frame.yres = yres;
	NDI_video_frame.FourCC = NDIlib_FourCC_type_RGBA;
	NDI_video_frame.p_data = &image_data[0];
	NDI_video_frame.line_stride_in_bytes = xres * 4;

	// We keep submiting the same frame. 
	printf("Sending the image over and over ...\n");
	for (int idx = 0; !exit_loop; idx++)
		NDIlib_send_send_video_v2(pNDI_send, &NDI_video_frame);

	// Destroy the NDI sender
	NDIlib_send_destroy(pNDI_send);
	
	// Not required, but nice
	NDIlib_destroy();

	// Success
	return 0;
}

