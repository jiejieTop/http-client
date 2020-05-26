aux_source_directory(. DIR_SRCS)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

string(REGEX REPLACE ".*/(.*)" "\\1" LIB_NAME ${PARENT_DIR}) 

if (DIR_SRCS)
    foreach(libname ${LIBNAMES})
        if (${LIB_NAME} STREQUAL ${libname})
            add_library(${libname} ${CMAKE_LIB_TYPE} ${DIR_SRCS})
            if (${CMAKE_LIB_TYPE} MATCHES "STATIC")
                target_link_libraries(${libname} "network")
            endif()
        endif()
    endforeach()

else()
    message(WARNING "not find is src file!")
endif()



