cmake_minimum_required(VERSION 3.2)
find_package(AnalysisBase QUIET)

IF(${AnalysisBase_FOUND})


    message("Hi I am here")
    # Declare the package name:
    atlas_subdir( wscanner )
    include_directories(WScanner)
    include_directories(../RooFitExtensions)
    include_directories(Root/atlasstyle)
    add_compile_definitions(WITH_ROOFITEXT)



    find_package(ROOT REQUIRED COMPONENTS RIO EG Minuit RooFit RooFitCore RooStats HistFactory Core RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread MultiProc TMVA)

    atlas_add_library(
        wscannerLib
        Root/*.cxx WScanner/*.h
        PUBLIC_HEADERS CPWorkspaces 
        INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} 
        LINK_LIBRARIES ${ROOT_LIBRARIES} RooFitExtensionsLib
        )

    file(GLOB_RECURSE exe_files "utils/*.cxx")
    foreach(_exe_file ${exe_files})
        get_filename_component(_theExec ${_exe_file} NAME_WE)
        atlas_add_executable(
            ${_theExec} ${_exe_file}
            INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
            LINK_LIBRARIES ${ROOT_LIBRARIES} wscannerLib
            )
    endforeach()

    atlas_install_scripts(scripts/*.py)
    atlas_install_python_modules(scripts/*.py)
    atlas_install_data(data/*)


ELSE()
    #cmake_policy(SET CMP0022 OLD)
    project(WScanner)
    message("announcing WScanner")
    set(CMAKE_BUILD_TYPE Debug)

    # You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
    #   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
    #   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
    list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

    #---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
    find_package(ROOT REQUIRED COMPONENTS RIO EG Minuit RooFit RooFitCore RooStats HistFactory Core RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread MultiProc TMVA)

    #---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
    include(${ROOT_USE_FILE})

    # compile options
    add_compile_options(-std=c++17 -fPIC -Wall -Wextra -Wno-unused-parameter -O2)

    #--- Find the roofit extension
    find_library(hasFoundRooFitExtension RooFitExtensions)
    if(NOT hasFoundRooFitExtension)
        message("Roofit extension not found. Compiling without it")
    else ()
        add_definitions(-DhasRooFitExtension)
    endif()

    if(hasFoundRooFitExtension)
        include_directories(${RooFitExtensions_INCLUDE_DIRS})
    endif()
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/WScanner)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Root/atlasstyle)

    #---Create a shared library with generated dictionary
    file(GLOB sources "Root/*.cxx")

    add_library(wscannerLib SHARED ${sources})

    if(hasFoundRooFitExtension)
        add_dependencies(wscannerLib RooFitExtensions)
        target_link_libraries(wscannerLib ${ROOT_LIBRARIES} RooFitExtensions)
    else()
        target_link_libraries(wscannerLib ${ROOT_LIBRARIES})
    endif()



    #---Link the exe files
    file(GLOB_RECURSE exe_files "utils/*.cxx")
    foreach(_exe_file ${exe_files})
        get_filename_component(_theExec ${_exe_file} NAME_WE)
        add_executable(${_theExec} ${_exe_file})
        target_link_libraries(${_theExec} wscannerLib)
    endforeach()

    set(ENV{WSCANNER_DATADIR} ${CMAKE_CURRENT_SOURCE_DIR}/data/)
    message("announcing WScanner" ${CMAKE_CURRENT_SOURCE_DIR})


    # ---------- Generating the setup.sh file
    if(hasFoundRooFitExtension)
        set(SETUP ${CMAKE_CURRENT_BINARY_DIR}/setup.sh)
        file(WRITE ${SETUP} "#!/bin/bash\n")
        file(APPEND ${SETUP} "# this is an auto-generated setup script\n" )
        file(APPEND ${SETUP} "source ${CMAKE_CURRENT_BINARY_DIR}/../RooFitExtensions/setup.sh\n")
        file(APPEND ${SETUP} "export wscanner_data=${CMAKE_CURRENT_SOURCE_DIR}/data/\n")
    else()
        set(SETUP ${CMAKE_CURRENT_BINARY_DIR}/../setup.sh)
        file(APPEND ${SETUP} "# this is an auto-generated setup script\n" )
        file(APPEND ${SETUP} "export PATH=\${PATH}:${CMAKE_CURRENT_BINARY_DIR}/\n")
        file(APPEND ${SETUP} "export wscanner_data=${CMAKE_CURRENT_SOURCE_DIR}/data/\n")    
    endif()

ENDIF()


