cmake_minimum_required(VERSION 3.16)

project(TicTacTueServer LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.8 REQUIRED COMPONENTS Core Network)
qt_standard_project_setup()

add_executable(TicTacTueServer
  main.cpp
  tictactueserver.h tictactueserver.cpp
  game.h
  countdowntimer.h
  board.h
  board.cpp
  countdowntimer.cpp
  game.cpp
  player.h
  player.cpp
  gameroom.h gameroom.cpp

)
target_link_libraries(TicTacTueServer Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)

include(GNUInstallDirs)
install(TARGETS TicTacTueServer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
