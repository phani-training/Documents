#pragma once
#pragma once
//#include <Windows.h>
#include <iostream>
#include <string>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Web.Http.Headers.h>
#include <winrt/Windows.Data.Json.h>
#include <locale>
#include <codecvt>

// using WinRT Framework that works from C++20 onwards
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Web::Http;
using namespace Windows::Data::Json;
// Converting JSON data into a structure
struct MeetingDetails {
 public:
  std::string attendee_id;
  std::string external_user_id;
  std::string join_token;
  std::string audio_host_url;
  std::string signaling_url;
  std::string meeting_id;
  std::string external_meeting_id;
};

std::string HStringToStdString(const winrt::hstring& hstr) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  std::wstring wstr = hstr.c_str();
  return converter.to_bytes(wstr);
}

// We are using C++ 20 libraries so old depreciated APIs of C++17 will give warnings and erros, so avoid it, we are
// adding a preprocessor directive called _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING;

MeetingDetails GetMeetingDetails(Uri request) {
  init_apartment();

  HttpClient httpClient{};
  HttpResponseMessage httpResponseMessage;

  hstring httpResponseBody;

  try {
    httpResponseMessage = httpClient.GetAsync(request).get();
    httpResponseMessage.EnsureSuccessStatusCode();
    httpResponseBody = httpResponseMessage.Content().ReadAsStringAsync().get();
    JsonValue jsonValue = JsonValue::Parse(httpResponseBody);

    auto attendee = jsonValue.GetObject().GetNamedObject(L"Attendee");  //.Stringify().c_str();
    auto meeting = jsonValue.GetObject().GetNamedObject(L"Meeting");
    auto mediaPlacement =
        jsonValue.GetObject().GetNamedObject(L"Meeting").GetObject().GetNamedObject(L"MediaPlacement");

    hstring attendee_id = attendee.GetNamedString(L"AttendeeId").c_str();
    hstring external_user_id = attendee.GetNamedString(L"ExternalUserId").c_str();
    hstring join_token = attendee.GetNamedString(L"JoinToken").c_str();
    hstring audio_host_url = mediaPlacement.GetNamedString(L"AudioHostUrl").c_str();
    hstring signaling_url = mediaPlacement.GetNamedString(L"SignalingUrl").c_str();
    hstring meeting_id = meeting.GetNamedString(L"MeetingId").c_str();
    hstring external_meeting_id = L"123";

    MeetingDetails details;
    details.attendee_id = HStringToStdString(attendee_id);
    details.external_meeting_id = HStringToStdString(external_meeting_id);
    details.join_token = HStringToStdString(join_token);
    details.audio_host_url = HStringToStdString(audio_host_url);
    details.signaling_url = HStringToStdString(signaling_url);
    details.meeting_id = HStringToStdString(meeting_id);
    details.external_user_id = HStringToStdString(external_user_id);
    return details;
  } catch (const std::exception& exception) {
    throw;
  }
}