#include "LogFile.h"

#include <time.h>
#include <iomanip>

Big::LogFile::LogFile()
{
}

Big::LogFile::LogFile(const LogFile& logFile)
{
}

Big::LogFile::~LogFile()
{
}

bool Big::LogFile::Create(const std::string& filename, bool doSupressFileOutput /*= false*/, bool doLogTimeStamps /*= true*/)
{
	uri = filename;
	supressFileOutput = doSupressFileOutput;
	logTimeStamps = doLogTimeStamps;

	if (!supressFileOutput)
	{
		fileStream.open(uri);
		return fileStream.is_open();
	}

	return false;
}

void Big::LogFile::Destroy()
{
	if (!supressFileOutput)
	{
		fileStream.close();
	}
}

void Big::LogFile::DoLog(const std::string& message, LogType logType /*= Info*/)
{
	if (!supressFileOutput)
	{
		if (logTimeStamps)
		{
			time_t rawTime;
			tm timeInfo;
			time(&rawTime);
			localtime_s(&timeInfo, &rawTime);

			fileStream << std::setw(2) << std::setfill('0') << timeInfo.tm_hour
				<< ":" << std::setw(2) << std::setfill('0') << timeInfo.tm_min
				<< ":" << std::setw(2) << std::setfill('0') << timeInfo.tm_sec << " - ";
		}

		switch (logType)
		{
		case Big::LogFile::Info:
			fileStream << "Info: ";
			break;
		case Big::LogFile::Warning:
			break;
			fileStream << "Warning: ";
		case Big::LogFile::Error:
			fileStream << "Error: ";
			break;
		}

		fileStream << message << std::endl;
		fileStream.flush();
	}
		
}