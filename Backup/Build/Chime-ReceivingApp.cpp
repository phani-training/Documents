#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <session/meeting_session_configuration.h>
#include <signaling/signaling_client_configuration.h>
#include <signaling/default_signaling_dependencies.h>
#include <signaling/default_signaling_client.h>
#include <signaling/default_signaling_client_factory.h>
#include <transport\default_signaling_transport_factory.h>
#include "MeetingInfo.h"
#include "MySignalingObserver.h"
#include <iostream>
#include <Windows.h>
#include "RestApi.h"

#include "controllers/meeting_controller.h"
#include "observers/audio_events_observer.h"
//#include "observers/test_lifecycle_observer.h"
#include "observers/presence_events_observer.h"
#include "observers/session_description_observer.h"
#include "observers/video_events_observer.h"
#include "observers/data_message_observer.h"
#include "observers/peer_connection_observer.h"

#include "rtc_base/ssl_adapter.h"
#include "rtc_base/win32_socket_init.h"
#include "api/peer_connection_interface.h"
#include "api/create_peerconnection_factory.h"
#include "api/peer_connection_interface.h"
#include"LibWebrtcCall.h"
using namespace chime;
using namespace std;

void PrintScreen(std::string text) { std::cout << text << std::endl; }

/// <summary>
/// Helper function to create a meeting Configuration
/// </summary>
/// <returns>MeetingSessionConfiguration object</returns>
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


int main()
{
  std::cout << "Test code" << std::endl;

  makeWrbRtcCall();

  MeetingSessionConfiguration meeting_configuration =
      createMeetingConfiguration();  // = // create configuration with given data
  PrintScreen("Meeting Config created");
  SignalingClientConfiguration signaling_client_configuration{};
  signaling_client_configuration.meeting_configuration = meeting_configuration;
  DefaultSignalingDependencies signaling_dependencies{};

  signaling_dependencies.signal_transport_factory = DefaultSignalingTransportFactory::Create();
  PrintScreen("Signal Transport factory is created");
  auto client =
      new DefaultSignalingClient(std::move(signaling_client_configuration), std::move(signaling_dependencies));
  std::unique_ptr<SignalingClient> signaling_client = std::unique_ptr<DefaultSignalingClient>(client);

  PrintScreen("Signalling Client is created");

  ////////////////////////Creating Meeting Controller Configuration////////////////////////////////////
  MeetingControllerConfiguration configuration;
  configuration.meeting_configuration = meeting_configuration;
  //configuration.input_audio_filename = result["send_audio_file_name"].as<std::string>();
  //configuration.log_level = LogLevel::kDebug;
  auto session_description_observer = std::make_unique<SessionDescriptionObserver>();
  std::shared_ptr<MeetingController> controller =
      MeetingController::Create(configuration, std::move(signaling_client), session_description_observer.get());
  session_description_observer->controller_ = controller.get();
  auto peer_connection_observer = std::make_unique<PeerConnectionObserver>(controller.get());
  auto audio_events_observer = std::make_unique<AudioEventsObserver>();
  controller->signaling_client_->AddSignalingClientObserver(audio_events_observer.get());
  auto data_message_observer = std::make_unique<MySignalingObserver>();
  controller->signaling_client_->AddSignalingClientObserver(data_message_observer.get());
  auto video_events_observer =
      std::make_unique<VideoEventsObserver>(controller.get(), session_description_observer.get());
  controller->signaling_client_->AddSignalingClientObserver(video_events_observer.get());

  controller->Start();
  std::chrono::seconds duration(5);
  while (true) {
    DataMessageToSend message;
    message.data = "Testing Code";
    message.topic = "Chat";
    controller->signaling_client_->SendDataMessage(message);
    //Sleep(1000);
    std::this_thread::sleep_for(duration);
  }
  controller->Stop();
  /////////////////////////////////////////////////////////////////////////////////////////////////////

  // std::unique_ptr<SignalingClient> signaling_client =
  // DefaultSignalingClientFactory::CreateSignalingClient(signaling_client_configuration,
  // std::move(signaling_dependencies));
  //signaling_client->AddSignalingClientObserver(new MySignalingObserver());
  //PrintScreen("About to start the signalling client");
  //signaling_client->Start();
  //signaling_client->Run();
  //while (true) {
  //  DataMessageToSend message;
  //  message.data = "Testing Code";
  //  message.topic = "Chat";
  //  signaling_client->SendDataMessage(message);
  //  Sleep(1000);
  //};
  PrintScreen("----------------------------WebRtc Part-----------------------------------");


}

