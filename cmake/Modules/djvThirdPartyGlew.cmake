find_package(GLEW REQUIRED)

#message("GLEW_INCLUDE_DIR = " ${GLEW_INCLUDE_DIR})
#message("GLEW_LIBRARIES = " ${GLEW_LIBRARIES})

if (djvPackageThirdParty)

    if (WIN32)
	
		install(
			FILES
			$ENV{CMAKE_PREFIX_PATH}/bin/GLEW.dll
			DESTINATION bin)

    elseif (APPLE)

        install(
            FILES
            $ENV{CMAKE_PREFIX_PATH}/lib/libGLEW.dylib
            $ENV{CMAKE_PREFIX_PATH}/lib/libGLEW.2.1.dylib
            $ENV{CMAKE_PREFIX_PATH}/lib/libGLEW.2.1.0.dylib
            DESTINATION lib)

    elseif (UNIX)

        install(
            FILES
            $ENV{CMAKE_PREFIX_PATH}/lib/libGLEW.so
            $ENV{CMAKE_PREFIX_PATH}/lib/libGLEW.so.2.1
            $ENV{CMAKE_PREFIX_PATH}/lib/libGLEW.so.2.1.0
            DESTINATION lib)

    endif (WIN32)

endif (djvPackageThirdParty)

