cmake_minimum_required(VERSION 3.21)
project(server LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(server src/main.cpp)
target_link_libraries(server PRIVATE sfml-network)
target_compile_features(server PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:server> $<TARGET_FILE_DIR:server> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS server)
