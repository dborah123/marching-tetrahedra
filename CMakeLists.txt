add_executable( marchingtets_exe EXCLUDE_FROM_ALL main.cpp marchingtet.cpp tet-functions.cpp )
target_link_libraries( marchingtets_exe flux_shared )

target_compile_definitions( marchingtets_exe PUBLIC -DFLUX_FULL_UNIT_TEST=false )

add_custom_target( marchingtets command $<TARGET_FILE:marchingtets_exe> 1 WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/projects/marchingtets )

ADD_DEBUG_TARGETS( marchingtets ${CMAKE_SOURCE_DIR}/projects/marchingtets/ )