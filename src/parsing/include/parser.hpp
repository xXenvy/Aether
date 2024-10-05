#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "http_types.hpp"

namespace aether {
    class HttpParser {
    public:
        aether::HttpRequest parse(std::string request);
        std::string parse(aether::HttpResponse response);
    private:
        const std::string readUntil(std::string& source, char end_char);
        const std::string getContentType(const std::string& body);
        inline void trim(std::string& source) noexcept;

        void parseGeneral(std::string& source, aether::HttpRequest& request);
        void parseHeaders(std::string& source, aether::HttpRequest& request);
        void parseBody(std::string& source, aether::HttpRequest& request);
    };

    std::string renderHtmlFile(const std::string& path);

};
#endif // PARSER_H