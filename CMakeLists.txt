cmake_minimum_required (VERSION 3.11)
project(wiegand_analyzer)

# custom CMake Modules are located in the cmake directory.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(ExternalAnalyzerSDK)

set(SOURCES 
src/WiegandAnalyzer.cpp
src/WiegandAnalyzer.h
src/WiegandAnalyzerResults.cpp
src/WiegandAnalyzerResults.h
src/WiegandAnalyzerSettings.cpp
src/WiegandAnalyzerSettings.h
src/WiegandSimulationDataGenerator.cpp
src/WiegandSimulationDataGenerator.h
)

add_analyzer_plugin(wiegand_analyzer SOURCES ${SOURCES})
