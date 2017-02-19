//#include "CInputHandler.h"
//
//namespace VolkPhetamine {
//
//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	////  ================================================================================================================================  ////
//	////    ---- Internal methods -----                                                                                                     ////
//	////  ================================================================================================================================  ////
//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	//----------------------------------------------------------------------------------------------------------------------------------------//
//	/// 
//	//----------------------------------------------------------------------------------------------------------------------------------------//
//	void CInputHandler::_updateInput(SDL_Keycode a_key, vlUInt16 a_value) {
//		EInput input = static_cast<EInput>(a_key);
//		if (m_inputs.find(input) != m_inputs.end()) {
//			m_inputs.at(input) = a_value;
//		}
//	}
//
//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	////  ================================================================================================================================  ////
//	////    ---- Constructor/Destructor -----                                                                                               ////
//	////  ================================================================================================================================  ////
//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	//----------------------------------------------------------------------------------------------------------------------------------------//
//	/// 
//	//----------------------------------------------------------------------------------------------------------------------------------------//
//	CInputHandler::CInputHandler(SDL_Window* window) {
//		m_window = window;
//		SDL_GetWindowSize(window, &M_WINDOW_CENTER_X, &M_WINDOW_CENTER_Y);
//		M_WINDOW_CENTER_X = M_WINDOW_CENTER_X / 2;
//		M_WINDOW_CENTER_Y = M_WINDOW_CENTER_Y / 2;
//		SDL_WarpMouseInWindow(m_window, M_WINDOW_CENTER_X, M_WINDOW_CENTER_Y);
//
//		// Input initialisation
//		const vlVector<EInput> mappedInputs{
//			EInput::DOWN,
//			EInput::LEFT,
//			EInput::UP,
//			EInput::DOWN,
//			EInput::RIGHT,
//			EInput::LEFT,
//			EInput::QUIT
//		};
//		for (auto const& it_input : mappedInputs) {
//			m_inputs[it_input] = 0;
//		}
//	}
//
//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	////  ================================================================================================================================  ////
//	////    ---- Core -----                                                                                                                 ////
//	////  ================================================================================================================================  ////
//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	//----------------------------------------------------------------------------------------------------------------------------------------//
//	/// 
//	//----------------------------------------------------------------------------------------------------------------------------------------//
//	void CInputHandler::update() {
//		// Incrementing the input duration of pressed keys and mouse_buttons
//		for (auto it_input : m_inputs) {
//			it_input.second++;
//		}
//		if (m_mouseButton1)
//			m_mouseButton1++;
//		if (m_mouseButton2)
//			m_mouseButton2++;
//
//		// Process events from the SDL event's qeue
//		while (SDL_PollEvent(&event)) {
//			if (event.type == SDL_QUIT) {
//				m_endEvent = true;
//			}
//			if (event.type == SDL_KEYDOWN) {
//				_updateInput(event.key.keysym.sym, 1);
//			}
//			if (event.type == SDL_KEYUP) {
//				_updateInput(event.key.keysym.sym, 0);
//			}
//			if (event.type == SDL_MOUSEMOTION) {
//				m_mouseMotionX = event.motion.x - M_WINDOW_CENTER_X;
//				m_mouseMotionY = event.motion.y - M_WINDOW_CENTER_Y;
//			}
//			if (event.type == SDL_MOUSEBUTTONDOWN) {
//				if (event.button.button == SDL_BUTTON_LEFT) { m_mouseButton1 = 1; }
//				if (event.button.button == SDL_BUTTON_RIGHT) { m_mouseButton2 = 1; }
//			}
//			if (event.type == SDL_MOUSEBUTTONUP) {
//				if (event.button.button == SDL_BUTTON_LEFT) { m_mouseButton1 = 0; }
//				if (event.button.button == SDL_BUTTON_RIGHT) { m_mouseButton2 = 0; }
//			}
//			if (event.type == SDL_WINDOWEVENT) {
//				if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
//					m_pause = true;
//				if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
//					m_pause = false;
//			}
//		}
//
//		// Block the mouse at the center of the window!
//		if (m_pause == false) {
//			SDL_WarpMouseInWindow(m_window, M_WINDOW_CENTER_X, M_WINDOW_CENTER_Y);
//		}
//	}
//}