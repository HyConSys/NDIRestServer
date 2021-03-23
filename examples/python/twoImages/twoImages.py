import numpy
import cv2
import platform
import ctypes
import time


def getImageDataAndResolution(imageFile):
    img = cv2.imread(imageFile, cv2.IMREAD_UNCHANGED)
    xres, yres, c = img.shape
    image_bytes = bytes(img.reshape(-1).tolist())
    return [image_bytes, xres, yres]

def LoadNDIRestServerLib():
    OS = platform.system()
    LIB_BIN_PATH = "../../../bin/"

    if OS == "Windows":
        libFile = LIB_BIN_PATH + "libNDIRestServer.dll"
    elif OS == "Linux":
        libFile = LIB_BIN_PATH + "libNDIRestServer.so"
    elif OS == "Darwin":
        libFile = LIB_BIN_PATH + "libNDIRestServer.dylib"
    else:
        print("Unsupported OS!")
        return []

    return ctypes.cdll.LoadLibrary(libFile)


def main():
    # load the library
    libNDIRestServer = LoadNDIRestServerLib()
    if libNDIRestServer == []:
        print("Failed to load the lib OS!")
        return
    

    # create an NDISender object
    NDI_SENDER_NAME = b'My_PNG'
    libNDIRestServer.NDIImageSender_create.restype = ctypes.c_void_p
    NDISender = libNDIRestServer.NDIImageSender_create(NDI_SENDER_NAME, 10)

    # load the images
    [image1_data, xres1, yres1] = getImageDataAndResolution("images/image1.png")
    [image2_data, xres2, yres2] = getImageDataAndResolution("images/image2.png")

    # send the images
    while True:
        libNDIRestServer.NDIImageSender_setImage(ctypes.c_void_p(NDISender), ctypes.c_char_p(image1_data), ctypes.c_int(xres1), ctypes.c_int(yres1))
        time.sleep(1)
        libNDIRestServer.NDIImageSender_setImage(ctypes.c_void_p(NDISender), ctypes.c_char_p(image2_data), ctypes.c_int(xres2), ctypes.c_int(yres2))
        time.sleep(1)

    # delete the sender object
    libNDIRestServer.NDIImageSender_delete(ctypes.c_void_p(NDISender))


# main wrapper
if __name__ == "__main__": 
    main()