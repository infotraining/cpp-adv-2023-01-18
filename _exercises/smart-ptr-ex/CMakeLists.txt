##################
# Target
get_filename_component(DIRECTORY_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" TARGET_MAIN ${DIRECTORY_NAME})
set(TARGET_MAIN tests-${TARGET_MAIN})

####################
# Sources & headers
aux_source_directory(. SRC_LIST)
file(GLOB HEADERS_LIST "*.h" "*.hpp")

add_executable(${TARGET_MAIN} ${SRC_LIST} ${HEADERS_LIST})