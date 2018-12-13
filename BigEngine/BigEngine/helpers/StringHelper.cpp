#include "StringHelper.h"
#include <locale>

Big::StringHelper::StringHelper()
{
}


Big::StringHelper::~StringHelper()
{
}

bool Big::StringHelper::StringToBool(const std::string& value)
{
	if (value.compare("true") || value.compare("1"))
	{
		//Todo: Fix to lowercase.
		return false;
	}

	return true;
}

int Big::StringHelper::StringToInt(const std::string& value)
{
	return std::stoi(value);
}

void Big::StringHelper::BoolToString(const bool& value, std::string& outValue)
{
	if (value)
	{
		outValue = '1';
	}
	else
	{
		outValue = '0';
	}
}

void Big::StringHelper::IntToString(const int& value, std::string& outValue)
{
	outValue = std::to_string(value);
}
