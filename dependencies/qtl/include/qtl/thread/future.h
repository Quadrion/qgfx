#ifndef __future_h_
#define __future_h_

#include "qtl/shared_ptr.h"
#include "qtl/utility.h"
#include "qtl/thread/condition_variable.h"
#include "qtl/thread/mutex.h"

namespace qtl
{
	template <typename T>
	class promise;

	template <typename T>
	class future;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	namespace qinternal
	{
		template <typename T>
		struct future_implementation
		{
			T __value;
			condition_variable __cond;
			mutex __mutex;
			bool __set;
		};
	}
#endif

	/// <summary>
	/// Represents a value that will become available at some point
	/// in the future, usually by another thread of execution
	/// </summary>
	/// <typeparam name="T"> 
	/// Type of variable stored
	/// </typeparam>
	template <typename T>
	class future
	{
		template <typename T>
		friend class promise;
	public:
		/// <summary>
		/// Constructs a default future from no promise.  Calling get() and wait()
		/// results in undefined behavior, unless a future derived from a promise
		/// is copied into this future.
		/// </summary>
		future();

		/// <summary>
		/// Deleted copy constructor
		/// </summary>
		future(const future&) = delete;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="other">
		/// Future to move implementation from
		/// </param>
		future(future&& other) noexcept;

		/// <summary>
		/// Deconstructs future
		/// </summary>
		~future();

		/// <summary>
		/// Deleted copy operation
		/// </summary>
		/// <returns>
		/// Reference to this
		/// </returns>
		future& operator=(const future&) = delete;

		/// <summary>
		/// Move operation
		/// </summary>
		/// <param name="other">
		/// Future to move into this 
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		future& operator=(future&& other) noexcept;

		/// <summary>
		/// Call to get the value of the future.  If the value is not ready,
		/// blocks until ready.
		/// </summary>
		/// <returns>
		/// Value captured by future
		/// </returns>
		T get();

		/// <summary>
		/// Blocks calling thread until value is ready to fetch
		/// </summary>
		void wait();
	private:
		shared_ptr<qinternal::future_implementation<T>> __future_impl;
	};

	template<typename T>
	inline future<T>::future()
	{
	}

	template<typename T>
	inline future<T>::future(future && other) noexcept
		: __future_impl(qtl::move(other.__future_impl))
	{
	}

	template<typename T>
	inline future<T>::~future()
	{
	}

	template<typename T>
	inline future<T>& future<T>::operator=(future && other) noexcept
	{
		__future_impl = qtl::move(other.__future_impl);
		return *this;
	}

	template<typename T>
	inline T future<T>::get()
	{
		unique_lock<mutex> lock(__future_impl->__mutex);
		__future_impl->__cond.wait(lock, [=]() { return __future_impl->__set; });
		T res = __future_impl->__value;
		return res;
	}

	template<typename T>
	inline void future<T>::wait()
	{
		__future_impl->__mutex.lock();
		__future_impl->__cond.wait(__future_impl->__mutex, [=]() { return __future_impl->__set; });
		__future_impl->__mutex.unlock();
	}

	/// <summary>
	/// Represents a contractual agreement to fetch a value
	/// </summary>
	/// <typeparam name="T">
	/// Type of value in agreement
	/// </typeparam>
	template <typename T>
	class promise
	{
	public:
		/// <summary>
		/// Creates a new promise
		/// </summary>
		promise();

		/// <summary>
		/// Deleted copy constructor
		/// </summary>
		promise(const promise&) = delete;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="other">
		/// Promise to move agreement from
		/// </param>
		promise(promise&& other) noexcept;

		/// <summary>
		/// Destroys promise
		/// </summary>
		~promise();

		/// <summary>
		/// Deleted copy operator
		/// </summary>
		/// <returns>
		/// Reference to this
		/// </returns>
		promise& operator=(const promise&) = delete;

		/// <summary>
		/// Moves other's contract to this, releasing this contract.  If a future
		/// is waiting on the released contract, the wait() or get() call becomes
		/// undefined.
		/// </summary>
		/// <param name="other">
		/// Promise to move contract from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		promise& operator=(promise&& other) noexcept;

		/// <summary>
		/// Constructs a future representing the other side of the contractual agreement.
		/// </summary>
		/// <returns>
		/// Future representing the other portion of the contractual agreement for the result
		/// value
		/// </returns>
		future<T> get_future();

		/// <summary>
		/// Sets value and satisfies the promise's contractual agreement
		/// </summary>
		/// <param name="other">
		/// Value to set internal promise/future result to
		/// </param>
		void set_value(T && other);

		/// <summary>
		/// Sets value and satisfies the promise's contractual agreement
		/// </summary>
		/// <param name="other">
		/// Value to set internal promise/future result to
		/// </param>
		void set_value(const T& other);
	private:
		shared_ptr<qinternal::future_implementation<T>> __future_impl;
	};

	template<typename T>
	inline promise<T>::promise()
	{
		__future_impl = make_shared<qinternal::future_implementation<T>>();
	}

	template<typename T>
	inline promise<T>::promise(promise && other) noexcept
		: __future_impl(qtl::move(other.__future_impl))
	{
	}

	template<typename T>
	inline promise<T>::~promise()
	{
	}

	template<typename T>
	inline promise<T>& promise<T>::operator=(promise && other) noexcept
	{
		__future_impl = qtl::move(other.__future_impl);
		return *this;
	}

	template<typename T>
	inline future<T> promise<T>::get_future()
	{
		future<T> res;
		res.__future_impl = __future_impl;
		return qtl::move(res);
	}

	template<typename T>
	inline void promise<T>::set_value(T && other)
	{
		__future_impl->__mutex.lock();
		__future_impl->__value = qtl::move(other);
		__future_impl->__mutex.unlock();
		__future_impl->__cond.notify_one();
	}

	template<typename T>
	inline void promise<T>::set_value(const T& other)
	{
		__future_impl->__value = qtl::move(other);
	}
}

#endif