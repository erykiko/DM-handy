cmake_minimum_required(VERSION 3.14)
project(dm_handy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
        GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)

add_executable(dm_handy main.cpp
        CharacterCard.cpp
        CharacterCard.h
        Enums.h
        DiceRoller.h
        CharacterManager.h
        InitiativeTracker.h)

target_link_libraries(dm_handy PRIVATE ftxui::screen ftxui::dom ftxui::component)