#ifndef queue_h__
#define queue_h__

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
		struct queue_node
		{
			T* value;
			queue_node<T>* next;
			queue_node<T>* prev;
		};
	}
#endif

	/// <summary>
	/// FIFO data structure
	/// </summary>
	/// <typeparam name="T">
	/// Type of data stored
	/// </typeparam>
	template <typename T>
	class queue
	{
	public:
		/// <summary>
		/// Constructs an empty queue.  Complexity O(1)
		/// </summary>
		queue();
		
		/// <summary>
		/// Constructs a copy of a queue.  Complexity O(n)
		/// </summary>
		/// <param name="q">
		/// Queue to copy from 
		/// </param>
		explicit queue(const queue& q);

		/// <summary>
		/// Constructs a queue from another queue by move semantics.  Complexity O(1)
		/// </summary>
		/// <param name="q">
		/// Queue to move from
		/// </param>
		explicit queue(queue&& q);

		/// <summary>
		/// Deconstructs the queue object.  Complexity O(n)
		/// </summary>
		~queue();

		/// <summary> 
		/// Copies a queue from a referenced queue.  Complexity O(n)
		/// </summary>
		/// <param name="q">
		/// Queue to copy from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		queue& operator=(const queue& q);

		/// <summary>
		/// Movies another queue to this.  Complexity O(1)
		/// </summary>
		/// <param name="q">
		/// Queue to move from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		queue& operator=(queue&& q);

		/// <summary>
		/// Checks for equality between two queues.  Complexity O(n)
		/// </summary>
		/// <param name="other">
		/// Queue to check for equality against
		/// </param>
		/// <returns>
		/// True if equal, else false
		/// </returns>
		bool operator==(const queue& other) const;

		/// <summary>
		/// Checks for inequality between two queues.  Complexity O(n)
		/// </summary>
		/// <param name="other">
		/// Queue to check for inequality against
		/// </param>
		/// <returns>
		/// False if equal, else true
		/// </returns>
		bool operator!=(const queue& other) const;

		/// <summary>
		/// Checks if queue is empty.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// True if empty, else false
		/// </returns>
		bool empty() const noexcept;

		/// <summary>
		/// Gets the number of elements in the queue.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// Number of elements in queue
		/// </returns>
		std::size_t size() const noexcept;

		/// <summary>
		/// Adds an element to the back of the queue.  Complexity O(1)
		/// </summary>
		/// <param name="value">
		/// Value to add to queue
		/// </param>
		void push(const T& value);

		/// <summary>
		/// Adds an element to the back of the queue.  Complexity O(1)
		/// </summary>
		/// <param name="value">
		/// Value to add to queue
		/// </param>
		void push(T&& value);

		/// <summary>
		/// Removes the first element from the queue.  Complexity O(1)
		/// </summary>
		void pop();

		/// <summary>
		/// Gets a reference to the front element in the queue
		/// </summary>
		/// <returns>
		/// Reference to first element
		/// </returns>
		T& front();

		/// <summary>
		/// Gets a reference to the front element in the queue
		/// </summary>
		/// <returns>
		/// Reference to first element
		/// </returns>
		const T& front() const;

		/// <summary>
		/// Gets a reference to the last element in the queue
		/// </summary>
		/// <returns>
		/// Reference to last element
		/// </returns>
		T& back();

		/// <summary>
		/// Gets a reference to the last element in the queue
		/// </summary>
		/// <returns>
		/// Reference to last element
		/// </returns>
		const T& back() const;
	private:
		std::size_t __size;
		qinternal::queue_node<T>* __head;
		qinternal::queue_node<T>* __tail;

		void __link_nodes(qinternal::queue_node<T>* left, qinternal::queue_node<T>* right);
		void __clear();
	};

	template<typename T>
	inline queue<T>::queue()
	{
		__size = 0;
		__head = new qinternal::queue_node<T>;
		__tail = new qinternal::queue_node<T>;
		__link_nodes(__head, __tail);
	}

	template<typename T>
	inline queue<T>::queue(const queue<T>& q)
	{
		__head = new qinternal::queue_node<T>;
		__tail = new qinternal::queue_node<T>;
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
	inline queue<T>::queue(queue<T>&& q)
	{
		__head = q.__head;
		__tail = q.__tail;
		__size = q.__size;
		q.__head = nullptr;
		q.__tail = nullptr;
		q.__size = 0;
	}

	template<typename T>
	inline queue<T>::~queue()
	{
		__clear();
		if (__head) delete __head;
		if (__tail) delete __tail;
	}

	template<typename T>
	inline queue<T>& queue<T>::operator=(const queue<T>& q)
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
	inline queue<T>& queue<T>::operator=(queue<T>&& q)
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
	inline bool queue<T>::operator==(const queue<T>& other) const
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
	inline bool queue<T>::operator!=(const queue<T>& other) const
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
	inline bool queue<T>::empty() const noexcept
	{
		return __size == 0;
	}

	template<typename T>
	inline std::size_t queue<T>::size() const noexcept
	{
		return __size;
	}

	template<typename T>
	inline void queue<T>::push(const T & value)
	{
		qinternal::queue_node<T> * node = new qinternal::queue_node<T>;
		node->value = new T(value);
		__link_nodes(__tail->prev, node);
		__link_nodes(node, __tail);
		__size++;
	}

	template<typename T>
	inline void queue<T>::push(T && value)
	{
		qinternal::queue_node<T> * node = new qinternal::queue_node<T>;
		node->value = new T(value);
		__link_nodes(__tail->prev, node);
		__link_nodes(node, __tail);
		__size++;
	}

	template<typename T>
	inline void queue<T>::pop()
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
	inline T & queue<T>::front()
	{
#ifdef _DEBUG
		assert(__size > 0);
#endif
		return *(__head->next->value);
	}

	template<typename T>
	inline const T & queue<T>::front() const
	{
#ifdef _DEBUG
		assert(__size > 0);
#endif
		return *(__head->next->value);
	}

	template<typename T>
	inline T & queue<T>::back()
	{
#ifdef _DEBUG
		assert(__size > 0);
#endif
		return *(__tail->prev->value);
	}

	template<typename T>
	inline const T & queue<T>::back() const
	{
#ifdef _DEBUG
		assert(__size > 0);
#endif
		return *(__tail->prev->value);
	}

	template<typename T>
	inline void queue<T>::__link_nodes(qinternal::queue_node<T>* left, qinternal::queue_node<T>* right)
	{
		left->next = right;
		right->prev = left;
	}

	template<typename T>
	inline void queue<T>::__clear()
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
