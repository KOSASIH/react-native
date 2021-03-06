cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

add_library(ReactCommon STATIC)

set(REACT_COMMON_YOGA_SOURCE
    yoga/yoga/event/event.cpp
    yoga/yoga/event/event.h
    yoga/yoga/CompactValue.h
    yoga/yoga/instrumentation.h
    yoga/yoga/log.cpp
    yoga/yoga/log.h
    yoga/yoga/Utils.cpp
    yoga/yoga/Utils.h
    yoga/yoga/YGConfig.cpp
    yoga/yoga/YGConfig.h
    yoga/yoga/YGEnums.cpp
    yoga/yoga/YGEnums.h
    yoga/yoga/YGFloatOptional.h
    yoga/yoga/YGLayout.cpp
    yoga/yoga/YGLayout.h
    yoga/yoga/YGMacros.h
    yoga/yoga/YGMarker.cpp
    yoga/yoga/YGMarker.h
    yoga/yoga/YGNode.cpp
    yoga/yoga/YGNode.h
    yoga/yoga/YGNodePrint.cpp
    yoga/yoga/YGNodePrint.h
    yoga/yoga/YGStyle.cpp
    yoga/yoga/YGStyle.h
    yoga/yoga/YGValue.cpp
    yoga/yoga/YGValue.h
    yoga/yoga/Yoga-internal.h
    yoga/yoga/Yoga.cpp
    yoga/yoga/Yoga.h
)

set(REACT_COMMON_MICRO_PROFILER_SOURCE
    microprofiler/MicroProfiler.cpp
    microprofiler/MicroProfiler.h
)

set(REACT_COMMON_JS_INSPECTOR_SOURCE
    jsinspector/InspectorInterfaces.cpp
    jsinspector/InspectorInterfaces.h
)

set(REACT_COMMON_JSC_CXXREACT_SOURCE
    jsi/JSCRuntime.cpp
    jsi/JSCRuntime.h
    jsi/jsi/decorator.h
    jsi/jsi/instrumentation.h
    jsi/jsi/jsi-inl.h
    jsi/jsi/jsi.cpp
    jsi/jsi/jsi.h
    jsi/jsi/jsilib-posix.cpp
    jsi/jsi/jsilib.h
    jsi/jsi/threadsafe.h
)

set(REACT_COMMON_CXXREACT_SOURCE
    jsi/jsi/JSIDynamic.cpp
    jsi/jsi/JSIDynamic.h
    jsiexecutor/jsireact/JSIExecutor.cpp
    jsiexecutor/jsireact/JSIExecutor.h
    jsiexecutor/jsireact/JSINativeModules.cpp
    jsiexecutor/jsireact/JSINativeModules.h
    cxxreact/CxxNativeModule.cpp
    cxxreact/CxxNativeModule.h
    cxxreact/Instance.cpp
    cxxreact/Instance.h
    cxxreact/JSBigString.cpp
    cxxreact/JSBigString.h
    cxxreact/JSBundleType.cpp
    cxxreact/JSBundleType.h
    cxxreact/JSDeltaBundleClient.cpp
    cxxreact/JSDeltaBundleClient.h
    cxxreact/JSExecutor.cpp
    cxxreact/JSExecutor.h
    cxxreact/JSIndexedRAMBundle.cpp
    cxxreact/JSIndexedRAMBundle.h
    cxxreact/JSModulesUnbundle.h
    cxxreact/JsArgumentHelpers-inl.h
    cxxreact/JsArgumentHelpers.h
    cxxreact/MessageQueueThread.h
    cxxreact/MethodCall.cpp
    cxxreact/MethodCall.h
    cxxreact/ModuleRegistry.cpp
    cxxreact/ModuleRegistry.h
    cxxreact/NativeModule.h
    cxxreact/NativeToJsBridge.cpp
    cxxreact/NativeToJsBridge.h
    cxxreact/RAMBundleRegistry.cpp
    cxxreact/RAMBundleRegistry.h
    cxxreact/ReactMarker.cpp
    cxxreact/ReactMarker.h
    cxxreact/RecoverableError.h
    cxxreact/SharedProxyCxxModule.h
    cxxreact/SystraceSection.h
)

target_sources(ReactCommon
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ReactCommonStub.cpp # Stub for OSes other than OSX/iOS/tvOS
    ${REACT_COMMON_YOGA_SOURCE}
    ${REACT_COMMON_MICRO_PROFILER_SOURCE}
    ${REACT_COMMON_JS_INSPECTOR_SOURCE}
    ${REACT_COMMON_CXXREACT_SOURCE}
)

target_compile_definitions(ReactCommon
    PUBLIC
        REACTCOMMON_VERSION=60
)

target_include_directories(ReactCommon PRIVATE jsi)
target_include_directories(ReactCommon PRIVATE jsiexecutor)

if(USE_JSC)
    # USE_JSC now means that JSC is available, not necessarily that ReactCommon must be used with JSC. So we build the library for any platform that can have WebKit built.
    add_library(ReactCommonJSC STATIC)

    target_include_directories(ReactCommonJSC PRIVATE jsi)
    target_sources(ReactCommonJSC PRIVATE ${REACT_COMMON_JSC_CXXREACT_SOURCE})
    target_include_directories(ReactCommonJSC PRIVATE jsiexecutor)
    target_link_libraries(ReactCommonJSC PUBLIC ReactCommon)
endif()

if(VS2017 OR USE_MMAN_WIN32)
    target_sources(ReactCommon PRIVATE port/sys/mman.c port/sys/mman.h) 
endif()

hunter_add_package(folly)
find_package(folly CONFIG REQUIRED)

hunter_add_package(double-conversion)
find_package(double-conversion CONFIG REQUIRED)

hunter_add_package(glog)
find_package(glog CONFIG REQUIRED)

if (USE_JSC AND NOT APPLE)
    # matches the target_link_libraries below
    hunter_add_package(WebKit)
    find_package(WebKit CONFIG REQUIRED)

    # matches the hunter_add_package above
    target_link_libraries(ReactCommonJSC PUBLIC WebKit::JavaScriptCore)
endif()

include(GNUInstallDirs)
set(_ReactCommon_CMake_EXPORT_NAME "ReactCommon")
set(_ReactCommon_CMake_BINDIR ${CMAKE_INSTALL_BINDIR})
set(_ReactCommon_CMake_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR}/ReactCommon)
set(_ReactCommon_CMake_LIBDIR ${CMAKE_INSTALL_LIBDIR})
set(_ReactCommon_CMake_INSTALLDIR ${_ReactNative_CMake_INSTALLDIR})

target_include_directories(ReactCommon PUBLIC
    $<INSTALL_INTERFACE:${_ReactCommon_CMake_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${_ReactCommon_CMake_INCLUDE_DIR}/yoga>
    $<INSTALL_INTERFACE:${_ReactCommon_CMake_INCLUDE_DIR}/jsi>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/yoga>
)

if (USE_MMAN_WIN32)
    target_include_directories(ReactCommon PUBLIC
        $<INSTALL_INTERFACE:${_ReactCommon_CMake_INCLUDE_DIR}/port>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/port>
    )
endif()

target_link_libraries(ReactCommon PUBLIC
    Folly::folly
    double-conversion::double-conversion
    glog::glog
)

if (CMAKE_CXX_STANDARD GREATER_EQUAL 17)
    target_compile_definitions(ReactCommon
        PRIVATE _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
    )

    if(USE_JSC)
    target_compile_definitions(ReactCommonJSC
        PRIVATE _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
    )
    endif(USE_JSC)

endif()

if (MSVC)
    target_compile_definitions(ReactCommon PRIVATE 
        _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
        _CRT_SECURE_NO_WARNINGS
    )

    if (BUILD_SHARED_LIBS)
        target_compile_definitions(ReactCommon PRIVATE
            PUBLIC RN_IMPORT_DLL
            PRIVATE RN_EXPORT_DLL
        )
    endif()

    if(USE_JSC)
        target_compile_definitions(ReactCommonJSC PRIVATE 
            _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
            _CRT_SECURE_NO_WARNINGS
        )
        if (BUILD_SHARED_LIBS)
            target_compile_definitions(ReactCommonJSC PRIVATE
                PUBLIC RN_IMPORT_DLL
                PRIVATE RN_EXPORT_DLL
            )
        endif()
    endif()
endif()

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/${_ReactCommon_CMake_EXPORT_NAME}Config.cmake
    INSTALL_DESTINATION ${_ReactCommon_CMake_INSTALLDIR}
    NO_SET_AND_CHECK_MACRO
)

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/generated/${_ReactCommon_CMake_EXPORT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY ExactVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/generated/${_ReactCommon_CMake_EXPORT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/generated/${_ReactCommon_CMake_EXPORT_NAME}ConfigVersion.cmake
    DESTINATION ${_ReactCommon_CMake_INSTALLDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${_ReactCommon_CMake_INCLUDE_DIR}
    FILES_MATCHING 
    PATTERN "*.h*"
)

install(TARGETS ReactCommon EXPORT ${_ReactCommon_CMake_EXPORT_NAME}
    ARCHIVE DESTINATION ${_ReactCommon_CMake_LIBDIR}
    LIBRARY DESTINATION ${_ReactCommon_CMake_LIBDIR}
    INCLUDES DESTINATION ${_ReactCommon_CMake_INCLUDEDIR}
)

install(EXPORT ${_ReactCommon_CMake_EXPORT_NAME} NAMESPACE ${_ReactNative_EXPORTS_NAMESPACE}
    DESTINATION ${_ReactCommon_CMake_INSTALLDIR}
    FILE "${_ReactCommon_CMake_EXPORT_NAME}Targets.cmake"
)

##################################
##################################

if(USE_JSC)
    set(_ReactCommon_CMake_EXPORT_NAME "ReactCommonJSC")
    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/generated/${_ReactCommon_CMake_EXPORT_NAME}Config.cmake
        INSTALL_DESTINATION ${_ReactCommon_CMake_INSTALLDIR}
        NO_SET_AND_CHECK_MACRO
    )

    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/generated/${_ReactCommon_CMake_EXPORT_NAME}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY ExactVersion
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/generated/${_ReactCommon_CMake_EXPORT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/generated/${_ReactCommon_CMake_EXPORT_NAME}ConfigVersion.cmake
        DESTINATION ${_ReactCommon_CMake_INSTALLDIR}
    )

    install(TARGETS ReactCommonJSC EXPORT ${_ReactCommon_CMake_EXPORT_NAME}
        ARCHIVE DESTINATION ${_ReactCommon_CMake_LIBDIR}
        LIBRARY DESTINATION ${_ReactCommon_CMake_LIBDIR}
        INCLUDES DESTINATION ${_ReactCommon_CMake_INCLUDEDIR}
    )

    install(EXPORT ${_ReactCommon_CMake_EXPORT_NAME} NAMESPACE ${_ReactNative_EXPORTS_NAMESPACE}
        DESTINATION ${_ReactCommon_CMake_INSTALLDIR}
        FILE "${_ReactCommon_CMake_EXPORT_NAME}Targets.cmake"
    )
endif()

