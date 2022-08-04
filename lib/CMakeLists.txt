option(QTK_DEBUG "Enable debugger" ON)
option(BUILD_SHARED_LIBS "Build shared library" ON)

find_package(assimp REQUIRED)
option(ASSIMP_NEW_INTERFACE "Use assimp::assimp as target instead of assimp" OFF)

set(PUBLIC_HEADERS
    qtkwidget.h
    qtkapi.h
    qtkscene.h
    qtkcamera3d.h
    qtktransform3d.h
    qtkmeshrenderer.h
    qtkmodel.h
    qtkskybox.h
    qtktexture.h
    qtkmodelmesh.h
    qtkobject.h
    qtkshaders.h
)
set(SOURCE_FILES
    qtkwidget.cpp
    qtkscene.cpp
    qtkinput.h
    qtkinput.cpp
    qtkcamera3d.cpp
    qtktransform3d.cpp
    qtkmeshrenderer.cpp
    qtkmodel.cpp
    qtkskybox.cpp
    qtkmesh.h
    qtkmesh.cpp
    qtktexture.cpp
    qtkmodelmesh.cpp
    qtkobject.cpp
    qtkshaders.cpp
)

qt_add_library(qtk 
    ${PUBLIC_HEADERS}
    ${SOURCE_FILES}
)
set_target_properties(qtk PROPERTIES
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
    VERSION ${PROJECT_VERSION}
)
target_include_directories(qtk PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
if(BUILD_SHARED_LIBS)
    target_compile_definitions(qtk PRIVATE QTK_EXPORT PUBLIC QTK_SHARED)
    target_link_libraries(qtk PRIVATE Qt6::OpenGLWidgets)
    if(ASSIMP_NEW_INTERFACE)
        target_link_libraries(qtk PRIVATE assimp::assimp)
    else()
        target_link_libraries(qtk PRIVATE assimp)
    endif()
else()
    target_compile_definitions(qtk PUBLIC QTK_STATIC)
    target_link_libraries(qtk PUBLIC Qt6::OpenGLWidgets)
    if(ASSIMP_NEW_INTERFACE)
        target_link_libraries(qtk PUBLIC assimp::assimp)
    else()
        target_link_libraries(qtk PUBLIC assimp)
    endif()
endif()
if(QTK_DEBUG)
    target_compile_definitions(qtk PUBLIC QTK_DEBUG)
endif()
if(WIN32)
    find_package(OpenGL REQUIRED)
    target_link_libraries(qtk PUBLIC OpenGL::GL)
endif()

# Install files
install(TARGETS qtk
    EXPORT qtk-export
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
)

# Install export
install(EXPORT qtk-export
    FILE qtkTargets.cmake
    CONFIGURATIONS Debug|Release
    DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
)

# Install config file
#install(FILES qtkConfig.cmake
#    CONFIGURATIONS Debug|Release
#    DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
#)
