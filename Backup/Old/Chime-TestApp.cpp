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

using namespace chime;
using namespace std;
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
int main() {
  std::cout << "Test code" << std::endl;
  MeetingSessionConfiguration meeting_configuration =
      createMeetingConfiguration();  // = // create configuration with given data
  SignalingClientConfiguration signaling_client_configuration{};
  signaling_client_configuration.meeting_configuration = meeting_configuration;
  DefaultSignalingDependencies signaling_dependencies{};

  signaling_dependencies.signal_transport_factory = DefaultSignalingTransportFactory::Create();
  auto* client =
      new DefaultSignalingClient(std::move(signaling_client_configuration), std::move(signaling_dependencies));
  std::unique_ptr<SignalingClient> signaling_client = std::unique_ptr<DefaultSignalingClient>(client);

  // std::unique_ptr<SignalingClient> signaling_client =
  // DefaultSignalingClientFactory::CreateSignalingClient(signaling_client_configuration,
  // std::move(signaling_dependencies));
  signaling_client->AddSignalingClientObserver(new MySignalingObserver());
  signaling_client->Start();
  signaling_client->Run();
  while (true) {
    DataMessageToSend message;
    message.data = "Testing Code";
    message.topic = "Chat";
    signaling_client->SendDataMessage(message);
    Sleep(1000);
  };
}