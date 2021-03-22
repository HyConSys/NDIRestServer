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
	std::vector<unsigned char> png1_data;
	std::vector<unsigned char> png2_data;
	loadFile(png1_data, "../data/image1.png");
	if (png1_data.empty()) {
		printf("PNG 1 Data Empty.\n");
		return 0;
	}
	loadFile(png2_data, "../data/image2.png");
	if (png2_data.empty()) {
		printf("PNG 2 Data Empty.\n");
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
	NDIImageSender myNDISender(NDSI_SENDER_NAME, 10);
	
			
	while(!exit_program){
		myNDISender.setImage(image1_data, xres2, yres2);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		myNDISender.setImage(image2_data, xres2, yres2);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	printf("Exiting ...\n");

	// Success
	return 0;
}

