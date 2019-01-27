#ifndef vector_h__
#define vector_h__

#include "qtl/type_traits.h"
#include "qtl/utility.h"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <new>

namespace qtl
{
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	namespace qinternal
	{
		template <typename T>
		class forward_vector_iterator
		{
		public:
			forward_vector_iterator(T* ptr, const std::size_t offset);
			forward_vector_iterator<T>& operator++();

			bool operator!=(const forward_vector_iterator<T>& it);
			bool operator==(const forward_vector_iterator<T>& it);

			T& operator*();
			const T& operator*() const;
		private:
			T* ptr;
		};

		template<typename T>
		inline forward_vector_iterator<T>::forward_vector_iterator(T * ptr, const std::size_t offset)
			: ptr(ptr + offset)
		{
		}

		template<typename T>
		inline forward_vector_iterator<T>& qinternal::forward_vector_iterator<T>::operator++()
		{
			++ptr;
			return *this;
		}

		template<typename T>
		inline bool forward_vector_iterator<T>::operator!=(const forward_vector_iterator<T>& it)
		{
			return ptr != it.ptr;
		}

		template<typename T>
		inline bool forward_vector_iterator<T>::operator==(const forward_vector_iterator<T>& it)
		{
			return ptr == it.ptr;
		}

		template<typename T>
		inline T & forward_vector_iterator<T>::operator*()
		{
			return *ptr;
		}

		template<typename T>
		inline const T & forward_vector_iterator<T>::operator*() const
		{
			return *ptr;
		}
	}
#endif

	/// <summary>
	/// Linearly allocated container storing a single type
	/// </summary>
	/// <typeparam name="T"> 
	/// Type of value stored in the vector
	/// </typeparam>
	template <typename T>
	class vector
	{		
	public: // member methods
		template <typename Type>
		using forward_iterator = qinternal::forward_vector_iterator<Type>;

		typedef T value;

		/// <summary>
		/// Constructs a blank vector with no size or capacity. Complexity O(1)
		/// </summary>
		vector();

		/// <summary>
		/// Constructs a blank vector with no size and initial capacity. Complexity O(1)
		/// </summary>
		/// <param name="capacity">
		/// Defines the initial capacity of the vector
		/// </param>
		explicit vector(const std::size_t capacity);

		/// <summary>
		/// Constructs a vector from a pre-existing vector with deep copied data. Complexity O(n)
		/// </summary>
		/// <param name="vec">
		/// vector to copy from
		/// </param>
		vector(const vector<T>& vec);

		/// <summary>
		/// Constructs a vector from a pre-existing vector with moved data. Complexity O(1)
		/// </summary>
		/// <param name="vec">
		/// vector to move from
		/// </param>
		vector(vector<T>&& vec) noexcept;

		/// <summary>
		/// Destructor of vector.  Deletes all objects inside the memory of the vector and 
		/// cleans up resources allocated by the vector. Complexity O(n)
		/// </summary>
		~vector();

		/// <summary>
		/// Sets a vector's value from a pre-existing vector with copied data. Complexity O(n)
		/// </summary>
		/// <param name="vec">
		/// vector to move from
		/// </param>
		vector<T>& operator=(const vector<T>& vec);

		/// <summary>
		/// Sets a vector's value from a pre-existing vector with moved data. Complexity O(1)
		/// </summary>
		/// <param name="vec">
		/// vector to move from
		/// </param>
		vector<T>& operator=(vector<T>&& vec) noexcept;

		/// <summary>
		/// Checks for equality of elements in vector. Complexity O(n)
		/// </summary>
		/// <param name="other">
		/// Vector to check against
		/// </param>
		/// <returns>
		/// True if equivalent, else false
		/// </returns>
		bool operator==(const vector<T>& other) const;

		/// <summary>
		/// Checks for inequality of elements in vector. Complexity O(n)
		/// </summary>
		/// <param name="other">
		/// Vector to check against
		/// </param>
		/// <returns>
		/// True if not equivalent, else false
		/// </returns>
		bool operator!=(const vector<T>& other) const;

		/// <summary> 
		/// Gets the number of elements in the vector. Complexity: O(1)
		/// </summary>
		/// <returns>
		/// Number of elements in vector
		/// </returns>
		std::size_t size() const noexcept;

		/// <summary> 
		/// Gets the number of elements that the vector can store without needing to resize 
		/// the underlying buffer. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Number of elements that can be stored in the vector
		/// </returns>
		std::size_t capacity() const noexcept;

		/// <summary>
		/// Resizes the underlying data structure to something larger.  If the requested size
		/// is smaller than the underlying structure, nothing will occur.  Complexity: O(n)
		/// </summary>
		/// <param name="capacity">
		/// New capacity of the vector
		/// </param>
		void reserve(const std::size_t capacity);

		/// <summary>
		/// Provides element access to vector. Complexity O(1)
		/// </summary>
		/// <param name="index>
		/// Index to retrieve value at
		/// </param>
		/// <returns>
		/// Reference to element at index
		/// </returns>
		T& operator[](const std::size_t index);

		/// <summary>
		/// Provides element access to vector. Complexity O(1)
		/// </summary>
		/// <param name="index>
		/// Index to retrieve value at
		/// </param>
		/// <returns>
		/// Reference to element at index
		/// </returns>
		const T& operator[](const std::size_t index) const;

		/// <summary>
		/// Provides a pointer to the buffer.  Addition or deletion to data
		/// in this buffer is undefined behavior. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Points to buffer
		/// </returns>
		T* data();

		/// <summary>
		/// Provides a pointer to the buffer.  Addition or deletion to data
		/// in this buffer is undefined behavior. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Points to buffer
		/// </returns>
		const T* data() const;
		
		/// <summary>
		/// Gets a forward iterator to the first element in the vector. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Forward iterator to first element
		/// </returns>
		forward_iterator<T> begin();

		/// <summary>
		/// Gets a forward iterator to the first element in the vector. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Forward iterator to first element
		/// </returns>
		const forward_iterator<T> begin() const;

		/// <summary>
		/// Gets a forward iterator to the last element in the vector. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Forward iterator to last element
		/// </returns>
		forward_iterator<T> end();

		/// <summary>
		/// Gets a forward iterator to the last element in the vector. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Forward iterator to last element
		/// </returns>
		const forward_iterator<T> end() const;

		/// <summary>
		/// Clears the contents of the vector.  After executing, size() returns 0, but capacity
		/// is maintained.  Clear will invalidate any pointers or references that refer to elements
		/// that were inside the vector.  Complexity O(n)
		/// </summary>
		void clear() noexcept;

		/// <summary>
		/// Provides element access to first element. Complexity O(1)
		/// Operation is undefined on an empty vector
		/// </summary>
		/// <returns>
		/// Reference to first element
		/// </returns>
		T& front();

		/// <summary>
		/// Provides element access to first element. Complexity O(1)
		/// Operation is undefined on an empty vector
		/// </summary>
		/// <returns>
		/// Reference to first element
		/// </returns>
		const T& front() const;

		/// <summary>
		/// Provides element access to last element. Complexity O(1)
		/// Operation is undefined on an empty vector
		/// </summary>
		/// <returns>
		/// Reference to last element
		/// </returns>
		T& back();

		/// <summary>
		/// Provides element access to last element. Complexity O(1)
		/// Operation is undefined on an empty vector
		/// </summary>
		/// <returns>
		/// Reference to last element
		/// </returns>
		const T& back() const;

		/// <summary>
		/// Adds a single element to the back of the vector.  Complexity O(1) if no resize is
		/// needed, O(n) if resize is needed
		/// </summary>
		/// <param name="value">
		/// Value to add to vector
		/// </param>
		void push_back(const T& value);

		/// <summary>
		/// Adds a single element to the back of the vector using move semantics.  Complexity 
		/// O(1) if no resize is needed, O(n) if resize is needed
		/// </summary>
		/// <param name="value">
		/// Value to add to vector
		/// </param>
		void push_back(T&& value);

		/// <summary>
		/// Takes arguments for construction of an element of type T and constructs an object
		/// at the provided index of the vector.  Complexity O(1) if no resize needed, O(n) if resize is
		/// needed
		/// </summary>
		/// <typeparam name="Arguments">
		/// Argument types needed for construction of element
		/// </typeparam>
		/// <param name="pos">
		/// Position to place element at
		/// </param>
		/// <param name="args">
		/// Argument values for construction of element
		/// </param>
		template <typename ... Arguments>
		void emplace(forward_iterator<T> pos, Arguments && ... args);

		/// <summary>
		/// Takes arguments for construction of an element of type T and constructs an object
		/// at the back of the vector.  Complexity O(1) if no resize needed, O(n) if resize is
		/// needed
		/// </summary>
		/// <typeparam name="Arguments">
		/// Argument types needed for construction of element
		/// </typeparam>
		/// <param name="args">
		/// Argument values for construction of element
		/// </param>
		template <typename ... Arguments>
		void emplace_back(Arguments && ... args);

		/// <summary>
		/// Inserts value into the array at the specified index. Complexity O(n)
		/// </summary>
		/// <param name="pos">
		/// Position to insert element at
		/// </param>
		/// <param name="value">
		/// Value to insert
		/// </param>
		void insert(forward_iterator<T> pos, const T& value);

		/// <summary>
		/// Inserts value into the array at the specified index. Complexity O(n)
		/// </summary>
		/// <param name="pos">
		/// Position to insert element at
		/// </param>
		/// <param name="value">
		/// Value to insert
		/// </param>
		void insert(forward_iterator<T> pos, T&& value);

		/// <summary>
		/// Erases the element specified from the vector. Complexity O(n)
		/// </summary>
		/// <param name="element">
		/// Iterator pointing to element to erase
		/// </param>
		void erase(forward_iterator<T> element);

		/// <summary>
		/// Checks if the vector is empty. Complexity O(1)
		/// </summary>
		/// <returns>
		/// Is vector empty
		/// </returns>
		bool empty() const noexcept;
	private:
		void* __data;
		std::size_t __size;
		std::size_t __capacity;
	private:
		void __expand_capacity(const std::size_t capacity);
		void __make_hole(std::size_t pos);
	};

	template <typename T>
	inline vector<T>::vector()
	{
		__data = nullptr;
		__size = 0;
		__capacity = 0;
	}

	template <typename T>
	inline vector<T>::vector(std::size_t capacity)
	{
		__data = nullptr;
		__size = 0;
		__capacity = 0;
		__expand_capacity(capacity);
	}

	template<typename T>
	inline vector<T>::vector(const vector<T>& vec)
		: vector<T>(vec.capacity())
	{
		for (T & v : vec)
		{
			push_back(v);
		}
	}

	template<typename T>
	inline vector<T>::vector(vector<T>&& vec) noexcept
	{
		__size = vec.__size;
		__capacity = vec.__capacity;
		__data = vec.__data;
		vec.__size = 0;
		vec.__capacity = 0;
		vec.__data = nullptr;
	}

	template <typename T>
	inline vector<T>::~vector()
	{
		if (__data)
		{
			clear();
			free(__data);
			__data = nullptr;
#ifdef _DEBUG
			__size = 0;
			__capacity = 0;
#endif
		}
	}

	template <typename T>
	inline vector<T>& vector<T>::operator=(const vector<T>& vec)
	{
		clear();
		reserve(vec.capacity());
		for (auto & v : vec)
		{
			push_back(v);
		}
		return *this;
	}

	template <typename T>
	inline vector<T>& vector<T>::operator=(vector<T>&& vec) noexcept
	{
		__size = vec.__size;
		__capacity = vec.__capacity;
		__data = vec.__data;

		vec.__size = 0;
		vec.__capacity = 0;
		vec.__data = nullptr;

		return *this;
	}

	template<typename T>
	inline bool vector<T>::operator==(const vector<T>& other) const
	{
		if (__size == other.__size)
		{
			for (std::size_t i = 0; i < __size; i++)
			{
				if (__data[i] != other.__data[i])
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}

	template<typename T>
	inline bool vector<T>::operator!=(const vector<T>& other) const
	{
		if (__size == other.__size)
		{
			for (std::size_t i = 0; i < __size; i++)
			{
				if (__data[i] != other.__data[i])
				{
					return true;
				}
			}
			return false;
		}
		return true;
	}

	template<typename T>
	inline std::size_t vector<T>::size() const noexcept
	{
		return __size;
	}

	template<typename T>
	inline std::size_t vector<T>::capacity() const noexcept
	{
		return __capacity;
	}

	template<typename T>
	inline void vector<T>::reserve(const std::size_t capacity)
	{
		// only reserve if new capacity is greater than old capacity
		if (capacity > __capacity)
		{
			__expand_capacity(capacity);
		}
	}

	template<typename T>
	inline T & vector<T>::operator[](const std::size_t index)
	{
#ifdef _DEBUG
		assert(index < __size);
#endif
		return static_cast<T*>(__data)[index];
	}

	template<typename T>
	inline const T & vector<T>::operator[](const std::size_t index) const
	{
#ifdef _DEBUG
		assert(index < __size);
#endif
		return static_cast<T*>(__data)[index];
	}

	template<typename T>
	inline T * vector<T>::data()
	{
		return reinterpret_cast<T*>(__data);
	}

	template<typename T>
	inline const T * vector<T>::data() const
	{
		return reinterpret_cast<T*>(__data);
	}

	template<typename T>
	inline typename vector<T>::template forward_iterator<T> vector<T>::begin()
	{
		return vector<T>::forward_iterator<T>((T*)__data, 0);
	}

	template<typename T>
	inline const typename vector<T>::template forward_iterator<T> vector<T>::begin() const
	{
		return vector<T>::forward_iterator<T>((T*)__data, 0);
	}

	template<typename T>
	inline typename vector<T>::template forward_iterator<T> vector<T>::end()
	{
		return vector<T>::forward_iterator<T>((T*)__data, __size);
	}

	template<typename T>
	inline const typename vector<T>::template forward_iterator<T> vector<T>::end() const
	{
		return vector<T>::forward_iterator<T>((T*)__data, __size);
	}

	template<typename T>
	inline T & vector<T>::front()
	{
#ifdef _DEBUG
		assert(__size != 0);
#endif
		return static_cast<T*>(__data)[0];
	}

	template<typename T>
	inline const T & vector<T>::front() const
	{
#ifdef _DEBUG
		assert(__size != 0);
#endif
		return static_cast<T*>(__data)[0];
	}

	template<typename T>
	inline T & vector<T>::back()
	{
#ifdef _DEBUG
		assert(__size != 0);
#endif
		return static_cast<T*>(__data)[__size - 1];
	}

	template<typename T>
	inline const T & vector<T>::back() const
	{
#ifdef _DEBUG
		assert(__size != 0);
#endif
		return static_cast<T*>(__data)[__size - 1];
	}

	template<typename T>
	inline void vector<T>::push_back(const T & value)
	{
		if (__size == __capacity)
		{
			__expand_capacity(__capacity + 16);
		}
		assert(__data);
		T* data = static_cast<T*>(__data);
		::new (data + __size) T(value);
		__size++;
	}

	template<typename T>
	inline void vector<T>::push_back(T && value)
	{
		if (__size == __capacity)
		{
			__expand_capacity(__capacity + 16);
		}
		assert(__data);
		T* data = static_cast<T*>(__data);
		::new (data + __size) T(value);
		__size++;
	}

	template<typename T>
	template<typename ...Arguments>
	inline void vector<T>::emplace(forward_iterator<T> pos, Arguments && ...args)
	{
		std::size_t idx = &pos.operator*() - __data;
		if (__size == __capacity)
		{
			__expand_capacity(__capacity + 16);
		}

		__make_hole(idx);
		::new (static_cast<T*>(__data) + idx) T(qtl::forward<Arguments>(args)...);
		__size++;
	}

	template<typename T>
	template<typename ...Arguments>
	inline void vector<T>::emplace_back(Arguments && ... args)
	{
		if (__size == __capacity)
		{
			__expand_capacity(__capacity + 16);
		}
		assert(__data);
		T* data = static_cast<T*>(__data);
		::new (data + __size) T(qtl::forward<Arguments>(args)...);
		__size++;
	}

	template<typename T>
	inline void vector<T>::clear() noexcept
	{
		for (std::size_t i = 0; i < __size; i++)
		{
			static_cast<T*>(__data)[i].~T();
		}
		__size = 0;
	}

	template<typename T>
	inline void vector<T>::insert(forward_iterator<T> pos, const T & value)
	{
		std::size_t idx = &pos.operator*() - __data;
		if (__size == __capacity)
		{
			__expand_capacity(__capacity + 16);
		}

		__make_hole(idx);
		::new (static_cast<T*>(__data) + idx) T(value);
		__size++;
	}

	template<typename T>
	inline void vector<T>::insert(forward_iterator<T> pos, T && value)
	{
		std::size_t idx = &pos.operator*() - static_cast<T*>(__data);
		if (__size == __capacity)
		{
			__expand_capacity(__capacity + 16);
		}

		__make_hole(idx);
		::new (static_cast<T*>(__data) + idx) T(qtl::move(value));
		__size++;
	}

	template<typename T>
	inline void vector<T>::erase(forward_iterator<T> element)
	{
		std::size_t idx = &element.operator*() - static_cast<T*>(__data);
		T* data = static_cast<T*>(__data);
		data[idx].~T();
		for (std::size_t i = idx; i < __size - 1; i++)
		{
			data[i] = qtl::move(data[i + 1]);
		}
		__size--;
	}

	template<typename T>
	inline bool vector<T>::empty() const noexcept
	{
		return !static_cast<bool>(__size);
	}

	template<typename T>
	inline void vector<T>::__expand_capacity(const std::size_t capacity)
	{
		if (__data)
		{
			auto tmp = realloc(__data, sizeof(T) * capacity);
			if (tmp)
			{
				__data = tmp;
			}
		}
		else
		{
			__data = malloc(sizeof(T) * capacity);
		}
		__capacity = capacity;
	}

	template<typename T>
	inline void vector<T>::__make_hole(std::size_t pos)
	{
		T* data = static_cast<T*>(__data);
		for (std::size_t i = __size; i > pos; i--)
		{
			data[i] = qtl::move(data[i - 1]);
		}
	}
}; // end namespace qtl

#endif