//
// Created by sckomoroh on 5/8/18.
//

#ifndef COMMUTATOR_CNPMESSAGE_H
#define COMMUTATOR_CNPMESSAGE_H

#include <map>
#include <list>
#include <functional>

#include "CnpVersions.h"

namespace network
{
    namespace cnp
    {
        namespace message
        {
            class CnpMessage
            {
            public:
                CnpMessage(const CnpMessage &) = delete;

                CnpMessage(CnpMessage &&) = delete;

                // TODO: Make deleted the operator =

            protected:
				CnpMessage() = default;
			
            public:
                virtual std::string toString() const = 0;

            protected:
                static std::map<std::string, std::string> requestToMap(const std::string &request);

                static CnpVersion stringToVersion(const std::string &strVersion);

                static const std::string versionToString(CnpVersion version);

            private:
                static const std::string getMessageVersion(const std::string &requestString);

                static std::list<std::string> splitMessageString(const std::string &inputString,
                                                                 char delimiter,
                                                                 bool single,
                                                                 std::function<std::string(
                                                                         std::string)> itemProcessingFunc);

                static std::pair<std::string, std::string> parseMessageItem(const std::string &requestField);;
            };
        }
    }
}

#endif //COMMUTATOR_CNPMESSAGE_H
