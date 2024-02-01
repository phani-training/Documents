#pragma once
#include <iostream>
#include <iomanip>
#include <ctime>
#include <utils/logging.h>
#include <controllers/meeting_controller.h>

using namespace std;

std::string getTime() {
  std::time_t now = std::time(nullptr);
  std::tm timeinfo = *std::localtime(&now);
  char buffer[80];
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return buffer;
}

enum MessageType {
	MT_DATA, MT_KEYBOARD, MT_MOUSE, MT_VIDEO, MT_AUDIO
};

class SignallingClientApi {
 private:
  static void PostDataMessage(shared_ptr<MeetingController> controller, string message) {
    DataMessageToSend msg;
    msg.data = message;
    msg.topic = "BasicDemo";
    controller->signaling_client_->SendDataMessage(msg);
  }
 public:
  static string GetTime() { return getTime(); }
};
