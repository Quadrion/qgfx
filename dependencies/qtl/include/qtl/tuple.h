#ifndef __tuple_h_
#define __tuple_h_

#include <cstddef>

#include "qtl/type_traits.h"
#include "qtl/utility.h"

namespace qtl
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	namespace qinternal
	{
		template <std::size_t __idx, typename Type, bool IsEmpty>
		struct TupleHeadBase;

		template <std::size_t __idx, typename Type>
		struct TupleHeadBase<__idx, Type, true> : public Type
		{
			TupleHeadBase();
			TupleHeadBase(const Type& type);

			template <typename UType>
			TupleHeadBase(UType&& h);

			Type& get_head();
			const Type& get_head_const() const;
			void swap_impl(Type&); // no op
		};

		template <std::size_t __idx, typename Type>
		struct TupleHeadBase<__idx, Type, false>
		{
			TupleHeadBase();
			TupleHeadBase(const Type& type);

			template <typename UType>
			TupleHeadBase(UType&& h);

			Type& get_head();
			const Type& get_head_const() const;
			void swap_impl(Type& type);
		protected:
			Type __type_impl;
		};

		template <std::size_t __idx, typename ... Elements>
		struct TupleImpl;

		template <std::size_t __idx>
		struct TupleImpl<__idx>
		{
			void swap_impl(TupleImpl&); // no op
		};

		template <std::size_t __idx, typename Front, typename ... Rest>
		struct TupleImpl<__idx, Front, Rest...> : public TupleImpl<__idx + 1, Rest...>, TupleHeadBase<__idx, Front, qtl::is_empty<Front>::value>
		{
			typedef TupleImpl<__idx + 1, Rest...> __inherited_type;
			typedef TupleHeadBase<__idx, Front, qtl::is_empty<Front>::value> __base_type;

			Front& get_front();
			const Front& get_front() const;

			__inherited_type& get_rest();
			const __inherited_type& get_rest() const;

			TupleImpl();
			explicit TupleImpl(const Front& front, const Rest & ... rest);

			template <typename UFront, typename ... URest>
			explicit TupleImpl(UFront& front, URest&& ... rest);

			TupleImpl(const TupleImpl& t);
			TupleImpl(TupleImpl&& t);

			template <typename ... Elements>
			TupleImpl(const TupleImpl<__idx, Elements...>& t);

			template <typename ... Elements>
			TupleImpl(TupleImpl<__idx, Elements...>&& t);

			TupleImpl& operator=(const TupleImpl& t);
			TupleImpl& operator=(TupleImpl&& t);

			template <typename ... Elements>
			TupleImpl& operator=(const TupleImpl<__idx, Elements...>& t);

			template <typename ... Elements>
			TupleImpl& operator=(TupleImpl<__idx, Elements...>&& t);

			void swap_impl(TupleImpl& other);
		};

		template<std::size_t __idx, typename Front, typename ...Rest>
		inline Front & TupleImpl<__idx, Front, Rest...>::get_front()
		{
			return __base_type::get_head();
		}

		template<std::size_t __idx, typename Front, typename ...Rest>
		inline const Front & TupleImpl<__idx, Front, Rest...>::get_front() const
		{
			return __base_type::get_head_const();
		}

		template<std::size_t __idx, typename Front, typename ...Rest>
		inline typename TupleImpl<__idx, Front, Rest...>::__inherited_type & TupleImpl<__idx, Front, Rest...>::get_rest()
		{
			return *this;
		}

		template<std::size_t __idx, typename Front, typename ...Rest>
		inline const typename TupleImpl<__idx, Front, Rest...>::__inherited_type & TupleImpl<__idx, Front, Rest...>::get_rest() const
		{
			return *this;
		}

		template<std::size_t __idx, typename Front, typename ...Rest>
		inline TupleImpl<__idx, Front, Rest...>::TupleImpl()
			: __inherited_type(), __base_type()
		{
		}

		template<std::size_t __idx, typename Front, typename ...Rest>
		inline TupleImpl<__idx, Front, Rest...>::TupleImpl(const Front & front, const Rest & ...rest)
			: __inherited_type(rest...), __base_type(front)
		{
		}

		template<std::size_t __idx, typename Front, typename ...Rest>
		inline TupleImpl<__idx, Front, Rest...>::TupleImpl(const TupleImpl & t)
			: __inherited_type(t.get_rest()), __base_type(t.get_front())
		{
		}

		template<std::size_t __idx, typename Front, typename ...Rest>
		inline TupleImpl<__idx, Front, Rest...>::TupleImpl(TupleImpl && t)
			: __inherited_type(qtl::move(t.get_rest())), __base_type(qtl::forward<Front>(t.get_front()))
		{
		}

		template<std::size_t __idx, typename Front, typename ...Rest>
		inline TupleImpl<__idx, Front, Rest...> & TupleImpl<__idx, Front, Rest...>::operator=(const TupleImpl & t)
		{
			this->get_front() = t.get_front();
			this->get_rest() = t.get_rest();
			return *this;
		}

		template<std::size_t __idx, typename Front, typename ...Rest>
		inline TupleImpl<__idx, Front, Rest...> & TupleImpl<__idx, Front, Rest...>::operator=(TupleImpl && t)
		{
			this->get_front() = qtl::move(t.get_front());
			this->get_rest() = qtl::move(t.get_rest());
			return *this;
		}

		template<std::size_t __idx, typename Front, typename ...Rest>
		inline void TupleImpl<__idx, Front, Rest...>::swap_impl(TupleImpl & other)
		{
			__base_type::swap_impl(other.get_front());
			__inherited_type::swap_impl(other.get_front());
		}

		template<std::size_t __idx, typename Front, typename ... Rest>
		template<typename ... Elements>
		inline TupleImpl<__idx, Front, Rest...> & TupleImpl<__idx, Front, Rest...>::operator=(const TupleImpl<__idx, Elements...>& t)
		{
			this->get_front() = t.get_front();
			this->get_rest() = t.get_rest();
			return *this;
		}

		template<std::size_t __idx, typename Front, typename ... Rest>
		template<typename ... Elements>
		inline TupleImpl<__idx, Front, Rest...> & TupleImpl<__idx, Front, Rest...>::operator=(TupleImpl<__idx, Elements...>&& t)
		{
			this->get_front() = qtl::move(t.get_front());
			this->get_rest() = qtl::move(t.get_rest());
			return *this;
		}

		template<std::size_t __idx, typename Front, typename ...Rest>
		template<typename UFront, typename ...URest>
		inline TupleImpl<__idx, Front, Rest...>::TupleImpl(UFront & front, URest && ...back)
			: __inherited_type(qtl::forward<URest>(back)...), __base_type(qtl::forward<UFront>(front))
		{
		}

		template<std::size_t __idx, typename Front, typename ... Rest>
		template<typename ... Elements>
		inline TupleImpl<__idx, Front, Rest...>::TupleImpl(const TupleImpl<__idx, Elements...>& t)
			: __inherited_type(t.get_rest()), __base_type(t.get_front())
		{
		}

		template<std::size_t __idx, typename Front, typename ... Rest>
		template<typename ... Elements>
		inline TupleImpl<__idx, Front, Rest...>::TupleImpl(TupleImpl<__idx, Elements...>&& t)
			: __inherited_type(qtl::move(t.get_rest())), __base_type(qtl::move(t.get_front()))
		{
		}
	}
#endif

	/// <summary>
	/// Fixed size collection of heterogenous values
	/// </summary>
	/// <typeparam name="Types">
	/// Types stored in tuple
	/// </typeparam>
	template <typename ... Types>
	class tuple : public qinternal::TupleImpl<0, Types...>
	{
		typedef qinternal::TupleImpl<0, Types...> __inherited_type;
	public:
		/// <summary>
		/// Constructs tuple with default constructors for types.  Complexity O(n)
		/// </summary>
		tuple();

		/// <summary>
		/// Constructs a tuple given the initial values.  Complexity O(n)
		/// </summary>
		/// <param name="args">
		/// Initial values in tuple
		/// </param>
		explicit tuple(const Types & ... args);

		/// <summary>
		/// Constructs a tuple given the initial values.  Complexity O(n)
		/// </summary>
		/// <typeparam name="UTypes">
		/// Types of values to store in tuple
		/// </typeparam>
		/// <param name="args">
		/// Initial values in tuple
		/// </param>
		template <typename ... UTypes>
		explicit tuple(const UTypes & ... args);

		/// <summary>
		/// Copy constructor for tuple given tuple of different stored types.  Complexity O(n)
		/// </summary>
		/// <typeparam name="UTypes">
		/// Types stored in other tuple
		/// </typeparam>
		/// <param name="args">
		/// Tuple to copy from
		/// </param>
		template <typename ... UTypes>
		explicit tuple(const tuple<UTypes...>& args);

		/// <summary>
		/// Move constructor for tuple given tuple of different stored types.  Complexity O(n)
		/// </summary>
		/// <typeparam name="UTypes">
		/// Types stored in other tuple
		/// </typeparam>
		/// <param name="other">
		/// Tuple to move from
		/// </param>
		template <typename ... UTypes>
		explicit tuple(tuple<UTypes...>&& other);

		/// <summary>
		/// Copy constructor for tuple.  Complexity O(n)
		/// </summary>
		/// <param name="other">
		/// Tuple to copy from 
		/// </param>
		tuple(const tuple& other) = default;

		/// <summary>
		/// Move constructor for tuple.  Complexity O(n)
		/// </summary>
		/// <param name="other">
		/// Tuple to move from 
		/// </param>
		tuple(tuple&& other) = default;

		/// <summary>
		/// Copies other tuple to this.  Complexity O(n)
		/// </summary>
		/// <param name="t">
		/// Tuple to copy from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		tuple& operator=(const tuple& t);

		/// <summary>
		/// Moves other tuple to this.  Complexity O(n)
		/// </summary>
		/// <param name="t">
		/// Tuple to move from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		tuple& operator=(tuple&& t);

		/// <summary>
		/// Copies tuple with other types to this.  Complexity O(n)
		/// </summary>
		/// <typeparam name="UTypes">
		/// Types stored in other tuple
		/// </typeparam>
		/// <param name="t">
		/// Tuple to copy from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		template<typename ... UTypes>
		tuple& operator=(const tuple<UTypes...>& t);

		/// <summary>
		/// Moves tuple with other types to this.  Complexity O(n)
		/// </summary>
		/// <typeparam name="UTypes">
		/// Types stored in other tuple
		/// </typeparam>
		/// <param name="t">
		/// Tuple to move from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		template<typename ... UTypes>
		tuple& operator=(tuple<UTypes...>&& t);

		/// <summary>
		/// Swaps tuple values.  Complexity O(n)
		/// </summary>
		/// <param name="t">
		/// Tuple to swap with
		/// </param>
		void swap(tuple& t);
	};

	template <typename ... Types>
	tuple(Types...)->tuple<Types...>;

	/// <summary>
	/// Empty tuple specialization
	/// </summary>
	template <>
	class tuple<>
	{
	public:
		/// <summary>
		/// Swaps elements of tuple.  No op.  Complexity O(1)
		/// </summary>
		void swap(tuple&);
	};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	namespace qinternal
	{
		template<std::size_t __idx, typename Type>
		struct TupleElement;

		template<std::size_t __idx, typename Head, typename ... Tail>
		struct TupleElement<__idx, tuple<Head, Tail...>> : TupleElement<__idx - 1, tuple<Tail...>>
		{
		};

		template<typename Head, typename ... Tail>
		struct TupleElement<0, tuple<Head, Tail...>>
		{
			typedef Head type;
		};

		template<typename T>
		struct TupleSize;

		template<typename ... Elements>
		struct TupleSize<tuple<Elements...>>
		{
			static constexpr std::size_t value = sizeof...(Elements);
		};

		template<std::size_t __idx, typename Type>
		inline TupleHeadBase<__idx, Type, true>::TupleHeadBase()
			: Type()
		{
		}

		template<std::size_t __idx, typename Type>
		inline TupleHeadBase<__idx, Type, true>::TupleHeadBase(const Type & type)
			: Type(type)
		{
		}

		template<std::size_t __idx, typename Type>
		inline Type & TupleHeadBase<__idx, Type, true>::get_head()
		{
			return *this;
		}

		template<std::size_t __idx, typename Type>
		inline const Type & TupleHeadBase<__idx, Type, true>::get_head_const() const
		{
			return *this;
		}

		template<std::size_t __idx, typename Type>
		inline void TupleHeadBase<__idx, Type, true>::swap_impl(Type &)
		{
		}

		template<std::size_t __idx, typename Type>
		template<typename UType>
		inline TupleHeadBase<__idx, Type, true>::TupleHeadBase(UType && h)
			: Type(qtl::forward<UType>(h))
		{
		}

		template<std::size_t __idx, typename Type>
		inline TupleHeadBase<__idx, Type, false>::TupleHeadBase()
			: __type_impl()
		{
		}

		template<std::size_t __idx, typename Type>
		inline TupleHeadBase<__idx, Type, false>::TupleHeadBase(const Type & type)
			: __type_impl(type)
		{
		}

		template<std::size_t __idx, typename Type>
		inline Type & TupleHeadBase<__idx, Type, false>::get_head()
		{
			return __type_impl;
		}

		template<std::size_t __idx, typename Type>
		inline const Type & TupleHeadBase<__idx, Type, false>::get_head_const() const
		{
			return __type_impl;
		}

		template<std::size_t __idx, typename Type>
		inline void TupleHeadBase<__idx, Type, false>::swap_impl(Type & type)
		{
			auto tmp = qtl::move(__type_impl);
			__type_impl = qtl::move(type);
			type = qtl::move(tmp);
		}

		template<std::size_t __idx, typename Type>
		template<typename UType>
		inline TupleHeadBase<__idx, Type, false>::TupleHeadBase(UType && h)
			: Type(qtl::forward<UType>(h))
		{
		}

		template<std::size_t __idx, typename Head, typename ... Tail>
		inline Head& __get_helper(TupleImpl<__idx, Head, Tail...>& t)
		{
			return t.get_front();
		}

		template<std::size_t __idx, typename Head, typename ... Tail>
		inline const Head& __get_helper(const TupleImpl<__idx, Head, Tail...>& t)
		{
			return t.get_front();
		}
	}
#endif

	template<typename ...Types>
	inline tuple<Types...>::tuple()
		: __inherited_type()
	{
	}

	template<typename ...Types>
	inline tuple<Types...>::tuple(const Types & ...args)
		: __inherited_type(args...)
	{
	}

	template<typename ...Types>
	inline tuple<Types...>& tuple<Types...>::operator=(const tuple<Types...> & t)
	{
		static_cast<__inherited_type&>(*this) = t;
		return *this;
	}

	template<typename ...Types>
	inline tuple<Types...>& tuple<Types...>::operator=(tuple<Types...>&& t)
	{
		static_cast<__inherited_type&>(*this) = qtl::move(t);
		return *this;
	}

	template<typename ...Types>
	inline void tuple<Types...>::swap(tuple<Types...> & t)
	{
		__inherited_type::swap_impl(t);
	}

	template<typename ...Types>
	template<typename ...UTypes>
	inline tuple<Types...>::tuple(const UTypes & ...args)
		: __inherited_type(qtl::forward<UTypes>(args)...)
	{
	}

	template<typename ...Types>
	template<typename ...UTypes>
	inline tuple<Types...>::tuple(const tuple<UTypes...>& t)
		: __inherited_type(static_cast<const qinternal::TupleImpl<0, UTypes...>&>(t))
	{
	}

	template<typename ...Types>
	template<typename ...UTypes>
	inline tuple<Types...>::tuple(tuple<UTypes...>&& t)
		: __inherited_type(static_cast<qinternal::TupleImpl<0, UTypes...>&&>(t))
	{
	}

	template<typename ...Types>
	template<typename ...UTypes>
	inline tuple<Types...>& tuple<Types...>::operator=(const tuple<UTypes...>& t)
	{
		static_cast<__inherited_type&>(*this) = t;
		return *this;
	}

	template<typename ...Types>
	template<typename ...UTypes>
	inline tuple<Types...>& tuple<Types...>::operator=(tuple<UTypes...>&& t)
	{
		static_cast<__inherited_type&>(*this) = qtl::move(t);
		return *this;
	}

	inline void tuple<>::swap(tuple &)
	{
	}

	template <std::size_t __idx, typename ... Elements>
	inline typename qinternal::TupleElement<__idx, tuple<Elements...>>::type& get(tuple<Elements...>& t)
	{
		return qinternal::__get_helper<__idx>(t);
	}

	template <std::size_t __idx, typename ... Elements>
	inline const typename qinternal::TupleElement<__idx, tuple<Elements...>>::type& get(const tuple<Elements...>& t)
	{
		return qinternal::__get_helper<__idx>(t);
	}

	template <typename ... Elements>
	inline tuple<Elements & ...> tie(Elements & ... args)
	{
		return tuple<Elements & ...>(args...);
	}
	
} // qtl

#endif