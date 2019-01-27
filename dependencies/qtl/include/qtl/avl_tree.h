#ifndef avltree_h__
#define avltree_h__

#include "qtl/list.h"
#include "qtl/math.h"
#include "qtl/utility.h"

#include <cassert>
#include <cstdint>

namespace qtl
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	namespace qinternal
	{
		template <typename T>
		struct avl_node
		{
			avl_node<T>* left;
			avl_node<T>* right;
			T* value;
			std::size_t height;
		};
	} // end qinternal
#endif
	/// <summary>
	/// Balanced binary search tree.  Has best search const coeffecient of 
	/// any BST.  The maximum height of the tree is 1.44 * log(n)
	/// </summary>
	/// <typeparam name="T">
	/// Type of value stored. Type should have operators "<", "<=", ">=", and ">" overriden
	/// </typeparam>
	template <typename T>
	class avl_tree
	{
	public:
		/// <summary>
		/// Constructs an empty avl tree. Complexity O(1)
		/// </summary>
		avl_tree();

		/// <summary>
		/// Constructs a new avl tree from another tree. Complexity O(n)
		/// </summary>
		/// <param name="tree">
		/// AVL tree to copy values from 
		/// </param>
		explicit avl_tree(const avl_tree& tree);

		/// <summary>
		/// Constructs a new avl tree from another tree. Complexity O(1)
		/// </summary>
		/// <param name="tree">
		/// AVL tree to move values from 
		/// </param>
		explicit avl_tree(avl_tree&& tree) noexcept;

		/// <summary>
		/// Destructs the AVL tree. Complexity O(n)
		/// </summary>
		~avl_tree();

		/// <summary>
		/// Copies the values from another tree to this tree. Complexity O(n)
		/// </summary>
		/// <param name="tree">
		/// Tree to copy values from
		/// </param>
		/// <returns>
		/// Reference for this tree
		/// </returns>
		avl_tree& operator=(const avl_tree& tree);

		/// <summary>
		/// Moves the values from another tree to this tree. Complexity O(1)
		/// </summary>
		/// <param name="tree">
		/// Tree to move values from
		/// </param>
		/// <returns>
		/// Reference for this tree
		/// </returns>
		avl_tree& operator=(avl_tree&& tree) noexcept;
		
		/// <summary>
		/// Checks if two trees have both identical values and structure. Complexity O(n)
		/// </summary>
		/// <param name="tree">
		/// Tree to check equality against
		/// </param>
		/// <returns>
		/// True if trees have identical values and structures, else false
		/// </returns>
		bool operator==(const avl_tree& tree) const;

		/// <summary>
		/// Checks if two trees do not have both identical values or structure. Complexity O(n)
		/// </summary>
		/// <param name="tree">
		/// Tree to check inequality against
		/// </param>
		/// <returns>
		/// True if trees do not have identical values or structures, else false
		/// </returns>
		bool operator!=(const avl_tree& tree) const;

		/// <summary>
		/// Inserts value into the tree. Complexity O(log(n))
		/// </summary>
		/// <param name="value">
		/// Value to insert by copy
		/// </param>
		void insert(const T& value);

		/// <summary>
		/// Inserts value into the tree. Complexity O(log(n))
		/// </summary>
		/// <param name="value">
		/// Value to insert by move
		/// </param>
		void insert(T&& value);

		/// <summary>
		/// Erases value from the tree. Complexity O(log(n))
		/// </summary>
		/// <param name="value">
		/// Value to erase
		/// </param>
		void erase(const T& value);

		/// <summary>
		/// Erases value from the tree. Complexity O(log(n))
		/// </summary>
		/// <param name="value">
		/// Value to erase
		/// </param>
		void erase(T&& value);

		/// <summary>
		/// Checks if the tree contains a value. Complexity O(log(n))
		/// </summary>
		/// <param name="value">
		/// Value to check if tree contains
		/// </param>
		/// <returns>
		/// True if contained, else false
		/// </returns>
		bool contains(const T& value) const;

		/// <summary>
		/// Checks if the tree contains a value. Complexity O(log(n))
		/// </summary>
		/// <param name="value">
		/// Value to check if tree contains
		/// </param>
		/// <returns>
		/// True if contained, else false
		/// </returns>
		bool contains(T&& value) const;

		/// <summary>
		/// Gets the number of elements in the tree. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Number of elements in tree
		/// </returns>
		std::size_t size() const noexcept;
	private:
		qinternal::avl_node<T>* __root;
		std::size_t __size;

		void __recursive_copy_construct(qinternal::avl_node<T>* node);
		qinternal::avl_node<T>* __recursive_insert(qinternal::avl_node<T>* node, const T& key);
		qinternal::avl_node<T>* __recursive_insert(qinternal::avl_node<T>* node, T&& key);
		bool __recursive_contains(qinternal::avl_node<T>* node, const T& value);
		bool __recursive_contains(qinternal::avl_node<T>* node, T&& value);
		void __recursive_delete(qinternal::avl_node<T>* node);
		qinternal::avl_node<T>* __recursive_erase(qinternal::avl_node<T>* node, const T& key);
		qinternal::avl_node<T>* __recursive_erase(qinternal::avl_node<T>* node, T&& key);
		int64_t __get_balance_factor(qinternal::avl_node<T>* node) const;
		std::size_t __get_height(qinternal::avl_node<T>* node) const noexcept;
		qinternal::avl_node<T>* __get_min_node(qinternal::avl_node<T>* node);
		qinternal::avl_node<T>* __rotate_left(qinternal::avl_node<T>* node);
		qinternal::avl_node<T>* __rotate_right(qinternal::avl_node<T>* node);
		bool __identical(qinternal::avl_node<T>* a, qinternal::avl_node<T>* b) const;
		bool __not_identical(qinternal::avl_node<T>* a, qinternal::avl_node<T>* b) const;
	};

	template<typename T>
	inline avl_tree<T>::avl_tree()
	{
		__root = nullptr;
		__size = 0;
	}

	template<typename T>
	inline avl_tree<T>::avl_tree(const avl_tree<T>& tree)
	{
		__root = nullptr;
		__size = 0;
		__recursive_copy_construct(tree.__root);
		assert(__size == tree.__size);
	}

	template<typename T>
	inline avl_tree<T>::avl_tree(avl_tree<T>&& tree) noexcept
	{
		__recursive_delete(__root);
		__root = tree.__root;
		__size = tree.__size;
		tree.__root = nullptr;
		tree.__size = 0;
	}

	template <typename T>
	inline avl_tree<T>::~avl_tree()
	{
		__recursive_delete(__root);
		assert(!__size);
	}

	template<typename T>
	inline avl_tree<T>& avl_tree<T>::operator=(const avl_tree<T>& tree)
	{
		__recursive_delete(__root);
		__recursive_copy_construct(tree.__root);
		assert(__size == tree.__size);
	}

	template<typename T>
	inline avl_tree<T>& avl_tree<T>::operator=(avl_tree<T>&& tree) noexcept
	{
		__recursive_delete(__root);
		__root = tree.__root;
		__size = tree.__size;
		tree.__root = nullptr;
		tree.__size = 0;
	}

	template<typename T>
	inline bool avl_tree<T>::operator==(const avl_tree<T>& tree) const
	{
		return __identical(__root, tree.__root);
	}

	template<typename T>
	inline bool avl_tree<T>::operator!=(const avl_tree<T>& tree) const
	{
		return __not_identical(__root, tree.__root);
	}

	template<typename T>
	inline void avl_tree<T>::insert(const T & value)
	{
		__root = __recursive_insert(__root, value);
	}

	template<typename T>
	inline void avl_tree<T>::insert(T && value)
	{
		__root = __recursive_insert(__root, value);
	}

	template<typename T>
	inline void avl_tree<T>::erase(const T & value)
	{
		__root = __recursive_erase(__root, value);
	}

	template<typename T>
	inline void avl_tree<T>::erase(T && value)
	{
		__root = __recursive_erase(__root, value);
	}

	template<typename T>
	inline bool avl_tree<T>::contains(const T & value) const
	{
		return __recursive_contains(__root, value);
	}

	template<typename T>
	inline bool avl_tree<T>::contains(T && value) const
	{
		return __recursive_contains(__root, value);
	}

	template<typename T>
	inline std::size_t avl_tree<T>::size() const noexcept
	{
		return __size;
	}

	template<typename T>
	inline void avl_tree<T>::__recursive_delete(qinternal::avl_node<T>* node)
	{
		if (node)
		{
			__recursive_delete(node->left);
			__recursive_delete(node->right);
			delete node->value;
			delete node;
			__size--;
		}
	}

	template<typename T>
	inline void avl_tree<T>::__recursive_copy_construct(qinternal::avl_node<T>* node)
	{
		if (!node)
		{
			return;
		}
		const T& val = *(node->value);
		insert(val);
		__recursive_copy_construct(node->left);
		__recursive_copy_construct(node->right);
	}

	template<typename T>
	inline qinternal::avl_node<T>* avl_tree<T>::__recursive_insert(qinternal::avl_node<T>* node, const T & key)
	{
		if (node == nullptr)
		{
			node = new qinternal::avl_node<T>();
			node->left = nullptr;
			node->right = nullptr;
			node->height = 0;
			node->value = new T(key);
			__size++;
			return node;
		}
		else if (key < *(node->value))
		{
			node->left = __recursive_insert(node->left, key);
		}
		else if (key >= *(node->value))
		{
			node->right = __recursive_insert(node->right, key);
		}
		else
		{
			return node;
		}

		node->height = __get_height(node);
		auto balance = __get_balance_factor(node);

		if (balance > 1 && key < *(node->left->value))
		{
			return __rotate_right(node);
		}
		if (balance > 1 && key >= *(node->left->value))
		{
			node->left = __rotate_left(node->left);
			return __rotate_right(node);
		}
		if (balance < -1 && key < *(node->right->value))
		{
			node->right = __rotate_right(node->right);
			return __rotate_left(node);
		}
		if (balance < -1 && key >= *(node->right->value))
		{
			return __rotate_left(node);
		}
		return node;
	}

	template<typename T>
	inline qinternal::avl_node<T>* avl_tree<T>::__recursive_insert(qinternal::avl_node<T>* node, T && key)
	{
		if (node == nullptr)
		{
			node = new qinternal::avl_node<T>();
			node->left = nullptr;
			node->right = nullptr;
			node->height = 0;
			node->value = new T(qtl::move(key));
			__size++;
			return node;
		}
		else if (key < *(node->value))
		{
			node->left = __recursive_insert(node->left, key);
			node->left->parent = node;
		}
		else if (key >= *(node->value))
		{
			node->right = __recursive_insert(node->right, key);
			node->right->parent = node;
		}
		else
		{
			return node;
		}

		node->height = __get_height(node);
		auto balance = __get_balance_factor(node);

		if (balance > 1 && key < *(node->left->value))
		{
			return __rotate_right(node);
		}
		if (balance > 1 && key >= *(node->left->value))
		{
			node->left = __rotate_left(node->left);
			return __rotate_right(node);
		}
		if (balance < -1 && key < *(node->right->value))
		{
			node->right = __rotate_right(node->right);
			return __rotate_left(node);
		}
		if (balance < -1 && key >= *(node->right->value))
		{
			return __rotate_left(node);
		}
		return node;
	}

	template<typename T>
	inline bool avl_tree<T>::__recursive_contains(qinternal::avl_node<T>* node, const T & value)
	{
		if (node == nullptr)
		{
			return false;
		}
		else if (value == *(node->value))
		{
			return true;
		}
		else if (value < *(node->value))
		{
			return __recursive_contains(node->left, value);
		}
		else
		{
			return __recursive_contains(node->right, value);
		}
	}

	template<typename T>
	inline bool avl_tree<T>::__recursive_contains(qinternal::avl_node<T>* node, T && value)
	{
		if (node == nullptr)
		{
			return false;
		}
		else if (value == *(node->value))
		{
			return true;
		}
		else if (value < *(node->value))
		{
			return __recursive_contains(node->left, value);
		}
		else
		{
			return __recursive_contains(node->right, value);
		}
	}

	template<typename T>
	inline qinternal::avl_node<T>* avl_tree<T>::__recursive_erase(qinternal::avl_node<T>* node, const T & key)
	{
		if (node == nullptr)
		{
			return node;
		}
		else if (key < *(node->value))
		{
			node->left = __recursive_erase(node->left, key);
		}
		else if (key > *(node->value))
		{
			node->right = __recursive_erase(node->right, key);
		} 
		else
		{
			if ((node->left == nullptr) || (node->right == nullptr))
			{
				auto tmp = node->left != nullptr ? node->left : node->right;
				if (tmp == nullptr)
				{
					tmp = node;
					delete node->value;
					node = nullptr;
				}
				else
				{
					delete node->value;
					*node = *tmp;
				}
				
				delete tmp;
			}
			else
			{
				auto tmp = __get_min_node(node->right);
				delete node->value;
				node->value = tmp->value;
				delete tmp;
			}
			node->height = __get_height(node);
			__size--;
		}

		if (node == nullptr)
		{
			return node;
		}

		node->height = __get_height(node);
		
		auto bal = __get_balance_factor(node);
		if (bal > 1 && __get_balance_factor(node->left) >= 0)
		{
			return __rotate_right(node);
		}
		else if (bal > 1 && __get_balance_factor(node->left) < 0)
		{
			node->left = __rotate_left(node->left);
			return __rotate_right(node);
		}
		else if (bal < -1 && __get_balance_factor(node->right) <= 0)
		{
			return __rotate_left(node);
		}
		else if (bal < -1 && __get_balance_factor(node->right) > 0)
		{
			node->right = __rotate_right(node->right);
			return __rotate_left(node);
		}

		return node;
	}

	template<typename T>
	inline qinternal::avl_node<T>* avl_tree<T>::__recursive_erase(qinternal::avl_node<T>* node, T && key)
	{
		if (node == nullptr)
		{
			return node;
		}
		else if (key < *(node->value))
		{
			node->left = __recursive_erase(node->left, key);
		}
		else if (key > *(node->value))
		{
			node->right = __recursive_erase(node->right, key);
		}
		else
		{
			if ((node->left == nullptr) || (node->right == nullptr))
			{
				auto tmp = node->left != nullptr ? node->left : node->right;
				if (tmp == nullptr)
				{
					tmp = node;
					delete node->value;
					node = nullptr;
				}
				else
				{
					delete node->value;
					*node = *tmp;
				}

				delete tmp;
			}
			else
			{
				auto tmp = __get_min_node(node->right);
				delete node->value;
				node->value = tmp->value;
				delete tmp;
			}
			node->height = __get_height(node);
			__size--;
		}

		if (node == nullptr)
		{
			return node;
		}

		node->height = __get_height(node);

		auto bal = __get_balance_factor(node);
		if (bal > 1 && __get_balance_factor(node->left) >= 0)
		{
			return __rotate_right(node);
		}
		else if (bal > 1 && __get_balance_factor(node->left) < 0)
		{
			node->left = __rotate_left(node->left);
			return __rotate_right(node);
		}
		else if (bal < -1 && __get_balance_factor(node->right) <= 0)
		{
			return __rotate_left(node);
		}
		else if (bal < -1 && __get_balance_factor(node->right) > 0)
		{
			node->right = __rotate_right(node->right);
			return __rotate_left(node);
		}

		return node;
	}

	template<typename T>
	inline int64_t avl_tree<T>::__get_balance_factor(qinternal::avl_node<T>* node) const
	{
		int64_t left_height = static_cast<int64_t>(__get_height(node->left));
		int64_t right_height = static_cast<int64_t>(__get_height(node->right));
		int64_t diff = left_height - right_height;

		return diff;
	}

	template<typename T>
	inline std::size_t avl_tree<T>::__get_height(qinternal::avl_node<T>* node) const noexcept
	{
		if (node)
		{
			int64_t left_height = static_cast<int64_t>(__get_height(node->left));
			int64_t right_height = static_cast<int64_t>(__get_height(node->right));
			return 1 + max(left_height, right_height);
		}
		return 0;
	}

	template<typename T>
	inline qinternal::avl_node<T>* avl_tree<T>::__get_min_node(qinternal::avl_node<T>* node)
	{
		auto current = node;
		while (current->left != nullptr)
		{
			current = current->left;
		}
		return current;
	}

	template<typename T>
	inline qinternal::avl_node<T>* avl_tree<T>::__rotate_left(qinternal::avl_node<T>* x)
	{
		auto y = x->right;
		auto t = y->left;

		y->left = x;
		x->right = t;

		x->height = __get_height(x);
		y->height = __get_height(y);

		return y;
	}

	template<typename T>
	inline qinternal::avl_node<T>* avl_tree<T>::__rotate_right(qinternal::avl_node<T>* y)
	{
		auto x = y->left;
		auto t = x->right;
		x->right = y;
		y->left = t;
		y->height = __get_height(y);
		x->height = __get_height(x);

		return x;
	}

	template<typename T>
	inline bool avl_tree<T>::__identical(qinternal::avl_node<T>* a, qinternal::avl_node<T>* b) const
	{
		if (a == nullptr && b == nullptr)
		{
			return true;
		}
		if (a != nullptr && b != nullptr)
		{
			return *(a->value) == *(b->value) && __identical(a->left, b->left) && __identical(a->right, b->right);
		}
		return false;
	}

	template<typename T>
	inline bool avl_tree<T>::__not_identical(qinternal::avl_node<T>* a, qinternal::avl_node<T>* b) const
	{
		if (a == nullptr && b == nullptr)
		{
			return false;
		}
		if (a != nullptr && b != nullptr)
		{
			return *(a->value) != *(b->value) || __not_identical(a->left, b->left) || __not_identical(a->right, b->right);
		}
		return true;
	}

} // end qtl

#endif