# Signalling Client installation

## Steps for building Signalling Client App:
1. Create a new folder called Chime;
2. Clone the SignallingSDK Demo from the git repo: https://github.com/aws/amazon-chime-sdk-cpp.git
3. Run the CMake Command from the C:\Chime\amazon-chime-sdk-cpp\chime-sdk-signaling-cpp folder to generate the projects and files for the Signalling Client SDK. Remember that the CMakeList file is in this folder only.

### Installing Signaling Client Project that uses BoringSSL
```
cmake -S . -B build -G "Visual Studio 16 2019" -A x64 -DLWS_OPENSSL_LIBRARIES="C:/webrtc/src/out/msvc/win/Release/x64/obj/third_party/boringssl/boringssl.lib;C:/webrtc/src/out/msvc/win/Release/x64/obj/third_party/boringssl/boringssl_asm.lib" -DLWS_OPENSSL_INCLUDE_DIRS="C:/webrtc/src/third_party/boringssl/src/include”
```
- It was investigated that libwebrtc uses Boring SSL instead of Open SSL. So had to use this command to generate the BoringSSL libraries. BoringSSL is also part of the libwebrtc library, so we can use the folders accordingly.

5. Open the Solution from the build directory that is generated after this command is executed.
6. Make sure all the projects of the Solution should be in Release x64 mode
7. Open base.h file in  C:\webrtc\src\third_party\boringssl\src\include\openssl and add the Following Lines of Code in the Top of the File:
```
#ifdef X509_NAME
#undef X509_NAME
#undef X509_EXTENSIONS
#endif
```
8. Make all the project Code Generations as MT. Build the Solution for success.
9. Create a new project inside the same Solution of the type: Console App.
10. Implement the code as available in the Signalling-ClientPOC's  SignallingClientPoc.cpp
11.  U can clone the project that is available in this documents repository.
12. For the Rest API, use the C++ 20 features. Add the pre-processor directives  for C++20 warnings.
13. Add the include folders as per the directories of chime-SDK-lib
14. Add the include libs as per the libs available in the Unit-test.
15. Build and Test the Application.

#### NOTE:
- As Signalling client Samples are using the folder structure called webrtc, our webrtc that we have generated uses the src folder. It is hard to replace all the src with webrtc or vice versa.
- Tempororily I have created a copy of src into another folder called as webrtc. So please create this folder and add the contents of the src into it. 


### List of Preprocessor directives

USE_AURA=1;_HAS_EXCEPTIONS=0;__STD_C;_CRT_RAND_S;_CRT_SECURE_NO_DEPRECATE;_ATL_NO_OPENGL;_WINDOWS;CERT_CHAIN_PARA_HAS_EXTRA_FIELDS;PSAPI_VERSION=2;WIN32;_SECURE_ATL;WINUWP;
__WRL_NO_DEFAULT_LIB__;WINAPI_FAMILY=WINAPI_FAMILY_PC_APP;WIN10=_WIN32_WINNT_WIN10;WIN32_LEAN_AND_MEAN;NOMINMAX;_UNICODE;UNICODE;NTDDI_VERSION=NTDDI_WIN10_RS2;_WIN32_WINNT=0x0A00;WINVER=0x0A00;NDEBUG;NVALGRIND;DYNAMIC_ANNOTATIONS_ENABLED=0;WEBRTC_ENABLE_PROTOBUF=0;WEBRTC_INCLUDE_INTERNAL_AUDIO_DEVICE;RTC_ENABLE_VP9;HAVE_SCTP;WEBRTC_LIBRARY_IMPL;WEBRTC_NON_STATIC_TRACE_EVENT_HANDLERS=0;WEBRTC_WIN;ABSL_ALLOCATOR_NOTHROW=1;WEBRTC_VIDEO_CAPTURE_WINRT;WEBRTC_DUMMY_FILE_DEVICES;_CONSOLE;_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING;_SILENCE_ALL_CXX20_DEPRECATION_WARNINGS;_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS;%(PreprocessorDefinitions)
--------------------------------------------------------------------------------------------------------------------------------------- 
### Additional Include Directories copied from the chime-sdk-lib :
C:\Chime\amazon-chime-sdk-cpp\chime-sdk-signaling-cpp\src;C:\Chime\amazon-chime-sdk-cpp\chime-sdk-signaling-cpp\build\_deps\libwebsockets-src\lib\..\include;C:\Chime\amazon-chime-sdk-cpp\chime-sdk-signaling-cpp\build\_deps\libwebsockets-build\lib\..\include;C:\webrtc\src\third_party\boringssl\src\include;C:\Chime\amazon-chime-sdk-cpp\chime-sdk-signaling-cpp\build\_deps\protobuf-src\src;%(AdditionalIncludeDirectories)

### Additional Dependencies (From Unit Test Project)
..\Release\amazon-chime-signaling-sdk-cpp-lib.lib;..\lib\Release\gtest.lib;..\lib\Release\gmock.lib;..\_deps\protobuf-build\Release\libprotobuf.lib;..\_deps\libwebsockets-build\lib\Release\websockets_static.lib;C:\Program Files\OpenSSL-Win64\lib\libssl.lib;C:\Program Files\OpenSSL-Win64\lib\libcrypto.lib;ws2_32.lib;userenv.lib;psapi.lib;iphlpapi.lib;crypt32.lib;kernel32.lib;user32.lib;gdi32.lib;winspool.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;comdlg32.lib;advapi32.lib
