import signal
import numpy
import cv2
import platform
import ctypes
import time


KillMain = False
def main_killer(signum, frame):
    global KillMain
    print("Exiting ... ")
    KillMain = True

def getImageDataAndResolution(imageFile):
    img = cv2.imread(imageFile, cv2.IMREAD_UNCHANGED)
    img = img[:,:,[2,1,0,3]]    # this converts 4-channel BGR to 4-c RGB
    xres, yres, c = img.shape
    return [bytes(img), xres, yres]

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
    global KillMain

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
    while not KillMain:
        libNDIRestServer.NDIImageSender_setImage(ctypes.c_void_p(NDISender), ctypes.c_char_p(image1_data), ctypes.c_int(xres1), ctypes.c_int(yres1))
        time.sleep(1)
        libNDIRestServer.NDIImageSender_setImage(ctypes.c_void_p(NDISender), ctypes.c_char_p(image2_data), ctypes.c_int(xres2), ctypes.c_int(yres2))
        time.sleep(1)

    # delete the sender object
    libNDIRestServer.NDIImageSender_delete(ctypes.c_void_p(NDISender))


# main wrapper
if __name__ == "__main__": 
    signal.signal(signal.SIGINT, main_killer)
    signal.signal(signal.SIGTERM, main_killer)
    main()