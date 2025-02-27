cmake_minimum_required(VERSION 3.28)
project(NtpConfiguration)

set(CMAKE_CXX_STANDARD 20)

set(Wt_DIR "/usr/local/lib/cmake/Wt")
find_package(Wt REQUIRED COMPONENTS all)
find_package(Boost REQUIRED COMPONENTS system filesystem)

add_executable(NtpConfiguration main.cpp
        MenuPage.cpp
        MenuPage.h
        TimeConfiguration/TimeWidget.cpp
        TimeConfiguration/TimeWidget.h
        TimeConfiguration/ShowTimeWidget.cpp
        TimeConfiguration/ShowTimeWidget.h
        TimeConfiguration/EditTimeWidget.cpp
        TimeConfiguration/EditTimeWidget.h
        TimeConfiguration/NtpWidget.cpp
        TimeConfiguration/NtpWidget.h
        StatusCommands/NtpStatusCommandsWidget.cpp
        StatusCommands/NtpStatusCommandsWidget.h)

target_link_libraries(NtpConfiguration PRIVATE
        Wt::Wt
        Wt::HTTP
        Wt::Dbo
        Boost::system
        Boost::filesystem)
