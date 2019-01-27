#ifndef list_h__
#define list_h__

#include "qtl/type_traits.h"
#include "qtl/utility.h"

#include <cassert>
#include <cstdint>
#include <stdlib.h>

namespace qtl {

	template <typename T>
	class list;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
	namespace qinternal
	{
		template <typename T>
		struct linked_node
		{
			linked_node<T>* next;
			linked_node<T>* prev;
			T* value;

			bool operator==(const linked_node<T>& other) const noexcept;
			bool operator!=(const linked_node<T>& other) const noexcept;
		};

		template <typename T>
		class forward_list_iterator
		{
		public:
			explicit forward_list_iterator(linked_node<T>* ptr);
			forward_list_iterator<T>& operator++();

			bool operator==(const forward_list_iterator<T>& other) const;
			bool operator!=(const forward_list_iterator<T>& other) const;
			T& operator*();
			const T& operator*() const;
		private:
			friend class qtl::list<T>;
			linked_node<T>* node;
		};

		template <typename T>
		inline bool linked_node<T>::operator==(const linked_node<T>& other) const noexcept
		{
			return (value == other.value) && (next == other.next) && (prev == other.prev);
		}

		template <typename T>
		inline bool linked_node<T>::operator!=(const linked_node<T>& other) const noexcept
		{
			return (value != other.value) || (next != other.next) || (prev != other.prev);
		}

		template<typename T>
		inline forward_list_iterator<T>::forward_list_iterator(linked_node<T>* ptr)
			: node(ptr)
		{	}

		template<typename T>
		inline forward_list_iterator<T>& forward_list_iterator<T>::operator++()
		{
			node = node->next;
			return *this;
		}

		template<typename T>
		inline bool forward_list_iterator<T>::operator==(const forward_list_iterator<T>& other) const
		{
			return node == other.node;
		}

		template<typename T>
		inline bool forward_list_iterator<T>::operator!=(const forward_list_iterator<T>& other) const
		{
			return node != other.node;
		}

		template<typename T>
		inline T & forward_list_iterator<T>::operator*()
		{
			return *(node->value);
		}

		template<typename T>
		inline const T & forward_list_iterator<T>::operator*() const
		{
			return *(node->value);
		}
	} // end of qinternal
#endif

	/// <summary>
	/// Linked list using doubly linked nodes
	/// </summary>
	/// <typeparam name="T">
	/// Type to store
	/// </typeparam>
	template <typename T>
	class list
	{
		/*
		 head <===> value 1 <===> value2 <===> value3 <===> tail
		*/
	public:
		template <typename Type>
		using forward_iterator = qinternal::forward_list_iterator<Type>;

		/// <summary>
		/// Creates a new list. Complexity O(1)
		/// </summary>
		list() noexcept;

		/// <summary>
		/// Creates a new list from an existing list. Complexity O(n)
		/// </summary>
		/// <param name="other">
		/// List to construct from
		/// </param>
		explicit list(const list& other);

		/// <summary>
		/// Deleted move constructor
		/// </summary>
		explicit list(list&&) = delete;

		/// <summary>
		/// Frees all relevant data. Complexity O(n)
		/// </summary>
		~list();

		/// <summary>
		/// Sets this list to have the same contents as the other list. Complexity O(n)
		/// </summary>
		/// <param name="other">
		/// Vector to copy from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		list& operator=(const list& other);

		/// <summary>
		/// Deleted move assignment operator
		/// </summary>
		list& operator=(list&&) = delete;

		/// <summary>
		/// Checks if two lists are equivalent. Complexity O(n)
		/// </summary>
		/// <param name="other">
		/// List to check against
		/// </param>
		/// <returns>
		/// True if equivalent, else false
		/// </param>
		bool operator==(const list& other) const;

		/// <summary>
		/// Checks if two lists are not equivalent. Complexity O(n)
		/// </summary>
		/// <param name="other">
		/// List to check against
		/// </param>
		/// <returns>
		/// True if not equivalent, else false
		/// </param>
		bool operator!=(const list& other) const;

		/// <summary>
		/// Gets the front element in the list.  Undefined if list is empty.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// Reference to first element
		/// </returns>
		T& front();

		/// <summary>
		/// Gets the front element in the list.  Undefined if list is empty.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// Reference to first element
		/// </returns>
		const T& front() const;

		/// <summary>
		/// Gets the back element in the list.  Undefined if list is empty.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// Reference to last element
		/// </returns>
		T& back();

		/// <summary>
		/// Gets the back element in the list.  Undefined if list is empty.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// Reference to last element
		/// </returns>
		const T& back() const;

		/// <summary>
		/// Pushes an element to the front of the list by reference. Complexity O(1)
		/// </summary>
		/// <param name="value">
		/// Value to add
		/// </param>
		void push_front(const T& value);

		/// <summary>
		/// Pushes an element to the front of the list by move. Complexity O(1)
		/// </summary>
		/// <param name="value">
		/// Value to add
		/// </param>
		void push_front(T&& value);

		/// <summary>
		/// Pushes an element to the front of the list by arguments. Complexity O(1)
		/// </summary>
		/// <typeparam name="Arguments">
		/// Type of arguments used in construction
		/// </typeparam>
		/// <param name="args">
		/// Arguments for construction
		/// </param>
		template <typename ... Arguments>
		void emplace_front(Arguments && ... args);

		/// <summary>
		/// Pushes an element to the back of the list by reference. Complexity O(1)
		/// </summary>
		/// <param name="value">
		/// Value to add
		/// </param>
		void push_back(const T& value);

		/// <summary>
		/// Pushes an element to the back of the list by move
		/// </summary>
		/// <param name="value">
		/// Value to add
		/// </param>
		void push_back(T&& value);

		/// <summary>
		/// Pushes an element to the back of the list by arguments. Complexity O(1)
		/// </summary>
		/// <typeparam name="Arguments">
		/// Type of arguments used in construction
		/// </typeparam>
		/// <param name="args">
		/// Arguments for construction
		/// </param>
		template <typename ... Arguments>
		void emplace_back(Arguments && ... args);
		
		/// <summary>
		/// Inserts a value at the given location by reference. Complexity O(1)
		/// </summary>
		/// <param name="it">
		/// Iterator representing position
		/// </param>
		/// <param name="value">
		/// Value to add
		/// </param>
		void insert(forward_iterator<T> & it, const T& value);

		/// <summary>
		/// Inserts a value at the given location by move. Complexity O(1)
		/// </summary>
		/// <param name="it">
		/// Iterator representing position
		/// </param>
		/// <param name="value">
		/// Value to add
		/// </param>
		void insert(forward_iterator<T> & it, T&& value);

		/// <summary>
		/// Inserts a value at the given location by construction. Complexity O(1)
		/// </summary>
		/// <typeparam name="Arguments">
		/// Types of arguments used in construction
		/// </typeparam>
		/// <param name="it">
		/// Iterator representing position
		/// </param>
		/// <param name="args">
		/// Arguments for construction
		/// </param>
		template <typename ... Arguments>
		void emplace(forward_iterator<T> & it, Arguments && ... args);

		/// <summary>
		/// Erases the value at the given iterator. Complexity O(1)
		/// </summary>
		/// <param name="it">
		/// Position to erase in list
		/// </param>
		void erase(forward_iterator<T> & it);

		/// <summary>
		/// Clears contents of vector. Complexity O(n)
		/// </summary>
		void clear();

		/// <summary>
		/// Checks if list is empty. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Returns true if list is empty, else false
		/// </returns>
		bool empty() const noexcept;

		/// <summary>
		/// Gets the size of the list. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Size of list
		/// </returns>
		std::size_t size() const noexcept;

		/// <summary>
		/// Gets an iterator to the beginning of the list. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Iterator pointing to beginning of list
		/// </returns>
		forward_iterator<T> begin();

		/// <summary>
		/// Gets an iterator to the end of the list. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Iterator pointing to end of list
		/// </returns>
		forward_iterator<T> end();

		/// <summary>
		/// Gets an iterator to the beginning of the list. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Iterator pointing to beginning of list
		/// </returns>
		forward_iterator<T> begin() const;

		/// <summary>
		/// Gets an iterator to the end of the list. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Iterator pointing to end of list
		/// </returns>
		forward_iterator<T> end() const;
	private:
		qinternal::linked_node<T>* __head; // sentinel node
		qinternal::linked_node<T>* __tail; // sentinel node
		std::size_t __size;

		bool __is_interior(const qinternal::linked_node<T>* node);

		// PRECONDITION: node_to_attach has been allocated
		void __insert_between(qinternal::linked_node<T>* node_to_attach, qinternal::linked_node<T>* front, qinternal::linked_node<T>* back);
		// POSTCONDITION: node_to_remove->value is not deallocated
		void __remove_node(qinternal::linked_node<T>* node_to_remove);
	};

	template <typename T>
	inline list<T>::list() noexcept
	{
		__head = new qinternal::linked_node<T>;
		__tail = new qinternal::linked_node<T>;
		__head->next = __tail;
		__head->prev = nullptr;
		__head->value = nullptr;
		__tail->next = nullptr;
		__tail->prev = __head;
		__tail->value = nullptr;
		__size = 0;
	}

	template<typename T>
	inline list<T>::list(const list<T>& other)
		: list<T>()
	{
		for (auto & it : other)
		{
			push_back(it);
		}
	}

	template<typename T>
	inline list<T>::~list()
	{
		clear();
		delete __head;
		delete __tail;
	}

	template<typename T>
	inline list<T>& list<T>::operator=(const list<T>& other)
	{
		clear();
		for (auto & it : other)
		{
			push_back(it);
		}
	}

	template<typename T>
	inline bool list<T>::operator==(const list<T>& other) const
	{
		if (__size == other.__size)
		{
			auto this_it = begin();
			auto that_it = other.begin();
			while (this_it != end() && that_it != other.end())
			{
				if (*this_it != *that_it)
				{
					return false;
				}
				++this_it;
				++that_it;
			}
			return true;
		}
		return false;
	}

	template<typename T>
	inline bool list<T>::operator!=(const list<T>& other) const
	{
		if (__size == other.__size)
		{
			auto this_it = begin();
			auto that_it = other.begin();
			while (this_it != end() && that_it != other.end())
			{
				if (*this_it == *that_it)
				{
					return false;
				}
				++this_it;
				++that_it;
			}
			return false;
		}
		return true;
	}

	template<typename T>
	inline T & list<T>::front()
	{
		return *(__head->next->value);
	}

	template<typename T>
	inline const T & list<T>::front() const
	{
		return *(__head->next->value);
	}

	template<typename T>
	inline T & list<T>::back()
	{
		return *(__tail->prev->value);
	}

	template<typename T>
	inline const T & list<T>::back() const
	{
		return *(__tail->prev->value);
	}

	template<typename T>
	inline void list<T>::push_front(const T & value)
	{
		qinternal::linked_node<T> * node = new qinternal::linked_node<T>;
		node->value = new T(value);
		__insert_between(node, __head, __head->next);
		__size++;
	}

	template<typename T>
	inline void list<T>::push_front(T && value)
	{
		qinternal::linked_node<T> * node = new qinternal::linked_node<T>;
		node->value = new T(value);
		__insert_between(node, __head, __head->next);
		__size++;
	}

	template<typename T>
	template<typename ...Arguments>
	inline void list<T>::emplace_front(Arguments && ...args)
	{
		qinternal::linked_node<T> * node = new qinternal::linked_node<T>;
		node->value = new T(qtl::forward<Arguments>(args)...);
		__insert_between(node, __head, __head->next);
		__size++;
	}

	template<typename T>
	inline void list<T>::push_back(const T & value)
	{
		qinternal::linked_node<T> * node = new qinternal::linked_node<T>;
		node->value = new T(value);
		__insert_between(node, __tail->prev, __tail);
		__size++;
	}

	template<typename T>
	inline void list<T>::push_back(T && value)
	{
		qinternal::linked_node<T> * node = new qinternal::linked_node<T>;
		node->value = new T(value);
		__insert_between(node, __tail->prev, __tail);
		__size++;
	}

	template<typename T>
	template<typename ...Arguments>
	inline void list<T>::emplace_back(Arguments && ...args)
	{
		qinternal::linked_node<T> * node = new qinternal::linked_node<T>;
		node->value = new T(qtl::forward<Arguments>(args)...);
		__insert_between(node, __tail->prev, __tail);
		__size++;
	}

	template<typename T>
	inline void list<T>::insert(forward_iterator<T>& it, const T & value)
	{
		auto node = it.node;
		__insert_between(new T(value), node->prev, node);
		__size++;
	}

	template<typename T>
	inline void list<T>::insert(forward_iterator<T>& it, T && value)
	{
		auto node = it.node;
		__insert_between(new T(value), node->prev, node);
		__size++;
	}

	template<typename T>
	template<typename ...Arguments>
	inline void list<T>::emplace(forward_iterator<T>& it, Arguments && ...args)
	{
		auto node = it.node;
		__insert_between(new T(qtl::forward<Arguments>(args)...), node->prev, node);
		__size++;
	}

	template<typename T>
	inline void list<T>::erase(forward_iterator<T>& it)
	{
#ifdef _DEBUG
		assert(__is_interior(it.node));
#endif
		__remove_node(it.node);
		delete it.node->value;
		delete it.node;
		__size--;
	}

	template<typename T>
	inline void list<T>::clear()
	{
		using qinternal::linked_node;
		linked_node<T>* node = __head->next;
		while (__is_interior(node))
		{
			delete node->value;
			auto tmp = node;
			node = node->next;
			delete tmp;
		}
		__head->next = __tail;
		__tail->prev = __head;
		__size = 0;
	}

	template<typename T>
	inline bool list<T>::empty() const noexcept
	{
		return !__size;
	}

	template<typename T>
	inline std::size_t list<T>::size() const noexcept
	{
		return __size;
	}

	template<typename T>
	inline typename list<T>::template forward_iterator<T> list<T>::begin()
	{
		return qinternal::forward_list_iterator<T>(__head->next);
	}

	template<typename T>
	inline typename list<T>::template forward_iterator<T> list<T>::end()
	{
		return qinternal::forward_list_iterator<T>(__tail);
	}

	template<typename T>
	inline typename list<T>::template forward_iterator<T> list<T>::begin() const
	{
		return qinternal::forward_list_iterator<T>(__head->next);
	}

	template<typename T>
	inline typename list<T>::template forward_iterator<T> list<T>::end() const
	{
		return qinternal::forward_list_iterator<T>(__tail);
	}

	template<typename T>
	inline bool list<T>::__is_interior(const qinternal::linked_node<T>* node)
	{
		return (node->prev) && (node->next);
	}

	template<typename T>
	inline void list<T>::__insert_between(qinternal::linked_node<T>* node_to_attach, qinternal::linked_node<T>* front, qinternal::linked_node<T>* back)
	{
		/*
			BEFORE: front <===> back
			AFTER: front <===> node_to_attach <===> back
		*/
		node_to_attach->prev = front;
		front->next = node_to_attach;

		node_to_attach->next = back;
		back->prev = node_to_attach;
	}

	template<typename T>
	inline void list<T>::__remove_node(qinternal::linked_node<T>* node_to_remove)
	{
		/*
			BEFORE: front <===> node_to_attach <===> back
			AFTER: front <===> back
		*/
		node_to_remove->next->prev = node_to_remove->prev;
		node_to_remove->prev->next = node_to_remove->next;
#ifdef _DEBUG
		node_to_remove->prev = nullptr;
		node_to_remove->next = nullptr;
#endif
	}

} // end of qtl

#endif