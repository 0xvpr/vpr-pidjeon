cmake_minimum_required( VERSION 3.22.0 )

if ( "${PROJECT_ARCHITECTURE}" STREQUAL "x64")
    ## Add testing support
    project( run-tests C )

    ## Set output directory
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )

    ## Source directories
    set( SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
    set( TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

    ## Include directories
    include_directories( ${PROJECT_NAME} ${SOURCE_DIR} )
    include_directories( ${PROJECT_NAME} /usr/local/share/mingw64/include )
    set( INCLUDE_DIR ${SOURCE_DIR} )


    ## Add CXX sources
    set(
        TEST_SOURCES
        # CXX sources
        ${TEST_DIR}/unit_test_pidjeon.c
        ${SOURCE_DIR}/crt.c
        ${SOURCE_DIR}/injector.c
        ${SOURCE_DIR}/loadlibrary.c
        ${SOURCE_DIR}/logger.c
        ${SOURCE_DIR}/manualmap.c
        ${SOURCE_DIR}/mem.c
        ${SOURCE_DIR}/parser.c
        ${SOURCE_DIR}/util.c
        # CXX includes
        ${INCLUDE_DIR}/crt.h
        ${INCLUDE_DIR}/definitions.h
        ${INCLUDE_DIR}/injector.h
        ${INCLUDE_DIR}/loadlibrary.h
        ${INCLUDE_DIR}/logger.h
        ${INCLUDE_DIR}/manualmap.h
        ${INCLUDE_DIR}/mem.h
        ${INCLUDE_DIR}/parser.h
        ${INCLUDE_DIR}/util.h
    )

    # Set the download URL and the expected SHA256 hash of the file
    #set( CUNIT_URL "https://mirror.msys2.org/mingw/mingw64/mingw-w64-x86_64-cunit-2.1.3-3-any.pkg.tar.xz" )
    #set( CUNIT_SHA256 "3e8c3d42af34245ee6fcd38aa2c8d22d2f2ab1b9e9b056e8dfc37b0c799ea0a5" )

    ## Define the target for downloading and extracting the file
    #add_custom_target(
      #DownloadCUnit
      #COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/cunit
      #COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR}/cunit
          #${CMAKE_COMMAND} -E echo "Downloading CUnit..."
          #${CMAKE_COMMAND} -E get_file ${CUNIT_URL} cunit.tar.xz
          #${CMAKE_COMMAND} -E echo "Verifying SHA256 hash..."
          #${CMAKE_COMMAND} -E hash_sha256_check(cunit.tar.xz "${CUNIT_SHA256}")
          #${CMAKE_COMMAND} -E echo "Extracting CUnit..."
          #${CMAKE_COMMAND} -E tar(x)Jxf cunit.tar.xz
          #${CMAKE_COMMAND} -E copy_directory cunit/include /usr/local/include/cunit
          #${CMAKE_COMMAND} -E copy_directory cunit/lib /usr/local/lib/cunit
          #${CMAKE_COMMAND} -E copy_directory cunit/share /usr/local/share/cunit
      #COMMENT "Downloading and installing CUnit..."
      #VERBATIM
    #)

    ## Test executable and link dependencies
    add_executable( ${PROJECT_NAME} ${TEST_SOURCES} )

    ## Add dependencies
    #add_dependencies( ${PROJECT_NAME} DownloadCUnit )

    ## Add link directories
    target_link_directories(
        ${PROJECT_NAME}
        PRIVATE
        /usr/local/share/mingw64/lib
    )

    ## Link Dependencies
    target_link_libraries( ${PROJECT_NAME} libcunit.a )

    set_target_properties(
        ${PROJECT_NAME}
	PROPERTIES
	LINK_STATIC_LIBRARIES TRUE
    )
endif()
