#include "Logger.h"

#include <chrono>
#include <iostream>

std::vector<LogEntry> Logger::messages;

LogEntry::LogEntry(LogType type, const std::string& message) : type(type), message(message)
{
}

void Logger::Log(const std::string& msg)
{
    std::cout << "\x1B[32m" <<"LOG : [ " << GetCurrentTime() << " ] - " << msg <<"\033[0m" << '\n';
    messages.emplace_back(LOG_INFO, msg);
}

void Logger::Warning(const std::string& msg)
{
    std::cout << "\x1B[93m" << "WARNING : [ " << GetCurrentTime() << " ] - " << msg <<"\033[0m" << '\n'; 
    messages.emplace_back(LOG_WARNING, msg);
}

void Logger::Error(const std::string& msg)
{
    std::cout << "\x1B[91m" << "ERROR : [ " << GetCurrentTime() << " ] - " << msg <<"\033[0m" << '\n'; 
    messages.emplace_back(LOG_ERROR, msg);
}

const std::vector<LogEntry>& Logger::GetMessage()
{
    return messages;
}

std::string Logger::GetCurrentTime()
{
    tm temp;
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string ret(30, '\0');
    (void)localtime_s(&temp, &t);
    (void)std::strftime(&ret[0], ret.size(), "%d-%b-%Y %H:%M:%S", &temp);
    return ret;
}
