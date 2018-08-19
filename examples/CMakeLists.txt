cmake_minimum_required(VERSION 3.10 FATAL_ERROR)


file(GLOB_RECURSE examples_source ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_definitions(-DCONFIG_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/configs/\")

foreach (example ${examples_source})
    get_filename_component (file_name ${example} NAME)

    string (REGEX REPLACE "\\.[^.]*$" "" target ${file_name}) # remove ext 

    add_executable (${target} ${example})
	
    target_link_libraries(${target}
        ${library_static}    
    )
endforeach ()

