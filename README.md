1- Download and install NDI 4 SDK:
    - For Windows at this [link](https://downloads.ndi.tv/SDK/NDI_SDK/NDI%204%20SDK.exe).
    - For iOS and macOS at this [link](https://downloads.ndi.tv/SDK/NDI_SDK_Mac/InstallNDISDK_v4_Apple.pkg).
    - For Linux at this [link](https://downloads.ndi.tv/SDK/NDI_SDK_Linux/InstallNDISDK_v4_Linux.tar.gz).

2-$ mkdir build; cd build; cmake ..; cmake --build .;

3- Binaries copied to ./bin. Run the program

4- To receive/test the stream download the NDI tools andd install the NDI-Video-Monitor (a.k.a Studio Monitor) tool:
    - For Mac, use the [link](https://downloads.ndi.tv/Tools/NewTekNDIToolsForMacOS.dmg). 
    - For windows, use the [link](https://downloads.ndi.tv/Tools/NDI%204%20Tools.exe)

5- To publish on Ventuz, use the ventuz project in ./ventuz/NDIReceiver. You should run the presentation file. This will load a presentation with NDI receiver node.