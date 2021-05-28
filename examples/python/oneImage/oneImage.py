import signal
import numpy
import cv2
import platform
import ctypes
import time
import os


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
    #os.add_dll_directory("C:\\Windows\\System32")
    OS = platform.system()
    LIB_BIN_PATH = "../../../bin/"

    if OS == "Windows":
        libFile = LIB_BIN_PATH + "NDIRestServer.dll"
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

    # load the image
    [image1_data, xres1, yres1] = getImageDataAndResolution("images/image1.png")

    # send the image
    libNDIRestServer.NDIImageSender_setImage(ctypes.c_void_p(NDISender), ctypes.c_char_p(image1_data), ctypes.c_int(xres1), ctypes.c_int(yres1))
    time.sleep(1)

    # wait
    while not KillMain:
        continue


    # delete the sender object
    libNDIRestServer.NDIImageSender_delete(ctypes.c_void_p(NDISender))


# main wrapper
if __name__ == "__main__": 
    signal.signal(signal.SIGINT, main_killer)
    signal.signal(signal.SIGTERM, main_killer)
    main()