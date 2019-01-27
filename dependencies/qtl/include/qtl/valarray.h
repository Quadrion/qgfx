#ifndef __valarray_h_
#define __valarray_h_

#include <cassert>
#include <cstddef>

namespace qtl
{
	/// <summary>
	/// Represents a slice from an array
	/// </summary>
    class slice
    {
    public:
		/// <summary>
		/// Default slice, start = 0, size = 0, stride = 0
		/// </summary>
		slice() noexcept;

		/// <summary>
		/// Represents a slice
		/// </summary>
		/// <param name="start">
		/// Starting index
		/// </param>
		/// <param name="size">
		/// Number of elements in slice
		/// </param>
		/// <param name="stride">
		/// Stride between elements
		/// </param>
        slice(std::size_t start, std::size_t size, std::size_t stride) noexcept;

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="other">
		/// Slice to copy from
		/// </param>
        slice(const slice& other) noexcept;

		/// <summary>
		/// Gets start index
		/// </summary>
		/// <returns>
		/// Start index
		/// </returns>
        std::size_t start() const noexcept;

		/// <summary>
		/// Gets size
		/// </summary>
		/// <returns>
		/// Size
		/// </returns>
        std::size_t size() const noexcept;

		/// <summary>
		/// Gets stride
		/// </summary>
		/// <returns>
		/// Stride
		/// </returns>
        std::size_t stride() const noexcept;
    private:
        std::size_t __start;
        std::size_t __size;
        std::size_t __stride;
    };

	/// <summary>
	/// Represents a value array
	/// </summary>
	/// <typeparam name="T">
	/// Type stored in array
	/// </typeparam>
    template <typename T>
    class valarray
    {
    public:
		/// <summary>
		/// Default value array
		/// </summary>
        valarray() noexcept;

		/// <summary>
		/// Constructs a valarray of size count, initializing each value
		/// to the default value
		/// </summary>
		/// <param name="count">
		/// Number of elements in value array
		/// </param>
        explicit valarray(std::size_t count);

		/// <summary>
		/// Constructs a valarray of size count initialized to the given value
		/// </summary>
		/// <param name="val">
		/// Value of all elements in the array
		/// </param>
		/// <param name="count">
		/// Number of elements in the array
		/// </param>
        valarray(const T& val, std::size_t count);

		/// <summary>
		/// Creates a valarray from an array and a count
		/// </summary>
		/// <param name="vals">
		/// Array to copy
		/// </param>
		/// <param name="count">
		/// Number of elements in array
		/// </param>
        valarray(const T* vals, std::size_t count);

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="other">
		/// valarray to copy
		/// </param>
        valarray(const valarray& other);

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="other">
		/// valarray to move
		/// </param>
        valarray(valarray&& other) noexcept;

		/// <summary>
		/// Deallocates backing array
		/// </summary>
        ~valarray();

		/// <summary>
		/// Copy operator
		/// </summary>
		/// <param name="other">
		/// Array to copy from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
        valarray& operator=(const valarray& other);

		/// <summary>
		/// Move operator
		/// </summary>
		/// <param name="other">
		/// Array to move from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
        valarray& operator=(valarray&& other) noexcept;

		/// <summary>
		/// Sets valarray to val
		/// </summary>
		/// <param name="val">
		/// Value to set to
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
        valarray& operator=(const T& val);

		/// <summary>
		/// Gets value at index
		/// </summary>
		/// <param name="pos">
		/// Position
		/// </param>
		/// <returns>
		/// Reference to value at index
		/// </returns>
        T& operator[](std::size_t pos);

		/// <summary>
		/// Gets value at index
		/// </summary>
		/// <param name="pos">
		/// Position
		/// </param>
		/// <returns>
		/// Reference to value at index
		/// </returns>
        const T& operator[](std::size_t pos) const;

		/// <summary>
		/// Gets values given by an array of indices
		/// </summary>
		/// <param name="indices">
		/// Indices to fetch
		/// </param>
		/// <returns>
		/// valarray containing copy of values at provided indices
		/// </returns>
        valarray operator[](const valarray<std::size_t>& indices) const;

		/// <summary>
		/// Gets values given by a slice
		/// </summary>
		/// <param name="s">
		/// Slice to fetch
		/// </param>
		/// <returns>
		/// valarray containing slice
		/// </returns>
        valarray operator[](const slice& s) const;

		/// <summary>
		/// Gets number of elements in valarray
		/// </summary>
		/// <returns>
		/// Number of elements in valarray
		/// </returns>
        std::size_t size() const noexcept;
    private:
        T* __data;
        std::size_t __size;
    };

    template<typename T>
    valarray<T>::valarray() noexcept
        : __data(nullptr), __size(0)
    {   }

    template<typename T>
    valarray<T>::valarray(std::size_t count)
    {
        __data = new T[count];
        __size = count;
    }

    template<typename T>
    valarray<T>::valarray(const T& val, std::size_t count)
    {
        __data = new T[count];
        for (std::size_t i = 0; i < count; i++)
        {
            __data[i] = val;
        }
        __size = count;
    }

    template<typename T>
    valarray<T>::valarray(const T* vals, std::size_t count)
    {
        __data = new T[count];
        for (std::size_t i = 0; i < count; i++)
        {
            __data[i] = vals[i];
        }
        __size = count;
    }

    template<typename T>
    valarray<T>::valarray(const valarray<T>& other)
    {
        __data = new T[other.__size];
        __size = other.__size;
        for (std::size_t i = 0; i < __size; i++)
        {
            __data[i] = other.__data[i];
        }
    }

    template<typename T>
    valarray<T>::valarray(valarray<T>&& other) noexcept
    {
        __data = other.__data;
        __size = other.__size;
        other.__data = nullptr;
    }

    template<typename T>
    valarray<T>::~valarray()
    {
        if (__data)
        {
            delete[] __data;
        }
    }

    template<typename T>
    valarray<T>& valarray<T>::operator=(const valarray<T>& other)
    {
        if (__data) delete[] __data;
        __data = new T[other.__size];
        __size = other.__size;
        for (std::size_t i = 0; i < __size; i++)
        {
            __data[i] = other.__data[i];
        }
        return *this;
    }

    template<typename T>
    valarray<T>& valarray<T>::operator=(valarray<T>&& other) noexcept
    {
        if (__data) delete[] __data;
        __data = other.__data;
        __size = other.__size;
        other.__data = nullptr;
        return *this;
    }

    template<typename T>
    valarray<T>& valarray<T>::operator=(const T& other)
    {
        if (__data) delete[] __data;
        __data = new T[1];
        __size = 1;
        __data[0] = other;
        return *this;
    }

    template<typename T>
    T& valarray<T>::operator[](std::size_t pos)
    {
        assert(pos < __size);
        return __data[pos];
    }

    template<typename T>
    const T& valarray<T>::operator[](std::size_t pos) const
    {
        assert(pos < __size);
        return __data[pos];
    }

    template<typename T>
    valarray<T> valarray<T>::operator[](const valarray<std::size_t>& indices) const
    {
        valarray<T> res(indices.size());
        for (std::size_t i = 0; i < indices.size(); i++)
        {
            assert(indices[i] < __size);
            res[i] = __data[indices[i]];
        }
        return res;
    }

    template<typename T>
    valarray<T> valarray<T>::operator[](const slice& s) const
    {
        valarray<T> res(s.size());
        std::size_t c = 0;
        std::size_t d = 0;
        for (std::size_t i = s.start(); i < __size; i++)
        {
            if (d % s.stride() == 0)
            {
                res[c] = __data[i];
                c++;
            }
            d++;
        }
        return res;
    }

    template <typename T>
    std::size_t valarray<T>::size() const noexcept
    {
        return __size;
    }
}

#endif