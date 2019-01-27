#ifndef __thread_h_
#define __thread_h_

#include "qtl/function.h"
#include "qtl/hash.h"

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
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	namespace qinternal
	{
		template<size_t ...>
		struct IntegerSequence
		{
		};

		template<size_t N, size_t ... S>
		struct GeneralIntegerSequence : GeneralIntegerSequence<N - 1, N - 1, S...>
		{
		};

		template<size_t ... S>
		struct GeneralIntegerSequence<0, S...>
		{
			typedef IntegerSequence<S...> type;
		};

		class ICallable
		{
		public:
			virtual ~ICallable() = default;
		};

		template<typename Func, typename ... Args>
		class ThreadFunctionCall : public ICallable
		{
			typedef qtl::tuple<Args...> Tuple;
		public:
			ThreadFunctionCall(Func&& fun, Args&& ... args);
			~ThreadFunctionCall() override = default;
			void callFunc();
		private:
			Func __func;
			qtl::tuple<Args...> __arguments;

			template<size_t ... S>
			void callFunc(IntegerSequence<S...>);
		};

		template<typename Func, typename ...Args>
		inline ThreadFunctionCall<Func, Args...>::ThreadFunctionCall(Func && fun, Args && ...args)
			: __func(qtl::forward<Func>(fun)), __arguments(qtl::forward<Args>(args)...)
		{
		}

		template<typename Func, typename ...Args>
		inline void ThreadFunctionCall<Func, Args...>::callFunc()
		{
			callFunc(typename GeneralIntegerSequence<sizeof...(Args)>::type());
		}

		template<typename Func, typename ...Args>
		template<size_t ...S>
		inline void ThreadFunctionCall<Func, Args...>::callFunc(IntegerSequence<S...>)
		{
			invoke(qtl::forward<Func>(__func),
				qtl::get<S>(qtl::forward<Tuple>(__arguments))...);
		}
	}
#endif

	/// <summary>
	/// Represents a single thread of execution.  Thread objects allow for multiple functions
	/// to execute concurrently, starting with the function provided in the argument.
	/// </summary>
	class thread
	{
	public:
#if defined ( _WIN32 )
		// MSVC

		/// <summary>
		/// Native handle type
		/// </summary>
		typedef HANDLE native_handle_type;
#elif defined ( __GNUG__ )
		// GNU C++
		typedef pthread_t * native_handle_type;
#elif defined ( __clang__ )
		// Clang
#endif
		/// <summary>
		/// Representation of thread identifier received from the 
		/// system 
		/// </summary>
		class id
		{
		public:
			/// <summary>
			/// Constructs a default id
			/// </summary>
			id() noexcept;

			/// <summary>
			/// Constructs an id from the native handle
			/// </summary>
			/// <param name="id">
			/// Native handle of thread
			/// </param>
			explicit id(native_handle_type id);
		private:
			native_handle_type __handle;

			friend class thread;
			friend struct hash<thread::id>;
		public:

			/// <summary>
			/// Checks if two IDs are equal
			/// </summary>
			/// <param name="x">
			/// Left operand
			/// </param>
			/// <param name="y">
			/// Right operand
			/// </param>
			/// <returns>
			/// Return if equal
			/// </returns>
			friend bool operator == (thread::id x, thread::id y) noexcept;

			/// <summary>
			/// Checks if two IDs are not equal
			/// </summary>
			/// <param name="x">
			/// Left operand
			/// </param>
			/// <param name="y">
			/// Right operand
			/// </param>
			/// <returns>
			/// Return if not equal
			/// </returns>
			friend bool operator != (thread::id x, thread::id y) noexcept;

			/// <summary>
			/// Checks if left id is less than right id
			/// </summary>
			/// <param name="x">
			/// Left operand
			/// </param>
			/// <param name="y">
			/// Right operand
			/// </param>
			/// <returns>
			/// Return if left id is less than right id
			/// </returns>
			friend bool operator <  (thread::id x, thread::id y) noexcept;

			/// <summary>
			/// Checks if left id is less than or equal to right id
			/// </summary>
			/// <param name="x">
			/// Left operand
			/// </param>
			/// <param name="y">
			/// Right operand
			/// </param>
			/// <returns>
			/// Return if left id is less than or equal to right id
			/// </returns>
			friend bool operator <= (thread::id x, thread::id y) noexcept;

			/// <summary>
			/// Checks if left id is greater than right id
			/// </summary>
			/// <param name="x">
			/// Left operand
			/// </param>
			/// <param name="y">
			/// Right operand
			/// </param>
			/// <returns>
			/// Return if left id is greater than right id
			/// </returns>
			friend bool operator >  (thread::id x, thread::id y) noexcept;

			/// <summary>
			/// Checks if left id is greater than or equal to right id
			/// </summary>
			/// <param name="x">
			/// Left operand
			/// </param>
			/// <param name="y">
			/// Right operand
			/// </param>
			/// <returns>
			/// Return if left id is greater than or equal to right id
			/// </returns>
			friend bool operator >= (thread::id x, thread::id y) noexcept;
		};
	private:
		id __id;
		qinternal::ICallable * __callable = nullptr;
	public:
		/// <summary>
		/// Creates a default thread object, does not spawn new thread
		/// </summary>
		thread() noexcept = default;

		/// <summary>
		/// Creates a thread with a function
		/// </summary>
		/// <typeparam name="Func">
		/// Type of to start with
		/// </typeparam>
		/// <typeparam name="Arguments">
		/// Type of function's arguments
		/// </typeparam>
		/// <param name="fn">
		/// Function to execute
		/// </param>
		/// <param name="args">
		/// Arguments for executing function.  Default semantic is by reference.  Use qtl::move for by 
		/// value semantics.
		/// </param>
		template<typename Func, typename ... Arguments>
		explicit thread(Func && fn, Arguments && ... args);

		/// <summary>
		/// Deleted copy constructor
		/// </summary>
		thread(const thread&) = delete;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="other">
		/// Thread to move
		/// </param>
		thread(thread&& other) noexcept;

		/// <summary>
		/// Deconstructs thread after waiting for it to return
		/// </summary>
		~thread();

		/// <summary>
		/// Deleted thread copy
		/// </summary>
		/// <returns>
		/// Reference to this
		/// </returns>
		thread& operator=(const thread&) = delete;

		/// <summary>
		/// Moves thread
		/// </summary>
		/// <param name="other">
		/// Thread to move
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		thread& operator=(thread&& other) noexcept;

		/// <summary>
		/// Swaps threads of execution maintained by this thread object with
		/// other's thread of execution
		/// </summary>
		/// <param name="other">
		/// Thread to swap with
		/// </param>
		void swap(thread& other) noexcept;

		/// <summary>
		/// Gets if a thread can be joined
		/// </summary>
		/// <returns>
		/// If thread can be joined
		/// </returns>
		bool joinable() const noexcept;

		/// <summary>
		/// Joins thread of execution.  Behavior is undefined if 
		/// joinable() returns false
		/// </summary>
		void join();

		/// <summary>
		/// Detaches thread and closes handle.  Thread is not joinable
		/// upon return of function
		/// </summary>
		void detach();

		/// <summary>
		/// Gets the ID of the thread
		/// </summary>
		/// <returns>
		/// Thread identifier
		/// </summary>
		thread::id get_id() const noexcept;

		/// <summary>
		/// Gets thread's native handle
		/// </summary>
		/// <returns>
		/// Native handle of thread
		/// </returns>
		native_handle_type native_handle();
	private:
#if defined (_WIN32)
		template<typename Call>
		static DWORD WINAPI threadfunc(void* args)
		{
			Call* call = new Call(*static_cast<Call*>(args));
			call->callFunc();
			delete call;
			return NULL;
		}
#endif
	};

	/// <summary>
	/// Specialization for hashing thread ids
	/// </summary>
	template<>
	struct hash<thread::id>
	{
		/// <summary>
		/// Hash function for thread ids
		/// </summary>
		/// <param name="key">
		/// Value to hash
		/// </param>
		/// <returns>
		/// Hashed value
		/// </returns>
		size_t operator()(const thread::id& key) const;
	};

#if defined ( _WIN32 )
	template<typename Func, typename ... Arguments>
	inline thread::thread(Func && fn, Arguments && ... args)
	{
		DWORD threadId;
		qinternal::ICallable * call = new qinternal::ThreadFunctionCall<Func, Arguments...>(qtl::forward<Func>(fn), qtl::forward<Arguments>(args)...);
		HANDLE handle = CreateThread(NULL, 0, threadfunc<qinternal::ThreadFunctionCall<Func, Arguments...>>, static_cast<LPVOID>(call), 0, &threadId);

		if (handle == NULL)
		{
			__id.__handle = nullptr;
			delete call;
		}
		else
		{
			__id.__handle = handle;
		}
		__callable = call;
	}
#endif
}

#endif