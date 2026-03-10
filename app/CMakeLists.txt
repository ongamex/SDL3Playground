
set(SHADERS
	"./triangle.vert"
	"./triangle.frag"
)

set(SHADERS_SLANG
	"./triangle_slang.vert"
	"./triangle_slang.frag"
)

add_executable(app main.cpp ${SHADERS} ${SHADERS_SLANG})
target_link_libraries(app PUBLIC SDL3::SDL3)

set_target_properties(app PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:app>)

# The type of the shader (vertex, pixel and so on) of glslc and slang can be guessed from the extension.
# That is why it is not specified.

# GLSL shaders:
foreach(SHADER ${SHADERS})
	#cmake_path(GET ${CMAKE_CURRENT_LIST_DIR}/${SHADER}  FILENAME SHADER_FILE_NAME)
	add_custom_command(
		TARGET app 
		PRE_BUILD COMMAND 
		${MY_GLSLC_PATH} ${CMAKE_CURRENT_LIST_DIR}/${SHADER} -o $<TARGET_FILE_DIR:app>/${SHADER}.spv
	)
endforeach()

# SLang shaders:
foreach(SHADER ${SHADERS})
	#cmake_path(GET ${CMAKE_CURRENT_LIST_DIR}/${SHADER}  FILENAME SHADER_FILE_NAME)

	# Actual Spir-V that we re going to use.
	add_custom_command(
		TARGET app 
		PRE_BUILD COMMAND 
		${MY_SLANG_PATH} ${CMAKE_CURRENT_LIST_DIR}/${SHADER} -profile glsl_330 -target spirv  -o $<TARGET_FILE_DIR:app>/${SHADER}.spv.slang -entry main

		# Example:
		# .\slangc.exe hello-world.slang -profile glsl_450 -target glsl -o hello-world.glsl -entry computeMain
	)

	# GLSL JUST FOR FUN:
	add_custom_command(
		TARGET app 
		PRE_BUILD COMMAND 
		${MY_SLANG_PATH} ${CMAKE_CURRENT_LIST_DIR}/${SHADER} -profile glsl_330 -target glsl  -o $<TARGET_FILE_DIR:app>/${SHADER}.slang.glsl -entry main
	)
endforeach()

add_custom_command(TARGET app POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL3-shared> $<TARGET_FILE_DIR:app>/
)

