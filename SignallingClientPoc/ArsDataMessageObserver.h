#pragma once
#include "signaling/signaling_client_observer.h"
#include "data_message/data_message_error.h"
#include "data_message/data_message_received.h"

#include "controllers/meeting_controller.h"

#include <vector>
#include"RestApi.h"
#include<Windows.h>
#include<WinUser.h>
#include "InputHandler.h"


using namespace chime;
class ArsDataMessageObserver : public chime::SignalingClientObserver {
 public:
  virtual void OnSignalingClientStarted(const SignalingClientStartInfo& start_info) 
  {
    RTC_LOG(LS_INFO) << "Signaling client has started by " + start_info.credentials.username;
  }
  
  virtual void OnSignalingClientStopped(const SignalingClientStatus& status) {
    RTC_LOG(LS_INFO) << "Signaling client has stopped";
  }

  /// <summary>
  /// THis function will be called when the App receives the Data Message.
  /// </summary>
  /// <param name="messages"></param>
  void OnDataMessageReceived(const std::vector<DataMessageReceived>& messages) {
    for (const auto& message : messages) {
      //InputEvent ev = GetEventInfo(message.data);
      InputEvent ev = GetEventDetails(message.data);
      if (ev.eventId == "WM_LBUTTONDOWN") {
        SendMouseDown(0, MAKELPARAM(ev.xPos, ev.yPos));
        RTC_LOG(LS_INFO) << "Mouse left button is pressed";
      }else if (ev.eventId == "WM_KEYDOWN") {
        SendKeyDown(ev.keyCode, 0);
        char character = MapVirtualKey(ev.keyCode, 0);
        RTC_LOG(LS_INFO) << "Key pressed: " << character;
      } else if (ev.eventId == "WM_RBUTTONDOWN") {
        SendMouseDown(1, MAKELPARAM(ev.xPos, ev.yPos));
        RTC_LOG(LS_INFO) << "RButton is pressed";
        
      } else{
        RTC_LOG(LS_INFO) << message.data;
      }
    }
  }
  virtual void OnRemoteDescriptionReceived(const std::string& sdp_answer) {}
};