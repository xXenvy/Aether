#include <catch2/catch_test_macros.hpp>
#include "../src/network/include/router.hpp"

TEST_CASE("Default callback returns 404 with empty response", "[Router]") {
    aether::HttpRequest request;
    aether::Router router;

    aether::Callback default_callback = router.getCallback("/invalid_path");
    aether::HttpResponse response = default_callback(request);
    REQUIRE(response.status == 404);
    REQUIRE(response.contentType == "");
    REQUIRE(response.body == "");
}

TEST_CASE("Custom default callback returns 404 with custom body", "[Router]") {
    aether::HttpRequest request;
    aether::Router router;
    router.setDefaultCallback([](aether::HttpRequest&) {
        return aether::HttpResponse(404, "<h1>Site not found</h1>");
    });

    aether::Callback default_callback = router.getCallback("/invalid_path");
    aether::HttpResponse response = default_callback(request);
    REQUIRE(response.status == 404);
    REQUIRE(response.contentType == "");
    REQUIRE(response.body == "<h1>Site not found</h1>");
}

TEST_CASE("Custom route callback returns 200 with 'Hello' in body", "[Router]") {
    aether::HttpRequest request;
    aether::Router router;
    router.addCallback("/home", [](aether::HttpRequest&) {
        return aether::HttpResponse(200, "<p>Hello</p>");
    });

    for (int i = 0; i < 2; i++) {
        aether::Callback callback = router.getCallback("/home");
        aether::HttpResponse response = callback(request);

        REQUIRE(response.status == 200);
        REQUIRE(response.body == "<p>Hello</p>");
        REQUIRE(response.contentType == "");
    }
}