#pragma once
#include "controllers/meeting_controller.h"
#include "controllers/meeting_controller_configuration.h"
#include "controllers/meeting_controller_dependencies.h"
#include "controllers/keypress_controller.h"
#include "observers/audio_events_observer.h"
#include "observers/lifecycle_observer.h"
#include "observers/presence_events_observer.h"
#include "observers/session_description_observer.h"
#include "observers/video_events_observer.h"
#include "observers/data_message_observer.h"
#include "observers/peer_connection_observer.h"

#include "signaling/signaling_client.h"
#include "signaling/signaling_client_configuration.h"
#include "signaling/default_signaling_client_factory.h"

#include <iostream>
#include <memory>
#include "RestApi.h"

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

int testAudioFileSharing() {
 // try {
    auto config = createMeetingConfiguration();

  //  SignalingClientConfiguration signaling_configuration;
  //  MeetingSessionConfiguration meeting_configuration = createMeetingConfiguration();

  //    DefaultSignalingDependencies signaling_dependencies{};
  //   auto client =
  //       DefaultSignalingClientFactory::CreateSignalingClient(signaling_configuration,
  //       std::move(signaling_dependencies));

  //   MeetingControllerConfiguration configuration;
  //   configuration.meeting_configuration = meeting_configuration;
  //   configuration.input_audio_filename = "samplefile.pcm";
  //   configuration.log_level = "Info";

  //   auto session_description_observer = std::make_unique<SessionDescriptionObserver>();
  //   std::shared_ptr<MeetingController> controller =
  //       MeetingController::Create(configuration, std::move(client), session_description_observer.get());

  //   session_description_observer->controller_ = controller.get();
  //   //auto peer_connection_observer = std::make_unique<PeerConnectionObserver>(controller.get());
  //   auto audio_events_observer = std::make_unique<AudioEventsObserver>();
  //   controller->signaling_client_->AddSignalingClientObserver(audio_events_observer.get());
  //   auto data_message_observer = std::make_unique<DataMessageObserver>();
  //   controller->signaling_client_->AddSignalingClientObserver(data_message_observer.get());

  //   auto video_events_observer =
  //       std::make_unique<VideoEventsObserver>(controller.get(), session_description_observer.get());
  //   controller->signaling_client_->AddSignalingClientObserver(video_events_observer.get());
  //   
  //   auto presence_events_observer = std::make_unique<PresenceEventsObserver>();
  //   controller->signaling_client_->AddSignalingClientObserver(presence_events_observer.get());

  //   controller->Start();
  //   controller->signaling_client_->Run();
  //   ////////////////////////////Code for posting data messages to the meeting///////////////////////////////////
  //   while (true) {
  //     DataMessageToSend msg;
  //     msg.data = "Test";
  //     msg.topic = "Chime Testing";
  //     controller->signaling_client_->SendDataMessage(msg);
  //     Sleep(5000);
  //   }
  //   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //   auto keypressController = std::make_unique<KeypressController>(controller);
  //   return keypressController->Exec();
  //} catch (const std::exception& e) {
  //  std::cout << e.what() << std::endl;
  //  return 0;
  //}
}

