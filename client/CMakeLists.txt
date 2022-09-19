cmake_minimum_required(VERSION 3.21)
project(client LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(client src/main.cpp)
target_link_libraries(client PRIVATE sfml-graphics sfml-network)
target_compile_features(client PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:client> $<TARGET_FILE_DIR:client> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS client)
