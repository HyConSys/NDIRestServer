// Some needed libs
#include <csignal>
#include <iostream>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define DLLIMPORT __declspec(dllimport)
#else
    #define DLLIMPORT
#endif

// The NDI Sender Interface
#define NDIImageSender_DLL_IMPORTS
#include "NDIImageSender.h"

// PNG loader in a single file from http://lodev.org/lodepng/ 
#include "picopng.hpp"

static std::atomic<bool> exit_program(false);
static void sigint_handler(int){exit_program = true;}

#define NDI_SENDER_NAME "My_PNG"
int main(int argc, char* argv[])
{	
	// Catch interrupt so that we can shut down gracefully
    signal(SIGINT, sigint_handler);

	// Lets load the file from disk
	std::vector<unsigned char> png1_data;
	std::vector<unsigned char> png2_data;
	loadFile(png1_data, "images/image1.png");
	if (png1_data.empty()) {
		std::cout << "PNG 1 Data Empty." << std::endl;
		return 0;
	}
	loadFile(png2_data, "images/image2.png");
	if (png2_data.empty()) {
		std::cout << "PNG 2 Data Empty." << std::endl;
		return 0;
	}
	
	// Decode the PNG file
	std::vector<unsigned char> image1_data;
	std::vector<unsigned char> image2_data;
	unsigned long xres1 = 0, yres1 = 0;
	unsigned long xres2 = 0, yres2 = 0;
	if (decodePNG(image1_data, xres1, yres1, &png1_data[0], png1_data.size(), true)) {
		printf("Failed to decode the image.\n");
		return 0;
	}
	if (decodePNG(image2_data, xres2, yres2, &png2_data[0], png2_data.size(), true)) {
		printf("Failed to decode the image.\n");
		return 0;
	}	

	// create a sender and start sending
	NDIImageSender* pNDISender = NDIImageSender_create(NDI_SENDER_NAME, 10);
	
	// keep pushing different images
	while(!exit_program){
		NDIImageSender_setImage(pNDISender, image1_data.data(), xres1, yres1);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		NDIImageSender_setImage(pNDISender, image2_data.data(), xres2, yres2);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	std::cout << "Exiting ..." << std::endl << std::flush;
	NDIImageSender_delete(pNDISender);

	// Success
	return 0;
}

