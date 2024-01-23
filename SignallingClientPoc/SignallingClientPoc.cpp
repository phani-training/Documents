// SignallingClientPoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _SILENCE_CXX20_IS_POD_DEPRECATION_WARNING
#include <Windows.h>
#include <iostream>
#include "RestApi.h"
#include"HelperFunctions.h"
#include "signaling/signaling_client.h"
#include "signaling/signaling_client_configuration.h"
#include "signaling/default_signaling_client_factory.h"

#include <controllers/meeting_controller_configuration.h>
#include <controllers/meeting_controller.h>
#include <controllers/keypress_controller.h>
#include "observers/session_description_observer.h"  //--->creates errors as shared (5 Errors)

#include <observers/audio_events_observer.h>
#include <observers/peer_connection_observer.h>
#include <observers/video_events_observer.h>
#include <observers/lifecycle_observer.h>
#include <observers/presence_events_observer.h>
#include "MyAudioVideoObserver.h"
using namespace chime;

/// <summary>
/// Creates the Meeting and returns the configuration to connect
/// </summary>
/// <returns>A Meeting Session Configuration object</returns>
MeetingSessionConfiguration createMeetingConfiguration() {
  Uri request{L"https://pulud6u8je.execute-api.us-east-1.amazonaws.com/Prod/create?m=899901"};
//Uri request{L"https://pulud6u8je.execute-api.us-east-1.amazonaws.com/Prod/join?m=899901"};

  MeetingDetails details = GetMeetingDetails(request);
  try {
    MeetingSessionCredentials credentials{details.attendee_id, details.external_user_id, details.join_token};
    MeetingSessionURLs urls{details.audio_host_url, details.signaling_url};
    MeetingSessionConfiguration configuration{details.meeting_id, details.external_meeting_id, std::move(credentials),std::move(urls)};
    return configuration;
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  MeetingSessionConfiguration c{};
  return c;
}

int main() {
    SignalingClientConfiguration signaling_client_config;
    auto meeting_session_config = createMeetingConfiguration();
    CHIME_LOG(LogLevel::kInfo, "Meeting id: " + meeting_session_config.meeting_id);
    signaling_client_config.meeting_configuration = meeting_session_config;
    CHIME_LOG(LogLevel::kInfo, "Attendee ID: " + meeting_session_config.credentials.attendee_id);
    DefaultSignalingDependencies signaling_dependencies{};

    auto signaling_client = DefaultSignalingClientFactory::CreateSignalingClient(signaling_client_config,
                                                                                 std::move(signaling_dependencies));
    MeetingControllerConfiguration configuration;
    configuration.meeting_configuration = meeting_session_config;
    configuration.input_audio_filename =
        "C:/Chime/amazon-chime-sdk-cpp/chime-sdk-signaling-cpp/demo/cli/media_in/SampleFile_48k_16bit.pcm";

    //****************************Adding the Meeting Controller to the Application*********************************
    auto session_description_observer = std::make_unique<SessionDescriptionObserver>();
    std::shared_ptr<MeetingController> controller =
        MeetingController::Create(configuration, std::move(signaling_client), session_description_observer.get());

    session_description_observer->controller_ = controller.get();
    auto peer_connection_observer = std::make_unique<PeerConnectionObserver>(controller.get());
    
    auto audio_events_observer = std::make_unique<AudioEventsObserver>();
    controller->signaling_client_->AddSignalingClientObserver(audio_events_observer.get());

    auto data_message_observer = std::make_unique<DataMessageObserver>();
    controller->signaling_client_->AddSignalingClientObserver(data_message_observer.get());

    auto video_events_observer =
        std::make_unique<VideoEventsObserver>(controller.get(), session_description_observer.get());
    controller->signaling_client_->AddSignalingClientObserver(video_events_observer.get());
    ////////////////////////Issue with the LifeCycleObserver////////////////////////////////////////////////////////
    /*auto lifecycle_observer =
        std::make_unique<LifecycleObserver>(controller.get(), peer_connection_observer.get(),
                                            video_events_observer.get(), session_description_observer.get());
    controller->signaling_client_->AddSignalingClientObserver(lifecycle_observer.get());*/
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    auto presence_events_observer = std::make_unique<PresenceEventsObserver>();
    controller->signaling_client_->AddSignalingClientObserver(presence_events_observer.get());
    controller->Start();
    
    while (true) {
      DataMessageToSend msg;
      msg.data = "Test Message sent at " + getTime();
      msg.topic = "BasicDemo";
      msg.lifetime_ms = 5000;
      controller->signaling_client_->SendDataMessage(msg);
      Sleep(5000);
    }

   /* LocalVideoConfiguration lVc;
    lVc.max_bitrate_kbps = 1000;
    controller->signaling_client_->AddLocalVideo("abc", lVc);
  auto keypressController = std::make_unique<KeypressController>(controller);
  return keypressController->Exec();*/
}
