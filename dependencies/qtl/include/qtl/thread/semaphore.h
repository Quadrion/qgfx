#ifndef __semaphore_h_
#define __semaphore_h_

#if defined ( _WIN32 )
#include <Windows.h>
#elif defined ( __linux__ )
#include <semaphore.h>
#endif

namespace qtl
{

	/// <summary>
	/// Semaphore is a synchronization primitive that maintains a count above zero.  The 
	/// count is decremented on wait and incremented on post.  When wait is 0, subsequent calls
	/// to wait result in blocking until the next post call.  Semaphores are useful for sharing 
	/// resources that can handle only a specific number of consumers at one time.
	/// </summary>
	class semaphore
	{
	public:
#if defined ( _WIN32 )
		/// <summary>
		/// Native handle type
		/// </summary>
		typedef HANDLE native_handle_type;
#elif defined ( __linux__ )
		/// <summary>
		/// Native handle type
		/// </summary>
		typedef sem_t * native_handle_type;
#endif

		/// <summary>
		/// Constructs a new semaphore with count 0
		/// </summary>
		semaphore() noexcept;

		/// <summary>
		/// Deleted copy constructor
		/// </summary>
		semaphore(const semaphore&) = delete;

		/// <summary>
		/// Deallocates semaphore
		/// </summary>
		~semaphore();

		/// <summary>
		/// Deleted copy assignment
		/// </summary>
		/// <returns>
		/// Reference to this
		/// </returns>
		semaphore& operator=(const semaphore&) = delete;

		/// <summary>
		/// Wait decrements the semaphore's counter.  If the counter is 0 upon the wait call, the semaphore will block 
		/// until a post call wakes the thread.
		/// </summary>
		void wait();

		/// <summary>
		/// Tries to decrement a semaphore's counter.  If the counter is 0 at the time of the call, returns false as the
		/// semaphore cannot be acquired by the thread.  If the semaphore can be acquired, returns true.
		/// </summary>
		/// <returns>
		/// True if semaphore acquired, else false.
		/// </returns>
		bool try_wait();

		/// <summary>
		/// Increases the semaphore's counter by 1.
		/// </summary>
		void post();

		/// <summary>
		/// Retrieves the native handle of the mutex
		/// </summary>
		/// <returns>
		/// Native handle
		/// </returns>
		native_handle_type native_handle();
	private:
		native_handle_type __handle;
	};

}

#endif