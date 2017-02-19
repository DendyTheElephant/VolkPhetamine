////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////====================================================================================================================================////
/// | \file		CInputHandler
///	| \author	Daniel Huc
/// | \date		February 2017
/// |----------------------------------------------------------------------------------------------------------------------------------
/// | \brief	Blabla
/// | \details	Blabla
////====================================================================================================================================////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
/* Standard library includes */
#include <iostream>

/* External dependencies */
#include <SDL2/SDL.h>

/* Internal headers includes */
#include "HInternalTypeAliases.h"

/* Specific defines */

namespace VolkPhetamine {

	class CInputHandler {
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//// ---- Enumerations -----                                                                                                            ////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	public:
		enum class EInput {
			UP = SDLK_z,
			DOWN = SDLK_s,
			RIGHT = SDLK_q,
			LEFT = SDLK_d,
			QUIT = SDLK_ESCAPE
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//// ---- Members -----																													//// 
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	private:
		SDL_Event event;
		vlMap<EInput, vlUInt16> m_inputs;
		vlInt m_mouseMotionX{ 0 };
		vlInt m_mouseMotionY{ 0 };
		vlUInt16 m_mouseButton1{ 0 };
		vlUInt16 m_mouseButton2{ 0 };
		vlBool m_endEvent{ false };
		vlBool m_pause{ false };

		SDL_Window* m_window;
		vlInt M_WINDOW_CENTER_X;
		vlInt M_WINDOW_CENTER_Y;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//// ---- Methods -----																													////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	private:
		void _updateInput(SDL_Keycode key, vlUInt16 value);

	public:
		//// ---- Constructor/Destructor ---- ////
		CInputHandler(SDL_Window* window);

		//// ---- Getter ---- ////
		vlUInt getKey(EInput const& requiredInput) const { return m_inputs.at(requiredInput); }
		vlInt getMouseMotionX() const { return m_mouseMotionX; }
		vlInt getMouseMotionY() const { return m_mouseMotionY; }
		vlBool isQuit() const { return (m_endEvent || getKey(EInput::QUIT)); }

		//// ---- Core ---- ////
		void update();
	};
}