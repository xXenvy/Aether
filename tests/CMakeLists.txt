set(SRC
    test_parser.cpp
    test_router.cpp
)
set(Catch2Version v3.7.1)
Include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG ${Catch2Version}
)
FetchContent_MakeAvailable(Catch2)

add_executable(Tests ${SRC})
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain AetherLib)

include(CTest)
include(Catch)
catch_discover_tests(Tests)