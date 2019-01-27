#ifndef __sharedmutex_h_
#define __sharedmutex_h_

#if defined ( _WIN32 )
// MSVC
#include <Windows.h>
#elif defined ( __linux__ )
// GNU C++
#include <pthread.h>
#endif

namespace qtl
{
	/// <summary>
	/// Shared mutex is a synchronization primitive allowing for both shared
	/// and exclusive access to a protected variable.  Shared access should be
	/// used for read only operations, whereas exclusive access should be used
	/// for writing operations.
	/// </summary>
	class shared_mutex
	{
	public:
#if defined ( _WIN32 )
		// MSVC
		typedef PSRWLOCK native_handle_type;
#elif defined ( __linux__ )
		// GNU C++
		typedef pthread_rwlock_t * native_handle_type;
#endif
		/// <summary>
		/// Constructs a shared mutex
		/// </summary>
		shared_mutex();

		/// <summary>
		/// Deleted copy constructor
		/// </summary>
		shared_mutex(const shared_mutex&) = delete;

		/// <summary>
		/// Deallocates resources held by shared mutex
		/// </summary>
		~shared_mutex();

		/// <summary>
		/// Deleted copy of shared mutex
		/// </summary>
		/// <returns>
		/// Reference to this
		/// </returns>
		shared_mutex& operator=(const shared_mutex&) = delete;

		/// <summary>
		/// Locks mutex in exclusive mode.  Blocks if mutex is held.
		/// </summary>
		void lock();

		/// <summary>
		/// Tries to lock mutex in exclusive mode
		/// </summary>
		/// <returns>
		/// True if lock acquired, else false
		/// </returns>
		bool try_lock();

		/// <summary>
		/// Unlocks mutex from exclusive mode.  Undefined if lock is not
		/// held in exclusive mode by calling thread
		/// </summary>
		void unlock();

		/// <summary>
		/// Locks mutex in shared mode.  Blocks if mutex is held in exclusive
		/// mode.
		/// </summary>
		void lock_shared();

		/// <summary>
		/// Tries to lock mutex in shared mode.  Returns false if mutex is 
		/// held in exclusive mode or could not be acquired.
		/// </summary>
		/// <returns>
		/// True if lock acquired, else false
		/// </returns>
		bool try_lock_shared();

		/// <summary>
		/// Unlocks mutex from shared mode.  Undefined if lock is not
		/// held in exclusive mode by calling thread
		/// </summary> 
		void unlock_shared();

		/// <summary>
		/// Gets shared mutex's native handle
		/// </summary>
		/// <returns>
		/// Native handle of shared mutex
		/// </returns>
		native_handle_type native_handle();
	private:
		native_handle_type __handle;
	};
}

#endif