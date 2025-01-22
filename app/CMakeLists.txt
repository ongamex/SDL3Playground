

set(SHADERS
	"./triangle.vert"
	"./triangle.frag"
)

add_executable(app main.cpp ${SHADERS})
target_link_libraries(app SDL3-shared)

set_target_properties(app PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:app>)

foreach(SHADER ${SHADERS})
	#cmake_path(GET ${CMAKE_CURRENT_LIST_DIR}/${SHADER}  FILENAME SHADER_FILE_NAME)

    add_custom_command(
		TARGET app 
		PRE_BUILD COMMAND 
		${MY_GLSLC_PATH} ${CMAKE_CURRENT_LIST_DIR}/${SHADER} -o $<TARGET_FILE_DIR:app>/${SHADER}.spv
	)
endforeach()

