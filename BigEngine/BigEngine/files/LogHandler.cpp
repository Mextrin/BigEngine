#include "LogHandler.h"
#include <assert.h>
#include "LogFile.h"
#include "GLFW/glfw3.h"

Big::LogHandler* Big::LogHandler::instance = nullptr;
Big::LogFile* Big::LogHandler::defaultLog = nullptr;

Big::LogHandler* Big::LogHandler::InstancePointer()
{
	assert(instance != nullptr);
	return instance;
}

Big::LogHandler& Big::LogHandler::Instance()
{
	assert(instance != nullptr);
	return *instance;
}

Big::LogHandler* Big::LogHandler::CreateInstance(const std::string& folderPath)
{
	if (!instance)
	{
		//Todo: Create folder if it doesn't exist
		glfwSetErrorCallback(glfwErrorCallback);
		instance = new LogHandler(folderPath);
	}

	return instance;
}

void Big::LogHandler::DestroyInstance()
{
	if (instance)
	{
		for (std::map<std::string, class LogFile*>::iterator it = instance->logs.begin(); it != instance->logs.end(); it++)
		{
			it->second->Destroy();
			delete it->second;
		}
		defaultLog = nullptr;
		delete instance;
		instance = nullptr;
	}
}

bool Big::LogHandler::CreateLog(const std::string name, bool doSupressFileOutput /*= false*/, bool doLogTimestamps /*= false*/)
{
	if (logs.find(name) == logs.end())
	{
		LogFile* log = new LogFile();
		if (log->Create(basePath + name, doSupressFileOutput, doLogTimestamps))
		{
			if (logs.empty())
			{
				defaultLog = log;
			}
			logs.insert(std::make_pair(name, log));
			return true;
		}
		delete log;
	}
	return false;
}

void Big::LogHandler::DoLog(const std::string& message, LogFile::LogType logType /* = LogFile::LogType::Info*/)
{
	assert(defaultLog != nullptr);
	defaultLog->DoLog(message, logType);
}

void Big::LogHandler::DoLog(const std::string& name, const std::string& message, LogFile::LogType logType /*= LogFile::LogType::Info*/)
{
	std::map<std::string, LogFile*>::iterator it = logs.find(name);
	if (it != logs.end())
	{
		it->second->DoLog(message, logType);
	}
}

void Big::LogHandler::glfwErrorCallback(int error, const char* description)
{
	DoLog(description, LogFile::LogType::Error);
}

Big::LogHandler::LogHandler()
{
}


Big::LogHandler::LogHandler(const std::string& pathToFiles) : basePath(pathToFiles)
{
	
}

Big::LogHandler::~LogHandler()
{
}

