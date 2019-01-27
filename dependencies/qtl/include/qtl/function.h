#ifndef __function_h_
#define __function_h_

#include "qtl/shared_ptr.h"
#include "qtl/tuple.h"
#include "qtl/type_traits.h"

#include <cassert>
#include <cstddef>

namespace qtl
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	namespace qinternal
	{
		template <bool PMemFunc, bool PMemData>
		struct Invoker
		{
			template<typename F, typename ... Args>
			inline static typename result_of<F(Args...)>::type invoke(F&& f, Args&& ... args)
			{
				return qtl::forward<F>(f)(qtl::forward<Args>(args)...);
			}
		};

		template<bool>
		struct InvokeHelper;

		template<>
		struct InvokeHelper<false>
		{
			template<typename T>
			inline static auto get(T&& t) -> decltype(*qtl::forward<T>(t))
			{
				return qtl::forward<T>(t);
			}

			template<typename T>
			inline static auto get(const qtl::reference_wrapper<T>& t) -> decltype(t.get())
			{
				return t.get();
			}
		};

		template<>
		struct InvokeHelper<true>
		{
			template<typename T>
			inline static auto get(T&& t) -> decltype(qtl::forward<T>(t))
			{
				return qtl::forward<T>(t);
			}
		};

		template<>
		struct Invoker<true, false>
		{
			template<typename T, typename F, typename T1, typename... Args>
			inline static auto invoke(F T::* f, T1&& t1, Args&&... args) -> decltype((InvokeHelper<qtl::is_base_of<T, typename qtl::decay<T1>::type>::value>::get(qtl::forward<T1>(t1)).*f)(qtl::forward<Args>(args)...))
			{
				return (InvokeHelper<qtl::is_base_of<T, typename qtl::decay<T1>::type>::value>::get(qtl::forward<T1>(t1)).*f)(qtl::forward<Args>(args)...);
			}
		};

		template<>
		struct Invoker<false, true>
		{
			template<typename T, typename F, typename T1, typename... Args>
			inline static auto invoke(F T::* f, T1&& t1, Args&& ... args) -> decltype(InvokeHelper<qtl::is_base_of<T, typename qtl::decay<T1>::type>::value>::get(t1).*f)
			{
				return InvokeHelper<qtl::is_base_of<T, typename qtl::decay<T1>::type>::value>::get(t1).*f;
			}
		};

		template<typename F, typename ... Args>
		struct InvokeResult
		{
			typedef Invoker<qtl::is_member_function_pointer<typename remove_reference<F>::type>::value, is_member_object_pointer<typename remove_reference<F>::type>::value && (sizeof...(Args) == 1)> invoker;

			inline static auto invoke(F&& f, Args&& ... args) -> decltype(Invoker<qtl::is_member_function_pointer<typename remove_reference<F>::type>::value, is_member_object_pointer<typename remove_reference<F>::type>::value && (sizeof...(Args) == 1)>::invoke(qtl::forward<F>(f), qtl::forward<Args>(args)...))
			{
				return Invoker<qtl::is_member_function_pointer<typename remove_reference<F>::type>::value, is_member_object_pointer<typename remove_reference<F>::type>::value && (sizeof...(Args) == 1)>::invoke(qtl::forward<F>(f), qtl::forward<Args>(args)...);
			}
		};
	}
#endif

	/// <summary>
	/// Invokes a function
	/// </summary>
	/// <typeparam name="F">
	/// Function type
	/// </typeparam>
	/// <typeparam name="Args">
	/// Argument types
	/// </typeparam>
	/// <param name="f">
	/// Function to invoke
	/// </param>
	/// <param name="args">
	/// Arguments for invocation
	/// </param>
	/// <returns>
	/// Result of invocation
	/// </returns>
	template<typename F, class ... Args>
	auto invoke(F&& f, Args&& ... args) -> decltype(qinternal::InvokeResult<F, Args...>::invoke(qtl::forward<F>(f), qtl::forward<Args>(args)...))
	{
		return qinternal::InvokeResult<F, Args...>::invoke(qtl::forward<F>(f), qtl::forward<Args>(args)...);
	}

	/// <summary>
	/// Template for function
	/// </summary>
	/// <typeparam name="T">
	/// Type of function
	/// </typeparam>
	template <typename T>
	class function;

	/// <summary>
	/// Represents a compile time polymorphic function wrapper.  Functions can
	/// store, copy, and invoke callable objects stored by the wrapper.
	/// </summary>
	/// <typeparam name="ReturnType">
	/// Return type of function
	/// </typeparam>
	/// <typeparam name="ArgumentTypes">
	/// Argument types of function
	/// </typeparam>
	template <typename ReturnType, typename ... ArgumentTypes>
	class function<ReturnType(ArgumentTypes...)>
	{
	public:
		/// <summary>
		/// Creates a blank, no op function.  Complexity O(1)
		/// </summary>
		function();

		/// <summary>
		/// Copy constructor of function.  Complexity O(1)
		/// </summary>
		/// <param name="fun">
		/// Function to copy from
		/// </param>
		function(const function& fun);

		/// <summary>
		/// Move constructor of function.  Complexity O(1)
		/// </summary>
		/// <param name="fun">
		/// Function to move from
		/// </param>
		function(function&& fun);
		
		/// <summary>
		/// Creates a function from either a lambda or a defined function.  Complexity O(1)
		/// </summary>
		/// <typeparam name="T">
		/// Type of function
		/// </typeparam>
		/// <param name="aT">
		/// Function to store
		/// </param>
		template<typename T>
		function(T aT);

		/// <summary>
		/// Destroys the function.  Complexity O(1)
		/// </summary>
		~function() = default;

		/// <summary>
		/// Copies another function to this function.  Complexity O(1)
		/// </summary>
		/// <param name="fun">
		/// Function to copy
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		function& operator=(const function& fun);

		/// <summary>
		/// Moves another function to this function.  Complexity O(1)
		/// </summary>
		/// <param name="fun">
		/// Function to move
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		function& operator=(function&& fun) noexcept;

		/// <summary>
		/// Sets function to be a null function.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// Reference to this
		/// </returns>
		function& operator=(std::nullptr_t) noexcept;

		/// <summary>
		/// Checks if function is valid.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// True if valid function, else false
		/// </returns>
		explicit operator bool() const;

		/// <summary>
		/// Executes function
		/// </summary>
		/// <param name="args">
		/// Arguments for the execution of the function
		/// </param>
		/// <returns>
		/// Result of function
		/// </returns>
		ReturnType operator()(ArgumentTypes ... args);
	private:
		class ICallable
		{
		public:
			virtual ~ICallable() = default;
			virtual ReturnType invoke(ArgumentTypes ...) = 0;
		};

		template<typename T>
		class Callable : public ICallable
		{
		public:
			explicit Callable(const T& value);
			~Callable() override = default;
			
			ReturnType invoke(ArgumentTypes ... args) override;
		private:
			T __t;
		};

		qtl::shared_ptr<ICallable> __func;
	};

	template<typename ReturnType, typename ...ArgumentTypes>
	template<typename T>
	inline function<ReturnType(ArgumentTypes...)>::Callable<T>::Callable(const T & value)
		: __t(value)
	{
	}

	template<typename ReturnType, typename ... ArgumentTypes>
	template<typename T>
	inline ReturnType function<ReturnType(ArgumentTypes...)>::Callable<T>::invoke(ArgumentTypes ...args)
	{
		return __t(args...);
	}

	template<typename ReturnType, typename ...ArgumentTypes>
	inline function<ReturnType(ArgumentTypes...)>::function()
	{
		__func = nullptr;
	}

	template<typename ReturnType, typename ...ArgumentTypes>
	inline function<ReturnType(ArgumentTypes...)>::function(const function & fun)
	{
		__func = fun.__func;
	}

	template<typename ReturnType, typename ...ArgumentTypes>
	inline function<ReturnType(ArgumentTypes...)>::function(function && fun)
	{
		__func = fun.__func;
		fun.__func = nullptr;
	}

	template<typename ReturnType, typename ...ArgumentTypes>
	inline function<ReturnType(ArgumentTypes ...)>& function<ReturnType(ArgumentTypes...)>::operator=(const function & fun)
	{
		__func = fun.__func;
		return *this;
	}

	template<typename ReturnType, typename ...ArgumentTypes>
	inline function<ReturnType(ArgumentTypes ...)>& function<ReturnType(ArgumentTypes...)>::operator=(function && fun) noexcept
	{
		__func = fun.__func;
		fun.__func = nullptr;
		return *this;
	}

	template<typename ReturnType, typename ...ArgumentTypes>
	inline function<ReturnType(ArgumentTypes ...)>& function<ReturnType(ArgumentTypes...)>::operator=(std::nullptr_t) noexcept
	{
		__func = nullptr;
		return *this;
	}

	template<typename ReturnType, typename ...ArgumentTypes>
	inline function<ReturnType(ArgumentTypes...)>::operator bool() const
	{
		return __func && __func != nullptr;
	}

	template<typename ReturnType, typename ...ArgumentTypes>
	inline ReturnType function<ReturnType(ArgumentTypes...)>::operator()(ArgumentTypes ...args)
	{
#ifdef _DEBUG
		assert(__func);
#endif
		return __func->invoke(args...);
	}

	template<typename ReturnType, typename ...ArgumentTypes>
	template<typename T>
	inline function<ReturnType(ArgumentTypes...)>::function(T type)
	{
		__func = qtl::make_shared<Callable<T>>(type);
	}

}

#endif