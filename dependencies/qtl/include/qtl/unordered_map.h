#ifndef unordered_map__
#define unordered_map__

#include "qtl/hash.h"
#include "qtl/utility.h"

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>

namespace qtl
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	namespace qinternal
	{
		template <typename Key, typename Value>
		struct unordered_map_node
		{
			pair<Key, Value> kv;
			std::size_t probe_count = 0;
			bool is_occupied = false;

			void swap(unordered_map_node & other)
			{
				qtl::swap(kv, other.kv);
				qtl::swap(probe_count, other.probe_count);
				qtl::swap(is_occupied, other.is_occupied);
			}
		};

		template <typename Key, typename Value>
		class forward_unordered_map_iterator
		{
			unordered_map_node<Key, Value>* __data;
			std::size_t __cursor;
			std::size_t __capacity;
		public:
			forward_unordered_map_iterator(unordered_map_node<Key, Value>* ptr, std::size_t it, std::size_t cap);
			forward_unordered_map_iterator<Key, Value>& operator++();

			bool operator!=(const forward_unordered_map_iterator<Key, Value>& it) const;
			bool operator==(const forward_unordered_map_iterator<Key, Value>& it) const;

			qtl::pair<Key, Value>& operator*();
			const qtl::pair<Key, Value>& operator*() const;

			pair<Key, Value>* operator->();
			const pair<Key, Value>* operator->() const;
		};

		template<typename Key, typename Value>
		inline forward_unordered_map_iterator<Key, Value>::forward_unordered_map_iterator(unordered_map_node<Key, Value>* ptr, std::size_t it, std::size_t cap)
			: __data(ptr), __cursor(it), __capacity(cap)
		{
		}

		template<typename Key, typename Value>
		inline forward_unordered_map_iterator<Key, Value>& forward_unordered_map_iterator<Key, Value>::operator++()
		{
			while (++__cursor < __capacity)
			{
				if (__data[__cursor].is_occupied)
				{
					break;
				}
			}
			return *this;
		}

		template<typename Key, typename Value>
		inline bool forward_unordered_map_iterator<Key, Value>::operator!=(const forward_unordered_map_iterator<Key, Value>& it) const
		{
			return (__data != it.__data) || (__cursor != it.__cursor);
		}

		template<typename Key, typename Value>
		inline bool forward_unordered_map_iterator<Key, Value>::operator==(const forward_unordered_map_iterator<Key, Value>& it) const
		{
			return (__data == it.__data) && (__cursor == it.__cursor);
		}

		template<typename Key, typename Value>
		inline qtl::pair<Key, Value> & forward_unordered_map_iterator<Key, Value>::operator*()
		{
			return __data[__cursor].kv;
		}

		template<typename Key, typename Value>
		inline const qtl::pair<Key, Value> & forward_unordered_map_iterator<Key, Value>::operator*() const
		{
			return __data[__cursor].kv;
		}

		template<typename Key, typename Value>
		inline pair<Key, Value>* forward_unordered_map_iterator<Key, Value>::operator->()
		{
			return &(__data + __cursor)->kv;
		}

		template<typename Key, typename Value>
		inline const pair<Key, Value>* forward_unordered_map_iterator<Key, Value>::operator->() const
		{
			return &(__data + __cursor)->kv;
		}
	} // qinternal
#endif

	/// <summary>
	/// Unordered map using hashing scheme for item placement.  Uses robin hood heuristic to reduce
	/// probe sequence length.  The map allows for no duplicate keys.
	/// </summary>
	/// <param name="Key">
	/// Type of value to be stored in the key
	/// </param>
	/// <param name="Value">
	/// Type of value to be stored in the value associated with the key
	/// </param>
	/// <param name="Hash">
	/// Hashing function to be used.  By default, it is qtl::hash<Key>
	/// </param>
	/// <param name="DefaultSize">
	/// Initial size of the hash map.  By default, it is 8
	/// </param>
	template <typename Key, typename Value, typename Hash = qtl::hash<Key>, std::size_t DefaultSize = 8>
	class unordered_map
	{
	public:

		/// <summary>
		/// Forward iterator for unordered map. 
		/// </summary>
		/// <typeparam name="K">
		/// Key type
		/// </typeparam>
		/// <typeparam name="V">
		/// Value type
		/// </typeparam>
		template <typename K, typename V>
		using forward_iterator = typename qinternal::forward_unordered_map_iterator<K, V>;

		/// <summary>
		/// Constructs a new, empty unordered map.  Complexity O(1)
		/// </summary>
		unordered_map() noexcept;

		/// <summary>
		/// Copy constructor.  Complexity O(n)
		/// </summary>
		/// <param name="other">
		/// Map to copy from
		/// </param>
		unordered_map(const unordered_map& other);

		/// <summary>
		/// Move constructor.  Complexity O(1)
		/// </summary>
		/// <param name="other">
		/// Map to move from
		/// </param>
		unordered_map(unordered_map&& other) noexcept;

		/// <summary>
		/// Deconstructs an unordered map.  Complexity O(n)
		/// </summary>
		~unordered_map();

		/// <summary>
		/// Copy operator.  Complexity O(n)
		/// </summary>
		/// <param name="other">
		/// Map to copy from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		unordered_map& operator=(const unordered_map& other);

		/// <summary>
		/// Move operator.  Complexity O(1)
		/// </summary>
		/// <param name="other">
		/// Map to move from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		unordered_map& operator=(unordered_map&& other) noexcept;

		Value& operator[](const Key& sz);
		const Value& operator[](const Key& sz) const;

		/// <summary>
		/// Returns if the map is empty.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// True if map is empty
		/// </returns>
		bool empty() const;

		/// <summary>
		/// Returns size of the map.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// Size of map
		/// </returns>
		std::size_t size() const;

		/// <summary>
		/// Clears the contents from the map.  Complexity O(n)
		/// </summary>
		void clear();

		/// <summary>
		/// Sets the number of buckets in the map when the requested size is greater than the 
		/// current usage.  Average and Worst Case - Complexity O(n), Best Case - Complexity O(1)
		/// </summary>
		/// <param name="newSize">
		/// Requested size
		/// </param>
		void resize(const std::size_t newSize);
		
		/// <summary>
		/// Inserts an element into the map.  Best and Average Case - Complexity O(1), Worst Case - Complexity O(n)
		/// </summary>
		/// <param name="pair">
		/// Pair to insert
		/// </param>
		void insert(const qtl::pair<Key, Value>& pair);

		/// <summary>
		/// Inserts an element into the map.  Best and Average Case - Complexity O(1), Worst Case - Complexity O(n)
		/// </summary>
		/// <param name="pair">
		/// Pair to insert
		/// </param>
		void insert(qtl::pair<Key, Value>&& pair);

		/// <summary>
		/// Searches for an element in the map.  If the element it found, it has its probe count increased.  If the
		/// probe count is greater than the previous element in the probe sequence, the elements are swapped per the
		/// robin hood heuristic.  Best and average case - Complexity O(1), Worse Case - Complexity O(n)
		/// </summary>
		/// <param name="key">
		/// Key to search for in map
		/// </param>
		/// <returns>
		/// Iterator to element found.  If element not found, returns end iterator
		/// </returns>
		forward_iterator<Key, Value> find(const Key& key);

		/// <summary>
		/// Searches for an element in the map.  If the element it found, it has its probe count increased.  If the
		/// probe count is greater than the previous element in the probe sequence, the elements are swapped per the
		/// robin hood heuristic.  Best and average case - Complexity O(1), Worse Case - Complexity O(n)
		/// </summary>
		/// <param name="key">
		/// Key to search for in map
		/// </param>
		/// <returns>
		/// Iterator to element found.  If element not found, returns end iterator
		/// </returns>
		const forward_iterator<Key, Value> find(const Key& key) const;

		/// <summary>
		/// Erases an element from the map. Best and average case - Complexity O(1), Worse Case - Complexity O(n)
		/// </summary>
		/// <param name="key">
		/// Key value to search for
		/// </param>
		/// <returns>
		/// 0 if element not found
		/// </returns>
		std::size_t erase(const Key& key);

		/// <summary>
		/// Gets an iterator to the first element in the map.  Best and average case - Complexity O(1), Worse Case - Complexity O(n)
		/// </summary>
		/// <returns>
		/// Iterator to first element.
		/// </returns>
		forward_iterator<Key, Value> begin();

		/// <summary>
		/// Gets an iterator to the first element in the map.  Best and average case - Complexity O(1), Worse Case - Complexity O(n)
		/// </summary>
		/// <returns>
		/// Iterator to first element.
		/// </returns>
		const forward_iterator<Key, Value> begin() const;

		/// <summary>
		/// Gets an iterator to the end element in the map.  Best and average case - Complexity O(1), Worse Case - Complexity O(n)
		/// </summary>
		/// <returns>
		/// Iterator to end element.
		/// </returns>
		forward_iterator<Key, Value> end();

		/// <summary>
		/// Gets an iterator to the end element in the map.  Best and average case - Complexity O(1), Worse Case - Complexity O(n)
		/// </summary>
		/// <returns>
		/// Iterator to end element.
		/// </returns>
		const forward_iterator<Key, Value> end() const;
	private:
		static constexpr double __rebalance_factor = 0.7;
		std::size_t __capacity;
		std::size_t __usage;
		qinternal::unordered_map_node<Key, Value>* __data;
		Hash __hasher;

		void __resize(const std::size_t newSize);
		bool __insert(const qtl::pair<Key, Value>& data, qinternal::unordered_map_node<Key, Value>* buffer, const std::size_t bufferSize);
		bool __insert(qtl::pair<Key, Value>&& data, qinternal::unordered_map_node<Key, Value>* buffer, const std::size_t bufferSize);
	};

	template <typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline unordered_map<Key, Value, Hash, DefaultSize>::unordered_map() noexcept
		: __capacity(DefaultSize), __usage(0), __data(nullptr)
	{
		__resize(__capacity);
	}

	template <typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline unordered_map<Key, Value, Hash, DefaultSize>::unordered_map(const unordered_map<Key, Value, Hash, DefaultSize>& other)
		: __capacity(other.__capacity), __usage(0), __data(nullptr)
	{
		__resize(__capacity);
		for (auto it : other)
		{
			insert({ it.first, it.second });
		}
	}

	template <typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline unordered_map<Key, Value, Hash, DefaultSize>::unordered_map(unordered_map<Key, Value, Hash, DefaultSize>&& other) noexcept
		: __capacity(other.__capacity), __usage(other.__usage), __data(other.__data)
	{
		other.__data = nullptr;
	}

	template <typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline unordered_map<Key, Value, Hash, DefaultSize>::~unordered_map()
	{
		if (__data)
		{
			clear();
			delete[] reinterpret_cast<char*>(__data);
		}
	}

	template <typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline unordered_map<Key, Value, Hash, DefaultSize>& unordered_map<Key, Value, Hash, DefaultSize>::operator=(const unordered_map<Key, Value, Hash, DefaultSize>& other)
	{
		if (__data)
		{
			clear();
			delete[] reinterpret_cast<char*>(__data);
		}

		resize(other.__capacity);

		for (auto it : other)
		{
			insert({ it.first, it.second });
		}
	}

	template <typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline unordered_map<Key, Value, Hash, DefaultSize>& unordered_map<Key, Value, Hash, DefaultSize>::operator=(unordered_map<Key, Value, Hash, DefaultSize>&& other) noexcept
	{
		__data = other.__data;
		__usage = other.__usage;
		__capacity = other.__capacity;
		
		other.__data = nullptr;
	}

	template<typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline Value & unordered_map<Key, Value, Hash, DefaultSize>::operator[](const Key & sz)
	{
		auto it = find(sz);
		if (it == end())
		{
			insert({ sz, Value() });
			return find(sz)->second;
		}
		return it->second;
	}

	template<typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline const Value & unordered_map<Key, Value, Hash, DefaultSize>::operator[](const Key & sz) const
	{
		return find(sz)->second;
	}

	template<typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline bool unordered_map<Key, Value, Hash, DefaultSize>::empty() const
	{
		return __usage == 0;
	}

	template<typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline std::size_t unordered_map<Key, Value, Hash, DefaultSize>::size() const
	{
		return __usage;
	}

	template<typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline void unordered_map<Key, Value, Hash, DefaultSize>::clear()
	{
		for (std::size_t i = 0; i < __capacity; i++)
		{
			auto & v = __data[i];
			if (v.is_occupied)
			{
				using qinternal::unordered_map_node;

				v.~unordered_map_node<Key, Value>();
			}
		}
		__usage = 0;
	}

	template<typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline void unordered_map<Key, Value, Hash, DefaultSize>::resize(const std::size_t newSize)
	{
		if (newSize > __usage)
			__resize(newSize);
	}

	template<typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline void unordered_map<Key, Value, Hash, DefaultSize>::insert(const qtl::pair<Key, Value>& pair)
	{
		if (static_cast<double>(__usage) / static_cast<double>(__capacity) > __rebalance_factor)
		{
			__resize(__capacity * 2);
		}
		if (__insert(pair, __data, __capacity))
		{
			__usage++;
		}
	}

	template<typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline void unordered_map<Key, Value, Hash, DefaultSize>::insert(qtl::pair<Key, Value>&& pair)
	{
		if (static_cast<double>(__usage) / static_cast<double>(__capacity) > __rebalance_factor)
		{
			__resize(__capacity * 2);
		}
		if (__insert(pair, __data, __capacity))
		{
			__usage++;
		}
	}

	template<typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline typename unordered_map<Key, Value, Hash, DefaultSize>::template forward_iterator<Key, Value> unordered_map<Key, Value, Hash, DefaultSize>::find(const Key & key)
	{
		const std::size_t hashed = __hasher(key);
		std::size_t idx = (hashed) % __capacity;
		if (__data[idx].is_occupied && __data[idx].kv.first == key)
		{
			__data[idx].probe_count++;
			return unordered_map<Key, Value, Hash, DefaultSize>::forward_iterator<Key, Value>(__data, idx, __capacity);
		}

		for (std::size_t i = 0; i < __capacity; i++)
		{
			idx = (hashed + i) % __capacity;
			std::size_t next_idx = (hashed + i + 1) % __capacity;
			if (__data[idx].is_occupied)
			{
				if (__data[next_idx].is_occupied && __data[next_idx].kv.first == key)
				{
					__data[next_idx].probe_count++;
					if (__data[next_idx].probe_count > __data[idx].probe_count)
					{
						qtl::swap(__data[next_idx], __data[idx]);
						return unordered_map<Key, Value, Hash, DefaultSize>::forward_iterator<Key, Value>(__data, idx, __capacity);
					}
				}
			}
			else
			{
				break;
			}
		}

		return unordered_map<Key, Value, Hash, DefaultSize>::forward_iterator<Key, Value>(__data, __capacity, __capacity);
	}

	template<typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline const typename unordered_map<Key, Value, Hash, DefaultSize>::template forward_iterator<Key, Value> unordered_map<Key, Value, Hash, DefaultSize>::find(const Key & key) const
	{
		const std::size_t hashed = __hasher(key);
		std::size_t idx = (hashed) % __capacity;
		if (__data[idx].is_occupied && __data[idx].kv.first == key)
		{
			__data[idx].probe_count++;
			return unordered_map<Key, Value, Hash, DefaultSize>::forward_iterator<Key, Value>(__data, idx, __capacity);
		}

		for (std::size_t i = 0; i < __capacity; i++)
		{
			idx = (hashed + i) % __capacity;
			std::size_t next_idx = (hashed + i + 1) % __capacity;
			if (__data[idx].is_occupied)
			{
				if (__data[next_idx].is_occupied && __data[next_idx].kv.first == key)
				{
					__data[next_idx].probe_count++;
					if (__data[next_idx].probe_count > __data[idx].probe_count)
					{
						qtl::swap(__data[next_idx], __data[idx]);
						return unordered_map<Key, Value, Hash, DefaultSize>::forward_iterator<Key, Value>(__data, idx, __capacity);
					}
				}
			}
			else
			{
				break;
			}
		}

		return unordered_map<Key, Value, Hash, DefaultSize>::forward_iterator<Key, Value>(__data, __capacity, __capacity);
	}

	template<typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline std::size_t unordered_map<Key, Value, Hash, DefaultSize>::erase(const Key & key)
	{
		const std::size_t hashed = __hasher(key);
		std::size_t remove_loc = __capacity;
		for (std::size_t i = 0; i < __capacity; i++)
		{
			std::size_t search_idx = (hashed + i) % __capacity;
			if (__data[search_idx].kv.first == key)
			{
				remove_loc = search_idx;
				break;
			}
		}
		if (remove_loc != __capacity)
		{
			using qinternal::unordered_map_node;

			auto it_to_remove = __data[remove_loc];
			it_to_remove.~unordered_map_node<Key, Value>();
			new (__data + remove_loc) unordered_map_node<Key, Value>();
			it_to_remove.is_occupied = false;
			it_to_remove.probe_count = 0;

			std::size_t open_slot = remove_loc;
			std::size_t search_len = 1;

			for (std::size_t i = 1; i < __capacity; i++)
			{
				std::size_t search_idx = (open_slot + i) % __capacity;
				if (!__data[search_idx].is_occupied)
				{
					search_len = i;
					break;
				}
			}

			for (std::size_t i = 1; i < search_len; i++)
			{
				std::size_t search_idx = (remove_loc + i) % __capacity;
				
				const std::size_t hash_v = __hasher(__data[search_idx].kv.first);
				const std::size_t cur_probe_length = search_idx >= hash_v ? (search_idx - hash_v) : (__capacity - (hash_v - search_idx));
				const std::size_t new_probe_length = open_slot >= hash_v ? (open_slot - hash_v) : (__capacity - (hash_v - open_slot));
				if (new_probe_length < cur_probe_length)
				{
					__data[open_slot] = __data[search_idx];
					open_slot = search_idx;

					it_to_remove = __data[search_idx];
					it_to_remove.~unordered_map_node<Key, Value>();
					new (__data + search_idx) unordered_map_node<Key, Value>();
					it_to_remove.is_occupied = false;
					it_to_remove.probe_count = 0;
				}
			}
			__usage--;
		}
		return remove_loc == __capacity ? 0 : 1;
	}

	template<typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline typename unordered_map<Key, Value, Hash, DefaultSize>::template forward_iterator<Key, Value> unordered_map<Key, Value, Hash, DefaultSize>::begin()
	{
		std::size_t first = 0;
		while (first < __capacity && !__data[first].is_occupied)
		{
			first++;
		}
		return forward_iterator<Key, Value>(__data, first, __capacity);
	}

	template<typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline const typename unordered_map<Key, Value, Hash, DefaultSize>::template forward_iterator<Key, Value> unordered_map<Key, Value, Hash, DefaultSize>::begin() const
	{
		std::size_t first = 0;
		while (first < __capacity && !__data[first].is_occupied)
		{
			first++;
		}
		return forward_iterator<Key, Value>(__data, first, __capacity);
	}

	template<typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline typename unordered_map<Key, Value, Hash, DefaultSize>::template forward_iterator<Key, Value> unordered_map<Key, Value, Hash, DefaultSize>::end()
	{
		return forward_iterator<Key, Value>(__data, __capacity, __capacity);
	}

	template<typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline const typename unordered_map<Key, Value, Hash, DefaultSize>::template forward_iterator<Key, Value> unordered_map<Key, Value, Hash, DefaultSize>::end() const
	{
		return forward_iterator<Key, Value>(__data, __capacity, __capacity);
	}

	template <typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline void unordered_map<Key, Value, Hash, DefaultSize>::__resize(const std::size_t newSize)
	{
		std::size_t sz = newSize * sizeof(qinternal::unordered_map_node<Key, Value>);
		qinternal::unordered_map_node<Key, Value>* buf = reinterpret_cast<qinternal::unordered_map_node<Key, Value>*>(new char[sz]);
		memset(buf, 0, sz);
		if (__data == nullptr)
		{
			__data = buf;
			return;
		}
		for (std::size_t i = 0; i < __capacity; i++)
		{
			if (__data[i].is_occupied)
			{
				__insert(__data[i].kv, buf, newSize);
			}
		}
		delete[] reinterpret_cast<char*>(__data);
		__data = buf;
		__capacity = newSize;
	}

	template <typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline bool unordered_map<Key, Value, Hash, DefaultSize>::__insert(const qtl::pair<Key, Value>& data, qinternal::unordered_map_node<Key, Value>* buffer, const std::size_t bufferSize)
	{
		const std::size_t hashed = __hasher(data.first);
		for (std::size_t i = 0; i < bufferSize; i++)
		{
			std::size_t idx = (hashed + i) % bufferSize;

			if (!buffer[idx].is_occupied)
			{	
				// found an open slot
				auto loc = new (buffer + idx) qinternal::unordered_map_node<Key, Value>();
				loc->kv = data;
				loc->is_occupied = true;
				loc->probe_count = 0;
				return true;
			}
			else
			{
				if (buffer[idx].kv.first == data.first)
				{
					return false;
				}
			}
			assert(i != bufferSize - 1); // only breaks if the entire map is traversed without finding a slot. THIS SHOULD NEVER HAPPEN
		}
		return false;
	}

	template <typename Key, typename Value, typename Hash, std::size_t DefaultSize>
	inline bool unordered_map<Key, Value, Hash, DefaultSize>::__insert(qtl::pair<Key, Value>&& data, qinternal::unordered_map_node<Key, Value>* buffer, const std::size_t bufferSize)
	{
		const std::size_t hashed = __hasher(data.first);
		for (std::size_t i = 0; i < bufferSize; i++)
		{
			std::size_t idx = (hashed + i) % bufferSize;

			if (!buffer[idx].is_occupied)
			{
				// found an open slot
				auto loc = new (buffer + idx) qinternal::unordered_map_node<Key, Value>();
				loc->kv = data;
				loc->is_occupied = true;
				loc->probe_count = 0;
				return true;
			}
			else
			{
				if (buffer[idx].kv.first == data.first)
				{
					return false;
				}
			}
			assert(i != bufferSize - 1); // only breaks if the entire map is traversed without finding a slot. THIS SHOULD NEVER HAPPEN
		}
		return false;
	}

} // qtl

#endif