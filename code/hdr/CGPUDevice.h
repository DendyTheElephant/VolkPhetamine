////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////====================================================================================================================================////
/// | \file		CGPUDevice
///	| \author	Daniel Huc
/// | \date		February 2017
/// |----------------------------------------------------------------------------------------------------------------------------------
/// | \brief	Blabla
/// | \details	Blabla
////====================================================================================================================================////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
/* Standard library includes */

/* External dependencies */
#include <vulkan/vulkan.h>

/* Internal headers includes */
#include "DInternalTypeAliases.h"
#include "UErrorLogger.h"
#include "CVulkanDeleter.h"

//// ---- Namespaces ---- ////
namespace VolkPhetamine {
	namespace VulkanWrappers {

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////====================================================================================================================================////
		//// CGPUDevice
		/// \brief		Blabla
		/// \details	Blabla
		////====================================================================================================================================////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class CGPUDevice {

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//// ---- Internal structures -----                                                                                                     ////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		public:
			struct SProperties {
				vlString	name{ "" };								///< Device name
				vlUInt		score{ 0 };								///< Score of currently selected GPU: based on our own evaluation grid ;)
				vlUInt64	vramSize{ 0 };							///< Video memory size of the graphic device
				vlBool		isAvailableGeometryShader{ false };		///< May perform effects based on it, but not always available
				vlInt16		queueGraphicsFamilyIndex{ -1 };
				vlInt16		queueTransfertFamilyIndex{ -1 };
				vlInt16		queueComputeFamilyIndex{ -1 };
				vlInt16		queueSparseMemFamilyIndex{ -1 };
			};


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//// ---- Members -----																													//// 
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		private:
			const vlUInt				M_ONE_GIGA_BYTE_IN_BYTES{ 1073741824 };		///< Needed to know how many Gigs of VRAM the device have
			VkPhysicalDevice			m_vulkanPhysicalDevice{ VK_NULL_HANDLE };	///< Vulkan physical device
			CVulkanDeleter<VkDevice>	m_vulkanLogicalDevice{ vkDestroyDevice };	///< Vulkan logical device
			VkQueue						m_vulkanGraphicQueue{ VK_NULL_HANDLE };
			SProperties					m_deviceProperties;


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//// ---- Methods -----																													////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		private:
		//// ---- Internal methods ---- ////
			SProperties _analyseDevice(const VkPhysicalDevice* a_device);

		public:
		//// ---- Constructor/Destructor ---- ////
			CGPUDevice(const VkInstance* vulkanInstance);

		//// ---- Getter ---- ////
			

		//// ---- Core ---- ////


		};
	}
}
