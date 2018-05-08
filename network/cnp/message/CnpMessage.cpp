//
// Created by sckomoroh on 5/8/18.
//

#include "CnpMessage.h"
#include "../CnpException.h"

std::map<std::string, std::string> CnpMessage::requestToMap(const std::string &request)
{
    std::map<std::string, std::string> result;
    auto requestFields = splitMessageString(request, '\n', false, [](std::string item)
    { return item; });

    for (const auto &requestField : requestFields)
    {
        if (requestField.substr(0, 3) == "CNP")
        {
            auto messageVersion = getMessageVersion(requestField);
            if (messageVersion.length() > 0)
            {
                result.insert(std::make_pair("Version", messageVersion));
            }

            continue;
        }

        auto keyValue = parseMessageItem(requestField);

        result.insert(keyValue);
    }

    return result;
}

CnpVersion CnpMessage::stringToVersion(const std::string &strVersion)
{
    if (strVersion == "1.0")
    {
        return CnpVersion::Version10;
    }

    throw CnpException("Invalid CNP version");
}

const std::string CnpMessage::versionToString(CnpVersion version)
{
    switch (version)
    {
        case CnpVersion::Version10:
            return "1.0";
    }

    throw CnpException("Invalid CNP version");
}

const std::string CnpMessage::getMessageVersion(const std::string &requestString)
{
    auto separateIndex = requestString.find(' ');
    if (separateIndex == std::string::npos)
    {
        return "";
    }

    auto versionString = requestString.substr(separateIndex + 1);
    if (versionString[0] != 'v')
    {
        return "";
    }

    return versionString.substr(1);
}

std::list<std::string> CnpMessage::splitMessageString(const std::string &inputString,
                                                      char delimiter,
                                                      bool single,
                                                      std::function<std::string(std::string)> itemProcessingFunc)
{
    std::list<std::string> result;
    auto pos = 0ul;
    auto nextPos = 0ul;

    do
    {
        pos = inputString.find(delimiter, nextPos);
        if (pos != std::string::npos)
        {
            auto requestItem = inputString.substr(nextPos, pos - nextPos);
            requestItem = itemProcessingFunc(requestItem);
            if (requestItem.length() > 0)
            {
                result.push_back(requestItem);
            }

            nextPos = pos + 1;
        }
    }
    while (pos != std::string::npos && !single);

    auto requestItem = inputString.substr(nextPos);
    requestItem = itemProcessingFunc(requestItem);
    if (requestItem.length() > 0)
    {
        result.push_back(requestItem);
    }

    return result;
}

std::pair<std::string, std::string> CnpMessage::parseMessageItem(const std::string &requestField)
{
    auto requestKeyValue = splitMessageString(requestField, ':', true, [](std::string item)
    {
        // Trim spaces on start and end of the string

        auto start = 0;
        while (item[start] == ' ')
        {
            start++;
        }

        auto end = item.length();
        while (item[end - 1] == ' ')
        {
            end--;
        }

        return item.substr(start, end - start);
    });

    auto key = *requestKeyValue.begin();
    auto value = *(++requestKeyValue.begin());

    return std::make_pair<std::string, std::string>(std::move(key), std::move(value));
}
