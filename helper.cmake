

# Function to glob directories only
function(glob_directories output_var dir)
    # Get all files and directories
    file(GLOB_RECURSE children 
        LIST_DIRECTORIES true
        ${dir}/*)
    
    get_filename_component(abs_dir ${dir} ABSOLUTE)
    set(dir_list ${abs_dir})
    
    # Filter to keep only directories
    foreach(child ${children})
        if(IS_DIRECTORY ${child})
            list(APPEND dir_list ${child})
        endif()
    endforeach()
    
    set(${output_var} ${dir_list} PARENT_SCOPE)
endfunction()

# Function to finalize a target
function(combine_target target_name main_source engine_sources engine_includes engine_interface)
    add_executable(${target_name} ${main_source} ${engine_sources})
    target_include_directories(${target_name} PRIVATE ${engine_includes})
    target_link_libraries(${target_name} PRIVATE ${engine_interface})
endfunction()