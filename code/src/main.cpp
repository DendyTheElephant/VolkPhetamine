/* Standard library includes */
#include <iostream>
#include <fstream>
#include <string>

/* External dependencies */
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <vulkan/vulkan.h>

/* Internal headers includes */
#include "DInternalTypeAliases.h"
#include "CInputHandler.h"
#include "UErrorLogger.h"
#include "CVulkanDeleter.h"
#include "CGPUDevice.h"

/* Defines */
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

using namespace VolkPhetamine;


/* Entry point */
int main(int a_argc, char *a_argv[]) {
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////  ================================================================================================================================  ////
	////    ---- FIRST INIT -----                                                                                                           ////
	////  ================================================================================================================================  ////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	std::cout.precision(2);
	Utils::UErrorLogger::getInstance().setOutputFile("VolkPhetamine.err");

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////====================================================================================================================================////
	//// ---- GLFW -----
	////====================================================================================================================================////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	/* Sets callback error function */
	glfwSetErrorCallback(Utils::UErrorLogger::getInstance().glfwErrorCallback);

	/* Initialize GLFW */
	if (glfwInit() < 0) {
		VL_FATAL_ERROR("Unable to initialize GLFW!");
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Not a OpenGL context!
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Not resizable window

	/* Create our window centered */
	GLFWwindow* glfwWindow = glfwCreateWindow(800, 600, "VolkPhetamine", nullptr, nullptr);
	if (!glfwWindow) {
		VL_FATAL_ERROR("Unable to create the window!");
	}

	glfwSetWindowPos(glfwWindow, 400, 300);

	/* Get the GLFW Extensions */
	vlUInt16 glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////====================================================================================================================================////
	//// ---- VULKAN INIT -----
	////====================================================================================================================================////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	

	// VULKAN CHECK EXTENSIONS

	vlUInt extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::cout << extensionCount << " extensions supported" << std::endl;

	vlVector<VkExtensionProperties> extensions(extensionCount);

	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "available extensions:" << std::endl;

	for (const auto& extension : extensions) {
		std::cout << "\t" << extension.extensionName << std::endl;
	}

	// VULKAN INSTANCE

	VulkanWrappers::CVulkanDeleter<VkInstance> vulkanInstance{ vkDestroyInstance };

	VkApplicationInfo vulkanApplicationInfo = {};
	vulkanApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	vulkanApplicationInfo.pNext = nullptr;
	vulkanApplicationInfo.pApplicationName = "Dendy Tale";
	vulkanApplicationInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
	vulkanApplicationInfo.pEngineName = "VolkPhetamine";
	vulkanApplicationInfo.engineVersion = VK_MAKE_VERSION(0, 1, 0);
	vulkanApplicationInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo vulkanInstanceCreateInfo{};
	vulkanInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vulkanInstanceCreateInfo.pNext = nullptr;
	vulkanInstanceCreateInfo.flags = 0;
	vulkanInstanceCreateInfo.pApplicationInfo = &vulkanApplicationInfo;
	// Don't enable and layer
	vulkanInstanceCreateInfo.enabledLayerCount = 0;
	vulkanInstanceCreateInfo.ppEnabledLayerNames = nullptr;
	// Don't enable any extensions
	vulkanInstanceCreateInfo.enabledExtensionCount = glfwExtensionCount;
	vulkanInstanceCreateInfo.ppEnabledExtensionNames = glfwExtensions;

	if (vkCreateInstance(&vulkanInstanceCreateInfo, nullptr, vulkanInstance.replace()) != VK_SUCCESS) {
		VL_FATAL_ERROR("Error initializing Vulkan");
	}

	

	// VULKAN DEVICE

	VulkanWrappers::CGPUDevice device = VulkanWrappers::CGPUDevice(&vulkanInstance);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////  ================================================================================================================================  ////
	////    ---- OTHER INIT -----                                                                                                           ////
	////  ================================================================================================================================  ////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	//VolkPhetamine::CInputHandler inputHandler(SDL_WindowReference);

	// Critical error window:
	//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Window caption", "Message", NULL);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////  ================================================================================================================================  ////
	////    ---- MAIN LOOP -----                                                                                                            ////
	////  ================================================================================================================================  ////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	/*while (inputHandler.isQuit() == false) {
		inputHandler.update();
	}*/

	while (!glfwWindowShouldClose(glfwWindow)) {
		glfwPollEvents();
	}

	glfwDestroyWindow(glfwWindow);

	glfwTerminate();

	return EXIT_SUCCESS;
}