#ifndef stack_h__
#define stack_h__

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <stdlib.h>

#include "utility.h"

namespace qtl
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	namespace qinternal
	{
		template <typename T>
		struct stack_node
		{
			T* value;
			stack_node<T>* next;
			stack_node<T>* prev;
		};
	}
#endif

	/// <summary>
	/// LIFO data structure
	/// </summary>
	/// <typeparam name="T">
	/// Type stored
	/// </typeparam>
	template <typename T>
	class stack
	{
	public:
		/// <summary>
		/// Constructs an empty stack.  Complexity O(1)
		/// </summary>
		stack();

		/// <summary>
		/// Constructs a copy of a stack.  Complexity O(n)
		/// </summary>
		/// <param name="q">
		/// Queue to copy from
		/// </param>
		explicit stack(const stack& q);

		/// <summary>
		/// Move constructor for stack.  Complexity O(1)
		/// </summary>
		/// <param name="q">
		/// Queue to move from
		/// </param>
		explicit stack(stack&& q);

		/// <summary>
		/// Deconstructs stack and deallocates owned objects.  Complexity O(1)
		/// </summary>
		~stack();

		/// <summary>
		/// Copies a stack from another stack.  Complexity O(n)
		/// </summary>
		/// <param name="q">
		/// Queue to copy from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		stack& operator=(const stack& q);

		/// <summary>
		/// Moves a stack from another stack.  Complexity O(1)
		/// </summary>
		/// <param name="q">
		/// Queue to move from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		stack& operator=(stack&& q);

		/// <summary>
		/// Checks for equality between stacks.  Complexity O(n)
		/// </summary>
		/// <param name="other">
		/// Stack to check equality against
		/// </param>
		/// <returns>
		/// Return true if equal, else false.
		/// </returns>
		bool operator==(const stack& other) const;

		/// <summary>
		/// Checks for inequality between stacks.  Complexity O(n)
		/// </summary>
		/// <param name="other">
		/// Stack to check inequality against
		/// </param>
		/// <returns>
		/// Return true if inequal, else false.
		/// </returns>
		bool operator!=(const stack& other) const;

		/// <summary>
		/// Checks if stack is empty.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// True if empty, else false
		/// </returns>
		bool empty() const noexcept;

		/// <summary>
		/// Gets the number of elements in the stack.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// Number of elements in stack
		/// </returns>
		std::size_t size() const noexcept;

		/// <summary>
		/// Pushes an element onto the stack.  Complexity O(1)
		/// </summary>
		/// <param name="value">
		/// Value to push onto stack
		/// </param>
		void push(const T& value);

		/// <summary>
		/// Pushes an element onto the stack.  Complexity O(1)
		/// </summary>
		/// <param name="value">
		/// Value to push onto stack
		/// </param>
		void push(T&& value);

		/// <summary>
		/// Pops an element from the top of the stack.  Complexity O(1)
		/// </summary>
		void pop();

		/// <summary>
		/// Gets a reference to the top element in the stack.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// Reference to top element in stack
		/// </returns>
		T& top();

		/// <summary>
		/// Gets a reference to the top element in the stack.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// Reference to top element in stack
		/// </returns>
		const T& top() const;
	private:
		std::size_t __size;
		qinternal::stack_node<T>* __head;
		qinternal::stack_node<T>* __tail;

		void __link_nodes(qinternal::stack_node<T>* left, qinternal::stack_node<T>* right);
		void __clear();
	};

	template<typename T>
	inline stack<T>::stack()
	{
		__size = 0;
		__head = new qinternal::stack_node<T>;
		__tail = new qinternal::stack_node<T>;
		__link_nodes(__head, __tail);
	}

	template<typename T>
	inline stack<T>::stack(const stack<T>& q)
	{
		__head = new qinternal::stack_node<T>;
		__tail = new qinternal::stack_node<T>;
		__size = 0;
		__link_nodes(__head, __tail);
		auto node = q.__head->next;
		while (node != q.__tail)
		{
			push(*node->value);
			node = node->next;
		}
	}

	template<typename T>
	inline stack<T>::stack(stack<T>&& q)
	{
		__head = q.__head;
		__tail = q.__tail;
		__size = q.__size;
		q.__head = nullptr;
		q.__tail = nullptr;
		q.__size = 0;
	}

	template<typename T>
	inline stack<T>::~stack()
	{
		__clear();
		if (__head) delete __head;
		if (__tail) delete __tail;
	}

	template<typename T>
	inline stack<T>& stack<T>::operator=(const stack<T>& q)
	{
		__clear();
		__size = 0;
		__link_nodes(__head, __tail);
		auto node = q.__head->next;
		while (node != q.__tail)
		{
			push(*node->value);
			node = node->next;
		}
	}

	template<typename T>
	inline stack<T>& stack<T>::operator=(stack<T>&& q)
	{
		__clear();
		if (__head) delete __head;
		if (__tail) delete __tail;

		__head = q.__head;
		__tail = q.__tail;
		__size = q.__size;
		q.__head = nullptr;
		q.__tail = nullptr;
		q.__size = 0;
	}

	template<typename T>
	inline bool stack<T>::operator==(const stack<T>& other) const
	{
		if (__size != other.__size) return false;
		if (__size == 0 && other.__size == 0) return true;
		auto my_front = __head->next;
		auto ot_front = other.__head->next;
		while (my_front != __tail && ot_front != other.__tail)
		{
			if (*(my_front->value) != *(ot_front->value))
			{
				return false;
			}
			my_front = my_front->next;
			ot_front = ot_front->next;
		}
		return true;
	}

	template<typename T>
	inline bool stack<T>::operator!=(const stack<T>& other) const
	{
		if (__size != other.__size) return true;
		if (__size == 0 && other.__size == 0) return false;
		auto my_front = __head->next;
		auto ot_front = other.__head->next;
		while (my_front != __tail && ot_front != other.__tail)
		{
			if (*(my_front->value) != *(ot_front->value))
			{
				return true;
			}
			my_front = my_front->next;
			ot_front = ot_front->next;
		}
		return false;
	}

	template<typename T>
	inline bool stack<T>::empty() const noexcept
	{
		return __size == 0;
	}

	template<typename T>
	inline std::size_t stack<T>::size() const noexcept
	{
		return __size;
	}

	template<typename T>
	inline void stack<T>::push(const T & value)
	{
		qinternal::stack_node<T> * node = new qinternal::stack_node<T>;
		node->value = new T(value);
		__link_nodes(__tail->prev, node);
		__link_nodes(node, __tail);
		__size++;
	}

	template<typename T>
	inline void stack<T>::push(T && value)
	{
		qinternal::stack_node<T> * node = new qinternal::stack_node<T>;
		node->value = new T(value);
		__link_nodes(__tail->prev, node);
		__link_nodes(node, __tail);
		__size++;
	}

	template<typename T>
	inline void stack<T>::pop()
	{
#ifdef _DEBUG
		assert(__size > 0);
#endif
		auto node = __tail->prev;
		delete node->value;
		__link_nodes(node->prev, node->next);
		delete node;
		__size--;
	}

	template<typename T>
	inline T & stack<T>::top()
	{
#ifdef _DEBUG
		assert(__size > 0);
#endif
		return *(__tail->prev->value);
	}

	template<typename T>
	inline const T & stack<T>::top() const
	{
#ifdef _DEBUG
		assert(__size > 0);
#endif
		return *(__tail->prev->value);
	}

	template<typename T>
	inline void stack<T>::__link_nodes(qinternal::stack_node<T>* left, qinternal::stack_node<T>* right)
	{
		left->next = right;
		right->prev = left;
	}

	template<typename T>
	inline void stack<T>::__clear()
	{
		if (__head && __tail)
		{
			while (!empty())
			{
				pop();
			}
#ifdef _DEBUG
			assert(__size == 0);
#endif
		}
	}
}

#endif
