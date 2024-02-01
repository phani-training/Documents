#pragma once
#include "json.hpp"
using json = nlohmann::json;
struct InputEvent {
 public:
  UINT eventId;  // WM_LBUTTONDOWN, WM_RBUTTONDOWN, WM_MOUSEMOVE, WM_SCROLL, WM_KEYUP, WM_KEYDOWN
  int xPos;
  int yPos;
  int keyCode;
  std::string dataMessage;
};

InputEvent GetEventInfo(std::string jsonData) {
  auto data = json::parse(jsonData);
  InputEvent ev;
  ev.eventId = data["eventId"];
  ev.xPos = data["xPos"];
  ev.yPos = data["yPos"];
  ev.keyCode = data["keyCode"];
  return ev;
}

json ConvertToJson(InputEvent msg) {
  json jsonData;
  jsonData["eventId"] = msg.eventId;
  jsonData["xPos"] = msg.xPos;
  jsonData["yPos"] = msg.yPos;
  jsonData["keyCode"] = msg.keyCode;
  return jsonData;
}
