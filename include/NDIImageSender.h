#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

// The NDI SDK
#include <Processing.NDI.Lib.h>


// The NDIImageSender class
class NDIImageSender {
public:
    // Data shared between threads
    // -----------------------------
    std::atomic_flag lockAccessToLocalData = ATOMIC_FLAG_INIT;
    bool isImageInitialized = false;
    bool hasImageChanged = false;
    bool needsToExit = false;
    std::vector<unsigned char> imageData;
    int imageXres;
    int imageYres;
    // -----------------------------
    // End of data shared between threads

    size_t sendPeriodMs;
    NDIlib_send_instance_t pNDISender;
    NDIlib_send_create_t NDISendCreateDesc;
    std::string senderName;
    std::thread senderThread;

    NDIImageSender(const std::string& l_senderName, size_t l_sendPeriodMs);
    ~NDIImageSender();

    // expected image buffer: RGBA 32-bit (8 bit per channel) color format 
    void setImage(const std::vector<unsigned char>& l_imageData, int xres, int yres);
};


// if requested, you get the DLL Imports
#ifdef NDIImageSender_DLL_IMPORTS
extern "C" DLLIMPORT NDIImageSender* NDIImageSender_create(const char* l_senderName, size_t l_sendPeriodMs);
extern "C" DLLIMPORT void NDIImageSender_delete(NDIImageSender* instance);
extern "C" DLLIMPORT void NDIImageSender_setImage(NDIImageSender* instance, const unsigned char* l_imageData, int xres, int yres);
#endif