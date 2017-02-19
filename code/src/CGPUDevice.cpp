#include "CGPUDevice.h"

namespace VolkPhetamine {
	namespace VulkanWrappers {
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////  ================================================================================================================================  ////
		////    ---- Internal methods -----                                                                                                     ////
		////  ================================================================================================================================  ////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//----------------------------------------------------------------------------------------------------------------------------------------//
		///
		//----------------------------------------------------------------------------------------------------------------------------------------//
		CGPUDevice::SProperties CGPUDevice::_analyseDevice(const VkPhysicalDevice* a_device) {
			SProperties currentDevice;
			vlBool isUnusableDevice{ false };

			// Bonuses
			const vlUInt16 BONUS_VRAM_GIG			{ 400 };
			const vlUInt16 BONUS_DEDICATED_DEVICE	{ 2500 };
			const vlUInt16 BONUS_DEDICATED_CHIPSET	{ 1000 };
			const vlUInt16 BONUS_GEOMETRY_SHADER	{ 100 };
			const vlUInt16 BONUS_COMPUTATIONAL_UNIT	{ 100 };
			const vlUInt16 BONUS_TRANSFERT_UNIT		{ 100 };
			const vlUInt16 BONUS_SPARSE_MEM_UNIT	{ 100 };

			// Gathering of proprieties with Vulkan
			VkPhysicalDeviceProperties deviceProperties;
			VkPhysicalDeviceFeatures deviceFeatures;
			VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
			vkGetPhysicalDeviceProperties(*a_device, &deviceProperties);
			vkGetPhysicalDeviceMemoryProperties(*a_device, &deviceMemoryProperties);
			vkGetPhysicalDeviceFeatures(*a_device, &deviceFeatures);

			
			vlUInt queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(*a_device, &queueFamilyCount, nullptr);

			vlVector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(*a_device, &queueFamilyCount, queueFamilies.data());
			
			for (vlUInt16 i_queue = 0; i_queue < queueFamilies.size(); ++i_queue) {
				if (queueFamilies.at(i_queue).queueCount > 0) {
					if (queueFamilies.at(i_queue).queueFlags & VK_QUEUE_GRAPHICS_BIT) {
						currentDevice.queueGraphicsFamilyIndex = i_queue;
					} 
					else if (queueFamilies.at(i_queue).queueFlags & VK_QUEUE_TRANSFER_BIT) {
						currentDevice.queueTransfertFamilyIndex = i_queue;
					}
					else if (queueFamilies.at(i_queue).queueFlags & VK_QUEUE_COMPUTE_BIT) {
						currentDevice.queueComputeFamilyIndex = i_queue;
					}
					else if (queueFamilies.at(i_queue).queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
						currentDevice.queueSparseMemFamilyIndex = i_queue;
					}
				}
			}
			// Check if it is suitable at all (minimum configuration):
			// Here we just checks that this device is usable as rendering device (GRAPHICS QUEUE)
			if (currentDevice.queueGraphicsFamilyIndex == -1) {
				isUnusableDevice = true;
			}


			// Device infos
			currentDevice.name = vlString(deviceProperties.deviceName);
			currentDevice.isAvailableGeometryShader = deviceFeatures.geometryShader; // WARNING HERE!!! VkBool32 to true or false (performance warning)
			
			// VRAM size gathering, thanks http://gamedev.stackexchange.com/questions/124738/how-to-select-the-most-powerful-vkdevice
			auto heapsPointer = deviceMemoryProperties.memoryHeaps;
			auto heaps = std::vector<VkMemoryHeap>(heapsPointer, heapsPointer + deviceMemoryProperties.memoryHeapCount);

			for (const auto& heap : heaps) {
				if (heap.flags & VkMemoryHeapFlagBits::VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
					// Device local heap, should be size of total GPU VRAM.
					//heap.size will be the size of VRAM in bytes. (bigger is better)
					currentDevice.vramSize += heap.size;
				}
			}


			// BONUS: Discrete (=Dedicated) GPUs have a significant performance advantage
			currentDevice.score += BONUS_DEDICATED_DEVICE * (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
			// BONUS: Integrated (=Chipset) GPUs have advantage over CPU
			currentDevice.score += BONUS_DEDICATED_CHIPSET * (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU);
			// BONUS: VRAM size
			currentDevice.score += BONUS_VRAM_GIG * static_cast<vlUInt>(currentDevice.vramSize / M_ONE_GIGA_BYTE_IN_BYTES);
			// BONUS: Geometry shader is available
			currentDevice.score += BONUS_GEOMETRY_SHADER * currentDevice.isAvailableGeometryShader;
			currentDevice.score += BONUS_TRANSFERT_UNIT * (currentDevice.queueTransfertFamilyIndex != -1);
			currentDevice.score += BONUS_COMPUTATIONAL_UNIT * (currentDevice.queueComputeFamilyIndex != -1);
			currentDevice.score += BONUS_SPARSE_MEM_UNIT * (currentDevice.queueSparseMemFamilyIndex != -1);
			
			// Maximum possible size of textures affects graphics quality
			//score += deviceProperties.limits.maxImageDimension2D;

			//deviceProperties.limits.
			//deviceFeatures.shaderStorageImageMultisample

			if (isUnusableDevice) {
				currentDevice.score = 0;
			}

			return currentDevice;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////  ================================================================================================================================  ////
		////    ---- Constructor/Destructor -----                                                                                               ////
		////  ================================================================================================================================  ////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//----------------------------------------------------------------------------------------------------------------------------------------//
		///
		//----------------------------------------------------------------------------------------------------------------------------------------//
		CGPUDevice::CGPUDevice(const VkInstance* a_vulkanInstance) {

			// Gather number of available devices
			vlUInt16 deviceCount{ 0 };
			vkEnumeratePhysicalDevices(*a_vulkanInstance, &deviceCount, nullptr);
			if (deviceCount == 0) {
				VL_FATAL_ERROR("Error: failed to find any GPU with Vulkan support!");
			}
			// Gather all available devices
			vlVector<VkPhysicalDevice> availableDevices(deviceCount);
			vkEnumeratePhysicalDevices(*a_vulkanInstance, &deviceCount, availableDevices.data());

			// Choosing the most well suited GPU for our purpose
			for (const auto& currentDevice : availableDevices) {
				SProperties currentDeviceProperties = _analyseDevice(&currentDevice);
				// Here m_score will holds the best score so far
				if (currentDeviceProperties.score > m_deviceProperties.score) {
					m_deviceProperties = currentDeviceProperties;
					m_vulkanPhysicalDevice = currentDevice;
				}
			}

			// Check if the best candidate is suitable at all
			if (m_deviceProperties.score == 0) {
				VL_FATAL_ERROR("Error: Best GPU device is not compatible with Vulkan support!");
			}


			std::cout << m_deviceProperties.name << std::endl;
			std::cout << m_deviceProperties.score << std::endl;
			std::cout << m_deviceProperties.vramSize << std::endl;
			std::cout << m_deviceProperties.isAvailableGeometryShader << std::endl;
			std::cout << m_deviceProperties.queueGraphicsFamilyIndex << std::endl;
			std::cout << m_deviceProperties.queueTransfertFamilyIndex << std::endl;
			std::cout << m_deviceProperties.queueComputeFamilyIndex << std::endl;
			std::cout << m_deviceProperties.queueSparseMemFamilyIndex << std::endl;


			// Creation of the Logical device:

			// Set-up graphic queue
			const vlFloat queuePriority = 1.0f;
			VkDeviceQueueCreateInfo vulkanDeviceQueueCreateInfo = {};
			vulkanDeviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			vulkanDeviceQueueCreateInfo.queueFamilyIndex = m_deviceProperties.queueGraphicsFamilyIndex;
			vulkanDeviceQueueCreateInfo.queueCount = 1;
			vulkanDeviceQueueCreateInfo.pQueuePriorities = &queuePriority;

			VkPhysicalDeviceFeatures vulkanPhysicalDeviceFeatures = {};

			VkDeviceCreateInfo vulkanDeviceCreateInfo = {};
			vulkanDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			vulkanDeviceCreateInfo.pQueueCreateInfos = &vulkanDeviceQueueCreateInfo;
			vulkanDeviceCreateInfo.queueCreateInfoCount = 1;
			vulkanDeviceCreateInfo.pEnabledFeatures = &vulkanPhysicalDeviceFeatures;
			vulkanDeviceCreateInfo.enabledExtensionCount = 0;
			vulkanDeviceCreateInfo.enabledLayerCount = 0;
			

			if (vkCreateDevice(m_vulkanPhysicalDevice, &vulkanDeviceCreateInfo, nullptr, m_vulkanLogicalDevice.replace()) != VK_SUCCESS) {
				throw std::runtime_error("failed to create logical device!");
			}

			vkGetDeviceQueue(m_vulkanLogicalDevice, m_deviceProperties.queueGraphicsFamilyIndex, 0, &m_vulkanGraphicQueue);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////  ================================================================================================================================  ////
		////    ---- Getter -----                                                                                                               ////
		////  ================================================================================================================================  ////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//----------------------------------------------------------------------------------------------------------------------------------------//
		///
		//----------------------------------------------------------------------------------------------------------------------------------------//

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////  ================================================================================================================================  ////
		////    ---- Setter -----                                                                                                               ////
		////  ================================================================================================================================  ////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//----------------------------------------------------------------------------------------------------------------------------------------//
		///
		//----------------------------------------------------------------------------------------------------------------------------------------//

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////  ================================================================================================================================  ////
		////    ---- Core -----                                                                                                                 ////
		////  ================================================================================================================================  ////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//----------------------------------------------------------------------------------------------------------------------------------------//
		///
		//----------------------------------------------------------------------------------------------------------------------------------------//

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////  ================================================================================================================================  ////
		////    ---- Static -----                                                                                                               ////
		////  ================================================================================================================================  ////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//----------------------------------------------------------------------------------------------------------------------------------------//
		///
		//----------------------------------------------------------------------------------------------------------------------------------------//
	}
}