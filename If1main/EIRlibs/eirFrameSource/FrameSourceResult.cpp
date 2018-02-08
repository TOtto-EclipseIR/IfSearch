#include "FrameSourceResult.h"

#include "../eirBase/Severity.h"
#include "../eirExe/ResultId.h"

const ResultCode FrameSourceResult::Initializing("eFSInitg");
const ResultCode FrameSourceResult::Initialized("eFSInitd");
const ResultCode FrameSourceResult::Connecting("eFSConng");
const ResultCode FrameSourceResult::Connected("eFSConnd");
const ResultCode FrameSourceResult::ConnectError("eFSConEr");
const ResultCode FrameSourceResult::Started("eFSStrtg");
const ResultCode FrameSourceResult::StartError("eFSStErr");
const ResultCode FrameSourceResult::PlaybackComplete("eFSPbCpd");
const ResultCode FrameSourceResult::HttpConnectSuccess("eFSHCond");
const ResultCode FrameSourceResult::HttpConnectFailure("eFSHConE");
const ResultCode FrameSourceResult::LiveDirConnectFailure("eFSLConE");

const ResultInfo ri01(FrameSourceResult::Initializing,
                      Severity::Progress,
                 ResultId("eir", "FrameSource","Initializing"),
                 "Initializing %1 plugin",
                 "One time initialization started.",
                 QStringList() << "PluginName");

const ResultInfo ri08(FrameSourceResult::Initialized,
                      Severity::Progress,
                 ResultId("eir", "FrameSource","Initialized"),
                 "Initialized %1 plugin",
                 "One time initialization successful.",
                 QStringList() << "PluginName");

const ResultInfo ri02(FrameSourceResult::Connecting, Severity::Progress,
             ResultId("eir", "FrameSource","Connecting"),
             "Connecting to %1",
             "Trying to initiate a connection to the specified URL.",
             QStringList() << "URL");

const ResultInfo ri13(FrameSourceResult::Connected, Severity::Progress,
             ResultId("eir", "FrameSource","Connected"),
             "Connecting to %1",
             "Initiated a connection to the specified URL.",
             QStringList() << "URL");

const ResultInfo ri12(FrameSourceResult::ConnectError, Severity::Error,
             ResultId("eir", "FrameSource","ConnectError"),
             "Connect Error to %1",
             "Failed to initiate a connection to the specified URL.",
             QStringList() << "URL");

const ResultInfo ri09(FrameSourceResult::Started, Severity::Progress,
             ResultId("eir", "FrameSource","Starting"),
             "Starting %1 for %2 frames",
             "Starting collecting frames from the specified URL.",
             QStringList()
                      << "URL"
                      << "NumFrames");

const ResultInfo ri10(FrameSourceResult::StartError, Severity::Error,
             ResultId("eir", "FrameSource","StartError"),
             "Error Starting %1: %2",
             "Unable to start collecting frames from the specified URL.",
                      QStringList() << "URL" << "ErrorString");

const ResultInfo ri11(FrameSourceResult::PlaybackComplete, Severity::Progress,
             ResultId("eir", "FrameSource","PlaybackComplete"),
             "%1 frames complete for %2 from %3 to %4",
             "All frames for specified begin and end time from URL have been retrieved.",
             QStringList()
                      << "TotalFrames"
                      << "URL"
                      << "BeginTime"
                      << "EndTime");

const ResultInfo ri03(FrameSourceResult::HttpConnectSuccess, Severity::Progress,
                 ResultId("eir", "FrameSource","Http","ConnectSuccess"),
                 "Connected to %1",
                 "Connection was made to the specified URL",
                     QStringList() << "URL");

const ResultInfo ri04(FrameSourceResult::HttpConnectFailure, Severity::Error,
                 ResultId("eir", "FrameSource","Http","ConnectFailure"),
                 "Unable to connect to %1 [error=%2 %3]",
                 "The connection attempt resulted in an error; it will not continue.",
                     QStringList() << "URL"
                                   << "ErrorCode"
                                   << "ErrorString");

const ResultInfo ri05(FrameSourceResult::LiveDirConnectFailure, Severity::Error,
                 ResultId("eir", "FrameSource", "LiveDir", "ConnectFailure"),
                 "Directory %1 does not exist",
                 "The directory specified in the URL does not exist"
                 "or could not be created.",
                        QStringList() << "DirName");

