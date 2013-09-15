cmake_minimum_required( VERSION 2.8.11 )
project( AOS_all_dependencies )

message( STATUS "======== Art Of Sequence: All Projects Dependencies ========" )


####################################################
# Some configuration first...


if( MSVC ) # VS2012 doesn't support correctly the tuples yet
	add_definitions( /D _VARIADIC_MAX=10 )
endif()

####################################################
# Grouping all dependencies in a virtual directory.

set_property( GLOBAL PROPERTY USE_FOLDERS ON )
set( AOS_DEPENDENCIES_PROJECTS_GROUP_PATH 	dependencies )

####################################################
# Note that Boost is not provided in this package
# but is required for all C++ based projects.

####################################################
# Intel Threading Building Blocks
message( STATUS "--- Intel Threading Building Blocks ----" )

# if FindTBB is added in CMake, make sure it's used instead of this.
if( MSVC AND NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/itbb/build/vsproject/tbb.vcxproj )
	
	execute_process( 
		COMMAND cmd /k upgrade_itbb_msvc11.bat
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		OUTPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/upgrade.itbb.log
	)

endif()

if( MSVC )
	include_external_msproject( tbb ${CMAKE_CURRENT_SOURCE_DIR}/itbb/build/vsproject/tbb.vcxproj )
else()
	message( "No project file set for ITBB for this platform!" )
endif()

set( TBB_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/itbb )
set( TBB_INCLUDE_DIRS ${TBB_ROOT}/include )
set( TBB_INCLUDE_DIRS ${TBB_INCLUDE_DIRS} 
	CACHE PATH 
	"TBB include directory path."
	FORCE 
)

set_property( TARGET tbb		PROPERTY FOLDER ${AOS_DEPENDENCIES_PROJECTS_GROUP_PATH}/itbb )

####################################################
# Google Test
message( STATUS "--- Google test ----" )

set( gtest_force_shared_crt 	TRUE	CACHE BOOL "" )
add_subdirectory( gtest )

set_property( TARGET gtest		PROPERTY FOLDER ${AOS_DEPENDENCIES_PROJECTS_GROUP_PATH}/gtest )
set_property( TARGET gtest_main	PROPERTY FOLDER ${AOS_DEPENDENCIES_PROJECTS_GROUP_PATH}/gtest )

set( GTEST_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gtest )
set( GTEST_INCLUDE_DIR ${GTEST_ROOT_DIR}/include 
	CACHE PATH 
	"GTest include directory path."
	FORCE 
)



message( STATUS "======== AOS All Dependencies : END ========" )
message( STATUS "================================================\n" )



