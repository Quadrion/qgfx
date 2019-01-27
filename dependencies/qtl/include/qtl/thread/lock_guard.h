#ifndef __lockguard_h_
#define __lockguard_h_

#include "qtl/thread/mutex.h"

namespace qtl
{

	/// <summary>
	/// RAII wrapper for mutex that locks on acquisition/construction
	/// and unlocks on destruction
	/// </summary>
	/// <typeparam name="mutex_type">
	/// Type of mutex
	/// </typeparam>
	template<typename mutex_type>
	class lock_guard
	{
	public:
		/// <summary>
		/// Constructs a new lock guard with a mutex, locking it on acquisition.  Locking/unlocking the mutex
		/// in user code after acquisition is undefined behavior.
		/// </summary>
		/// <param name="m">
		/// Mutex to acquire and lock
		/// </param>
		explicit lock_guard(mutex_type & m);

		/// <summary>
		/// Constructs a new lock guard with a mutex.  Locking/unlocking the mutex in user code after acquisition 
		/// is undefined behavior.  Lock guard does not lock on acquisition, as it is denoted that the mutex is already
		/// locked.
		/// </summary>
		/// <param name="m">
		/// Mutex to acquire and lock
		/// </param>
		/// <param name="t">
		/// Flag that mutex is already locked
		/// </typeparam>
		lock_guard(mutex_type & m, adopt_lock_t t);

		/// <summary>
		/// Deleted copy constructor
		/// </summary>
		lock_guard(const lock_guard&) = delete;

		/// <summary>
		/// Unlocks held mutex
		/// </summary>
		~lock_guard();

		/// <summary>
		/// Deleted copy semantics
		/// </summary>
		/// <returns>
		/// Reference to this
		/// </returns>
		lock_guard& operator=(const lock_guard&) = delete;
	private:
		mutex_type & __mutex;
	};

	template<typename mutex_type>
	inline lock_guard<mutex_type>::lock_guard(mutex_type & m)
		: __mutex(m)
	{
		__mutex.lock();
	}

	template<typename mutex_type>
	inline lock_guard<mutex_type>::lock_guard(mutex_type & m, adopt_lock_t t)
		: __mutex(m)
	{
	}

	template<typename mutex_type>
	inline lock_guard<mutex_type>::~lock_guard()
	{
		__mutex.unlock();
	}

}

#endif