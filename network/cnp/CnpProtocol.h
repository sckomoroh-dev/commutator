//
// Created by Anna on 06.05.18.
//

#ifndef COMMUTATOR_CNPPROTOCOL_H
#define COMMUTATOR_CNPPROTOCOL_H

#include <string>
#include <map>
#include <list>
#include <sstream>

class CnpProtocol
{
protected:
    template<typename TCommand>
    const std::string buildRequestBody(TCommand&& command) const noexcept
    {
        std::ostringstream requestBodyStream;
        requestBodyStream << "CNP v1.0" << std::endl;
        requestBodyStream << "Command: " << command << std::endl << std::endl << "\0";

        return requestBodyStream.str();
    }

    template<typename TCommand, typename TData>
    const std::string buildRequestBody(TCommand&& command, TData&& data) const noexcept
    {
        std::ostringstream requestBodyStream;
        requestBodyStream << "CNP v1.0" << std::endl;
        requestBodyStream << "Command: " << command << std::endl;
        requestBodyStream << "Data: " << data << std::endl << std::endl << "\0";

        return requestBodyStream.str();
    };

    template<typename TCommand, typename TStatus, typename TData>
    std::string buildResponseBody(TCommand&& command, TStatus&& status, TData&& data)
    {
        std::ostringstream requestBodyStream;
        requestBodyStream << "CNP v1.0" << std::endl;
        requestBodyStream << "Command: " << command << std::endl;
        requestBodyStream << "Status: " << status << std::endl;
        requestBodyStream << "Data: " << data << std::endl << std::endl << "\0";

        return requestBodyStream.str();
    };

    std::map<std::string, std::string> parseRequest(const char* request) noexcept;

    std::list<std::string> splitRequestString(const std::string& inputString,
                                       char delimeter,
                                       bool single,
                                       std::function<std::string(std::string)> itemProcessingFunc) noexcept;

    std::pair<std::string, std::string> parseRequestFields(const std::string& requestField) noexcept;
};


#endif //COMMUTATOR_CNPPROTOCOL_H
