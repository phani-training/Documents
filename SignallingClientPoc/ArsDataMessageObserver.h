#pragma once
#include "signaling/signaling_client_observer.h"
#include "data_message/data_message_error.h"
#include "data_message/data_message_received.h"

#include "controllers/meeting_controller.h"

#include <vector>
#include "Messages.h"
#include<Windows.h>
#include<WinUser.h>
#include "InputHandler.h"
#define WM_DATA 1001

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
      RTC_LOG(LS_INFO) << "data message: " << message.data;
      InputEvent ev = GetEventInfo(message.data);
      switch (ev.eventId) {
        case WM_LBUTTONDOWN:
          SendMouseDown(0, MAKELPARAM(ev.xPos, ev.yPos));
          break;
        case WM_MOUSEMOVE:
          SendMouseMove(0, MAKELPARAM(ev.xPos, ev.yPos));
          SetCursorPos(ev.xPos, ev.yPos);
          break;
        case WM_KEYDOWN:
          SendKeyDown(ev.keyCode, 0);
          RTC_LOG(LS_INFO) << "Key is pressed";
          break;
        case WM_DATA:
          RTC_LOG(LS_INFO) << "Data Message: " << ev.dataMessage;
          break;
        case WM_KEYUP:
          SendKeyUp(ev.keyCode, 0);
          RTC_LOG(LS_INFO) << "KeyUp Event with Keycode " + ev.keyCode;
          break;
        default:
          break;
      }
      RTC_LOG(LS_INFO) << "EventId : " << ev.eventId;
      Sleep(1000);
    }
  }
  virtual void OnRemoteDescriptionReceived(const std::string& sdp_answer) {}
};