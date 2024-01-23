# Getting Started with WinRTC
### Prerequisites
##### Machine requirements
• Windows 10 (build 19041) or later.
• At least 8GB of RAM (16GB of RAM is recommended).
• At least 15GB of disk space.
• SSD drive formatted with NTFS.
#### External applications
- Microsoft Visual Studio 2019 16.6.2.
- Git Download.
- Command Prompt (Windows Terminal is recommended, but cmd.exe works as well).
- In the Visual Studio Installer app, please verify if Visual Studio 2019 has Desktop development with C++ and Universal Windows Platform development workloads installed. Switch to the Individual Components tab. Make sure C++ MFC for latest v142 build tools (x86 & x64) and C++ ATL for the latest v142 build tools (x86 & x64) are selected.
- If you want to build for ARM/ARM64, also select the C++ MFC and ATL for latest v142 build tools and C++ Universal Windows Platform support for v142 build tools for the corresponding architecture.
- When installed by Visual Studio, the Windows SDK doesn't have the SDK Debugging Tools installed. Please go to Control Panel → Programs → Programs and Features → Select the most recent Windows Software Development Kit → Change → Change → Select Debugging Tools For Windows → Change.
- Opening the developer command prompt
    - You'll need a command prompt configured for calling Visual Studio tools. The following are two options for opening a developer command prompt:
    - Using the shortcut in the start menu : Click on Start Menu → Visual Studio 2019 → x64 Native Tools Command Prompt for VS 2019.
    - Run it as Administrator.
    - Move to the C: Drive. 
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

Set the path environment variable to execute commands in the depot_tools folder
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
```

Create the folder where the code base will be placed in Drive
```
mkdir c:\webrtc
cd c:\webrtc
```

Downloading the bits
Tell to the gclient tool to initialize your local copy of the repos. If you have already created the Depot tools setup in the previous instance of the Native Tools terminal, then U should run an additional command before U run gclient
```
git config --global --add safe.directory C:/depot_tools
gclient
```
Request the tools to fetch the WebRTC code base. The following command will take time. Past experience shows that it might take around 1 hour and 10 minutes.
```
fetch --nohooks webrtc
```
Change to the branch-heads/4147 branch. This is the commit that the UWP/WIN patches (see below) are based on.
```
cd src
git checkout branch-heads/4147
```
Instruct the tools to bring the bits from all the sub repositories to your dev box. This may take a while.
```
gclient sync -D -r branch-heads/4147
```


### Applying the patches
Clone the winrtc repo into C Drive. 
```
git clone https://github.com/microsoft/winrtc.git
```

The patchWebRTCM84.cmd batch file needs to locate the WebRTC code base to be patched. The environment variable WEBRTCM84_ROOT should contain the path for the WebRTC code base you've just downloaded.
```
set WEBRTCM84_ROOT=c:\webrtc\src
```
Now, you just need to run the batch file that will patch all the necessary repos that form the WebRTC code base.
```
c:\WinRTC\patches_for_WebRTC_org\m84\patchWebRTCM84.cmd
```
### Setting up and building
Setting up WebRTC is an extensive project and not all of its modules are required for producing a Win app with real time communications capabilities. Because of that, some parts of the WebRTC code base that do not build for Win were not patched by the patchWebRTCM84.cmd script. As such, the following ninja command excludes the unnecessary modules and prepares to build for Windows.
```
gn gen --ide=vs2019 out\msvc\win\Release\x64 --filters=//:webrtc "--args=is_debug=false use_lld=false is_clang=false rtc_include_tests=false rtc_build_tools=false rtc_win_video_capture_winrt=true target_os=\"win\" rtc_build_examples=false rtc_win_use_mf_h264=true enable_libaom=false rtc_enable_protobuf=false"
```
This will set up the out\msvc\win\Release\x64 folder for building WebRTC for the UWP x64 Release configuration. Build settings can be customized. To view the full list of editable build arguments, you can run
```
gn args --list out\msvc\win\Release\x64
```
### Building
Now that the project is set up, you can build it directly with the command line.
#### With Command line:
With command line. Run the following command to build the patched WebRTC.
```
ninja -C out\msvc\win\Release\x64
```
- This generates the required folders and the libraries under out\msvc\win\Release\x64 folder.
- It also contains the libraries of webrtc, boringSSL and other dependencies of the SingalingClient Application.
- The obj folder and its sub folders contain all the required libraries.   
