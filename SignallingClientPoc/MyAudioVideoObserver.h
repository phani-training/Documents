#pragma once
#include <observers/data_message_observer.h>
#include "utils/logging.h"
using namespace chime;
#pragma once

class MyDataMessageObserver : public DataMessageObserver {
 public:
  virtual void OnDataMessagesReceived(const std::vector<DataMessageReceived>& messages)  {
   // std::unique_ptr<chime::Logger> logger = std::make_unique<chime::ConsoleLogger>(chime::LogLevel::kDebug);
    for (const auto& message : messages) {
     std::cout << message.data << std::endl;
    }
  }
};