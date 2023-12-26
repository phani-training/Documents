#include "utils/logging.h"

#include "controllers/meeting_controller.h"
#include "controllers/meeting_controller_configuration.h"
#include "controllers/meeting_controller_dependencies.h"
#include "controllers/keypress_controller.h"
#include "observers/audio_events_observer.h"
//#include "observers/lifecycle_observer.h"
#include "observers/presence_events_observer.h"
#include "observers/session_description_observer.h"
#include "observers/video_events_observer.h"
#include "observers/data_message_observer.h"
#include "observers/peer_connection_observer.h"

//#include "signaling/signaling_client.h"
#include "signaling/signaling_client_configuration.h"
#include "signaling/default_signaling_client_factory.h"
#include "RestApi.h"
#include"MeetingInfo.h"

#define CXXOPTS_NO_RTTI



#include <iostream>
#include <memory>

using namespace chime;

MeetingSessionConfiguration createMeetingConfiguration() {
    Uri request{L"https://pulud6u8je.execute-api.us-east-1.amazonaws.com/Prod/create?m=123"};
  MeetingDetails details = GetMeetingDetails(request);

  try {

    MeetingSessionCredentials credentials{details.attendee_id, details.external_user_id, details.join_token};

    MeetingSessionURLs urls{details.audio_host_url, details.signaling_url};

    MeetingSessionConfiguration configuration{details.meeting_id, details.external_meeting_id, std::move(credentials),
                                              std::move(urls)};
    return configuration;
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  MeetingSessionConfiguration c{};
  return c;
}

int main(int argc, char* argv[]) {

    auto config = createMeetingConfiguration();
  try {

  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return 0;
  }

  //auto result = options.parse(argc, argv);
  webrtc::PeerConnectionFactoryDependencies peer_connection_factory_dependencies;

  //std::string usr = result["external_user_id"].as<std::string>();
  std::string usr = config.credentials.external_user_id;
  std::cout << "\nAttendee Name: " << usr.substr(usr.find("#") + 1) << std::endl;
  std::cout << "Attendee ID: " << config.credentials.attendee_id << std::endl;
  std::cout << "Meeting Name: " << config.external_meeting_id << std::endl;
  std::cout << "Meeting ID: " << config.meeting_id << std::endl;
  //std::cout << "Audio Filename: " << result["send_audio_file_name"].as<std::string>() << std::endl;
  //std::cout << "Current Log Level: " << result["log_level"].as<std::string>() << std::endl;

  SignalingClientConfiguration signaling_configuration;
  MeetingSessionConfiguration meeting_configuration = createMeetingConfiguration();
  signaling_configuration.meeting_configuration = meeting_configuration;

  DefaultSignalingDependencies signaling_dependencies{};
  auto client =
      DefaultSignalingClientFactory::CreateSignalingClient(signaling_configuration, std::move(signaling_dependencies));

  MeetingControllerConfiguration configuration;
  configuration.meeting_configuration = meeting_configuration;
  //configuration.input_audio_filename = result["send_audio_file_name"].as<std::string>();
  configuration.log_level ="info";
  auto session_description_observer = std::make_unique<SessionDescriptionObserver>();
  std::shared_ptr<MeetingController> controller =
      MeetingController::Create(configuration, std::move(client), session_description_observer.get());

  session_description_observer->controller_ = controller.get();
  auto peer_connection_observer = std::make_unique<PeerConnectionObserver>(controller.get());
  auto audio_events_observer = std::make_unique<AudioEventsObserver>();
  controller->signaling_client_->AddSignalingClientObserver(audio_events_observer.get());
  auto data_message_observer = std::make_unique<DataMessageObserver>();
  controller->signaling_client_->AddSignalingClientObserver(data_message_observer.get());
  auto video_events_observer =
      std::make_unique<VideoEventsObserver>(controller.get(), session_description_observer.get());
  controller->signaling_client_->AddSignalingClientObserver(video_events_observer.get());
 // auto lifecycle_observer =
   //   std::make_unique<LifecycleObserver>(controller.get(), peer_connection_observer.get(), video_events_observer.get(),
 //                                         session_description_observer.get());
 // controller->signaling_client_->AddSignalingClientObserver(lifecycle_observer.get());
  auto presence_events_observer = std::make_unique<PresenceEventsObserver>();
  controller->signaling_client_->AddSignalingClientObserver(presence_events_observer.get());

  controller->Start();
  DataMessageToSend message;
  message.data = "Testing Code";
  message.topic = "Chat";
  Sleep(1000);
  controller->signaling_client_->SendDataMessage(message);
  auto keypressController = std::make_unique<KeypressController>(controller);
  return keypressController->Exec();
}
