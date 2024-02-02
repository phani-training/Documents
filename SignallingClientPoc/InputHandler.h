#pragma once
#include "framework.h"
#include "json.hpp"
#include <WinUser.h>

using json = nlohmann::json;

bool IsModKeyPressed() {
  if (GetKeyState(VK_CONTROL))
    return true;
  else if (GetKeyState(VK_MENU))
    return true;
  else
    return false;
}

void SendKeyDown(WPARAM wParam, LPARAM lParam) {
  INPUT input;
  input.type = INPUT_KEYBOARD;
  input.ki.wVk = wParam;
  if (IsModKeyPressed()) {
    input.ki.wScan = MapVirtualKey(wParam, MAPVK_VSC_TO_VK_EX);  // For CTRL,SHIFT, ALT Buttons...
    SendInput(1, &input, sizeof(INPUT));
  } else {
    char content[200];
    wsprintfA(content, "%c", wParam);  // For WM_CHAR Messages...
    SendInput(1, &input, sizeof(INPUT));
  }
}

void SendText(std::string transferText) {
  int length = transferText.length();
  INPUT* inputs = new INPUT[length];
  ZeroMemory(inputs, sizeof(inputs));
  for (int i = 0; i < length; i++) {
    SendKeyDown((int)transferText[i], 0);
  }
};

void SendKeyUp(WPARAM wParam, LPARAM lParam) {
  INPUT input;
  input.type = INPUT_KEYBOARD;
  input.ki.wVk = wParam;
  input.ki.dwExtraInfo = GetMessageExtraInfo();
  char content[200];
  wsprintfA(content, "%c", wParam);
  SendInput(1, &input, sizeof(INPUT));
}

void SendMouseDown(WPARAM wParam, LPARAM lParam) {
  int xPos = LOWORD(lParam);
  int yPos = LOWORD(lParam);

  INPUT input;
  input.type = INPUT_MOUSE;
  input.mi.dx = xPos;
  input.mi.dy = yPos;
  input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_VIRTUALDESK);
  SendInput(1, &input, sizeof(INPUT));
}

void SendMouseMove(WPARAM wParam, LPARAM lParam) {
  int xPos = LOWORD(lParam);
  int yPos = LOWORD(lParam);

  INPUT input;
  input.type = INPUT_MOUSE;
  input.mi.dx = xPos;
  input.mi.dy = yPos;
  input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_VIRTUALDESK);
  SendInput(1, &input, sizeof(INPUT));
}