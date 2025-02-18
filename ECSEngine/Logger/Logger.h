#pragma once

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>

enum LogType
{
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

struct LogEntry
{
    LogType type;
    std::string message;

    LogEntry(LogType type, const std::string& message);
    
};

class Logger
{
public:
    static void Log(const std::string& msg);
    static void Warning(const std::string& msg);
    static void Error(const std::string& msg);

    static const std::vector<LogEntry>& GetMessage();
    
private:
    static std::string GetCurrentTime();

    static std::vector<LogEntry> messages;
};

#endif