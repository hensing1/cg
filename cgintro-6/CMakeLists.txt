cmake_minimum_required(VERSION 3.26)

set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

if(APPLE)
    # Enable universal building on macOS
    set(CMAKE_OSX_ARCHITECTURES x86_64;arm64)
    set(CMAKE_MACOSX_BUNDLE True)
elseif(WIN32)
    # Make the application run without a console window in Release mode
    if($<CONFIG:Release>)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    endif()
endif()

project(cgintro-6 VERSION 1.0 LANGUAGES CXX C)
set(PROJECT_IDENTIFIER cgintro)
if(WIN32)
    set(CMAKE_CXX_STANDARD 20)
else()
    set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED True)

####################################################################
# Fetching libraries
include(FetchContent)

# GLM
add_compile_definitions(GLM_FORCE_RADIANS)
FetchContent_Declare(
    glm
    #GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    URL https://github.com/g-truc/glm/archive/1.0.1.tar.gz
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(glm)

# stb
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG ae721c50eaf761660b4f90cc590453cdb0c2acd0
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(stb)

add_library(stb_impl INTERFACE)
target_compile_definitions(stb_impl INTERFACE STB_IMAGE_IMPLEMENTATION)
target_include_directories(stb_impl INTERFACE ${stb_SOURCE_DIR})

####################################################################
# Source files
set(SRC
    src/main.cpp
)
set(INCLUDE
    src/
    src/framework/
)

####################################################################
# Resource files
set(ICON
    icons/AppIcon.icns
)
set(RES "${SHADERS};${MESHES};${TEXTURES}")
set_source_files_properties(${RES} PROPERTIES HEADER_FILE_ONLY True)

####################################################################
# Add executable
add_executable(${PROJECT_NAME}
    ${SRC}
    ${RES}
    ${ICON}
)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE})

target_link_libraries(${PROJECT_NAME} glm stb_impl)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/src/)

configure_file("src/config.hpp.in" "src/config.hpp")

####################################################################
# CPack deployment
if(APPLE)
    # Bundling macOS application
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUNDLE True
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_GUI_IDENTIFIER ${PROJECT_IDENTIFIER}.${PROJECT_NAME}
        MACOSX_BUNDLE_ICON_FILE AppIcon
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
    )

    # Copy resource files into bundle
    foreach(RES_FILE IN LISTS RES)
        cmake_path(GET RES_FILE PARENT_PATH LOC)
        set_source_files_properties(${RES_FILE}
            PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources/${LOC}"
        )
    endforeach()

    set_source_files_properties(${ICON}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )

    # Packaging macOS installer
    install(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION .)
    set(CPACK_GENERATOR DragNDrop)
    
elseif(WIN32)
    foreach(RES_FILE IN LISTS RES)
        cmake_path(GET RES_FILE PARENT_PATH LOC)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/${RES_FILE} $<TARGET_FILE_DIR:${PROJECT_NAME}>/${RES_FILE}
            COMMENT "Copying ${CMAKE_SOURCE_DIR}/${RES_FILE} to $<TARGET_FILE_DIR:${PROJECT_NAME}>/${RES_FILE}"
            VERBATIM
        )
        install(FILES ${RES_FILE} DESTINATION ${LOC})
    endforeach()

    # Packaging Windows portable
    include(InstallRequiredSystemLibraries)
    install(TARGETS ${PROJECT_NAME} DESTINATION .)
    set(CPACK_GENERATOR ZIP) # For portable ZIP
    #set(CPACK_GENERATOR NSIS) # For .EXE installer, requires NSIS installed (winget install NSIS.NSIS)
    #set(CPACK_GENERATOR WIX) # For .MSI installer, requires WiX Toolset installed
endif()
set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)
include(CPack)

####################################################################
# Zipping sources
set(ARCHIVE_FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.zip)
file(GLOB_RECURSE SRC_FILES *)
# Ignore build files
list(FILTER SRC_FILES EXCLUDE REGEX ".*.DS_Store$|build//*|.git//*|composed//*|.vscode//*")

add_custom_target(zip ALL
    COMMAND ${CMAKE_COMMAND} -E tar c "${ARCHIVE_FILE}" --format=zip -- ${SRC_FILES}
    COMMENT "Zipping sources"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    VERBATIM
)
