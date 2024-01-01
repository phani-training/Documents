# Signalling Client installation
## Install the webrtc as per the document mentioned in https://learn.microsoft.com/en-us/winrtc/getting-started

## Follow the steps in a separate folder:
```
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DLWS_OPENSSL_LIBRARIES="C:/Program Files/OpenSSL-Win64/lib/libssl.lib;C:/Program Files/OpenSSL-Win64/lib/libcrypto.lib" -DLWS_OPENSSL_INCLUDE_DIRS="C:/Program Files/OpenSSL-Win64/include"
- Use this for createing OPEN SSL Libraries
```
### For Boring SSL
```
cmake -S . -B build -G "Visual Studio 16 2019" -A x64 -DLWS_OPENSSL_LIBRARIES="C:/webrtc/src/out/msvc/win/Release/x64/obj/third_party/boringssl/boringssl.lib" -DLWS_OPENSSL_INCLUDE_DIRS="C:\webrtc\src\third_party\boringssl\src\include"
```
- It was investigated that libwebrtc uses Boring SSL instead of Open SSL. So had to use this command to generate the BoringSSL libraries. BoringSSL is also part of the libwebrtc library, so we can use the folders accordingly.

## Steps for building Signalling Client App:
1. Create a new folder called Chime
2. Clone the SignallingSDK Demo from the git repo
3. Run the CMake Command from the C:\Chime\amazon-chime-sdk-cpp\chime-sdk-signaling-cpp folder to generate the projects and files for the Signalling Client SDK. Remember that the CMakeList file is in this folder only.
4. Make sure all the projects of the Solution should be in Release x64 mode

---------------------------------------------------------------------------------------------------------------------------------------

- Install Open SSL 1.1.0: https://slproweb.com/download/Win64OpenSSL-1_1_1w.msi in Following Location C:\Program Files\OpenSSL-Win64.
- Run the Following Command in the Project Directory where you have cloned Signalling SDK and contains the CMake file.
- Open the solution from Visual Studio and resolve the warning as errors in the web-sockets projects.
- Build the Application with errors only on the Unit Test project. Ignore it.
- Create a new project in the Solution of the Console App Type.
- Implement the code as available for Signalling-Client,
- For the Rest API, use the C++ 20 features. Add the pre-processor directives  for C++20 warnings.
- Add the include folders as per the directories of chime-SDK-lib
- Add the include libs as per the libs available in the Unit-test.
- Build and Test the Application.  

Additional Include Directories copied from the chime-sdk-lib :
C:\Users\abhim\Downloads\chime-sdk-signaling-cpp\src;C:\Users\abhim\Downloads\chime-sdk-signaling-cpp\build\_deps\libwebsockets-src\lib\..\include;C:\Users\abhim\Downloads\chime-sdk-signaling-cpp\build\_deps\libwebsockets-build\lib\..\include;C:\Program Files\OpenSSL-Win64\include;C:\Users\abhim\Downloads\chime-sdk-signaling-cpp\build\_deps\protobuf-src\src;%(AdditionalIncludeDirectories)

Additional Dependencies (From Unit Test Project)
..\Release\amazon-chime-signaling-sdk-cpp-lib.lib;..\lib\Release\gtest.lib;..\lib\Release\gmock.lib;..\_deps\protobuf-build\Release\libprotobuf.lib;..\_deps\libwebsockets-build\lib\Release\websockets_static.lib;C:\Program Files\OpenSSL-Win64\lib\libssl.lib;C:\Program Files\OpenSSL-Win64\lib\libcrypto.lib;ws2_32.lib;userenv.lib;psapi.lib;iphlpapi.lib;crypt32.lib;kernel32.lib;user32.lib;gdi32.lib;winspool.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;comdlg32.lib;advapi32.lib
