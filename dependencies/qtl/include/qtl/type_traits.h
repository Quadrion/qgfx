#ifndef typetraits_h__
#define typetraits_h__

#include "qtl/utility.h"

#include <cstddef>

namespace qtl
{
	/// <summary>
	/// Gets the type with the reference removed
	/// </summary>
	/// <typeparam name="T">
	/// Base type of value stored
	/// </typeparam>
	template <typename T>
	struct remove_reference
	{
		typedef T type;
	};

	/// <summary>
	/// Gets the type with the reference removed
	/// </summary>
	/// <typeparam name="T">
	/// Base type of value stored
	/// </typeparam>
	template <typename T>
	struct remove_reference<T&>
	{
		typedef T type;
	};

	/// <summary>
	/// Gets the type with the reference removed
	/// </summary>
	/// <typeparam name="T">
	/// Base type of value stored
	/// </typeparam>
	template <typename T>
	struct remove_reference<T&&>
	{
		typedef T type;
	};

	/// <summary>
	/// Gets the type with the reference removed
	/// </summary>
	/// <typeparam name="T">
	/// Base type of value stored
	/// </typeparam>
	template<typename T>
	using remove_reference_t = typename remove_reference<T>::type;

	/// <summary>
	/// Removes const-ness from type
	/// </summary>
	/// <typeparam name="T">
	/// Type of variable to remove from
	/// </typeparam>
	template<typename T>
	struct remove_const
	{
		typedef T type;
	};

	/// <summary>
	/// Removes const-ness from type
	/// </summary>
	/// <typeparam name="T">
	/// Type of variable to remove from
	/// </typeparam>
	template<typename T>
	struct remove_const<const T>
	{
		typedef T type;
	};

	/// <summary>
	/// Removes const-ness from type
	/// </summary>
	/// <typeparam name="T">
	/// Type of variable to remove from
	/// </typeparam>
	template<typename T>
	using remove_const_t = typename remove_const<T>::type;

	/// <summary>
	/// Removes volatile-ness from type
	/// </summary>
	/// <typeparam name="T">
	/// Type of variable to remove from
	/// </typeparam>
	template<typename T>
	struct remove_volatile
	{
		typedef T type;
	};

	/// <summary>
	/// Removes volatile-ness from type
	/// </summary>
	/// <typeparam name="T">
	/// Type of variable to remove from
	/// </typeparam>
	template<typename T>
	struct remove_volatile<volatile T>
	{
		typedef T type;
	};

	/// <summary>
	/// Removes volatile-ness from type
	/// </summary>
	/// <typeparam name="T">
	/// Type of variable to remove from
	/// </typeparam>
	template<typename T>
	using remove_volatile_t = typename remove_volatile<T>::type;

	/// <summary>
	/// Removes const-ness and volatile-ness from type
	/// </summary>
	/// <typeparam name="T">
	/// Type to variable to remove from
	/// </typeparam>
	template <typename T>
	struct remove_cv
	{
		typedef typename remove_volatile<typename remove_const<T>::type>::type type;
	};

	/// <summary>
	/// Removes const-ness and volatile-ness from type
	/// </summary>
	/// <typeparam name="T">
	/// Type to variable to remove from
	/// </typeparam>
	template<typename T>
	using remove_cv_t = typename remove_cv<T>::type;

	/// <summary>
	/// Base class for integral constants
	/// </summary>
	/// <typeparam name="T">
	/// Integral type stored
	/// </typeparam>
	/// <typeparam name="v">
	/// Value of type
	/// </typeparam>
	template<typename T, T v>
	struct integral_constant
	{
		static constexpr T value = v;
		typedef T value_type;
		typedef integral_constant<T, v> type;
	};

	/// <summary>
	/// Typedef representing a true integral constant type
	/// </summary>
	typedef integral_constant<bool, true> true_type;

	/// <summary>
	/// Typedef representing a false integral constant type
	/// </summary>
	typedef integral_constant<bool, false> false_type;

	/// <summary>
	/// Utility class for checking if class is empty
	/// </summary>
	/// <typeparam name="T">
	/// Type to check
	/// </typeparam>
	template<typename T>
	struct is_empty : integral_constant<bool, __is_empty(T)>
	{
	};

	/// <summary>
	/// Utility class for checking if two types are related via inheritance
	/// </summary>
	/// <typeparam name="Base">
	/// Base class type to check against
	/// </typeparam>
	/// <typeparam name="Derived">
	/// Derived class type to see if it is derived from Base
	/// </typeparam>
	template<typename Base, typename Derived>
	struct is_base_of : integral_constant<bool, __is_base_of(Base, Derived)>
	{
	};

	/// <summary>
	/// Type for returning the type to be used based on a compile time constant
	/// </summary>
	/// <typeparam name="B">
	/// Boolean type parameter for determining which type to use
	/// </typeparam>
	/// <typeparam name="T">
	/// Type to use if the boolean parameter, B, evaluates to true
	/// </typeparam>
	/// <typeparam name="F">
	/// Type to use if the boolean parameter, B, evaluates to false
	/// </typeparam>
	template<bool B, typename T, typename F>
	struct conditional;

	/// <summary>
	/// Type for returning the type to be used based on a compile time constant.  Specialization
	/// for if boolean is true
	/// </summary>
	/// <typeparam name="T">
	/// Type to use if the boolean parameter, B, evaluates to true
	/// </typeparam>
	/// <typeparam name="F">
	/// Type to use if the boolean parameter, B, evaluates to false
	/// </typeparam>
	template<typename T, typename F>
	struct conditional<true, T, F>
	{
		typedef T type;
	};

	/// <summary>
	/// Type for returning the type to be used based on a compile time constant.  Specialization
	/// for if boolean is false
	/// </summary>
	/// <typeparam name="T">
	/// Type to use if the boolean parameter, B, evaluates to true
	/// </typeparam>
	/// <typeparam name="F">
	/// Type to use if the boolean parameter, B, evaluates to false
	/// </typeparam>
	template<typename T, typename F>
	struct conditional<false, T, F>
	{
		typedef F type;
	};

	/// <summary>
	/// Gets if the type is an array type
	/// </summary>
	/// <typeparam name="T">
	/// Type to determine if is array
	/// </typeparam>
	template<typename T>
	struct is_array : false_type
	{
	};

	/// <summary>
	/// Gets if the type is an array type.  Specialization for type with array notation
	/// </summary>
	/// <typeparam name="T">
	/// Type to determine if is array
	/// </typeparam>
	template<typename T>
	struct is_array<T[]> : true_type
	{
	};

	/// <summary>
	/// Gets if the type is an array type.  Specialization for type with array notation
	/// and size notation
	/// </summary>
	/// <typeparam name="T">
	/// Type to determine if is array
	/// </typeparam>
	/// <typeparam name="N">
	/// Size of array
	/// </typeparam>
	template<typename T, std::size_t N>
	struct is_array<T[N]> : true_type
	{
	};

	/// <summary>
	/// Removes array notation from array type.
	/// </summary>
	/// <typeparam name="T">
	/// Type to remove notation from
	/// </typeparam>
	template<typename T>
	struct remove_extent
	{
		typedef T type;
	};

	/// <summary>
	/// Removes array notation from array type.  Specialization for type with array notation
	/// </summary>
	/// <typeparam name="T">
	/// Type to remove notation from
	/// </typeparam>
	template<typename T>
	struct remove_extent<T[]>
	{
		typedef T type;
	};

	/// <summary>
	/// Removes array notation from array type.  Specialization for type with array notation and
	/// array dimension
	/// </summary>
	/// <typeparam name="T">
	/// Type to remove notation from
	/// </typeparam>
	/// <typeparam name="N">
	/// Dimension/Size of array
	/// </typeparam>
	template<typename T, std::size_t N>
	struct remove_extent<T[N]>
	{
		typedef T type;
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="T">
	/// Type of variable
	/// </typeparam>
	template<typename T>
	struct is_function : false_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename ... Args>
	struct is_function<Ret(Args...)> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...)> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) const> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) volatile> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) const volatile> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) const> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) volatile> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) const volatile> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) &> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) const &> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) volatile &> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) const volatile &> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) &> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) const &> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) volatile &> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) const volatile &> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) && > : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) const &&> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) volatile &&> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) const volatile &&> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) && > : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) const &&> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) volatile &&> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) const volatile &&> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) const noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) volatile noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) const volatile noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) const noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) volatile noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) const volatile noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) & noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) const & noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) volatile & noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) const volatile & noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) & noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) const & noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) volatile & noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) const volatile & noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) && noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) const && noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) volatile && noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args...) const volatile && noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) && noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) const && noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) volatile && noexcept> : true_type
	{
	};

	/// <summary>
	/// Gets if type is a function.
	/// </summary>
	/// <typeparam name="Ret">
	/// Type returned
	/// </typeparam>
	/// <typeparam name="Args">
	/// Type of arguments
	/// </typeparam>
	template<typename Ret, typename... Args>
	struct is_function<Ret(Args..., ...) const volatile && noexcept> : true_type
	{
	};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	namespace qinternal
	{
		template<typename T>
		struct is_member_function_pointer_helper : false_type
		{
		};

		template<typename T, class U>
		struct is_member_function_pointer_helper<T U::*> : is_function<T>
		{
		};

		template<typename T>
		struct is_member_pointer_helper : false_type
		{
		};

		template<typename T, typename U>
		struct is_member_pointer_helper<T U::*> : true_type
		{
		};
	}
#endif

	/// <summary>
	/// Struct for determining if object is member function pointer
	/// </summary>
	/// <typeparam name="T">
	/// Type to check
	/// </typeparam>
	template<typename T>
	struct is_member_function_pointer : qinternal::is_member_function_pointer_helper<typename remove_cv<T>::type>
	{
	};

	/// <summary>
	/// Struct for determining if object is member pointer
	/// </summary>
	/// <typeparam name="T">
	/// Type to check
	/// </typeparam>
	template<typename T>
	struct is_member_pointer : qinternal::is_member_pointer_helper<typename remove_cv<T>::type>
	{
	};

	/// <summary>
	/// Struct for determining if object is member object pointer
	/// </summary>
	/// <typeparam name="T">
	/// Type to check
	/// </typeparam>
	template<typename T>
	struct is_member_object_pointer : integral_constant<bool, is_member_pointer<T>::value && !is_member_function_pointer<T>::value>
	{
	};

	template<typename T, typename U>
	struct is_same : qtl::false_type
	{
	};

	template<typename T>
	struct is_same<T, T> : qtl::true_type
	{
	};

	template<class T, class U>
	inline constexpr bool is_same_v = is_same<T, U>::value;

	template<typename T>
	struct is_void : qtl::is_same<typename remove_cv<T>::type, void>
	{
	};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	namespace qinternal
	{
		template<typename T, bool is_function_type = false>
		struct add_pointer
		{
			using type = typename remove_reference<T>::type*;
		};

		template<typename T>
		struct add_pointer<T, true>
		{
			using type = T;
		};

		template<typename T, class ... Args>
		struct add_pointer<T(Args...), true>
		{
			using type = T(*)(Args...);
		};

		template<typename T, class ... Args>
		struct add_pointer<T(Args..., ...), true>
		{
			using type = T(*)(Args..., ...);
		};
	}
#endif

	/// <summary>
	/// Struct used to add pointer to type
	/// </summary>
	/// <typeparam name="T">
	/// Type to add pointer to
	/// </typeparam>
	template<typename T>
	struct add_pointer : qinternal::add_pointer<T, is_function<T>::value> 
	{
	};

	/// <summary>
	/// Applies l-value to r-value conversion, array to pointer, and function to pointer implicit
	/// conversions for some provided type.
	/// </summary>
	/// <typeparam name="T">
	/// Type to decay
	/// </typeparam>
	template<class T>
	struct decay 
	{
	private:
		typedef typename qtl::remove_reference<T>::type U;
	public:
		typedef typename qtl::conditional<
			qtl::is_array<U>::value,
			typename qtl::remove_extent<U>::type*,
			typename qtl::conditional<
				qtl::is_function<U>::value,
				typename qtl::add_pointer<U>::type,
				typename qtl::remove_cv<U>::type
			>::type
		>::type type;
	};

	template <typename T>
	class reference_wrapper;

	template<typename>
	struct result_of;

	template<typename F, typename ... ArgTypes>
	struct result_of<F(ArgTypes...)>;

	template<typename F, typename ... ArgTypes>
	struct invoke_result;

	/// <summary>
	/// Gets the result of type by fetching the result_of
	/// </summary>
	/// <typeparam name="T">
	/// Type to get result of type from
	/// </typeparam>
	template <typename T>
	using result_of_t = typename result_of<T>::type;

	/// <summary>
	/// Gets the invoke result type by fetching the result_of
	/// </summary>
	/// <typeparam name="T">
	/// Type to get invoke result type from
	/// </typeparam>
	template <typename F, typename ... Args>
	using invoke_result_t = typename invoke_result<F, Args...>::type;

	/// <summary>
	/// Enables a type only if the boolean is true
	/// </summary>
	/// <typeparam name="B">
	/// Enable type if this is true
	/// </typeparam>
	/// <typeparam name="T">
	/// Type to enable
	/// </typeparam>
	template<bool B, typename T = void>
	struct enable_if 
	{
	};

	/// <summary>
	/// Enables a type only if the boolean is true.  Specialization for true.
	/// </summary>
	/// <typeparam name="T">
	/// Type to enable
	/// </typeparam>
	template<typename T>
	struct enable_if<true, T>
	{
		typedef T type;
	};

	/// <summary>
	/// Template wrapping a copyable, assignable object.  Stores references inside
	/// of containers
	/// </summary>
	/// <typeparam name="T">
	/// Type to store
	/// </typeparam>
	template<typename T>
	class reference_wrapper
	{
	public: 
		/// <summary>
		/// Type stored
		/// </summary>
		typedef T type;

		/// <summary>
		/// Copy constructor for reference wrapper
		/// </summary>
		/// <param name="ref">
		/// Type to store
		/// </param>
		reference_wrapper(T& ref) noexcept
			: __ptr(__builtin_addressof(ref))
		{	}

		/// <summary>
		/// Move constructor for reference wrapper.  Deleted.
		/// </summary>
		reference_wrapper(T&&) = delete;

		/// <summary>
		/// Copy constructor for reference wrapper
		/// </summary>
		reference_wrapper(const reference_wrapper&) noexcept = default;

		/// <summary>
		/// Copies another reference wrapper to this
		/// </summary>
		/// <param name="ref">
		/// Type to store
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		reference_wrapper& operator=(const reference_wrapper& ref) noexcept = default;

		/// <summary>
		/// Gets reference to stored object
		/// </summary>
		/// <returns>
		/// Reference to stored object
		/// </returns>
		operator T& () const noexcept 
		{ 
			return *__ptr;  
		}

		/// <summary>
		/// Gets reference to stored object
		/// </summary>
		/// <returns>
		/// Reference to stored object
		/// </returns>
		T& get() const noexcept
		{
			return *__ptr;
		}

		/// <summary>
		/// Calls invoke on stored object
		/// </summary>
		/// <typeparam name="ArgTypes">
		/// Argument types for invocation
		/// </typeparam>
		/// <param name="args">
		/// Argument values for invocation
		/// </param>
		/// <returns>
		/// Result of invocation
		/// </returns>
		template <typename ... ArgTypes>
		invoke_result_t<T&, ArgTypes...> operator() (ArgTypes && ... args) const
		{
			return invoke(get(), qtl::forward<ArgTypes>(args)...);
		}

	private:
		T* __ptr;
	};

	template <typename T>
	reference_wrapper(reference_wrapper<T>)->reference_wrapper<T>;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	namespace qinternal 
	{
		template <typename T>
		struct is_reference_wrapper : false_type
		{
		};

		template <typename T>
		struct is_reference_wrapper<qtl::reference_wrapper<T>> : true_type
		{
		};

		template <typename T>
		struct invoke_impl
		{
			template <typename F, typename ... Args>
			static auto call(F&& f, Args&& ... args) -> decltype(qtl::forward<F>(f)(qtl::forward<Args>(args)...));
		};
		
		template<typename B, typename MT>
		struct invoke_impl<MT B::*>
		{
			template<typename T, typename Td = typename decay<T>::type, typename = typename enable_if<is_base_of<B, Td>::value>::type>
			static auto get(T&& t)->T&&;

			template<typename T, typename Td = typename decay<T>::type, typename = typename enable_if<is_reference_wrapper<Td>::value>::type>
			static auto get(T&& t)->decltype(t.get());

			template<typename T, typename Td = typename decay<T>::type, typename = typename enable_if<!is_base_of<B, Td>::value>::type, typename = typename enable_if<!is_reference_wrapper<Td>::value>::type>
			static auto get(T&& t)->decltype(*qtl::forward<T>(t));

			template<typename T, typename ... Args, typename MT1, typename = typename enable_if<is_function<MT1>::value>::type>
			static auto call(MT1 B::*pmf, T&& t, Args&& ... args)->decltype((invoke_impl::get(qtl::forward<T>(t)).*pmf)(qtl::forward<Args>(args)...));

			template<typename T>
			static auto call(MT B::*pmd, T&& t)->decltype(invoke_impl::get(qtl::forward<T>(t)).*pmd);
		};

		template<typename F, typename ... Args, class Fd = typename decay<F>::type>
		auto INVOKE(F&& f, Args&& ... args)->decltype(invoke_impl<Fd>::call(qtl::forward<F>(f), qtl::forward<Args>(args)...));
	}
#endif

	/// <summary>
	/// Adds an r-value reference to a type
	/// </summary>
	/// <typeparam name="T">
	/// Type to add r-value reference to
	/// </typeparam>
	template<typename T>
	struct add_rvalue_reference
	{
		using type = T && ;
	};

	/// <summary>
	/// Adds an r-value reference to a type.  Specialization for reference type
	/// </summary>
	/// <typeparam name="T">
	/// Type to add r-value reference to
	/// </typeparam>
	template<typename T>
	struct add_rvalue_reference<T&>
	{
		using type = T & ;
	};

	/// <summary>
	/// Adds an r-value reference to a type
	/// </summary>
	/// <typeparam name="T">
	/// Type to add r-value reference to
	/// </typeparam>
	template<typename T> using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

	/// <summary>
	/// Converts any type to a reference type, making it possible to use member functions
	/// in decltype expressions without the need for constructors.
	/// </summary>
	/// <typeparam name="T">
	/// Type to convert
	/// </typeparam>
	template<typename T>
	add_rvalue_reference_t<T> declval() noexcept;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	namespace qinternal
	{
		template <typename T, typename, typename ...>
		struct invoke_result
		{
		};

		template <typename F, typename ... Args>
		struct invoke_result<decltype(void(INVOKE(declval<F>(), declval<Args>()...))), F, Args...>
		{
			using type = decltype(qinternal::INVOKE(declval<F>(), declval<Args>()...));
		};
	}
#endif

	/// <summary>
	/// Gets the result of type of the function
	/// </summary>
	/// <typeparam name="F">
	/// Function type
	/// </typeparam>
	/// <typeparam name="Args">
	/// Argument types
	/// </typeparam>
	template <typename F, typename ... Args>
	struct result_of<F(Args...)> : qinternal::invoke_result<void, F, Args...>
	{
	};

	/// <summary>
	/// Gets the invoke result type of the function
	/// </summary>
	/// <typeparam name="F">
	/// Function type
	/// </typeparam>
	/// <typeparam name="Args">
	/// Argument types
	/// </typeparam>
	template <typename F, typename ... Args>
	struct invoke_result : qinternal::invoke_result<void, F, Args...>
	{
	};

	/// <summary>
	/// Gets the address of a reference
	/// </summary>
	/// <typeparam name="T">
	/// Type of reference
	/// </typeparam>
	/// <param name="arg">
	/// Reference to get address of
	/// </param>
	/// <returns>
	/// Pointer to referenced object
	/// </returns>
	template <typename T>
	constexpr T* addressof(T& arg) noexcept
	{
		return __builtin_addressof(arg);
	}

	template <typename T>
	const T* addressof(const T&&) = delete;

}; // end namespace qtl

#endif