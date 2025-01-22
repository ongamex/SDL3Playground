#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_gpu.h>

#include <string>
#include <vector>
#include <fstream>

std::vector<char> readFile(const char* const filename)
{
	std::vector<char> data;
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		return std::vector<char>();
	}

	const size_t fileSizeBytes = (size_t)file.tellg();
	data.resize(fileSizeBytes);

	file.seekg(0);
	file.read(data.data(), fileSizeBytes);

	file.close();

	return data;
}

struct AppState {
	SDL_Window* wnd = nullptr;
	SDL_GPUDevice* wndDevice = nullptr;
	SDL_GPUShader* vertexShader = nullptr;
	SDL_GPUShader* fragShader = nullptr;
	SDL_GPUGraphicsPipeline* trianglePipeline = nullptr;
};

SDL_AppResult SDL_AppInit(void** appstateRaw, int argc, char* argv[])
{
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	*appstateRaw = new AppState();
	AppState& appState = *(AppState*)(*appstateRaw);


	appState.wnd = SDL_CreateWindow("App test SDL3", 1024, 768, 0);
	appState.wndDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, NULL);

	bool succ = SDL_ClaimWindowForGPUDevice(appState.wndDevice, appState.wnd);

	// Shaders:
	{
		std::vector<char> vertShdrCode = readFile("triangle.vert.spv");

		SDL_GPUShaderCreateInfo ci = { 0 };
		ci.code_size = vertShdrCode.size();
		ci.code = (Uint8*)vertShdrCode.data();
		ci.entrypoint = "main";
		ci.format = SDL_GPU_SHADERFORMAT_SPIRV;
		ci.stage = SDL_GPU_SHADERSTAGE_VERTEX;
		ci.num_samplers = 0;
		ci.num_storage_textures = 0;
		ci.num_storage_buffers = 0;
		ci.num_uniform_buffers = 0;
		ci.props = 0;

		appState.vertexShader = SDL_CreateGPUShader(appState.wndDevice, &ci);
	}

	{
		std::vector<char> fragShdrCode = readFile("triangle.frag.spv");

		SDL_GPUShaderCreateInfo ci = { 0 };
		ci.code_size = fragShdrCode.size();
		ci.code = (Uint8*)fragShdrCode.data();
		ci.entrypoint = "main";
		ci.format = SDL_GPU_SHADERFORMAT_SPIRV;
		ci.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
		ci.num_samplers = 0;
		ci.num_storage_textures = 0;
		ci.num_storage_buffers = 0;
		ci.num_uniform_buffers = 0;
		ci.props = 0;

		appState.fragShader = SDL_CreateGPUShader(appState.wndDevice, &ci);
	}

	{
		SDL_GPUColorTargetDescription colorTargetDesc;
		memset(&colorTargetDesc, 0, sizeof(colorTargetDesc)); // For the blend state which is not initialized.
		colorTargetDesc.format = SDL_GetGPUSwapchainTextureFormat(appState.wndDevice, appState.wnd);

		SDL_GPUColorTargetDescription allColorTargetDesc[] = {
			colorTargetDesc
		};

		SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = { 0 };
		pipelineCreateInfo.target_info.num_color_targets = 1;
		pipelineCreateInfo.target_info.color_target_descriptions = allColorTargetDesc;
		pipelineCreateInfo.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
		pipelineCreateInfo.vertex_shader = appState.vertexShader;
		pipelineCreateInfo.fragment_shader = appState.fragShader;
		pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;

		appState.trianglePipeline = SDL_CreateGPUGraphicsPipeline(appState.wndDevice, &pipelineCreateInfo);
	}

	if (!succ) {
		SDL_Log("Couldn't claim window %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;  /* carry on with the program! */
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
	}

	return SDL_APP_CONTINUE;  /* carry on with the program! */
}

SDL_AppResult SDL_AppIterate(void* appstateRaw)
{
	AppState& appState = *(AppState*)(appstateRaw);
	SDL_GPUCommandBuffer* cmdbuf = SDL_AcquireGPUCommandBuffer(appState.wndDevice);

	SDL_GPUTexture* swapchainTexture;
	if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmdbuf, appState.wnd, &swapchainTexture, NULL, NULL)) {
		SDL_Log("WaitAndAcquireGPUSwapchainTexture failed: %s", SDL_GetError());
	}

	SDL_GPUColorTargetInfo colorTargetInfo = { 0 };
	colorTargetInfo.texture = swapchainTexture;
	colorTargetInfo.clear_color = SDL_FColor{ 0.0f, 0.0f, 0.0f, 1.0f };
	colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
	colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

	SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(cmdbuf, &colorTargetInfo, 1, NULL);
	SDL_GPUViewport vp = {0,0, 1024, 768, 0.f, 1.f};
	
	SDL_SetGPUViewport(renderPass, &vp);
	SDL_BindGPUGraphicsPipeline(renderPass, appState.trianglePipeline);

	SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);
	SDL_EndGPURenderPass(renderPass);

	SDL_SubmitGPUCommandBuffer(cmdbuf);

	return SDL_APP_CONTINUE;  /* carry on with the program! */
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{

}

