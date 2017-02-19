////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////====================================================================================================================================////
/// | \file		HInternalTypesAliases
///	| \author	Daniel Huc
/// | \date		February 2017
/// |----------------------------------------------------------------------------------------------------------------------------------
/// | \brief	Aliases for types
/// | \details	Using standart types may cause confusion in memory representation (depending on system) \n
///	|			Plus if some mathematical functions will be changed in the future, we want to avoid multiple modifications!
////====================================================================================================================================////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

/* Standard library includes */
#include <vector>
#include <memory>
#include <map>

/* External dependencies */
#include <glm/gtc/type_ptr.hpp>

///* Substitution Aliases */
using vlBool = bool;				/// 1b boolean GL_TRUE or GL_FALSE
using vlInt16 = int_fast16_t;		/// Signed 16b integer [-32 768,  32 767]
using vlUInt16 = uint_fast16_t;		/// Unsigned 16b integer [0,  65 535]
using vlInt = int_fast32_t;			/// Signed 32b integer [-2 147 483 648,  2 147 483 647]
using vlUInt = uint_fast32_t;		/// Unsigned 32b integer [0,  4 294 967 295]
using vlInt64 = int_fast64_t;		/// Signed 64b integer [-9 223 372 036 854 775 808,  9 223 372 036 854 775 807]
using vlUInt64 = uint_fast64_t;		/// Unsigned 64b integer [0,  18 446 744 073 709 551 615]
using vlFloat = float;				/// Signed 32b floating-point
using vlFloat64 = double;			/// Signed 64b floating-point
using vlVec2i = glm::i32vec2;		/// Signed 32*2b integer vector
using vlVec2f = glm::f32vec2;		/// Signed 32*2b floating-point vector
using vlVec3f = glm::f32vec3;		/// Signed 32*3b floating-point vector
using vlVec4f = glm::f32vec4;		/// Signed 32*4b floating-point vector
using vlVec3f64 = glm::f64vec3;		/// Signed 64*3b floating-point vector
using vlMat4f = glm::f32mat4;		/// Signed 32*4*4b floating-point matrix

using vlString = std::string;
template<class T, class U>
using vlMap = std::map<T,U>;
template<class T>
using vlVector = std::vector<T>;
template<class T>
using vlUniquePtr = std::unique_ptr<T>;
template<class T>
using vlSharedPtr = std::shared_ptr<T>;

//template<class T>
//vlUniquePtr<T>(*give)(vlUniquePtr<T> a_uniquePointer) = &std::move; /// vlUniquePtr = give(vlUniquePtr)


// Namespaces comments:

/// VolkPhetamine
namespace VolkPhetamine {}
/// VulkanWrappers
namespace VulkanWrappers{}