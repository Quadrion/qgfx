#ifndef deque_h__
#define deque_h__

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <stdlib.h>

#include "qtl/utility.h"

namespace qtl
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	namespace qinternal
	{
		template <typename T>
		struct deque_node
		{
			T* value;
			deque_node<T>* next = nullptr;
			deque_node<T>* prev = nullptr;
		};
	} // end qinternal
#endif
	/// <summary>
	/// Represents a double ended queue with insertion and deletion logic
	/// supported on both ends of the structure
	/// </summary>
	/// <typeparam name="t">
	/// Type stored in deque
	/// </typeparam>
	template <typename T>
	class deque
	{
	public:
		/// <summary>
		/// Constructs an empty deque. Complexity O(1)
		/// </summary>
		deque();

		/// <summary>
		/// Constructs a copy of a pre-existing deque. Complexity O(n)
		/// </summary>
		/// <param name="q">
		/// Deque to construct from
		/// </param>
		explicit deque(const deque& q);

		/// <summary>
		/// Moves a pre-existing deque to this deque. Complexity O(1)
		/// </summary>
		/// <param name="q">
		/// Deque to move from
		/// </param>
		explicit deque(deque&& q);

		/// <summary>
		/// Destructs the deque. Complexity O(n)
		/// </summary>
		~deque();

		/// <summary>
		/// Constructs a copy of a pre-existing deque. Complexity O(n)
		/// </summary>
		/// <param name="q">
		/// Deque to construct from
		/// </param>
		/// <returns>
		/// Reference to this deque
		/// </returns>
		deque& operator=(const deque& q);

		/// <summary>
		/// Constructs a copy of a pre-existing deque. Complexity O(1)
		/// </summary>
		/// <param name="q">
		/// Deque to construct from
		/// </param>
		/// <returns>
		/// Reference to this deque
		/// </returns>
		deque& operator=(deque&& q);

		/// <summary>
		/// Checks equality between two deques. Complexity O(n)
		/// </summary>
		/// <param name="other">
		/// Deque to check against
		/// </param>
		/// <returns>
		/// True if equivalent, else false
		/// </returns>
		bool operator==(const deque& other) const;

		/// <summary>
		/// Checks inequality between two deques. Complexity O(n)
		/// </summary>
		/// <param name="other">
		/// Deque to check against
		/// </param>
		/// <returns>
		/// True if not equivalent, else false
		/// </returns>
		bool operator!=(const deque& other) const;

		/// <summary>
		/// Checks if a deque is empty. Complexity O(1)
		/// </summary>
		/// <returns>
		/// True is empty, else false
		/// </returns>
		bool empty() const noexcept;
		
		/// <summary>
		/// Gets the number of elements in the deque. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Size of deque
		/// </returns>
		std::size_t size() const noexcept;

		/// <summary>
		/// Pushes an element to the back of the deque. Complexity O(1)
		/// </summary>
		/// <param name="value">
		/// Value to add
		/// </param>
		void push_back(const T& value);

		/// <summary>
		/// Pushes an element to the back of the deque. Complexity O(1)
		/// </summary>
		/// <param name="value">
		/// Value to add
		/// </param>
		void push_back(T&& value);

		/// <summary>
		/// Pushes an element to the front of the deque. Complexity O(1)
		/// </summary>
		/// <param name="value">
		/// Value to add
		/// </param>
		void push_front(const T& value);

		/// <summary>
		/// Pushes an element to the front of the deque. Complexity O(1)
		/// </summary>
		/// <param name="value">
		/// Value to add
		/// </param>
		void push_front(T&& value);

		/// <summary>
		/// Pops an element from the back of the deque. Complexity O(1)
		/// </summary>
		void pop_back();

		/// <summary>
		/// Pops an element from the front of the deque. Complexity O(1)
		/// </summary>
		void pop_front();

		/// <summary>
		/// Gets the front element in the deque. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Reference to front element
		/// </returns>
		T& front();

		/// <summary>
		/// Gets the front element in the deque. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Reference to front element
		/// </returns>
		const T& front() const;

		/// <summary>
		/// Gets the back element in the deque. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Reference to back element
		/// </returns>
		T& back();

		/// <summary>
		/// Gets the back element in the deque. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Reference to back element
		/// </returns>
		const T& back() const;
	private:
		std::size_t __size;
		qinternal::deque_node<T>* __head;
		qinternal::deque_node<T>* __tail;

		void __link_nodes(qinternal::deque_node<T>* left, qinternal::deque_node<T>* right);
		void __clear();
	};

	template<typename T>
	inline deque<T>::deque()
	{
		__size = 0;
		__head = new qinternal::deque_node<T>;
		__tail = new qinternal::deque_node<T>;
		__link_nodes(__head, __tail);
	}

	template<typename T>
	inline deque<T>::deque(const deque<T>& q)
	{
		__head = new qinternal::deque_node<T>;
		__tail = new qinternal::deque_node<T>;
		__size = 0;
		__link_nodes(__head, __tail);
		auto node = q.__head->next;
		while (node != q.__tail)
		{
			push_back(*node->value);
			node = node->next;
		}
	}

	template<typename T>
	inline deque<T>::deque(deque<T>&& q)
	{
		__head = q.__head;
		__tail = q.__tail;
		__size = q.__size;
		q.__head = nullptr;
		q.__tail = nullptr;
		q.__size = 0;
	}

	template<typename T>
	inline deque<T>::~deque()
	{
		__clear();
		if (__head) delete __head;
		if (__tail) delete __tail;
	}

	template<typename T>
	inline deque<T>& deque<T>::operator=(const deque<T>& q)
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
	inline deque<T>& deque<T>::operator=(deque<T>&& q)
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
	inline bool deque<T>::operator==(const deque<T>& other) const
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
	inline bool deque<T>::operator!=(const deque<T>& other) const
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
	inline bool deque<T>::empty() const noexcept
	{
		return __size == 0;
	}

	template<typename T>
	inline std::size_t deque<T>::size() const noexcept
	{
		return __size;
	}

	template<typename T>
	inline void deque<T>::push_back(const T & value)
	{
		qinternal::deque_node<T> * node = new qinternal::deque_node<T>;
		node->value = new T(value);
		__link_nodes(__tail->prev, node);
		__link_nodes(node, __tail);
		__size++;
	}

	template<typename T>
	inline void deque<T>::push_back(T && value)
	{
		qinternal::deque_node<T> * node = new qinternal::deque_node<T>;
		node->value = new T(value);
		__link_nodes(__tail->prev, node);
		__link_nodes(node, __tail);
		__size++;
	}

	template<typename T>
	inline void deque<T>::push_front(const T & value)
	{
		qinternal::deque_node<T> * node = new qinternal::deque_node<T>;
		node->value = new T(value);
		__link_nodes(node, __head->next);
		__link_nodes(__head, node);
		__size++;
	}

	template<typename T>
	inline void deque<T>::push_front(T && value)
	{
		qinternal::deque_node<T> * node = new qinternal::deque_node<T>;
		node->value = new T(value);
		__link_nodes(node, __head->next);
		__link_nodes(__head, node);
		__size++;
	}

	template<typename T>
	inline void deque<T>::pop_back()
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
	inline void deque<T>::pop_front()
	{
#ifdef _DEBUG
		assert(__size > 0);
#endif
		auto node = __head->next;
		delete node->value;
		__link_nodes(node->prev, node->next);
		delete node;
		__size--;
	}

	template<typename T>
	inline T & deque<T>::front()
	{
#ifdef _DEBUG
		assert(__size > 0);
#endif
		return *(__head->next->value);
	}

	template<typename T>
	inline const T & deque<T>::front() const
	{
#ifdef _DEBUG
		assert(__size > 0);
#endif
		return *(__head->next->value);
	}

	template<typename T>
	inline T & deque<T>::back()
	{
#ifdef _DEBUG
		assert(__size > 0);
#endif
		return *(__tail->prev->value);
	}

	template<typename T>
	inline const T & deque<T>::back() const
	{
#ifdef _DEBUG
		assert(__size > 0);
#endif
		return *(__tail->prev->value);
	}

	template<typename T>
	inline void deque<T>::__link_nodes(qinternal::deque_node<T>* left, qinternal::deque_node<T>* right)
	{
		left->next = right;
		right->prev = left;
	}

	template<typename T>
	inline void deque<T>::__clear()
	{
		if (__head && __tail)
		{
			while (!empty())
			{
				pop_front();
			}
#ifdef _DEBUG
			assert(__size == 0);
#endif
		}
	}
}

#endif
