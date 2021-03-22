// only for windows
#ifdef _WIN32
	#include <windows.h>
	#define strncasecmp _strnicmp
	#ifdef _WIN64
		#pragma comment(lib, "Processing.NDI.Lib.x64.lib")
	#else
		#pragma comment(lib, "Processing.NDI.Lib.x86.lib")
	#endif
#endif

// Some needed libs
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

// The NDI Sender Interface
#include "NDIImageSender.h"

// PNG loader in a single file from http://lodev.org/lodepng/ 
#include "picopng.hpp"

static std::atomic<bool> exit_program(false);
static void sigint_handler(int){exit_program = true;}


#define NDSI_SENDER_NAME "My_PNG"
int main(int argc, char* argv[])
{	
	// Catch interrupt so that we can shut down gracefully
    signal(SIGINT, sigint_handler);

	// Lets load the file from disk
	std::vector<unsigned char> png_data;
	loadFile(png_data, "../data/image.png");
	if (png_data.empty()) {
		printf("PNG Data Empty.\n");
		return 0;
	}
	
	// Decode the PNG file
	std::vector<unsigned char> image_data;
	unsigned long xres = 0, yres = 0;
	if (decodePNG(image_data, xres, yres, &png_data[0], png_data.size(), true)) {
		printf("Failed to decode the image.\n");
		return 0;
	}

	// create a sender and start sending
	NDIImageSender myNDISender(NDSI_SENDER_NAME, 10);
	myNDISender.setImage(image_data, xres, yres);
			
	while(!exit_program);
	printf("Exiting ...\n");

	// Success
	return 0;
}

