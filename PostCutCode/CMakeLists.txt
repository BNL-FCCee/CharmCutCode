cmake_minimum_required(VERSION 3.8)


project(PostCutCode)
message("announcing PostCutCode")
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-std=c++17 -fPIC -Wall -Wextra -Wno-unused-parameter -O2)

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO EG Minuit RooFit RooFitCore RooStats HistFactory Core RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread MultiProc TMVA)
# find_package(nlohmann_json REQUIRED)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

# compile options

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# link_directories(/cvmfs/sw.hsf.org/spackages7/intel-tbb/2020.3/x86_64-centos7-gcc11.2.0-opt/ey3ft/lib /cvmfs/sw.hsf.org/spackages7/zlib/1.2.13/x86_64-centos7-gcc11.2.0-opt/2wmsk/lib)

#---Create a shared library with generated dictionary
file(GLOB sources "Root/*.cxx")

add_library(PostCutCodeLib SHARED ${sources})
target_link_libraries(PostCutCodeLib ${ROOT_LIBRARIES})

#---Link the exe files
file(GLOB_RECURSE exe_files "utils/*.cxx")
foreach(_exe_file ${exe_files})
    get_filename_component(_theExec ${_exe_file} NAME_WE)
    add_executable(${_theExec} ${_exe_file})
    target_link_libraries(${_theExec} PostCutCodeLib)
endforeach()

set(ENV{WSCANNER_DATADIR} ${CMAKE_CURRENT_SOURCE_DIR}/data/)


# ---------- Generating the setup.sh file
set(SETUP ${CMAKE_CURRENT_BINARY_DIR}/../setup.sh)
file(WRITE ${SETUP} "#!/bin/bash\n")
file(APPEND ${SETUP} "# this is an auto-generated setup script\n" )
file(APPEND ${SETUP} "export PATH=\${PATH}:${CMAKE_CURRENT_BINARY_DIR}/\n")
