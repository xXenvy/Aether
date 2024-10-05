#include <iostream>
#include <fstream>
#include <sstream>

#include "include/parser.hpp"
#include "../errors.hpp"

namespace aether {
    aether::HttpRequest HttpParser::parse(std::string request_data) {
        aether::HttpRequest request;
        parseGeneral(request_data, request);
        parseHeaders(request_data, request);
        parseBody(request_data, request);
        return request;
    }

    std::string HttpParser::parse(aether::HttpResponse response) {
        std::string result = "HTTP/1.0 " + std::to_string(response.status) + "\n";
        if (response.contentType == "") {
            result += "Content-Type: " + getContentType(response.body) + "\n";
        }
        else {
            result += "Content-Type: " + response.contentType + "\n";
        }
        result += "Content-Length: " + std::to_string(response.body.size()) + "\n";
        result += "\n" + response.body;
        return result;
    }

    const std::string HttpParser::getContentType(const std::string& body) {
        if (body.empty()) return "text/html; charset=UTF-8";
        if ((body.front() == '{' && body.back() == '}') ||
            (body.front() == '[' && body.back() == ']')) {
            return "application/json";
        }
        if (body.find("<?xml") != std::string::npos) return "application/xml";
        return "text/html; charset=UTF-8";
    }

    const std::string HttpParser::readUntil(std::string& source, char end_char) {
        const size_t position = source.find(end_char);
        if (position == std::string::npos) {
            throw aether::ParsingError("End char not found.");
        }
        const std::string content = source.substr(0, position);
        source.erase(0, position + 1);
        return content;
    }

    inline void HttpParser::trim(std::string& source) noexcept {
        if (source.empty()) return;
        while (source.front() == '\n' || source.front() == ' ') {
            try {
                readUntil(source, source.front());
            }
            catch (const aether::ParsingError&) { break; }
        }
    }

    void HttpParser::parseGeneral(std::string& source, aether::HttpRequest& req) {
        trim(source);
        static const std::unordered_map<std::string, aether::Method> methodMap = {
            {"GET", aether::Method::GET},
            {"POST", aether::Method::POST},
            {"PUT", aether::Method::PUT},
            {"PATCH", aether::Method::PATCH},
            {"DELETE", aether::Method::DELETE_},
            {"HEAD", aether::Method::HEAD},
            {"OPTIONS", aether::Method::OPTIONS}
        };
        const std::string method = readUntil(source, ' ');
        auto it = methodMap.find(method);
        if (it == methodMap.end()) {
            throw aether::ParsingError("Invalid method.");
        }

        req.method = it->second;
        req.path = readUntil(source, ' ');
        req.version = readUntil(source, '\n');
    }

    void HttpParser::parseHeaders(std::string& source, aether::HttpRequest& req) {
        trim(source);
        if (source.find(':') == std::string::npos) return;

        while (true) {
            const std::string current_line = readUntil(source, '\n');
            const size_t colon_pos = current_line.find(':');
            if (colon_pos == std::string::npos) break;

            std::string key = current_line.substr(0, colon_pos);
            std::string value = current_line.substr(colon_pos + 1, current_line.length());
            trim(key);
            trim(value);
            req.headers[key] = value;
        }
    }

    void HttpParser::parseBody(std::string& source, aether::HttpRequest& req) {
        trim(source);
        req.body = source;
    }

    std::string renderHtmlFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw FileOpenError("Failed to open a file: " + path);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
};