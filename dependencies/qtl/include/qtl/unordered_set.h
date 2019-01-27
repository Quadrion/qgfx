#ifndef __unorderedset_h_
#define __unorderedset_h_

#include "qtl/hash.h"
#include "qtl/unordered_map.h"
#include "qtl/utility.h"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <new>

#ifndef qhashset_load_factor
#define qhashset_load_factor 0.7
#endif

namespace qtl
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	namespace qinternal
	{
		enum hash_status
		{
			OCCUPIED,
			TOMBSTONE,
			VACANT,
		};

		template <typename Key>
		class forward_unordered_set_iterator
		{
			forward_unordered_map_iterator<Key, Key> __it;
		public:
			forward_unordered_set_iterator(forward_unordered_map_iterator<Key, Key> ptr);
			forward_unordered_set_iterator<Key>& operator++();

			bool operator!=(const forward_unordered_set_iterator<Key>& it) const;
			bool operator==(const forward_unordered_set_iterator<Key>& it) const;

			Key& operator*();
			const Key& operator*() const;

			Key* operator->();
			const Key* operator->() const;
		};
		
		template<typename Key>
		inline forward_unordered_set_iterator<Key>::forward_unordered_set_iterator(forward_unordered_map_iterator<Key, Key> ptr)
			: __it(ptr)
		{
		}

		template<typename Key>
		inline forward_unordered_set_iterator<Key>& forward_unordered_set_iterator<Key>::operator++()
		{
			++__it;
			return *this;
		}

		template<typename Key>
		inline bool forward_unordered_set_iterator<Key>::operator!=(const forward_unordered_set_iterator<Key>& it) const
		{
			return __it != it.__it;
		}

		template<typename Key>
		inline bool forward_unordered_set_iterator<Key>::operator==(const forward_unordered_set_iterator<Key>& it) const
		{
			return __it == it.__it;
		}

		template<typename Key>
		inline Key & forward_unordered_set_iterator<Key>::operator*()
		{
			return __it.operator*().first;
		}

		template<typename Key>
		inline const Key & forward_unordered_set_iterator<Key>::operator*() const
		{
			return __it.operator*().first;
		}

		template<typename Key>
		inline Key * forward_unordered_set_iterator<Key>::operator->()
		{
			return &(__it.operator*().first);
		}

		template<typename Key>
		inline const Key * forward_unordered_set_iterator<Key>::operator->() const
		{
			return &(__it.operator*().first);
		}

	}
#endif

	/// <summary>
	/// Hash set implementation.  Hash sets cannot contain multiple
	/// objects of the same value, as determined by the equality (==)
	/// operator.
	/// </summary>
	/// <typeparam name="T">
	/// Type to store.  Must implement operator ==
	/// </typeparam>
	template <typename T>
	class unordered_set
	{
	public:
		template <typename Type>
		using forward_iterator = typename qinternal::forward_unordered_set_iterator<Type>;

		/// <summary>
		/// Default constructor of hash set.  Creates a hash set with 16 buckets
		/// and a load balance of 70%.  Complexity O(1)
		/// </summary>
		unordered_set() noexcept = default;

		/// <summary>
		/// Deallocates hash set. Complexity O(n)
		/// </summary>
		~unordered_set() = default;

		unordered_set(const unordered_set& other);
		unordered_set(unordered_set&& other) noexcept;

		unordered_set& operator=(const unordered_set& other);
		unordered_set& operator=(unordered_set&& other) noexcept;

		forward_iterator<T> find(const T& value);
		const forward_iterator<T> find(const T& value) const;

		/// <summary>
		/// Inserts a value into the hash set.  Complexity O(1) to O(n)
		/// </summary>
		/// <param name="value">
		/// Value to insert
		/// </param>
		void insert(const T& value);

		/// <summary>
		/// Inserts a value into the hash set.  Complexity O(1) to O(n)
		/// </summary>
		/// <param name="value">
		/// Value to insert
		/// </param>
		void insert(T&& value);

		/// <summary>
		/// Gets the number of elements contained in the hash set.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// Number of elements contained
		/// </returns>
		std::size_t size() const noexcept;

		bool empty() const noexcept;

		forward_iterator<T> begin();
		const forward_iterator<T> begin() const;

		forward_iterator<T> end();
		const forward_iterator<T> end() const;

		void erase(const T& value);
	private:
		qtl::unordered_map<T, T> __map;
	};

	template <typename T>
	inline unordered_set<T>::unordered_set(const unordered_set<T>& other)
	{
		__map = other.__map;
	}

	template <typename T>
	inline unordered_set<T>::unordered_set(unordered_set<T>&& other) noexcept
	{
		__map = other.__map;
	}

	template <typename T>
	inline unordered_set<T>& unordered_set<T>::operator=(const unordered_set<T>& other)
	{
		__map = other.__map;
	}

	template <typename T>
	inline unordered_set<T>& unordered_set<T>::operator=(unordered_set<T>&& other) noexcept
	{
		__map = other.__map;
	}

	template <typename T>
	inline typename unordered_set<T>::template forward_iterator<T> unordered_set<T>::find(const T& value)
	{
		return __map.find(value);
	}

	template <typename T>
	inline const typename unordered_set<T>::template forward_iterator<T> unordered_set<T>::find(const T& value) const
	{
		return __map.find(value);
	}

	template <typename T>
	inline void unordered_set<T>::insert(const T& value)
	{
		__map.insert(pair{ value, value });
	}

	template <typename T>
	inline void unordered_set<T>::insert(T&& value)
	{
		__map.insert(pair{ value, value });
	}

	template <typename T>
	inline std::size_t unordered_set<T>::size() const noexcept
	{
		return __map.size();
	}

	template <typename T>
	inline bool unordered_set<T>::empty() const noexcept
	{
		return __map.empty();
	}

	template <typename T>
	inline typename unordered_set<T>::template forward_iterator<T> unordered_set<T>::begin()
	{
		return qinternal::forward_unordered_set_iterator(__map.begin());
	}

	template <typename T>
	inline const typename unordered_set<T>::template forward_iterator<T> unordered_set<T>::begin() const
	{
		return qinternal::forward_unordered_set_iterator(__map.begin());
	}

	template <typename T>
	inline typename unordered_set<T>::template forward_iterator<T> unordered_set<T>::end()
	{
		return qinternal::forward_unordered_set_iterator(__map.end());
	}

	template <typename T>
	inline const typename unordered_set<T>::template forward_iterator<T> unordered_set<T>::end() const
	{
		return qinternal::forward_unordered_set_iterator(__map.end());
	}

	template<typename T>
	inline void unordered_set<T>::erase(const T & value)
	{
		__map.erase(value);
	}
}

#endif // __unorderedset_h_