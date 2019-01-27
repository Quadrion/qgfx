#ifndef redblacktree_h__
#define redblacktree_h__

#include "qtl/list.h"
#include "qtl/utility.h"

#include <cassert>
#include <cstdint>

namespace qtl 
{
	template <typename T>
	class redblack_tree;
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	namespace qinternal
	{
		enum COLOR 
		{ 
			RED, 
			BLACK 
		};

		template <typename T>
		struct rb_node
		{
			T* value;
			COLOR color;
			rb_node<T>* left;
			rb_node<T>* right;
			rb_node<T>* parent;
		private:
			template <typename Type> friend class qtl::redblack_tree;
			void __move_node_down(rb_node* node);
		};

		template<typename T>
		inline void rb_node<T>::__move_node_down(rb_node<T>* node)
		{
			if (parent)
			{
				if (this == parent->left)
				{
					parent->left = node;
				}
				else
				{
					parent->right = node;
				}
			}
			node->parent = parent;
			parent = node;
		}

		template <typename T>
		class rb_tree_forward_iterator
		{
		public:
			rb_tree_forward_iterator(rb_node<T>* node);
			rb_tree_forward_iterator& operator++();
			bool operator!=(const rb_tree_forward_iterator& it) const;
			bool operator==(const rb_tree_forward_iterator& it) const;
			T& operator*();
			const T& operator*() const;
			rb_node<T>* operator->();
			const rb_node<T>* operator->() const;
			rb_node<T>* node;
		};

		template<typename T>
		inline rb_tree_forward_iterator<T>::rb_tree_forward_iterator(rb_node<T>* node)
			: node(node)
		{
		}

		template<typename T>
		inline rb_tree_forward_iterator<T> & rb_tree_forward_iterator<T>::operator++()
		{
			if (node->right == nullptr)
			{
				while (node->parent != nullptr && node->parent->right == node)
				{
					node = node->parent;
				}
				node = node->parent;
			}
			else
			{
				node = node->right;
				while (node->left != nullptr)
				{
					node = node->left;
				}
			}

			return *this;
		}

		template<typename T>
		inline bool rb_tree_forward_iterator<T>::operator!=(const rb_tree_forward_iterator & it) const
		{
			return (it.node != node) ? true : (it.node == node && it.node == nullptr) ? false : *(node->value) != *(it.node->value);
		}

		template<typename T>
		inline bool rb_tree_forward_iterator<T>::operator==(const rb_tree_forward_iterator & it) const
		{
			return (it.node == node) && *(it.node->value) == *(node->value);
		}

		template<typename T>
		inline T& rb_tree_forward_iterator<T>::operator*()
		{
			return *(node->value);
		}
		template<typename T>
		inline const T & rb_tree_forward_iterator<T>::operator*() const
		{
			return *(node->value);
		}

		template<typename T>
		inline rb_node<T>* rb_tree_forward_iterator<T>::operator->()
		{
			return node;
		}

		template<typename T>
		inline const rb_node<T>* rb_tree_forward_iterator<T>::operator->() const
		{
			return node;
		}
	} // end qinternal
#endif
	/// <summary>
	/// Represents a red black tree.  Red black trees have O(1) insertions, but
	/// they have O(log(n)) other operations.  The max height of the tree is 2 * log(n)
	/// </summary>
	/// <typeparam name="T">
	/// Type of value stored
	/// </typeparam>
	template <typename T>
	class redblack_tree
	{
	public:
		template <typename Type>
		using forward_iterator = typename qinternal::rb_tree_forward_iterator<Type>;

		/// <summary>
		/// Creates an empty red black tree. Complexity O(1)
		/// </summary>
		redblack_tree();

		/// <summary>
		/// Creates a red black tree from another red black tree. Complexity O(n)
		/// </summary>
		/// <param name="tree">
		/// Tree to copy from
		/// </param>
		redblack_tree(const redblack_tree& tree);

		/// <summary>
		/// Creates a red black tree from another red black tree. Complexity O(1)
		/// </summary>
		/// <param name="tree">
		/// Tree to move from
		/// </param>
		explicit redblack_tree(redblack_tree&& tree) noexcept;
		
		/// <summary>
		/// Destructs a red black tree. Complexity O(n)
		/// </summary>
		~redblack_tree();

		/// <summary>
		/// Copies the values from another tree to this tree. Complexity O(n)
		/// </summary>
		/// <param name="tree">
		/// Tree to copy values from
		/// </param>
		/// <returns>
		/// Reference for this tree
		/// </returns>
		redblack_tree& operator=(const redblack_tree& tree);

		/// <summary>
		/// Moves the values from another tree to this tree. Complexity O(1)
		/// </summary>
		/// <param name="tree">
		/// Tree to move values from
		/// </param>
		/// <returns>
		/// Reference for this tree
		/// </returns>
		redblack_tree& operator=(redblack_tree&& tree) noexcept;

		/// <summary>
		/// Checks if two trees have both identical values and structure. Complexity O(n)
		/// </summary>
		/// <param name="tree">
		/// Tree to check equality against
		/// </param>
		/// <returns>
		/// True if trees have identical values and structures, else false
		/// </returns>
		bool operator==(const redblack_tree& tree) const;

		/// <summary>
		/// Checks if two trees do not have both identical values or structure. Complexity O(n)
		/// </summary>
		/// <param name="tree">
		/// Tree to check inequality against
		/// </param>
		/// <returns>
		/// True if trees do not have identical values or structures, else false
		/// </returns>
		bool operator!=(const redblack_tree& tree) const;

		/// <summary>
		/// Inserts a new element into the tree.  Complexity amoritized O(1)
		/// </summary>
		/// <param name="value">
		/// Value to store in tree
		/// </param>
		void insert(const T& value);

		/// <summary>
		/// Inserts a new element into the tree.  Complexity amoritized O(1)
		/// </summary>
		/// <param name="value">
		/// Value to store in tree
		/// </param>
		void insert(T&& value);

		/// <summary>
		/// Erases an element from the tree.  Complexity O(log(n))
		/// </summary>
		/// <param name="value">
		/// Value to erase from tree
		/// </param>
		void erase(const T& value);

		/// <summary>
		/// Erases an element from the tree.  Complexity O(log(n))
		/// </summary>
		/// <param name="value">
		/// Value to erase from tree
		/// </param>
		void erase(T&& value);

		/// <summary>
		/// Checks if a value is contained in the tree. Complexity O(log(n))
		/// </summary>
		/// <param name="value">
		/// Value to check if tree contains
		/// </param>
		/// <returns>
		/// True if value is contained in tree, else false
		/// </returns>
		bool contains(const T& value);

		/// <summary>
		/// Checks if a value is contained in the tree. Complexity O(log(n))
		/// </summary>
		/// <param name="value">
		/// Value to check if tree contains
		/// </param>
		/// <returns>
		/// True if value is contained in tree, else false
		/// </returns>
		bool contains(T&& value);

		/// <summary>
		/// Gets the number of elements in the tree. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Number of elements in tree
		/// </returns>
		std::size_t size() const noexcept;

		/// <summary>
		/// Clears the tree of its contents.  Complexity O(n)
		/// </summary>
		void clear();

		/// <summary>
		/// Gets an iterator to the element.  Complexity O(log(n))
		/// </summary>
		/// <param name="t">
		/// Element to search for
		/// </param>
		/// <returns>
		/// Iterator to element
		/// </returns>
		forward_iterator<T> find(const T& t);

		/// <summary>
		/// Gets an iterator to the element.  Complexity O(log(n))
		/// </summary>
		/// <param name="t">
		/// Element to search for
		/// </param>
		/// <returns>
		/// Iterator to element
		/// </returns>
		const forward_iterator<T> find(const T& t) const;

		/// <summary>
		/// Gets an iterator to the beginning element.  Complexity O(log(n))
		/// </summary>
		/// <returns>
		/// Iterator to first element
		/// </returns>
		forward_iterator<T> begin();

		/// <summary>
		/// Gets an iterator to the beginning element.  Complexity O(log(n))
		/// </summary>
		/// <returns>
		/// Iterator to first element
		/// </returns>
		const forward_iterator<T> begin() const;


		/// <summary>
		/// Gets an iterator to the ending element.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// Iterator to last element
		/// </returns>
		forward_iterator<T> end();

		/// <summary>
		/// Gets an iterator to the ending element.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// Iterator to last element
		/// </returns>
		const forward_iterator<T> end() const;
	private:
		qinternal::rb_node<T>* __root;
		std::size_t __size;

		void __recursive_delete(qinternal::rb_node<T>* node);
		qinternal::rb_node<T>* __node_search(const T& value) const;
		void __node_erase(qinternal::rb_node<T>* node);
		void __fix_red_red_violation(qinternal::rb_node<T>* node);
		void __fix_black_black_violation(qinternal::rb_node<T>* node);
		void __rotate_left(qinternal::rb_node<T>* node);
		void __rotate_right(qinternal::rb_node<T>* node);
		void __recursive_copy(qinternal::rb_node<T>* node);
		bool __identical(qinternal::rb_node<T>* a, qinternal::rb_node<T>* b) const;
		bool __not_identical(qinternal::rb_node<T>* a, qinternal::rb_node<T>* b) const;
		void __get_contents(qinternal::rb_node<T>* a, list<T>& list) const;
		qinternal::rb_node<T>* __node_successor(qinternal::rb_node<T>* node) const;
		qinternal::rb_node<T>* __node_replace(qinternal::rb_node<T>* node);
	};

	template<typename T>
	inline redblack_tree<T>::redblack_tree()
	{
		__root = nullptr;
		__size = 0;
	}

	template<typename T>
	inline redblack_tree<T>::redblack_tree(const redblack_tree<T>& tree)
	{
		__root = nullptr;
		__size = 0;
		__recursive_copy(tree.__root);
	}

	template<typename T>
	inline redblack_tree<T>::redblack_tree(redblack_tree<T>&& tree) noexcept
	{
		__root = tree.__root;
		__size = tree.__size;

		tree.__root = nullptr;
		tree.__size = 0;
	}

	template<typename T>
	inline redblack_tree<T>::~redblack_tree()
	{
		__recursive_delete(__root);
	}

	template<typename T>
	inline redblack_tree<T>& redblack_tree<T>::operator=(const redblack_tree<T>& tree)
	{
		__recursive_copy(tree.__root);
	}

	template<typename T>
	inline redblack_tree<T>& redblack_tree<T>::operator=(redblack_tree<T>&& tree) noexcept
	{
		__root = tree.__root;
		__size = tree.__size;

		tree.__root = nullptr;
		tree.__size = 0;
	}

	template<typename T>
	inline bool redblack_tree<T>::operator==(const redblack_tree<T>& tree) const
	{
		return __identical(__root, tree.__root);
	}

	template<typename T>
	inline bool redblack_tree<T>::operator!=(const redblack_tree<T>& tree) const
	{
		return __not_identical(__root, tree.__root);
	}

	template<typename T>
	inline void redblack_tree<T>::insert(const T & value)
	{
		using qinternal::BLACK;
		using qinternal::RED;

		auto ptr = new qinternal::rb_node<T>();
		ptr->value = new T(value);
		ptr->left = nullptr;
		ptr->right = nullptr;
		ptr->parent = nullptr;
		

		if (!__root)
		{
			ptr->color = BLACK;
			__root = ptr;
		}
		else
		{
			auto tmp = __node_search(value);
			ptr->parent = tmp;

			if (value < *(tmp->value))
			{
				tmp->left = ptr;
			}
			else
			{
				tmp->right = ptr;
			}

			__fix_red_red_violation(ptr);
		}

		__size++;
	}

	template<typename T>
	inline void redblack_tree<T>::insert(T && value)
	{
		using qinternal::BLACK;
		using qinternal::RED;

		auto ptr = new qinternal::rb_node<T>();
		ptr->value = new T(qtl::move(value));
		ptr->left = nullptr;
		ptr->right = nullptr;
		ptr->parent = nullptr;


		if (!__root)
		{
			ptr->color = BLACK;
			__root = ptr;
		}
		else
		{
			auto tmp = __node_search(value);
			ptr->parent = tmp;

			if (value < *(tmp->value))
			{
				tmp->left = ptr;
			}
			else
			{
				tmp->right = ptr;
			}

			__fix_red_red_violation(ptr);
		}

		__size++;
	}

	template<typename T>
	inline void redblack_tree<T>::erase(const T & value)
	{
		if (__root)
		{
			auto node = __node_search(value);
			if (node && *(node->value) == value)
			{
				__node_erase(node);
				__size--;
			}
		}
	}

	template<typename T>
	inline void redblack_tree<T>::erase(T && value)
	{
		if (__root)
		{
			const T val = qtl::move(value);
			auto node = __node_search(val);
			if (node && *(node->value) == val)
			{
				__node_erase(node);
				__size--;
			}
		}
	}

	template<typename T>
	inline bool redblack_tree<T>::contains(const T & value)
	{
		auto tmp = __root;
		while (tmp)
		{
			if (*(tmp->value) == value)
			{
				return true;
			}
			else if (value < *(tmp->value))
			{
				tmp = tmp->left;
			}
			else
			{
				tmp = tmp->right;
			}
		}
		return false;
	}

	template<typename T>
	inline bool redblack_tree<T>::contains(T && value)
	{
		auto tmp = __root;
		while (tmp)
		{
			const T& val = qtl::move(value);
			if (*(tmp->value) == val)
			{
				return true;
			}
			else if (val < *(tmp->value))
			{
				tmp = tmp->left;
			}
			else
			{
				tmp = tmp->right;
			}
		}
		return false;
	}

	template<typename T>
	inline std::size_t redblack_tree<T>::size() const noexcept
	{
		return __size;
	}

	template<typename T>
	inline void redblack_tree<T>::clear()
	{
		__recursive_delete(__root);
	}

	template<typename T>
	inline typename redblack_tree<T>::template forward_iterator<T> redblack_tree<T>::find(const T & t)
	{
		qinternal::rb_node<T> * it = __root;
		while (it != nullptr)
		{
			if (*it->value == t)
			{
				return it;
			}
			if (t < *it->value)
			{
				it = it->left;
			}
			else
			{
				it = it->right;
			}
		}
		return nullptr;
	}

	template<typename T>
	inline const typename redblack_tree<T>::template forward_iterator<T> redblack_tree<T>::find(const T & t) const
	{
		qinternal::rb_node<T> * it = __root;
		while (it != nullptr)
		{
			if (*it->value == t)
			{
				return it;
			}
			if (t < *it->value)
			{
				it = it->left;
			}
			else
			{
				it = it->right;
			}
		}
		return nullptr;
	}

	template<typename T>
	inline typename redblack_tree<T>::template forward_iterator<T> redblack_tree<T>::begin()
	{
		auto it = __root;
		while (it->left != nullptr)
		{
			it = it->left;
		}
		return forward_iterator<T>(it);
	}

	template<typename T>
	inline const typename redblack_tree<T>::template forward_iterator<T> redblack_tree<T>::begin() const
	{
		auto it = __root;
		while (it->left != nullptr)
		{
			it = it->left;
		}
		return forward_iterator<T>(it);
	}

	template<typename T>
	inline typename redblack_tree<T>::template forward_iterator<T> redblack_tree<T>::end()
	{
		return forward_iterator<T>(nullptr);
	}

	template<typename T>
	inline const typename redblack_tree<T>::template forward_iterator<T> redblack_tree<T>::end() const
	{
		return forward_iterator<T>(nullptr);
	}

	template<typename T>
	inline void redblack_tree<T>::__recursive_delete(qinternal::rb_node<T>* node)
	{
		if (node)
		{
			__recursive_delete(node->left);
			__recursive_delete(node->right);
			delete node->value;
			delete node;
		}
	}

	template<typename T>
	inline qinternal::rb_node<T>* redblack_tree<T>::__node_search(const T & value) const
	{
		auto tmp = __root;
		while (tmp)
		{
			if (value < *(tmp->value))
			{
				if (tmp->left)
				{
					tmp = tmp->left;
				}
				else
				{
					break;
				}
			}
			else if (value == *(tmp->value))
			{
				break;
			}
			else
			{
				if (tmp->right)
				{
					tmp = tmp->right;
				}
				else
				{
					break;
				}
			}
		}
		return tmp;
	}

	template<typename T>
	inline void redblack_tree<T>::__node_erase(qinternal::rb_node<T>* node)
	{
		using qinternal::BLACK;
		using qinternal::RED;

		auto u = __node_replace(node);
		bool blk = node->color == BLACK && u->color == BLACK;
		auto parent = node->parent;

		if (!u)
		{
			if (node == __root)
			{
				__root = nullptr;
			}
			else
			{
				if (blk)
				{
					__fix_black_black_violation(node);
				}
				else
				{
					auto sibling = node->parent->left == node ? node->parent->right : node->parent->left;
					if (sibling)
					{
						sibling->color = RED;
					}
				}
				if (node == node->parent->left)
				{
					parent->left = nullptr;
				}
				else
				{
					parent->right = nullptr;
				}
			}
			delete node->value;
			delete node;
			return;
		}

		if (!node->left || !node->right)
		{
			if (node == __root)
			{
				delete node->value;
				node->value = u->value;
				node->left = node->right = nullptr;
				delete u;
			}
			else
			{
				if (node == parent->left)
				{
					parent->left = u;
				}
				else
				{
					parent->right = u;
				}

				delete node->value;
				delete node;

				u->parent = parent;
				if (blk)
				{
					__fix_black_black_violation(u);
				}
				else
				{
					u->color = BLACK;
				}
			}
			return;
		}

		T* tmp = u->value;
		u->value = node->value;
		node->value = tmp;

		__node_erase(u);
	}

	template<typename T>
	inline void redblack_tree<T>::__fix_red_red_violation(qinternal::rb_node<T>* node)
	{
		using qinternal::BLACK;
		using qinternal::RED; 

		if (node == __root)
		{
			node->color = BLACK;
			return;
		}

		auto parent = node->parent;
		if (parent)
		{
			auto gparent = parent->parent;
			auto uncle = (parent == nullptr || gparent == nullptr) ? nullptr : ((parent == gparent->left) ? gparent->right : gparent->left);
			if (parent->color != BLACK)
			{
				if (uncle && uncle->color == RED)
				{
					parent->color = BLACK;
					uncle->color = BLACK;
					gparent->color = RED;
					__fix_red_red_violation(gparent);
				}
				else
				{
					if (gparent && parent == gparent->left)
					{
						if (node == parent->left)
						{
							auto col = parent->color;
							parent->color = gparent->color;
							gparent->color = col;
						}
						else
						{
							__rotate_left(parent);
							auto col = node->color;
							node->color = gparent->color;
							gparent->color = col;
						}
						__rotate_right(gparent);
					}
					else if (gparent)
					{
						if (node == parent->left)
						{
							__rotate_right(parent);
							auto col = node->color;
							node->color = gparent->color;
							gparent->color = col;
						}
						else
						{
							auto col = parent->color;
							parent->color = gparent->color;
							gparent->color = col;
						}
						__rotate_left(gparent);
					}
				}
			}
		}
	}

	template<typename T>
	inline void redblack_tree<T>::__fix_black_black_violation(qinternal::rb_node<T>* node)
	{
		using qinternal::RED;
		using qinternal::BLACK;

		if (node == __root)
		{
			return;
		}
		auto sibling = (node->parent == nullptr) ? nullptr : (node == node->parent->left ? node->parent->right : node->parent->left);
		auto parent = node->parent;
		if (!sibling)
		{
			__fix_black_black_violation(parent);
		}
		else
		{
			if (sibling->color == RED)
			{
				parent->color = RED;
				sibling->color = BLACK;
				if (sibling == parent->left)
				{
					__rotate_right(parent);
				}
				else
				{
					__rotate_left(parent);
				}
				__fix_black_black_violation(node);
			}
			else
			{
				if ((sibling->left && sibling->left->color == RED) || (sibling->right && sibling->right->color == RED))
				{
					if (sibling->left && sibling->right->color == RED) // left child is red
					{
						if (sibling == parent->left)
						{
							sibling->left->color = sibling->color;
							sibling->color = parent->color;
							__rotate_right(parent);
						}
						else
						{
							sibling->left->color = parent->color;
							__rotate_right(sibling);
							__rotate_left(parent);
						}
					}
					else
					{
						if (sibling == parent->left)
						{
							sibling->right->color = parent->color;
							__rotate_left(sibling);
							__rotate_right(parent);
						}
						else
						{
							sibling->right->color = sibling->color;
							sibling->color = parent->color;
							__rotate_left(parent);
						}
					}
					parent->color = BLACK;
				}
				else
				{
					sibling->color = RED;
					if (parent->color == BLACK)
					{
						__fix_black_black_violation(parent);
					}
					else
					{
						parent->color = BLACK;
					}
				}
			}
		}
	}

	template<typename T>
	inline void redblack_tree<T>::__rotate_left(qinternal::rb_node<T>* node)
	{
		auto new_parent = node->right;
		if (node == __root)
		{
			__root = new_parent;
		}
		node->__move_node_down(new_parent);
		node->right = new_parent->left;
		
		if (new_parent->left)
		{
			new_parent->left->parent = node;
		}

		new_parent->left = node;
	}

	template<typename T>
	inline void redblack_tree<T>::__rotate_right(qinternal::rb_node<T>* node)
	{
		auto new_parent = node->left;
		if (node == __root)
		{
			__root = new_parent;
		}
		node->__move_node_down(new_parent);
		node->left = new_parent->right;

		if (new_parent->right)
		{
			new_parent->right->parent = node;
		}

		new_parent->right = node;
	}

	template<typename T>
	inline void redblack_tree<T>::__recursive_copy(qinternal::rb_node<T>* node)
	{
		if (node)
		{
			insert(*(node->value));
			__recursive_copy(node->left);
			__recursive_copy(node->right);
		}
	}

	template<typename T>
	inline bool redblack_tree<T>::__identical(qinternal::rb_node<T>* a, qinternal::rb_node<T>* b) const
	{
		list<T> my_contents, other_contents;
		__get_contents(a, my_contents);
		__get_contents(b, other_contents);
		return my_contents == other_contents;
	}

	template<typename T>
	inline bool redblack_tree<T>::__not_identical(qinternal::rb_node<T>* a, qinternal::rb_node<T>* b) const
	{
		list<T> my_contents, other_contents;
		__get_contents(a, my_contents);
		__get_contents(b, other_contents);
		return my_contents != other_contents;
	}

	template<typename T>
	inline void redblack_tree<T>::__get_contents(qinternal::rb_node<T>* a, list<T>& list) const
	{
		if (a)
		{
			__get_contents(a->left, list);
			list.push_back(*(a->value));
			__get_contents(a->right, list);
		}
	}

	template<typename T>
	inline qinternal::rb_node<T>* redblack_tree<T>::__node_successor(qinternal::rb_node<T>* node) const
	{
		auto tmp = node;
		while (tmp->left)
		{
			tmp = tmp->left;
		}
		return tmp;
	}

	template<typename T>
	inline qinternal::rb_node<T>* redblack_tree<T>::__node_replace(qinternal::rb_node<T>* node)
	{
		if (node->left && node->right)
		{
			return __node_successor(node->right);
		}
		else if (!node->left && !node->right)
		{
			return nullptr;
		}
		return node->left ? node->left : node->right;
	}

} // end qtl

#endif