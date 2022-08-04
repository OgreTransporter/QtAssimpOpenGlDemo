file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/resources.h "#ifndef RESOURCES_H\n#define RESOURCES_H\n#define DEMO_RESOURCES_DIR \"${CMAKE_SOURCE_DIR}/resources\"\n#endif\n")

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    examplescene.h
    examplescene.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/resources.h
)

qt_add_executable(demo ${PROJECT_SOURCES})
target_link_libraries(demo PRIVATE Qt6::OpenGLWidgets qtk)

set_target_properties(demo PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS demo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(WIN32)
    get_target_property(_qt6_qmake_location Qt6::qmake IMPORTED_LOCATION)
    execute_process(COMMAND "${_qt6_qmake_location}" -query QT_INSTALL_PREFIX RESULT_VARIABLE return_code OUTPUT_VARIABLE qt6_install_prefix OUTPUT_STRIP_TRAILING_WHITESPACE)
    file(TO_NATIVE_PATH "${qt6_install_prefix}/bin" qt6_install_prefix)
    if(TARGET Qt6::windeployqt)
        add_custom_command(TARGET demo
            POST_BUILD
            COMMAND set PATH=%PATH%$<SEMICOLON>${qt6_install_prefix}
            COMMAND Qt6::windeployqt --dir "${CMAKE_BINARY_DIR}/windeployqt" "$<TARGET_FILE_DIR:demo>/$<TARGET_FILE_NAME:demo>"
        )
        install(DIRECTORY "${CMAKE_BINARY_DIR}/windeployqt/" DESTINATION bin)
    endif()
    if(MSVC AND TARGET Qt6::qmake)
        set(VSUSER_FILE ${CMAKE_CURRENT_BINARY_DIR}/demo.vcxproj.user)
        file(TO_NATIVE_PATH "${assimp_DIR}/bin" assimp_bin)
        file(WRITE  ${VSUSER_FILE} "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n")
        file(APPEND ${VSUSER_FILE} "<Project xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n")
        file(APPEND ${VSUSER_FILE} "  <PropertyGroup>\n")
        file(APPEND ${VSUSER_FILE} "    <LocalDebuggerEnvironment>Path=$(SolutionDir)\\lib\\$(Configuration);${qt6_install_prefix};${assimp_bin};$(Path)\n")
        file(APPEND ${VSUSER_FILE} "$(LocalDebuggerEnvironment)</LocalDebuggerEnvironment>\n")
        file(APPEND ${VSUSER_FILE} "    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>\n")
        file(APPEND ${VSUSER_FILE} "  </PropertyGroup>\n")
        file(APPEND ${VSUSER_FILE} "</Project>\n")
    endif()
endif()
