/* Standard library includes */
#include <iostream>
#include <fstream>
#include <string>

/* External dependencies */
#include <vulkan\vulkan.h>
#include <SDL2/SDL.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

/* Internal headers includes */
#include "HInternalTypeAliases.h"
#include "CInputHandler.h"
#include "UErrorLogger.h"

/* Defines */
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE


void checkSDLError() {
	const char *error = SDL_GetError();
	if (*error != '\0') {
		std::cout << "SDL Error: " + std::string(error) << std::endl;
		SDL_ClearError();
	}
}

/* Entry point */
int main(int a_argc, char *a_argv[]) {
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////  ================================================================================================================================  ////
	////    ---- FIRST INIT -----                                                                                                           ////
	////  ================================================================================================================================  ////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	std::cout.precision(2);
	VolkPhetamine::UErrorLogger::getInstance().setOutputFile("VolkPhetamine.err");

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////====================================================================================================================================////
	//// ---- SDL -----
	////====================================================================================================================================////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* Initialize SDL's Video subsystem */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {		
		VL_ERROR("Unable to initialize SDL!");
	}

	/* Hide the cursor (it's an FPS!) */
	SDL_ShowCursor(0);

	/* Create our window centered */
	SDL_Window* SDL_WindowReference = SDL_CreateWindow("Vulkan test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	if (!SDL_WindowReference) { /* Die if creation failed */
		VL_ERROR("Unable to create the window!");
	}

	checkSDLError();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////====================================================================================================================================////
	//// ---- VULKAN INIT -----
	////====================================================================================================================================////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::cout << extensionCount << " extensions supported" << std::endl;

	VkInstanceCreateInfo vulkanInstanceCreateInfo{};
	vulkanInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

	VkInstance vulkanInstance = nullptr;

	auto returnCheck = vkCreateInstance(&vulkanInstanceCreateInfo, nullptr, &vulkanInstance);
	if (returnCheck != VK_SUCCESS) {
		VL_ERROR("Error initializing Vulkan");
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////  ================================================================================================================================  ////
	////    ---- OTHER INIT -----                                                                                                           ////
	////  ================================================================================================================================  ////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	VolkPhetamine::CInputHandler inputHandler(SDL_WindowReference);

	// Critical error window:
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Window caption", "Message", NULL);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////  ================================================================================================================================  ////
	////    ---- MAIN LOOP -----                                                                                                            ////
	////  ================================================================================================================================  ////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	while (inputHandler.isQuit() == false) {
		inputHandler.update();
	}

	vkDestroyInstance(vulkanInstance, nullptr);
	vulkanInstance = nullptr;

	SDL_DestroyWindow(SDL_WindowReference);
	SDL_Quit();

	return EXIT_SUCCESS;
}