# libwebrtc
# Getting Started with WinRTC

## Patching WebRTC for Windows
As is, the WebRTC code base has a Win32 port that doesn't build for UWP. Get started with building WebRTC for Windows with the patches available in our GitHub repo. We recommend applying the M84 patch, which has the most recent security updates and features.
These patches are being contributed back. Some of these patches were already merged into their original repos, but didn't rolled over WebRTC yet.
### Prerequisites
##### Machine requirements
•	Windows 10 (build 19041) or later.
•	At least 8GB of RAM (16GB of RAM is recommended).
•	At least 15GB of disk space.
•	SSD drive formatted with NTFS.
#### External applications
•	Microsoft Visual Studio 2019 16.6.2.
•	Command Prompt (Windows Terminal is recommended, but cmd.exe works as well).

- In the Visual Studio Installer app, please verify if Visual Studio 2019 has Desktop development with C++ and Universal Windows Platform development workloads installed. Switch to the Individual Components tab. Make sure C++ MFC for latest v142 build tools (x86 & x64) and C++ ATL for the latest v142 build tools (x86 & x64) are selected.
- If you want to build for ARM/ARM64, also select the C++ MFC and ATL for latest v142 build tools and C++ Universal Windows Platform support for v142 build tools for the corresponding architecture.
- When installed by Visual Studio, the Windows SDK doesn't have the SDK Debugging Tools installed. Please go to Control Panel → Programs → Programs and Features → Select the most recent Windows Software Development Kit → Change → Change → Select Debugging Tools For Windows → Change.
- Opening the developer command prompt
    - You'll need a command prompt configured for calling Visual Studio tools. The following are two options for opening a developer command prompt:
    - Using the shortcut in the start menu : Click on Start Menu → Visual Studio 2019 → x64 Native Tools Command Prompt for VS 2019
or

### Acquiring WebRTC code base
#### Getting depot_tools
- WebRTC uses Chromium's build tools named depot_tools. You can download it with curl that is now shipped with Windows. The following command will download depot_tool.zip to the current folder.
```
curl https://storage.googleapis.com/chrome-infra/depot_tools.zip --output depot_tools.zip
```
- Use the following command to unzip depot_tools in the root folder of the c: drive.
```
mkdir c:\depot_tools
tar -xf depot_tools.zip -C c:\depot_tools

```
Let's not clutter our dev box, so feel free to delete the depot_tools.zip file.
```
del depot_tools.zip
```

Set the path environment variable to execute commands in the depot_tools folder.
```
set PATH=c:\depot_tools;%PATH%
```
Keep in mind that the previous command gave priority to programs sitting in that folder. One of the programs sitting in the depot_tools folder is a batch file named git. It means that for the duration of this terminal session, when you execute git you're calling Google's git, not the one was previously installed in your dev box.

Setting up the environment
Let's inform depot_tools that we don't have access to Google's internal tools.
```
set DEPOT_TOOLS_WIN_TOOLCHAIN=0
```
The GYP build tool must be informed about the version of the Visual Studio we're using.
```
set GYP_MSVS_VERSION=2019

Create the folder where the code base will be placed in Drive
```
mkdir c:\webrtc
cd c:\webrtc
```

Downloading the bits
Tell to the gclient tool to initialize your local copy of the repos.
```
gclient
```
Request the tools to fetch the WebRTC code base. The following command will take time. Past experience shows that it might take around 1 hour and 10 minutes.
```
fetch --nohooks webrtc
Change to the branch-heads/4147 branch. This is the commit that the UWP patches (see below) are based on.
```
cd src
git checkout branch-heads/4147
```
Instruct the tools to bring the bits from all the sub repositories to your dev box. This may take a while.
gclient sync -D -r branch-heads/4147

### Applying the patches
Clone the Application from https://github.com/microsoft/winrtc.git
The patchWebRTCM84.cmd batch file needs to locate the WebRTC code base to be patched. The environment variable WEBRTCM84_ROOT should contain the path for the WebRTC code base you've just downloaded.
```
set WEBRTCM84_ROOT=c:\webrtc\src
Now, you just need to run the batch file that will patch all the necessary repos that form the WebRTC code base.
```
c:\WinRTC\patches_for_WebRTC_org\m84\patchWebRTCM84.cmd
Setting up and building
Setting up
WebRTC is an extensive project and not all of its modules are required for producing a UWP app with real time communications capabilities. Because of that, some parts of the WebRTC code base that do not build for UWP were not patched by the patchWebRTCM84.cmd script. As such, the following ninja command excludes the unnecessary modules and prepares to build for UWP.
```
gn gen --ide=vs2019 out\msvc\win\Release\x64 --filters=//:webrtc "--args=is_debug=false use_lld=false is_clang=false rtc_include_tests=false rtc_build_tools=false rtc_win_video_capture_winrt=true target_os=\"win\" rtc_build_examples=false rtc_win_use_mf_h264=true enable_libaom=false rtc_enable_protobuf=false"
```
This will set up the out\msvc\win\Release\x64 folder for building WebRTC for the UWP x64 Release configuration. Build settings can be customized. To view the full list of editable build arguments, you can run
```
gn args --list out\msvc\win\Release\x64
```
Please note that not all settings are supported in a WIN build.
Building
Now that the project is set up, you can choose between building it inside Visual Studio or directly with the command line.
With Visual Studio 2019
Open the generated Visual Studio solution with the following command:
```
devenv out\msvc\win\Release\x64\all.sln
```
- Keep in mind that opening the all.sln solution from the Start Menu will render to different results. Calling Visual Studio from the current command prompt will seed Visual Studio with the environment variables defined earlier (DEPOT_TOOLS_WIN_TOOLCHAIN, GYP_MSVS_VERSION and PATH) and needed by VS2019 to build the solution from the IDE.
- To build on Visual Studio, make sure you can see the Solution Explorer window (View → Solution Explorer), then right-click on the webrtc project (it should be on the bottom of the window), and then click on Select as Startup Project. Finally, complete the build with Build → Build Solution.
- With command line Run the following command to build the patched WebRTC from the command line.
```
ninja -C out\msvc\win\Release\x64
```
### Additional libraries to add for libwebrtc:
webrtc.lib
winmm.lib
Secur32.lib

### Follow the setup for building Webrtc
https://learn.microsoft.com/en-us/winrtc/getting-started

### Use Visual Studio 2019 only

### Location of the BoringSSL library
C:\webrtc\src\out\msvc\win\Release\x64\obj\third_party\boringssl\boringssl.lib
