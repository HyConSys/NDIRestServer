#include "NDIImageSender.h"


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define DLLEXPORT __declspec(dllexport)
#else
    #define DLLEXPORT
#endif

void image_sender(NDIImageSender* thisSender){
    
    // a video frame to contain the iamge
    NDIlib_video_frame_v2_t NDIVideoFrame;

    while(true){
        
        while (thisSender->lockAccessToLocalData.test_and_set(std::memory_order_acquire));
        // should i update the image ?
        if(thisSender->hasImageChanged){
            NDIVideoFrame.xres = thisSender->imageXres;
            NDIVideoFrame.yres = thisSender->imageYres;
            NDIVideoFrame.FourCC = NDIlib_FourCC_type_RGBA;
            NDIVideoFrame.p_data = &thisSender->imageData[0];
            NDIVideoFrame.line_stride_in_bytes = thisSender->imageXres * 4;
            thisSender->hasImageChanged = false;
        }
        // ready for sedning ?
        if(thisSender->isImageInitialized)
            NDIlib_send_send_video_v2(thisSender->pNDISender, &NDIVideoFrame);
        thisSender->lockAccessToLocalData.clear(std::memory_order_release);

        // wait for some time
        std::this_thread::sleep_for(std::chrono::milliseconds(thisSender->sendPeriodMs));

        // should i exit ?
        while (thisSender->lockAccessToLocalData.test_and_set(std::memory_order_acquire));
        if(thisSender->needsToExit){
            thisSender->lockAccessToLocalData.clear(std::memory_order_release);
            break;    
        }
        thisSender->lockAccessToLocalData.clear(std::memory_order_release);
    }
}


NDIImageSender::NDIImageSender(const std::string& l_senderName, size_t l_sendPeriodMs){
	
    // Not required, but "correct" (see the SDK documentation.
    // if error happens, this is most likely because the CPU is not sufficient (see SDK documentation).
    // you can check this directly with a call to NDIlib_is_supported_CPU()
	if (!NDIlib_initialize())
        throw std::runtime_error("NDIImageSender::NDIImageSender: Cannot run NDI.");

    sendPeriodMs = l_sendPeriodMs;
    senderName = l_senderName;
    NDISendCreateDesc.p_ndi_name = senderName.c_str();
    
    // create the NDI sender
	pNDISender = NDIlib_send_create(&NDISendCreateDesc);
	if (!pNDISender)
		throw std::runtime_error("NDIImageSender::NDIImageSender: Failed to create an NDI sender.");

    // stard the sender thread
    senderThread = std::thread(&image_sender, this);
}
NDIImageSender::~NDIImageSender(){

    
    // lock
    while (lockAccessToLocalData.test_and_set(std::memory_order_acquire));

    // request end the sender thread
    needsToExit = true;

    // unlock
    lockAccessToLocalData.clear(std::memory_order_release);

    senderThread.join();
    
	// Destroy the NDI sender
	NDIlib_send_destroy(pNDISender);
	
	// Not required, but nice
	NDIlib_destroy();
}

void NDIImageSender::setImage(const std::vector<unsigned char>& l_imageData, int xres, int yres){

    // check passed image size based on x,y-res !
    size_t expectedSizeBytes = xres * yres * 4;
    if(l_imageData.size() != expectedSizeBytes)
        throw std::runtime_error("NDIImageSender::setImage: Invalid image size.");


    // lock
    while (lockAccessToLocalData.test_and_set(std::memory_order_acquire));

    // copy image contents/info
    imageData.resize(l_imageData.size());
    imageData.assign(l_imageData.begin(), l_imageData.end()); 
    imageXres = xres;
    imageYres = yres;

    // update the flags
    hasImageChanged = true;
    isImageInitialized = true;

    // unlock
    lockAccessToLocalData.clear(std::memory_order_release);
}

extern "C" DLLEXPORT NDIImageSender* NDIImageSender_create(const char* l_senderName, size_t l_sendPeriodMs){
    NDIImageSender* new_obj = new NDIImageSender(l_senderName, l_sendPeriodMs);    
    //std::cout << "Created sender with this address " << new_obj << std::endl;
    return new_obj;
}
extern "C" DLLEXPORT void NDIImageSender_delete(NDIImageSender* instance){
    delete instance;
}
extern "C" DLLEXPORT void NDIImageSender_setImage(NDIImageSender* instance, const unsigned char* l_imageData, int xres, int yres){
    //std::cout << "Someone asked me set image of a sender of this address " << instance << std::endl;    
    size_t num_bytes = xres*yres*4;
    //std::cout << "data size: " << num_bytes << std::endl;
    //std::cout << "first 3 data elements: " << l_imageData[0] << ", " << l_imageData[1] << ", " << l_imageData[0] << std::endl;
    std::vector<unsigned char> as_vec = std::vector<unsigned char>(l_imageData, l_imageData + num_bytes);
    instance->setImage(as_vec, xres, yres);
}
