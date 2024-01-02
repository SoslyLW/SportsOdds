cmake_minimum_required( VERSION "3.19.2" )

project( SportsOdds VERSION 1.0 )

add_executable( oddsMain oddsMain.cpp src/League.cpp src/Game.cpp src/SimulationDB.cpp src/SimulationTester.cpp src/Team.cpp src/SimulationHistory.cpp src/TimeFunctions.cpp ) # put your cpp files in a src/ directory
target_include_directories( oddsMain PUBLIC include ) # put the headers in an include/ directory
file(GLOB MY_FILES "*.csv") 
file(COPY ${MY_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB MY_FILES "*.txt") 
file(COPY ${MY_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
target_compile_features( oddsMain PUBLIC cxx_std_11 )