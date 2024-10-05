#include <catch2/catch_test_macros.hpp>

#include "../src/parsing/include/parser.hpp"
#include "../src/errors.hpp"

TEST_CASE("HttpParser parses GET request correctly", "[HttpParser]") {
    std::string request_data =
        "GET /home HTTP/1.1\n"
        "Host: localhost\n"
        "Connection: keep-alive\n"
        "\n"
        "Body content";
    aether::HttpParser parser;
    aether::HttpRequest request = parser.parse(request_data);

    REQUIRE(request.method == aether::Method::GET);
    REQUIRE(request.path == "/home");
    REQUIRE(request.version == "HTTP/1.1");
    REQUIRE(request.headers["Host"] == "localhost");
    REQUIRE(request.headers["Connection"] == "keep-alive");
    REQUIRE(request.body == "Body content");
}

TEST_CASE("HttpParser parses POST request with JSON body", "[HttpParser]") {
    std::string request_data =
        "POST /submit HTTP/1.1\n"
        "Host: example.com\n"
        "Content-Type: application/json\n"
        "Content-Length: 18\n"
        "\n"
        "{\"key\":\"value\"}";
    aether::HttpParser parser;
    aether::HttpRequest request = parser.parse(request_data);

    REQUIRE(request.method == aether::Method::POST);
    REQUIRE(request.path == "/submit");
    REQUIRE(request.version == "HTTP/1.1");
    REQUIRE(request.headers["Host"] == "example.com");
    REQUIRE(request.headers["Content-Type"] == "application/json");
    REQUIRE(request.headers["Content-Length"] == "18");
    REQUIRE(request.body == "{\"key\":\"value\"}");
}

TEST_CASE("HttpParser handles request with missing headers", "[HttpParser]") {
    std::string request_data =
        "DELETE /home HTTP/1.1\n"
        "\n"
        "Body content";
    aether::HttpParser parser;
    aether::HttpRequest request = parser.parse(request_data);

    REQUIRE(request.method == aether::Method::DELETE_);
    REQUIRE(request.path == "/home");
    REQUIRE(request.version == "HTTP/1.1");
    REQUIRE(request.headers.empty());
    REQUIRE(request.body == "Body content");
}

TEST_CASE("HttpParser trims leading and trailing whitespace", "[HttpParser]") {
    std::string request_data =
        "   \n"
        "       PUT /home HTTP/1.1\n"
        "   Host: example.com\n"
        "\n"
        "   Body content\n";
    aether::HttpParser parser;
    aether::HttpRequest request = parser.parse(request_data);

    REQUIRE(request.method == aether::Method::PUT);
    REQUIRE(request.path == "/home");
    REQUIRE(request.version == "HTTP/1.1");
    REQUIRE(request.headers["Host"] == "example.com");
    REQUIRE(request.body == "Body content\n");
}

TEST_CASE("HttpParser generates HTTP response for HTML content", "[HttpParser]") {
    aether::HttpResponse response(200, "<h1>Text</h1>");
    aether::HttpParser parser;
    std::string result = parser.parse(response);

    REQUIRE(result == R"(HTTP/1.0 200
Content-Type: text/html; charset=UTF-8
Content-Length: 13

<h1>Text</h1>)");
}

TEST_CASE("HttpParser generates HTTP response for JSON content", "[HttpParser]") {
    aether::HttpResponse response(200, R"({"name": "Adam", "age": 34})");
    aether::HttpParser parser;
    std::string result = parser.parse(response);

    REQUIRE(result == R"(HTTP/1.0 200
Content-Type: application/json
Content-Length: 27

{"name": "Adam", "age": 34})");
}

TEST_CASE("HttpParser generates HTTP response for empty body", "[HttpParser]") {
    aether::HttpResponse response(400, "", "text/html");
    aether::HttpParser parser;
    std::string result = parser.parse(response);

    REQUIRE(result == R"(HTTP/1.0 400
Content-Type: text/html
Content-Length: 0

)");
}