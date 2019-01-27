#ifndef __sharedptr_h_
#define __sharedptr_h_

#include <cstddef>
#include <cstdlib>

#include "qtl/utility.h"

namespace qtl 
{
	/// <summary>
	/// Reference counted smart pointer that retains shared ownership of an object through
	/// the pointer.  Shared pointers can own the same object.  The object is destroyed when
	/// the last remaining shared pointer is destroyed or the last remaining shared pointer is
	/// reset.  The pointer's contents is destroyed via the delete method.  Shared pointers can
	/// also own no objects (nullptr).
	/// </summary>
	/// <typeparam name="T">
	/// Type of pointer stored
	/// </typeparam>
	template<typename T>
	class shared_ptr
	{
	public:
		/// <summary>
		/// Constructs a new shared pointer object pointing to nullptr.  Complexity O(1)
		/// </summary>
		constexpr shared_ptr() noexcept;

		/// <summary>
		/// Explicitly constructs a shared pointer pointing to nullptr.  Complexity O(1)
		/// </summary>
		constexpr shared_ptr(std::nullptr_t) noexcept;

		/// <summary>
		/// Constructs a shared pointer from a pre-existing pointer.  Assigning a pointer to a shared
		/// pointer gives the shared pointer ownership of the object.  Deleting this pointer after passing
		/// it to the shared pointer is undefined behavior.  Complexity O(1)
		/// </summary>
		/// <param name="ptr">
		/// Pointer to make shared pointer of. 
		/// </param>
		explicit shared_ptr(T* ptr);

		/// <summary>
		/// Constructs a shared pointer from another shared pointer.  Complexity O(1)
		/// </summary>
		/// <param name="ptr">
		/// Shared pointer to copy
		/// </param>
		shared_ptr(const shared_ptr& ptr) noexcept;

		/// <summary>
		/// Moves a shared pointer from another shared pointer.  Complexity O(1)
		/// </summary>
		/// <param name="ptr">
		/// Shared pointer to move
		/// </param>
		shared_ptr(shared_ptr&& ptr) noexcept;
		
		/// <summary>
		/// Constructs a shared pointer from another shared pointer.  Complexity O(1)
		/// </summary>
		/// <typeparam name="T2">
		/// Type of other shared pointer
		/// </typeparam>
		/// <param name="ptr">
		/// Shared pointer to copy
		/// </param>
		template<typename T2>
		shared_ptr(const shared_ptr<T2>& ptr) noexcept;

		/// <summary>
		/// Constructs a shared pointer from another shared pointer.  Complexity O(1)
		/// </summary>
		/// <typeparam name="T2">
		/// Type of other shared pointer
		/// </typeparam>
		/// <param name="ptr">
		/// Shared pointer to move
		/// </param>
		template<typename T2>
		shared_ptr(shared_ptr<T2>&& ptr) noexcept;

		/// <summary>
		/// Deconstructs shared pointer.  When the reference count is 0,
		/// the owned object is deleted.  Complexity O(1)
		/// </summary>
		virtual ~shared_ptr();

		/// <summary>
		/// Copies a shared pointer from another shared pointer.  Complexity O(1)
		/// </summary>
		/// <param name="other">
		/// Pointer to copy from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		shared_ptr& operator=(const shared_ptr& other) noexcept;

		/// <summary>
		/// Copies a shared pointer from another shared pointer.  Complexity O(1)
		/// </summary>
		/// <param name="other">
		/// Pointer to move from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		shared_ptr& operator=(shared_ptr&& other) noexcept;

		/// <summary>
		/// Copies a shared pointer from a null pointer.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// Reference to this
		/// </returns>
		shared_ptr& operator=(std::nullptr_t) noexcept;
		
		/// <summary>
		/// Assigns this value from another shared pointer in the same inheritance hierarchy.  Complexity O(1)
		/// </summary>
		/// <typeparam name="T2">
		/// Type of other pointer to copy from
		/// </typeparam>
		/// <param name="other">
		/// Pointer to copy from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		template<typename T2>
		shared_ptr& operator=(const shared_ptr<T2>& other) noexcept;

		/// <summary>
		/// Assigns this value from another shared pointer in the same inheritance hierarchy.  Complexity O(1)
		/// </summary>
		/// <typeparam name="T2">
		/// Type of other pointer to copy from
		/// </typeparam>
		/// <param name="other">
		/// Pointer to move from
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
		template<typename T2>
		shared_ptr& operator=(shared_ptr<T2>&& other) noexcept;

		/// <summary>
		/// Checks if two shared pointers point to the same value.  Complexity O(1)
		/// </summary>
		/// <param name="other">
		/// Pointer to check for equality against
		/// </param>
		/// <returns>
		/// If pointer points to the same value
		/// </returns>
		bool operator==(const shared_ptr& other) const noexcept;

		/// <summary>
		/// Checks if two shared pointers do not point to the same value.  Complexity O(1)
		/// </summary>
		/// <param name="other">
		/// Pointer to check for inequality against
		/// </param>
		/// <returns>
		/// If pointer does not point to the same value
		/// </returns>
		bool operator!=(const shared_ptr& other) const noexcept;

		/// <summary>
		/// Checks if two shared pointers point to the same value.  Complexity O(1)
		/// </summary>
		/// <typeparam name="T2">
		/// Type of other pointer
		/// </typeparam>
		/// <param name="other">
		/// Pointer to check for equality against
		/// </param>
		/// <returns>
		/// If pointer points to the same value
		/// </returns>
		template<typename T2>
		bool operator==(const shared_ptr<T2>& other) const noexcept;
		
		/// <summary>
		/// Checks if two shared pointers do not point to the same value.  Complexity O(1)
		/// </summary>
		/// </typeparam>
		/// <param name="other">
		/// Pointer to check for equality against
		/// </param>
		/// <param name="other">
		/// Pointer to check for inequality against
		/// </param>
		/// <returns>
		/// If pointer does not point to the same value
		/// </returns>
		template<typename T2>
		bool operator!=(const shared_ptr<T2>& other) const noexcept;

		/// <summary>
		/// Resets this pointer to a nullptr.  Complexity O(1)
		/// </summary>
		void reset() noexcept;

		/// <summary>
		/// Resets this pointer to the supplied pointer.  Shared pointer now owns
		/// the provided pointer.  Operations on this pointer after assignment are undefined
		/// behaviors.  Complexity O(1)
		/// </summary>
		/// <param name="ptr">
		/// Pointer to set shared pointer to
		/// </param>
		void reset(T* ptr);

		/// <summary>
		/// Shared pointer to swap values with.  Complexity O(1)
		/// </summary>
		/// <param name="r">
		/// Pointer to swap with
		/// </param>
		void swap(shared_ptr& r) noexcept;

		/// <summary>
		/// Gets the value pointed to by the shared pointer.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// Pointer to owned object
		/// </returns>
		T* get() const noexcept;

		/// <summary>
		/// Dereferences owned object and returns reference to it.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// Reference to owned object
		/// </returns>
		T& operator*() const noexcept;

		/// <summary>
		/// Gets owned object and returns pointer to it.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// Pointer to owned object
		/// </returns>
		T* operator->() const noexcept;

		/// <summary>
		/// Gets the number of shared pointers owning the resource.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// Number of shared pointers owning the resource
		/// </returns>
		std::size_t use_count() const noexcept;

		/// <summary>
		/// Gets if there is only one shared pointer owning the object.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// If only one shared pointer owns the resource
		/// </returns>
		bool unique() const noexcept;

		/// <summary>
		/// Returns if the shared pointer points to a valid, non null resource.  Complexity O(1)
		/// </summary>
		/// <returns>
		/// True if object owned is valid and non null, else false
		/// </returns>
		explicit operator bool() const noexcept;
	private:
		struct ref_count
		{
			T* pointer = nullptr;
			std::size_t refs = 0;
		};

		ref_count * ref = nullptr;

		template <typename T2>
		friend class shared_ptr;
	};

	template<typename T>
	shared_ptr<T> make_shared()
	{
		return shared_ptr<T>(new T);
	}

	template<typename T, typename ... ArgumentTypes>
	shared_ptr<T> make_shared(ArgumentTypes ... args)
	{
		return shared_ptr<T>(new T(qtl::forward<ArgumentTypes>(args)...));
	}

	template<typename T>
	inline constexpr shared_ptr<T>::shared_ptr() noexcept
	{
		ref = new shared_ptr<T>::ref_count;
		ref->refs++;
	}

	template<typename T>
	inline constexpr shared_ptr<T>::shared_ptr(std::nullptr_t) noexcept
		: shared_ptr<T>()
	{
	}

	template<typename T>
	inline shared_ptr<T>::shared_ptr(T * ptr)
		: shared_ptr<T>()
	{
		ref->pointer = ptr;
	}

	template<typename T>
	inline shared_ptr<T>::shared_ptr(const shared_ptr& ptr) noexcept
	{
		ref = ptr.ref;
		ref->refs++;
	}

	template<typename T>
	inline shared_ptr<T>::shared_ptr(shared_ptr&& ptr) noexcept
	{
		ref = ptr.ref;
		ptr.ref = nullptr;
	}

	template<typename T>
	template<typename T2>
	inline shared_ptr<T>::shared_ptr(const shared_ptr<T2>& ptr) noexcept
	{
		ref = reinterpret_cast<T>(ptr.ref);
		ref->refs++;
	}

	template<typename T>
	template<typename T2>
	inline shared_ptr<T>::shared_ptr(shared_ptr<T2>&& ptr) noexcept
	{
		ref = reinterpret_cast<T>(ptr.ref);
		ptr.ref = nullptr;
	}

	template<typename T>
	inline shared_ptr<T>::~shared_ptr()
	{
		if (ref)
		{
			ref->refs--;
			if (ref->refs == 0)
			{
				if (ref->pointer) delete ref->pointer;
				delete ref;
				ref = nullptr;
			}
		}
	}

	template<typename T>
	inline shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr& other) noexcept
	{
		if (ref && ref->refs == 1)
		{
			if (ref->pointer) delete ref->pointer;
			delete ref;
		}
		ref = other.ref;
		ref->refs++;
		return *this;
	}

	template<typename T>
	inline shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr&& other) noexcept
	{
		if (ref && ref->refs == 1)
		{
			delete ref->pointer;
			delete ref;
		}
		ref = other.ref;
		other.ref = nullptr;
		return *this;
	}

	template<typename T>
	inline shared_ptr<T>& shared_ptr<T>::operator=(std::nullptr_t) noexcept
	{
		ref->refs--;
		ref = new shared_ptr<T>::ref_count;
		ref->refs++;
	}

	template<typename T>
	inline bool shared_ptr<T>::operator==(const shared_ptr & other) const noexcept
	{
		return ref == other.ref;
	}

	template<typename T>
	inline bool shared_ptr<T>::operator!=(const shared_ptr & other) const noexcept
	{
		return ref != other.ref;
	}

	template<typename T>
	template <typename T2>
	inline bool shared_ptr<T>::operator==(const shared_ptr<T2> & other) const noexcept
	{
		return ref == other.ref;
	}

	template<typename T>
	template <typename T2>
	inline bool shared_ptr<T>::operator!=(const shared_ptr<T2> & other) const noexcept
	{
		return ref != other.ref;
	}

	template <typename T>
	template <typename T2>
	inline shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<T2>& other) noexcept
	{
		ref = reinterpret_cast<shared_ptr<T>::ref_count*>(other.ref);
		ref->refs++;
		return *this;
	}

	template <typename T>
	template <typename T2>
	inline shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr<T2>&& other) noexcept
	{
		if (ref && ref->refs == 1)
		{
			delete ref->pointer;
			delete ref;
		}
		ref = reinterpret_cast<shared_ptr<T>::ref_count*>(other.ref);
		other.ref = nullptr;
		return *this;
	}

	template<typename T>
	inline void shared_ptr<T>::reset() noexcept
	{
		ref->refs--;
		if (ref->refs == 0)
		{
			delete ref->ref;
		}
		ref = new shared_ptr<T>::ref_count;
		ref->refs++;
	}

	template<typename T>
	inline void shared_ptr<T>::reset(T * ptr)
	{
		ref->refs--;
		if (ref->refs == 0)
		{
			delete ref->ref;
		}
		ref = new shared_ptr<T>::ref_count;
		ref->refs++;
		ref->pointer = ptr;
	}

	template<typename T>
	inline void shared_ptr<T>::swap(shared_ptr & r) noexcept
	{
		auto tmp = ref;
		ref = r.ref;
		r.ref = tmp;
	}

	template<typename T>
	inline T * shared_ptr<T>::get() const noexcept
	{
		return ref->pointer;
	}

	template<typename T>
	inline T & shared_ptr<T>::operator*() const noexcept
	{
		return *(ref->pointer);
	}

	template<typename T>
	inline T * shared_ptr<T>::operator->() const noexcept
	{
		return get();
	}

	template<typename T>
	inline std::size_t shared_ptr<T>::use_count() const noexcept
	{
		return ref->refs;
	}

	template<typename T>
	inline bool shared_ptr<T>::unique() const noexcept
	{
		return use_count() == 1;
	}

	template<typename T>
	inline shared_ptr<T>::operator bool() const noexcept
	{
		return ref == nullptr || get() != nullptr;
	}

}

#endif