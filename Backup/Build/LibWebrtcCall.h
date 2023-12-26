#pragma once
#include <iostream>
#include "rtc_base/ssl_adapter.h"
#include "rtc_base/win32_socket_server.h"
#include "rtc_base/win32_socket_init.h"
#include "api/peer_connection_interface.h"
#include "api/create_peerconnection_factory.h"
#include "api/peer_connection_interface.h"

class DummyObserver : public webrtc::PeerConnectionObserver {
 public:
  // Implementations of pure virtual methods
  void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state) override {}

  void OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) override {}

  void OnRenegotiationNeeded() override {}

  void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state) override {}

  void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override {}

  // Optional overrides of virtual methods with default implementations
  void OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override {}

  void OnRemoveStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override {}

  void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state) override {}

  void OnStandardizedIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state) override {}

  void OnConnectionChange(webrtc::PeerConnectionInterface::PeerConnectionState new_state) override {}

  void OnIceCandidateError(const std::string& host_candidate, const std::string& url, int error_code,
                           const std::string& error_text) override {}

  void OnIceCandidateError(const std::string& address, int port, const std::string& url, int error_code,
                           const std::string& error_text) override {}

  void OnIceCandidatesRemoved(const std::vector<cricket::Candidate>& candidates) override {}

  void OnIceConnectionReceivingChange(bool receiving) override {}

  void OnIceSelectedCandidatePairChanged(const cricket::CandidatePairChangeEvent& event) override {}

  void OnAddTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
                  const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>& streams) override {}

  void OnTrack(rtc::scoped_refptr<webrtc::RtpTransceiverInterface> transceiver) override {}

  void OnRemoveTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver) override {}

  void OnInterestingUsage(int usage_pattern) override {}

  // Add any other method implementations as needed...
};

int makeWrbRtcCall() {
  // Windows socket initialization
  std::cout << "Into webrtc Call" << std::endl;
  rtc::WinsockInitializer winsock_init;
  rtc::Win32SocketServer w32_ss;
  rtc::Win32Thread w32_thread(&w32_ss);
  rtc::ThreadManager::Instance()->SetCurrentThread(&w32_thread);

  // Initialize SSL, required by WebRTC
  rtc::InitializeSSL();

  // Initialize Google's logging library.
  rtc::LogMessage::LogToDebug(rtc::LS_INFO);

  rtc::Thread* network_thread = rtc::Thread::CreateWithSocketServer().release();
  rtc::Thread* worker_thread = rtc::Thread::Create().release();
  rtc::Thread* signaling_thread = rtc::Thread::Create().release();
  network_thread->Start();
  worker_thread->Start();
  signaling_thread->Start();

  // Create a PeerConnectionFactory.
  rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peer_connection_factory =
      webrtc::CreatePeerConnectionFactory(network_thread, worker_thread, signaling_thread,
                                          nullptr,  // Default audio device module
                                          nullptr,  // Audio encoder factory - set to nullptr if not available
                                          nullptr,  // Audio decoder factory - set to nullptr if not available
                                          nullptr,  // Video encoder factory - set to nullptr if not available
                                          nullptr,  // Video decoder factory - set to nullptr if not available
                                          nullptr,  // Video bitrate allocator factory - set to nullptr if not available
                                          nullptr   // Audio processing module - set to nullptr if not available
      );

  if (!peer_connection_factory) {
    std::cerr << "Failed to initialize PeerConnectionFactory" << std::endl;
    rtc::CleanupSSL();
    return -1;
  }

  // Create a PeerConnection configuration.
  webrtc::PeerConnectionInterface::RTCConfiguration config;
  config.sdp_semantics = webrtc::SdpSemantics::kUnifiedPlan;

  DummyObserver observer;

  // Create a PeerConnection.
  rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection =
      peer_connection_factory->CreatePeerConnection(config, nullptr, nullptr, &observer);

  if (!peer_connection) {
    std::cerr << "Failed to create PeerConnection" << std::endl;
    rtc::CleanupSSL();
    return -1;
  }

  std::cout << "PeerConnection successfully created." << std::endl;

  // Perform other necessary operations...

  // Clean up before exiting
  rtc::CleanupSSL();
  return 0;
}