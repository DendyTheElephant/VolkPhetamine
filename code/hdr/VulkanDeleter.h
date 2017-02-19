////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////====================================================================================================================================////
/// | \file		VulkanDeleter
///	| \author	Daniel Huc
/// | \date		February 2017
/// |----------------------------------------------------------------------------------------------------------------------------------
/// | \brief	Blabla
/// | \details	See https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Base_code
////====================================================================================================================================////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
/* Standard library includes */

/* External dependencies */
#include <vulkan\vulkan.h>

/* Internal headers includes */
#include "HInternalTypeAliases.h"

/* Specific defines */

namespace VolkPhetamine {

	namespace VulkanWrappers {

		template <typename T>
		class VulkanDeleter {

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//// ---- Members -----																													//// 
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		private:
			T m_object{ VK_NULL_HANDLE };		///< Vulkan object to wrap
			std::function<void(T)> m_deleter;	///< Delete function for Vulkan object


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//// ---- Methods -----																													////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		private:
		//// ---- Internal methods ---- ////
			/// Call the deleter on Vulkan object
			void _cleanup() {
				if (m_object != VK_NULL_HANDLE) {
					m_deleter(m_object);
				}
				m_object = VK_NULL_HANDLE;
			}

		public:
		//// ---- Constructor/Destructor ---- ////
			VulkanDeleter() : VulkanDeleter([](T, VkAllocationCallbacks*) {}) {}

			VulkanDeleter(std::function<void(T, VkAllocationCallbacks*)> a_deleteFunction) {
				this->deleter = [=](T obj) { a_deleteFunction(obj, nullptr); };
			}

			VulkanDeleter(const VDeleter<VkInstance>& a_instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> a_deleteFunction) {
				this->deleter = [&a_instance, a_deleteFunction](T obj) { a_deleteFunction(a_instance, obj, nullptr); };
			}

			VulkanDeleter(const VDeleter<VkDevice>& a_device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> a_deleteFunction) {
				this->deleter = [&a_device, a_deleteFunction](T obj) { a_deleteFunction(a_device, obj, nullptr); };
			}

			~VulkanDeleter() {
				_cleanup();
			}


		//// ---- Operator ---- ////
			const T* operator &() const {
				return &m_object;
			}

			T* replace() {
				_cleanup();
				return &m_object;
			}

			operator T() const {
				return m_object;
			}

			void operator=(T a_rhs) {
				if (a_rhs != m_object) {
					_cleanup();
					m_object = a_rhs;
				}
			}

			template<typename V>
			bool operator==(V a_rhs) {
				return m_object == T(a_rhs);
			}
		};
	}
}
