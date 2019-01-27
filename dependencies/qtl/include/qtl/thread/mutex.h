#ifndef __mutex_h_
#define __mutex_h_

#include <cassert>

#if defined ( _WIN32 )
#include <Windows.h>
#elif defined ( __linux__ )
#include <pthread.h>
#endif

namespace qtl
{
	/// <summary>
	/// Mutex is a primitive synchronization primitive used to protect shared data from simultaneous access by 
	/// multiple threads.  A mutex offers exclusive, non recursive ownership semantics.  The owning thread
	/// must be the thread to unlock the mutex when it is done.  If a mutex is owned, all subsequent calls to lock
	/// are blocked until the owning thread relinquishes ownership.  If a mutex is reaquired by the owning thread,
	/// this results in undefined behavior
	/// </summary>
	class mutex
	{
	public:
#if defined ( _WIN32 )
		/// <summary>
		/// Native handle type
		/// </summary>
		typedef PSRWLOCK native_handle_type;
#elif defined ( __linux__ )
		/// <summary>
		/// Native handle type
		/// </summary>
		typedef pthread_mutex_t * native_handle_type;
#endif
		/// <summary>
		/// Constructs a new mutex
		/// </summary>
		mutex() noexcept;

		/// <summary>
		/// Deleted copy constructor
		/// </summary>
		mutex(const mutex&) = delete;

		/// <summary>
		/// Deallocates mutex
		/// </summary>
		~mutex();

		/// <summary>
		/// Deleted copy assignment
		/// </summary>
		/// <returns>
		/// Reference to this
		/// </returns>
		mutex& operator=(const mutex&) = delete;

		/// <summary>
		/// Locks mutex for provided thread, giving that thread ownership of that 
		/// mutex.  If the mutex is held by another thread, the mutex blocks.  If the mutex
		/// is held by this thread, this results in undefined behavior.  A lock call must
		/// be followed by an unlock call.
		/// </summary>
		void lock();

		/// <summary>
		/// Tries to lock the mutex.  If the lock fails, returns false and does not block.  If 
		/// successful, locks and returns true.
		/// </summary>
		/// <returns>
		/// Returns true if lock succeeded, false if lock was already held.
		/// </returns>
		bool try_lock();

		/// <summary>
		/// Unlocks the mutex and releases ownership from the calling thread.  The calling thread
		/// must have ownership of the mutex.  If unlock is called by a non-owning thread, this
		/// results in undefined behavior.
		/// </summary>
		void unlock();

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

	/// <summary>
	/// Flag to tell lock to not acquire mutex upon construction
	/// </summary>
	struct defer_lock_t
	{
		/// <summary>
		/// Default constructor
		/// </summary>
		explicit defer_lock_t() = default;
	};

	/// <summary>
	/// Flag to tell lock to try to acquire lock upon construction without blocking
	/// </summary>
	struct try_to_lock
	{
		/// <summary>
		/// Default constructor
		/// </summary>
		explicit try_to_lock() = default;
	};

	/// <summary>
	/// Flag to tell lock that the thread already owns the mutex
	/// </summary>
	struct adopt_lock_t
	{
		/// <summary>
		/// Default constructor
		/// </summary>
		explicit adopt_lock_t() = default;
	};

	/// <summary>
	/// General purpose mutex wrapper with option for deferred locking schemes
	/// </summary>
	/// <typeparam name="Mutex">
	/// Type of mutex stored
	/// </typeparam>
	template<typename Mutex>
	class unique_lock
	{
	public:
		/// <summary>
		/// Constructs a unique lock with no associated mutex
		/// </summary>
		unique_lock() noexcept;

		/// <summary>
		/// Move constructor for lock
		/// </summary>
		/// <param name="other">
		/// Lock to move ownership from
		/// </param>
		unique_lock(unique_lock&& other) noexcept;
		
		/// <summary>
		/// Constructs a lock from a mutex and locks the mutex on construction
		/// of the lock
		/// </summary>
		/// <param name="m">
		/// Mutex to wrap and lock
		/// </param>
		explicit unique_lock(Mutex& m);

		/// <summary>
		/// Constructs a lock from a mutex, but defers locking to an explicit call
		/// to lock
		/// </summary>
		/// <param name="m">
		/// Mutex to wrap
		/// </param>
		/// <param name="t">
		/// Denotes to defer locking
		/// </param>
		unique_lock(Mutex& m, qtl::defer_lock_t t) noexcept;

		/// <summary>
		/// Constructs a lock from a mutex, but tries to lock the mutex without blocking
		/// </summary>
		/// <param name="m">
		/// Mutex to wrap
		/// </param>
		/// <param name="t">
		/// Denotes to try to lock without blocking
		/// </param>
		unique_lock(Mutex& m, qtl::try_to_lock t);

		/// <summary>
		/// Constructs a lock from a mutex, but the calling thread already holds the mutex
		/// </summary>
		/// <param name="m">
		/// Mutex to wrap
		/// </param>
		/// <param name="t">
		/// Denotes the calling thread already holds the mutex
		/// </param>
		unique_lock(Mutex& m, qtl::adopt_lock_t t);

		/// <summary>
		/// Releases and unlocks the mutex
		/// </summary>
		~unique_lock();

		/// <summary>
		/// Deleted copy method
		/// </summary>
		/// <returns>
		/// Reference to this
		/// </returns>
		unique_lock& operator=(const unique_lock&) noexcept = delete;

		/// <summary>
		/// Moves the other lock's mutex to this lock, unlocking the currently held mutex
		/// </summary>
		/// <param name="other">
		/// Lock to move mutex from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		unique_lock& operator=(unique_lock&& other) noexcept;

		/// <summary>
		/// Locks this mutex if it is not already held
		/// </summary>
		void lock();

		/// <summary>
		/// Tries to lock this mutex without blocking the thread
		/// </summary>
		/// <returns>
		/// True if lock acquired, else false
		/// </returns>
		bool try_lock();

		/// <summary>
		/// Unlocks the mutex if it is held
		/// </summary>
		void unlock();

		/// <summary>
		/// Swaps the mutex ownership between two locks
		/// </summary>
		/// <param name="lock">
		/// Lock to swap mutex ownership with
		/// </param>
		void swap(unique_lock& lock);

		/// <summary>
		/// Releases ownership of the mutex without unlocking it
		/// </summary>
		void release();

		/// <summary>
		/// Gets pointer to the wrapped mutex
		/// </summary>
		/// <returns>
		/// Pointer to wrapper mutex
		/// </returns>
		Mutex* mutex() const noexcept;

		/// <summary>
		/// Checks if lock owns the mutex
		/// </summary>
		/// <returns>
		/// True if mutex is owned, else false
		/// </returns>
		bool owns_lock() const noexcept;

		/// <summary>
		/// Checks if lock owns the mutex
		/// </summary>
		/// <returns>
		/// True if mutex is owned, else false
		/// </returns>
		explicit operator bool() const noexcept;
	private:
		Mutex* __mutex;
		bool __owns;
	};

	template<typename Mutex>
	inline unique_lock<Mutex>::unique_lock() noexcept
		: __mutex(nullptr), __owns(false)
	{
	}

	template<typename Mutex>
	inline unique_lock<Mutex>::unique_lock(unique_lock && other) noexcept
	{
		__mutex = other.__mutex;
		__owns = other.__owns;

		other.__mutex = nullptr;
		other.__owns = false;
	}

	template<typename Mutex>
	inline unique_lock<Mutex>::unique_lock(Mutex & m)
		: __mutex(__builtin_addressof(m))
	{
		__mutex->lock();
		__owns = true;
	}

	template<typename Mutex>
	inline unique_lock<Mutex>::unique_lock(Mutex & m, qtl::defer_lock_t t) noexcept
		: __mutex(__builtin_addressof(m)), __owns(false)
	{
	}

	template<typename Mutex>
	inline unique_lock<Mutex>::unique_lock(Mutex & m, qtl::try_to_lock t)
		: __mutex(__builtin_addressof(m))
	{
		__owns = __mutex->try_lock();
	}

	template<typename Mutex>
	inline unique_lock<Mutex>::unique_lock(Mutex & m, qtl::adopt_lock_t t)
		: __mutex(__builtin_addressof(m)), __owns(true)
	{
	}

	template<typename Mutex>
	inline unique_lock<Mutex>::~unique_lock()
	{
		if (__owns)
		{
			__mutex->unlock();
		}
#if defined(_DEBUG)
		__mutex = nullptr;
		__owns = false;
#endif
	}

	template<typename Mutex>
	inline unique_lock<Mutex>& unique_lock<Mutex>::operator=(unique_lock && other) noexcept
	{
		if (__owns) __mutex->unlock();
		__owns = other.__owns;
		__mutex = other.__mutex;
		other.__mutex = nullptr;
		other.__owns = false;
		return *this;
	}

	template<typename Mutex>
	inline void unique_lock<Mutex>::lock()
	{
		if (!__owns)
		{
			__mutex->lock();
		}
		else
		{
			assert(!__owns); // lock owns mutex
		}
	}

	template<typename Mutex>
	inline bool unique_lock<Mutex>::try_lock()
	{
		if (!__owns)
		{
			return __mutex->try_lock();
		}
		assert(!__owns); // lock owns mutex
		return false;
	}

	template<typename Mutex>
	inline void unique_lock<Mutex>::unlock()
	{
		if (__owns)
		{
			__mutex->unlock();
		}
		else
		{
			assert(__owns); // mutex does not own lock
		}
	}

	template<typename Mutex>
	inline void unique_lock<Mutex>::swap(unique_lock & lock)
	{
		Mutex mutex = __mutex;
		__mutex = lock.__mutex;
		lock.__mutex = mutex;

		bool owns = __owns;
		__owns = lock.__owns;
		lock.__owns = owns;
	}

	template<typename Mutex>
	inline void unique_lock<Mutex>::release()
	{
		__mutex = nullptr;
		__owns = false;
	}

	template<typename Mutex>
	inline Mutex * unique_lock<Mutex>::mutex() const noexcept
	{
		return __mutex;
	}

	template<typename Mutex>
	inline bool unique_lock<Mutex>::owns_lock() const noexcept
	{
		return __owns;
	}

	template<typename Mutex>
	inline unique_lock<Mutex>::operator bool() const noexcept
	{
		return __owns;
	}
}

#endif