#ifndef __conditionvariable_h_
#define __conditionvariable_h_

#include "qtl/thread/mutex.h"

#if defined ( _WIN32 )
// MSVC
#include <Windows.h>
#elif defined ( __GNUG__ )
// GNU C++
#include <pthread.h>
#elif defined ( __clang__ )
// Clang
#endif

namespace qtl
{

	/// <summary>
	/// Synchronization primitive used to block threads until a condition is met.  Requires
	/// a mutex to ensure atomicity of the condition check.
	/// </summary>
	class condition_variable
	{
	public:
#if defined ( _WIN32 )
		// MSVC

		/// <summary>
		/// Native handle type
		/// </summary>
		typedef PCONDITION_VARIABLE native_handle_type;
#elif defined ( __GNUG__ )
		// GNU C++
		typedef pthread_t * native_handle_type;
#elif defined ( __clang__ )
		// Clang
#endif
		/// <summary>
		/// Constructs a default condition variable
		/// </summary>
		condition_variable();

		/// <summary>
		/// Deleted copy constructor
		/// </summary>
		condition_variable(const condition_variable&) = delete;

		/// <summary>
		/// Deallocates condition variable
		/// </summary>
		~condition_variable();

		/// <summary>
		/// Deleted copy for condition variable
		/// </summary>
		/// <returns>
		/// Reference to this
		/// </returns>
		condition_variable& operator=(const condition_variable&) = delete;

		/// <summary>
		/// Notifies one waiting thread that the condition passed
		/// </summary>
		void notify_one() noexcept;

		/// <summary>
		/// Notifies all waiting threads that the condition passed
		/// </summary>
		void notify_all() noexcept;

		/// <summary>
		/// Waits for the condition to be valid
		/// </summary>
		/// <param name="lock">
		/// Lock for ensuring atomicity of condition variable wait
		/// </param>
		void wait(qtl::unique_lock<qtl::mutex>& lock);

		/// <summary>
		/// Waits for the condition to be valid
		/// </summary>
		/// <typeparam name="Predicate">
		/// Type of function, must be of form: bool function_name()
		/// </typeparam>
		/// <param name="lock">
		/// Lock for ensuring atomicity of condition variable wait
		/// </param>
		/// <param name="pred">
		/// Predicate function representing condition
		/// </param>
		template<typename Predicate>
		void wait(qtl::unique_lock<qtl::mutex>& lock, Predicate pred);

		native_handle_type native_handle();
	private:
		native_handle_type __handle;
	};

	template<typename Predicate>
	inline void condition_variable::wait(qtl::unique_lock<qtl::mutex>& lock, Predicate pred)
	{
		while (!pred())
		{
			wait(lock);
		}
	}

}

#endif