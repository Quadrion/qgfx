#ifndef __treemap_h_
#define __treemap_h_

#include "qtl/redblack_tree.h"
#include "qtl/type_traits.h"
#include "qtl/utility.h"

#include <cstddef>

namespace qtl
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	namespace qinternal
	{
		template<typename Key, typename Value>
		struct tree_map_node
		{
			qtl::pair<Key, Value> kv;
			
			bool operator==(const tree_map_node& node) const;
			bool operator!=(const tree_map_node& node) const;
			bool operator< (const tree_map_node& node) const;
			bool operator> (const tree_map_node& node) const;
			bool operator<=(const tree_map_node& node) const;
			bool operator>=(const tree_map_node& node) const;
		};

		template<typename Key, typename Value>
		tree_map_node(Key, Value)->tree_map_node<Key, Value>;

		template<typename Key, typename Value>
		inline bool tree_map_node<Key, Value>::operator==(const tree_map_node & node) const
		{
			return node.kv.first == kv.first;
		}

		template<typename Key, typename Value>
		inline bool tree_map_node<Key, Value>::operator!=(const tree_map_node & node) const
		{
			return node.kv.first != kv.first;
		}

		template<typename Key, typename Value>
		inline bool tree_map_node<Key, Value>::operator<(const tree_map_node & node) const
		{
			return node.kv.first > kv.first;
		}

		template<typename Key, typename Value>
		inline bool tree_map_node<Key, Value>::operator>(const tree_map_node & node) const
		{
			return node.kv.first < kv.first;
		}

		template<typename Key, typename Value>
		inline bool tree_map_node<Key, Value>::operator<=(const tree_map_node & node) const
		{
			return node.kv.first >= kv.first;
		}

		template<typename Key, typename Value>
		inline bool tree_map_node<Key, Value>::operator>=(const tree_map_node & node) const
		{
			return node.kv.first <= kv.first;
		}

		template<typename Key, typename Value>
		class tree_map_forward_iterator
		{
		public:
			tree_map_forward_iterator(const rb_tree_forward_iterator<qinternal::tree_map_node<Key, Value>> & it);
			tree_map_forward_iterator& operator++();

			bool operator!=(const tree_map_forward_iterator& other) const;
			bool operator==(const tree_map_forward_iterator& other) const;

			pair<Key, Value>& operator*();
			const pair<Key, Value>& operator*() const;
		private:
			rb_tree_forward_iterator<qinternal::tree_map_node<Key, Value>> __it;
		};

		template<typename Key, typename Value>
		inline tree_map_forward_iterator<Key, Value>::tree_map_forward_iterator(const rb_tree_forward_iterator<qinternal::tree_map_node<Key, Value>> & it)
			: __it(it)
		{
		}

		template<typename Key, typename Value>
		inline tree_map_forward_iterator<Key, Value>& tree_map_forward_iterator<Key, Value>::operator++()
		{
			++__it;
			return *this;
		}

		template<typename Key, typename Value>
		inline bool tree_map_forward_iterator<Key, Value>::operator!=(const tree_map_forward_iterator & other) const
		{
			return __it != other.__it;
		}

		template<typename Key, typename Value>
		inline bool tree_map_forward_iterator<Key, Value>::operator==(const tree_map_forward_iterator & other) const
		{
			return __it == other.__it;
		}

		template<typename Key, typename Value>
		inline pair<Key, Value>& tree_map_forward_iterator<Key, Value>::operator*()
		{
			return __it.node->value->kv;
		}

		template<typename Key, typename Value>
		inline const pair<Key, Value>& tree_map_forward_iterator<Key, Value>::operator*() const
		{
			return __it.node->value->kv;
		}
	}
#endif

	/// <summary>
	/// Map sorted by key in a lexigraphical order
	/// </summary>
	/// <typeparam name="Key">
	/// Key type
	/// </typeparam>
	/// <typeparam name="Value">
	/// Value type
	/// </typeparam>
	template<typename Key, typename Value>
	class tree_map
	{
	public:
		/// <summary>
		/// Forward iterator type for tree map
		/// </summary>
		/// <typeparam name="K">
		/// Key type
		/// </typeparam>
		/// <typeparam name="V">
		/// Value type
		/// </typeparam>
		template<typename K, typename V>
		using forward_iterator = typename qinternal::tree_map_forward_iterator<Key, Value>;

		/// <summary>
		/// Constructs empty tree map
		/// </summary>
		tree_map() = default;

		/// <summary>
		/// Copy constructor.  Complexity O(n)
		/// </summary>
		/// <param name="other">
		/// Map to copy from
		/// </param>
		tree_map(const tree_map& other);

		/// <summary>
		/// Move constructor.  Complexity O(1)
		/// </summary>
		/// <param name="other">
		/// Map to move from
		/// </param>
		tree_map(tree_map&& other) noexcept;

		/// <summary>
		/// Deallocates tree.  Complexity O(n)
		/// </summary>
		~tree_map() = default;

		/// <summary>
		/// Copy operator.  Complexity O(n)
		/// </summary>
		/// <param name="other">
		/// Map to copy from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		tree_map& operator=(const tree_map& other);

		/// <summary>
		/// Move operator.  Complexity O(1)
		/// </summary>
		/// <param name="other">
		/// Map to move from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		tree_map& operator=(tree_map&& other) noexcept;

		/// <summary>
		/// Clears map.  Complexity O(n)
		/// </summary>
		void clear();

		/// <summary>
		/// Erases an element from the map.  Complexity O(log(n))
		/// </summary>
		/// <param name="key">
		/// Key to erase
		/// </param>
		/// <returns>
		/// Returns 0 if not erased
		/// </returns>
		std::size_t erase(const Key& key);

		/// <summary>
		/// Find value in map.  Complexity O(log(n))
		/// </summary>
		/// <param name="key">
		/// Key to search for
		/// </param>
		/// <returns>
		/// Iterator to element found
		/// </returns>
		forward_iterator<Key, Value> find(const Key& key);

		/// <summary>
		/// Inserts value.  Complexity O(log(n))
		/// </summary>
		/// <param name="val">
		/// Pair to insert
		/// </param>
		void insert(const pair<Key, Value>& val);

		/// <summary>
		/// Inserts value.  Complexity O(log(n))
		/// </summary>
		/// <param name="val">
		/// Pair to insert
		/// </param>
		void insert(pair<Key, Value>&& val);

		/// <summary>
		/// Gets size of the map.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// Number of elements in the map
		/// </returns>
		std::size_t size() const noexcept;

		/// <summary>
		/// Gets if the map is empty.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// True if map is empty
		/// </returns>
		bool empty() const noexcept;

		/// <summary>
		/// Gets an iterator pointing to the beginning element
		/// </summary>
		/// <returns>
		/// Iterator pointing to beginning element
		/// </returns>
		forward_iterator<Key, Value> begin();

		/// <summary>
		/// Gets an iterator pointing to the beginning element
		/// </summary>
		/// <returns>
		/// Iterator pointing to beginning element
		/// </returns>
		const forward_iterator<Key, Value> begin() const;

		/// <summary>
		/// Gets an iterator pointing to the ending element
		/// </summary>
		/// <returns>
		/// Iterator pointing to ending element
		/// </returns>
		forward_iterator<Key, Value> end();

		/// <summary>
		/// Gets an iterator pointing to the ending element
		/// </summary>
		/// <returns>
		/// Iterator pointing to ending element
		/// </returns>
		const forward_iterator<Key, Value> end() const;
	private:
		qtl::redblack_tree<qinternal::tree_map_node<Key, Value>> __tree;
	};

	template<typename Key, typename Value>
	inline typename tree_map<Key, Value>::template forward_iterator<Key, Value> tree_map<Key, Value>::begin()
	{
		return __tree.begin();
	}

	template<typename Key, typename Value>
	inline const typename tree_map<Key, Value>::template forward_iterator<Key, Value> tree_map<Key, Value>::begin() const
	{
		return __tree.begin();
	}

	template<typename Key, typename Value>
	inline typename tree_map<Key, Value>::template forward_iterator<Key, Value> tree_map<Key, Value>::end()
	{
		return __tree.end();
	}

	template<typename Key, typename Value>
	inline const typename tree_map<Key, Value>::template forward_iterator<Key, Value> tree_map<Key, Value>::end() const
	{
		return __tree.end();
	}

	template<typename Key, typename Value>
	inline tree_map<Key, Value>::tree_map(const tree_map & other)
	{
		__tree = other.__tree;
	}

	template<typename Key, typename Value>
	inline tree_map<Key, Value>::tree_map(tree_map && other) noexcept
	{
		__tree = qtl::move(other.__tree);
	}

	template<typename Key, typename Value>
	inline tree_map<Key, Value>& tree_map<Key, Value>::operator=(const tree_map & other)
	{
		__tree = other.__tree;
		return *this;
	}

	template<typename Key, typename Value>
	inline tree_map<Key, Value>& tree_map<Key, Value>::operator=(tree_map && other) noexcept
	{
		__tree = qtl::move(other.__tree);
		return *this;
	}

	template<typename Key, typename Value>
	inline void tree_map<Key, Value>::clear()
	{
		__tree.clear();
	}

	template<typename Key, typename Value>
	inline std::size_t tree_map<Key, Value>::erase(const Key & key)
	{
		qinternal::tree_map_node<Key, Value> n;
		n.kv.first = key;
		if (__tree.contains(n))
		{
			__tree.erase(n);
			return 1;
		}
		return 0;
	}

	template<typename Key, typename Value>
	inline typename tree_map<Key, Value>::template forward_iterator<Key, Value> tree_map<Key, Value>::find(const Key & key)
	{
		qinternal::tree_map_node<Key, Value> n;
		n.kv.first = key;
		return __tree.find(n);
	}

	template<typename Key, typename Value>
	inline void tree_map<Key, Value>::insert(const pair<Key, Value>& val)
	{
		qinternal::tree_map_node<Key, Value> findn;
		findn.kv = val;
		
		if (!__tree.contains(findn))
		{
			__tree.insert(findn);
		}
	}

	template<typename Key, typename Value>
	inline void tree_map<Key, Value>::insert(pair<Key, Value>&& val)
	{
		qinternal::tree_map_node<Key, Value> findn;
		findn.kv = val;

		if (!__tree.contains(findn))
		{
			__tree.insert(findn);
		}
	}

	template<typename Key, typename Value>
	inline std::size_t tree_map<Key, Value>::size() const noexcept
	{
		return __tree.size();
	}
	template<typename Key, typename Value>
	inline bool tree_map<Key, Value>::empty() const noexcept
	{
		return __tree.empty();
	}
}

#endif