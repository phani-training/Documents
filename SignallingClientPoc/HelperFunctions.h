#pragma once
#include <iostream>
#include <iomanip>
#include <ctime>

using namespace std;

std::string getTime() {
  std::time_t now = std::time(nullptr);
  std::tm timeinfo = *std::localtime(&now);
  char buffer[80];
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return buffer;
}