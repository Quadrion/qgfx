#ifndef utility_h__
#define utility_h__

#if _WIN32 || _WIN64
#if _WIN64
typedef signed long long ssize_t;
#else
typedef signed int ssize_t;
#endif
#endif

namespace qtl
{
	template<typename T>
	struct remove_reference;

	template<typename T>
	void swap(T & lhs, T & rhs)
	{
		T tmp = lhs;
		lhs = rhs;
		rhs = tmp;
	}

	/// <summary>
	/// Casts a templated function paramter to an value, maintaining the value type
	/// </summary>
	/// <typeparam name="T">
	/// Type of parameter to forward
	/// </typeparam>
	/// <param name="value">
	/// Reference value to do perfect forwarding on
	/// </param>
	/// <returns>
	/// Perfectly forwarded value
	/// </returns>
	template <typename T>
	constexpr T&& forward(typename qtl::remove_reference<T>::type& value) noexcept
	{
		return static_cast<T&&>(value);
	}

	/// <summary>
	/// Casts a templated function paramter to an value, maintaining the value type
	/// </summary>
	/// <typeparam name="T">
	/// Type of parameter to forward
	/// </typeparam>
	/// <param name="value">
	/// Move value to do perfect forwarding on
	/// </param>
	/// <returns>
	/// Perfectly forwarded value
	/// </returns>
	template <typename T>
	constexpr T&& forward(typename qtl::remove_reference<T>::type&& value) noexcept
	{
		return static_cast<T&&>(value);
	}

	/// <summary>
	/// Converts a value to an r-value
	/// </summary>
	/// <typeparam name="T">
	/// Type of parameter to convert
	/// </typeparam>
	/// <param name="value">
	/// Value to convert to r-value
	/// </param>
	/// <returns>
	/// Parameter cast to r-value to allow for moving of value
	/// </returns>
	template <typename T>
	constexpr typename qtl::remove_reference<T>::type&& move(T&& value)
	{
		return static_cast<typename qtl::remove_reference<T>::type&&>(value);
	}

	template <typename First, typename Second>
	struct pair
	{
		First first;
		Second second;

		constexpr pair();
		pair(const pair& p) = default;
		pair(pair&& p) = default;

		constexpr pair(const First & first, const Second & second);
		constexpr pair(First&& first, Second&& second);

		constexpr pair& operator=(const pair&) = default;
		constexpr pair& operator=(pair&&) noexcept = default;

		void swap(pair& other) noexcept;

		~pair() = default;
	};

	template <typename T1, typename T2>
	pair(T1, T2)->pair<T1, T2>;

	template <typename First, typename Second>
	inline constexpr pair<First, Second>::pair()
		: first(First()), second(Second())
	{	}

	template<typename First, typename Second>
	inline constexpr pair<First, Second>::pair(const First & first, const Second & second)
		: first(first), second(second)
	{	}

	template<typename First, typename Second>
	inline constexpr pair<First, Second>::pair(First && first, Second && second)
		: first(first), second(second)
	{	}

	template<typename First, typename Second>
	inline void pair<First, Second>::swap(pair& other) noexcept
	{
		qtl::swap(this->first, other.first);
		qtl::swap(this->second, other.second);
	}

	template<typename First, typename Second>
	inline constexpr bool operator==(const pair<First, Second>& lhs, const pair<First, Second>& rhs)
	{
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}

	template<typename First, typename Second>
	inline constexpr bool operator!=(const pair<First, Second>& lhs, const pair<First, Second>& rhs)
	{
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}

	template<typename First, typename Second>
	inline constexpr bool operator<(const pair<First, Second>& lhs, const pair<First, Second>& rhs)
	{
		return lhs.first == rhs.first ? lhs.second < rhs.second : lhs.first < rhs.first;
	}

	template<typename First, typename Second>
	inline constexpr bool operator<=(const pair<First, Second>& lhs, const pair<First, Second>& rhs)
	{
		return lhs.first == rhs.first ? lhs.second <= rhs.second : lhs.first <= rhs.first;
	}

	template<typename First, typename Second>
	inline constexpr bool operator>(const pair<First, Second>& lhs, const pair<First, Second>& rhs)
	{
		return lhs.first == rhs.first ? lhs.second > rhs.second : lhs.first > rhs.first;
	}

	template<typename First, typename Second>
	inline constexpr bool operator>=(const pair<First, Second>& lhs, const pair<First, Second>& rhs)
	{
		return lhs.first == rhs.first ? lhs.second >= rhs.second : lhs.first >= rhs.first;
	}

	template<typename First, typename Second>
	inline constexpr pair<First, Second> make_pair(First&& f, Second&& s)
	{
		return qtl::pair { f, s };
	}

}; // end namespace qtl

#endif