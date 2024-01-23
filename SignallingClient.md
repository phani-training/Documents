
## Steps for building Signalling Client App:
1. Install the OpenSSL for Windows Win64OpenSSL-1_1_1w.msi available in https://slproweb.com/products/Win32OpenSSL.html. Download the 1.1.1 version. 
2. Open the x64 Native Tools Command Prompt for VS 2019 and run the installation using this Console.
3. Create a new folder called Chime in the C Drive as:
```
mkdir Chime
cd Chime
```
4. Clone the SignallingSDK Demo from the git repo: https://github.com/aws/amazon-chime-sdk-cpp.git inside the Chime folder. 
  
### Installing Signaling Client Project that uses BoringSSL
1. Move to the directory generated at C:\Chime\amazon-chime-sdk-cpp\chime-sdk-signaling-cpp
2. Run the CMake Command to generate the projects and files for the Signaling Client SDK. Remember that the CMakeList file is in this folder.
```
cmake -S . -B build -G "Visual Studio 16 2019" -A x64 -DLWS_OPENSSL_LIBRARIES="C:/webrtc/src/out/msvc/win/Release/x64/obj/third_party/boringssl/boringssl.lib;C:/webrtc/src/out/msvc/win/Release/x64/obj/third_party/boringssl/boringssl_asm.lib" -DLWS_OPENSSL_INCLUDE_DIRS="C:/webrtc/src/third_party/boringssl/src/include”
```
	- It was investigated that libwebrtc uses Boring SSL instead of Open SSL. So had to use this command to generate the Signaling Client Application that consumes BoringSSL libraries. BoringSSL is also part of the libwebrtc library, so we can use the folders accordingly.

3. Open the Solution from the build directory that is generated after this command is executed. 
4. Make sure all the projects of the Solution should be in Release x64 mode.
5. Make all the project Code Generations as MT at Project Properties->C/C++->Code Generation->RunTime Library. Build the Solution for success.
6. Except for the unit_test, rest of the projects should build for success, if required, U can exclude the Unit_test project from the Solution.
8. Include the SignalingClientPOC project available in the current branch into the current Solution.
9. U can remove OpenSSL.lib from the Project
![image](https://github.com/imentor-ltts/chime/assets/79626160/2b5f6603-5ea1-4bdd-bbaa-22f769b67768)

11. Build and Test the Application. If errors, make the Code modifications accordingly.

### Major code Modifications:
1. Open base.h file in  C:\webrtc\src\third_party\boringssl\src\include\openssl and add the Following Lines of Code in the Top of the File and save:
```
#ifdef X509_NAME
#undef X509_NAME
#undef X509_EXTENSIONS
#endif
```
2. Change the code in Line 620 of the C:\webrtc\src\third_party\abseil-cpp\absl\meta\type_traits.h as follows:
```
using result_of_t = typename std::invoke_result<T>::type;
```
3. In the C:\webrtc\src\modules\audio_device\dummy\file_audio_device_factory.cc file, Comment some # directives:
```
void FileAudioDeviceFactory::SetFilenamesToUse(
    const char* inputAudioFilename,
    const char* outputAudioFilename) {
//#ifdef WEBRTC_DUMMY_FILE_DEVICES
  RTC_DCHECK_LT(strlen(inputAudioFilename), MAX_FILENAME_LEN);
  RTC_DCHECK_LT(strlen(outputAudioFilename), MAX_FILENAME_LEN);

  // Copy the strings since we don't know the lifetime of the input pointers.
  strncpy(_inputAudioFilename, inputAudioFilename, MAX_FILENAME_LEN);
  strncpy(_outputAudioFilename, outputAudioFilename, MAX_FILENAME_LEN);
  _isConfigured = true;
//#else
  // Sanity: must be compiled with the right define to run this.
//  printf(
  //    "Trying to use dummy file devices, but is not compiled "
    //  "with WEBRTC_DUMMY_FILE_DEVICES. Bailing out.\n");
  //std::exit(1);
//#endif
}
```
4. Provide the default constructor for the MeetingControllerDependencies structure in the file C:\Chime\amazon-chime-sdk-cpp\chime-sdk-signaling-cpp\demo\shared\controllers\meeting_controller_dependencies.h:
```
MeetingControllerDependencies(){};
```
Also provide default constructor to C:\Chime\amazon-chime-sdk-cpp\chime-sdk-signaling-cpp\src\signaling\default_signaling_dependencies.h in the same manner. 

5. In the C:\Chime\amazon-chime-sdk-cpp\chime-sdk-signaling-cpp\demo\shared\controllers\meeting_controller.cc file, we need to change the methods of StopStandard and other functions as per the newer implementation of the webrtc
 ```
   //Line no 83:
   local_video_transceiver_->Stop();
   ```
Like wise, U may have to change the StopDirection method call to Stop method in the C:\Chime\amazon-chime-sdk-cpp\chime-sdk-signaling-cpp\demo\shared\observers\peer_connection_observer.cc file also. 
6. In the same file, U should modify the code block of Lineno 199 that is found in the StartLocalVideo Function to the below code. 
```
  if (IsValidTransceiver(local_video_transceiver_)) {
    RTC_LOG(LS_INFO) << "Setting local_video_transceiver direction to send";
    local_video_transceiver_->SetDirection(webrtc::RtpTransceiverDirection::kSendOnly);
    /*if (!err.ok()) {
      RTC_LOG(LS_ERROR) << "Failed to set video direction with error: " << err.message();
      return;
    }*/
    chime::LocalVideoConfiguration local_video_configuration;
    // Send it at max 800 kpbs. Modify this value based on number of attendees for better experience.
    local_video_configuration.max_bitrate_kbps = kMaxkbps;
    signaling_client_->UpdateLocalVideo(local_video_transceiver_->mid().value(), local_video_configuration);

    peer_connection_->CreateOffer(
        new rtc::RefCountedObject<CreateSessionDescriptionObserver>(session_description_observer_),
        webrtc::PeerConnectionInterface::RTCOfferAnswerOptions());
  }
```
7. Modify the file C:\Chime\amazon-chime-sdk-cpp\chime-sdk-signaling-cpp\demo\shared\observers\lifecycle_observer.cc as follows at line no 48 in the function block void LifecycleObserver::OnSignalingClientStarted(const SignalingClientStartInfo& join_info):
   ```
    auto result = controller_->peer_connection_factory_->CreatePeerConnection(config, std::move(dependencies));
 /* if (!result.ok()) {
    RTC_LOG(LS_ERROR) << "Failed to create peer connection.";
    return;
  } else {*/
    controller_->peer_connection_ = result.get();
   ```  
8. Add a new method implementation in the C:\Chime\amazon-chime-sdk-cpp\chime-sdk-signaling-cpp\demo\shared\observers\peer_connection_observer.h file to solve the Abstract class issue:
```
virtual void OnRenegotiationNeeded() {}
```
### NOTE:
- As Signalling client Samples are using the folder structure called webrtc, but our webrtc that we have generated uses the folder as src. It is hard to replace all the src with webrtc or vice versa.
- <b>Tempororily I have created a copy of src in the webrtc directory called as webrtc. So please create this folder and add the contents of the src into it.</b> 
- This might find a permanent solution later. 

### NOTE:
Discarding libcmt from the linking
Still on the Input tab, click on Ignore Specific Default Libraries and add libcmt.
It should be libcmtd on debug builds.
