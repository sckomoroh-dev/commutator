//
// Created by Anna on 06.05.18.
//

#include "CnpProtocol.h"



std::map<std::string, std::string> CnpProtocol::parseRequest(const char *request) noexcept
{
    std::map<std::string, std::string> result;
    std::string requestString(request);
    auto requestFields = splitRequestString(requestString, '\n', false, [](std::string item){ return item; });

    for (const auto &requestField : requestFields)
    {
        if (requestField.substr(0, 3) == "CNP")
        {
            continue;
        }

        auto keyValue = parseRequestFields(requestField);

        result.insert(keyValue);
    }

    return result;
}

std::list<std::string> CnpProtocol::splitRequestString(const std::string &inputString,
                                                       char delimeter,
                                                       bool single,
                                                       std::function<std::string(std::string)> itemProcessingFunc) noexcept
{
    std::list<std::string> result;
    auto pos = 0ul;
    auto nextPos = 0ul;

    do
    {
        pos = inputString.find(delimeter, nextPos);
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

std::pair<std::string, std::string> CnpProtocol::parseRequestFields(const std::string& requestField) noexcept
{
    auto requestKeyValue = splitRequestString(requestField, ':', true, [](std::string item)
    {
        // Trim spaces on start and end of the string

        auto start = 0;
        while (item[start] == ' ')
        {
            start++;
        }

        auto end = item.length();
        while (item[end-1] == ' ')
        {
            end--;
        }

        return item.substr(start, end - start);
    });

    auto key = *requestKeyValue.begin();
    auto value = *(++requestKeyValue.begin());

    return std::make_pair<std::string, std::string>(std::move(key), std::move(value));
}
