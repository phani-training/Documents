//#pragma once
//#include <string>
//#include "RestApi.h"
//struct MeetingInfo {
// public:
//  std::string attendee_id;
//  std::string external_user_id;
//  std::string join_token;
//  std::string audio_host_url;
//  std::string signaling_url;
//  std::string meeting_id;
//  std::string external_meeting_id;
//};

// https://pulud6u8je.execute-api.us-east-1.amazonaws.com/Prod/create?m=123
/*
{"Attendee":{"ExternalUserId":"Originator","AttendeeId":"a65006d2-519c-11b3-1b15-1113bcf5a3e0","JoinToken":"YTY1MDA2ZDItNTE5Yy0xMWIzLTFiMTUtMTExM2JjZjVhM2UwOjk3NTAzODI3LWVhNzQtNDAyNi05ZGVkLWE0ZTRjOGZiN2U3ZA"},"Meeting":{"MeetingId":"203f3360-9000-4a8e-a33f-6232bbc20706","ExternalMeetingId":"123","MediaPlacement":{"AudioHostUrl":"2b57322c9f52e7de90c3ec880af2cb02.k.m3.ue1.app.chime.aws:3478","AudioFallbackUrl":"wss://haxrp.m3.ue1.app.chime.aws:443/calls/203f3360-9000-4a8e-a33f-6232bbc20706","ScreenDataUrl":"wss://bitpw.m3.ue1.app.chime.aws:443/v2/screen/203f3360-9000-4a8e-a33f-6232bbc20706","ScreenSharingUrl":"wss://bitpw.m3.ue1.app.chime.aws:443/v2/screen/203f3360-9000-4a8e-a33f-6232bbc20706","ScreenViewingUrl":"wss://bitpw.m3.ue1.app.chime.aws:443/ws/connect?passcode=null&viewer_uuid=null&X-BitHub-Call-Id=203f3360-9000-4a8e-a33f-6232bbc20706","SignalingUrl":"wss://signal.m3.ue1.app.chime.aws/control/203f3360-9000-4a8e-a33f-6232bbc20706","TurnControlUrl":"https://ccp.cp.ue1.app.chime.aws/v2/turn_sessions","EventIngestionUrl":"https://data.svc.ue1.ingest.chime.aws/v1/client-events"},"MediaRegion":"us-east-1"}}

*/
//
//MeetingInfo CreateMeeting() {
//  MeetingInfo details{
//      "a65006d2-519c-11b3-1b15-1113bcf5a3e0",
//      "Originator",
//      "YTY1MDA2ZDItNTE5Yy0xMWIzLTFiMTUtMTExM2JjZjVhM2UwOjk3NTAzODI3LWVhNzQtNDAyNi05ZGVkLWE0ZTRjOGZiN2U3ZA",
//      "2b57322c9f52e7de90c3ec880af2cb02.k.m3.ue1.app.chime.aws:3478",
//      "wss://signal.m3.ue1.app.chime.aws/control/203f3360-9000-4a8e-a33f-6232bbc20706",
//      "203f3360-9000-4a8e-a33f-6232bbc20706",
//      "123"};
//  return details;
//}
