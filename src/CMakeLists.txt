set(SRC
	network/server.cpp
	network/session.cpp
	network/router.cpp
	parsing/parser.cpp

	parsing/include/parser.hpp
	parsing/include/http_types.hpp
)
find_package(Boost 1.86.0 REQUIRED COMPONENTS system log log_setup)

add_executable(Aether main.cpp ${SRC})
target_include_directories(Aether PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(Aether ${Boost_LIBRARIES})

add_library(AetherLib ${SRC})
target_include_directories(AetherLib PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(AetherLib PUBLIC ${Boost_LIBRARIES})