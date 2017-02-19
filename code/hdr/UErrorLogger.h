////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////====================================================================================================================================////
/// | \file		UErrorLogger
///	| \author	Daniel Huc
/// | \date		February 2017
/// |----------------------------------------------------------------------------------------------------------------------------------
/// | \brief	Blabla
/// | \details	Blabla
////====================================================================================================================================////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
/* Standard library includes */
#include <stack>
#include <iostream>
#include <fstream>
#include <string>

/* Internal headers includes */
#include "HInternalTypeAliases.h"
#include "HSingleton.h"

/* Specific defines */
// Define __CURRENT_FUNCTION__ macro to call __func__ define for c++11 gcc or __FUNCTION__ defined for MSVSC!
#ifndef __CURRENT_FUNCTION__
	#ifdef _MSC_VER
		#define __CURRENT_FUNCTION__ __FUNCTION__
	#else
		#define __CURRENT_FUNCTION__ PRINTVALUE(__func__)
	#endif
#endif

// Double macro to display the number of the line instead of "__LINE__"
#define S(value) #value
#define PRINTVALUE(value) S(value)

// Stack display message (file / line / function)
#define __CONTEXT__ ("> File " __FILE__ ":"  PRINTVALUE(__LINE__) " in function " __CURRENT_FUNCTION__ "\n")

#define VL_STACK_TRACE (&VolkPhetamine::UErrorLogger::getInstance())->stack(__CONTEXT__)
#define VL_STACK_MESSAGE(message) (&VolkPhetamine::UErrorLogger::getInstance())->stackMessage(message,__CONTEXT__)
#define VL_UNSTACK_TRACE (&VolkPhetamine::UErrorLogger::getInstance())->unstack()
#define VL_ERROR(message) (&VolkPhetamine::UErrorLogger::getInstance())->displayAndCrash(message,__CONTEXT__)
#define VL_ERROR_CONTINUE(message) (&VolkPhetamine::UErrorLogger::getInstance())->display(message,__CONTEXT__)

namespace VolkPhetamine {

	class UErrorLogger {//: public vlSingleton<UErrorLogger> {

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// ---- Members -----																													//// 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	private:
		vlBool m_isErrorStreamSpecified{ false }; ///< If Error's constructor is default, display error goes to print in cerr!
		std::stack<vlString> m_errorCallStack;
		std::ofstream m_outputStream;
		const vlString M_STACK_TRACE_MARK = ">" ;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// ---- Methods -----																													////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	private:
	//// ---- Constructor/Destructor ---- ////
		UErrorLogger() {/* VOID */ };
		~UErrorLogger();

	public:
	//// ---- Setter ---- ////
		void setOutputFile(vlString fileName);

	//// ---- Core ---- ////
		void display(vlString errorMessage, vlString context);
		void displayAndCrash(vlString errorMessage, vlString context);

		/// Called from VK_STACK_TRACE: push function context into the stack
		void stack(vlString a_context) {
			m_errorCallStack.push(M_STACK_TRACE_MARK);
			m_errorCallStack.push(a_context);
		}

		/// Called from VK_STACK_MESSAGE: push message in current context into the stack
		void stackMessage(vlString a_message, vlString a_context) {
			m_errorCallStack.push(a_context + " " + a_message + "\n");
		}

		/// Called from VK_UNSTACK_TRACE: pop up messages and last context off the stack
		void unstack() {
			// Drop message stack and call
			while (m_errorCallStack.top() != M_STACK_TRACE_MARK) {
				if (!m_errorCallStack.empty())
					m_errorCallStack.pop();
			}
			// Drop stack trace marker
			if (!m_errorCallStack.empty())
				m_errorCallStack.pop();
		}

	//// ---- Static ---- ////
		static UErrorLogger& getInstance();
		static void destroyInstance();
	};
}
