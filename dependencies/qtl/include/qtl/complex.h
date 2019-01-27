#ifndef __complex_h_
#define __complex_h_

#include <math.h>

namespace qtl
{
	/// <summary>
	/// Represents a complex number, a + bi
	/// </summary>
	/// <typeparam name="Type">
	/// Type of complex object
	/// </typeparam>
    template <typename Type>
    class complex
    {
    public:
		/// <summary>
		/// Constructs a new complex number
		/// </summary>
		/// <param name="re">
		/// Real component
		/// </param>
		/// <param name="im">
		/// Imaginary component
		/// </param>
        constexpr complex(const Type& re = Type(), const Type& im = Type());

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="other">
		/// Copy from
		/// </param>
        constexpr complex(const complex& other);

		/// <summary>
		/// Gets real component of complex number
		/// </summary>
		/// <returns>
		/// Real component
		/// </returns>
        Type real() const;

		/// <summary>
		/// Gets imaginary component of complex number
		/// </summary>
		/// <returns>
		/// Imaginary component
		/// </returns>
        Type imag() const;

		/// <summary>
		/// Sets real component of complex number
		/// </summary>
		/// <param name="value">
		/// Value to set real component to
		/// </param>
        void real(Type value);

		/// <summary>
		/// Sets imaginary component of complex number
		/// </summary>
		/// <param name="value">
		/// Value to set imaginary component to
		/// </param>
        void imag(Type value);

		/// <summary>
		/// += on both real and imaginary components by value provided
		/// </summary>
		/// <param name="other">
		/// Value to sum with
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
        complex& operator+=(const Type& other);

		/// <summary>
		/// += on both real and imaginary components by the corresponding components in 
		/// the complex value provided
		/// </summary>
		/// <param name="other">
		/// Value to sum with
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
        complex& operator+=(const complex<Type>& other);

		/// <summary>
		/// -= on both real and imaginary components by value provided
		/// </summary>
		/// <param name="other">
		/// Value to subtract with
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
        complex& operator-=(const Type& other);

		/// <summary>
		/// -= on both real and imaginary components by the corresponding components in 
		/// the complex value provided
		/// </summary>
		/// <param name="other">
		/// Value to subtract with
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
        complex& operator-=(const complex<Type>& other);

		/// <summary>
		/// *= on both real and imaginary components by value provided
		/// </summary>
		/// <param name="other">
		/// Value to multiply with
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
        complex& operator*=(const Type& other);

		/// <summary>
		/// *= on both real and imaginary components by the corresponding components in 
		/// the complex value provided
		/// </summary>
		/// <param name="other">
		/// Value to multiply with
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
        complex& operator*=(const complex<Type>& other);

		/// <summary>
		/// /= on both real and imaginary components by value provided
		/// </summary>
		/// <param name="other">
		/// Value to divide with
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
        complex& operator/=(const Type& other);

		/// <summary>
		/// /= on both real and imaginary components by the corresponding components in 
		/// the complex value provided
		/// </summary>
		/// <param name="other">
		/// Value to divide with
		/// </param>
		/// <returns>
		/// Reference to this
		/// </returns>
        complex& operator/=(const complex<Type>& other);
    private:
        Type __re;
        Type __im;
    };

    template <typename Type>
    inline constexpr complex<Type>::complex(const Type& re, const Type& im)
        : __re(re), __im(im)
    {
    }

    template <typename Type>
    inline constexpr complex<Type>::complex(const complex<Type>& other)
        : __re(other.__re), __im(other.__im)
    {
    }

    template <typename Type>
    inline Type complex<Type>::real() const
    {
        return __re;
    }

    template <typename Type>
    inline Type complex<Type>::imag() const
    {
        return __im;
    }

    template <typename Type>
    inline void complex<Type>::real(Type value)
    {
        __re = value;
    }

    template <typename Type>
    inline void complex<Type>::imag(Type value)
    {
        __im = value;
    }

    template <typename Type>
    inline complex<Type>& complex<Type>::operator+=(const Type& other)
    {
        __re += other;
        __im += other;
        return *this;
    }

    template <typename Type>
    inline complex<Type>& complex<Type>::operator+=(const complex<Type>& other)
    {
        __re += other.__re;
        __im += other.__im;
        return *this;
    }

    template <typename Type>
    inline complex<Type>& complex<Type>::operator-=(const Type& other)
    {
        __re -= other;
        __im -= other;
        return *this;
    }

    template <typename Type>
    inline complex<Type>& complex<Type>::operator-=(const complex<Type>& other)
    {
        __re -= other.__re;
        __im -= other.__im;
        return *this;
    }

    template <typename Type>
    inline complex<Type>& complex<Type>::operator*=(const Type& other)
    {
        __re *= other;
        __im *= other;
        return *this;
    }

    template <typename Type>
    inline complex<Type>& complex<Type>::operator*=(const complex<Type>& other)
    {
        __re *= other.__re;
        __im *= other.__im;
        return *this;
    }

    template <typename Type>
    inline complex<Type>& complex<Type>::operator/=(const Type& other)
    {
        __re /= other;
        __im /= other;
        return *this;
    }

    template <typename Type>
    inline complex<Type>& complex<Type>::operator/=(const complex<Type>& other)
    {
        __re /= other.__re;
        __im /= other.__im;
        return *this;
    }

	/// <summary>
	/// Addition operator
	/// </summary>
	/// <typeparam name="Type">
	/// Type of complex number
	/// </typeparam>
	/// <param name="lhs">
	/// Left hand argument
	/// </param>
	/// <param name="rhs">
	/// Right hand argument
	/// </param>
	/// <returns>
	/// Summation
	/// </summary>
    template <typename Type>
    inline complex<Type> operator+(const complex<Type>& lhs, const complex<Type>& rhs)
    {
        return complex<Type>(lhs.real() + rhs.real(), lhs.imag() + rhs.imag());
    }

	/// <summary>
	/// Subtraction operator
	/// </summary>
	/// <typeparam name="Type">
	/// Type of complex number
	/// </typeparam>
	/// <param name="lhs">
	/// Left hand argument
	/// </param>
	/// <param name="rhs">
	/// Right hand argument
	/// </param>
	/// <returns>
	/// Difference
	/// </summary>
    template <typename Type>
    inline complex<Type> operator-(const complex<Type>& lhs, const complex<Type>& rhs)
    {
        return complex<Type>(lhs.real() - rhs.real(), lhs.imag() - rhs.imag());
    }

	/// <summary>
	/// Multiplication operator
	/// </summary>
	/// <typeparam name="Type">
	/// Type of complex number
	/// </typeparam>
	/// <param name="lhs">
	/// Left hand argument
	/// </param>
	/// <param name="rhs">
	/// Right hand argument
	/// </param>
	/// <returns>
	/// Product
	/// </summary>
    template <typename Type>
    inline complex<Type> operator*(const complex<Type>& lhs, const complex<Type>& rhs)
    {
        return complex<Type>(lhs.real() * rhs.real(), lhs.imag() * rhs.imag());
    }

	/// <summary>
	/// Division operator
	/// </summary>
	/// <typeparam name="Type">
	/// Type of complex number
	/// </typeparam>
	/// <param name="lhs">
	/// Left hand argument
	/// </param>
	/// <param name="rhs">
	/// Right hand argument
	/// </param>
	/// <returns>
	/// Quotient
	/// </summary>
    template <typename Type>
    inline complex<Type> operator/(const complex<Type>& lhs, const complex<Type>& rhs)
    {
        return complex<Type>(lhs.real() / rhs.real(), lhs.imag() / rhs.imag());
    }

	/// <summary>
	/// Equality operator
	/// </summary>
	/// <typeparam name="Type">
	/// Type of complex number
	/// </typeparam>
	/// <param name="lhs">
	/// Left hand argument
	/// </param>
	/// <param name="rhs">
	/// Right hand argument
	/// </param>
	/// <returns>
	/// True if real and imaginary components in lhs and rhs are equivalent to their 
	/// corresponding component, else false
	/// </summary>
    template <typename Type>
    inline bool operator==(const complex<Type>& lhs, const complex<Type>& rhs)
    {
        return lhs.real() == rhs.real() && lhs.imag() == rhs.imag();
    }

	/// <summary>
	/// Inequality operator
	/// </summary>
	/// <typeparam name="Type">
	/// Type of complex number
	/// </typeparam>
	/// <param name="lhs">
	/// Left hand argument
	/// </param>
	/// <param name="rhs">
	/// Right hand argument
	/// </param>
	/// <returns>
	/// True if real or imaginary components in lhs and rhs are not equivalent to their 
	/// corresponding component, else false
	/// </summary>
    template <typename Type>
    inline bool operator!=(const complex<Type>& lhs, const complex<Type>& rhs)
    {
        return lhs.real() != rhs.real() || lhs.imag() != rhs.imag();
    }

	/// <summary>
	/// Gets real component of the complex value
	/// </summary>
	/// <typeparam name="Type">
	/// Type of value in complex number
	/// </typeparam>
	/// <param name="c">
	/// Complex value
	/// </param>
	/// <returns>
	/// Real component
	/// </returns>
    template <typename Type>
    inline Type real(const complex<Type>& c)
    {
        return c.real();
    }

	/// <summary>
	/// Gets imaginary component of the complex value
	/// </summary>
	/// <typeparam name="Type">
	/// Type of value in complex number
	/// </typeparam>
	/// <param name="c">
	/// Complex value
	/// </param>
	/// <returns>
	/// Imaginary component
	/// </returns>
    template <typename Type>
    inline Type imag(const complex<Type>& c)
    {
        return c.imag();
    }

	/// <summary>
	/// Gets the norm of the complex number
	/// </summary>
	/// <typeparam name="Type">
	/// Type of value in complex number
	/// </typeparam>
	/// <param name="c">
	/// Complex value
	/// </param>
	/// <returns>
	/// Norm of complex number
	/// </returns>
    template <typename Type>
    inline Type abs(const complex<Type>& c)
    {
        return static_cast<Type>(sqrt(static_cast<double>((c.real() * c.real() + c.imag() * c.imag()))));
    }

	/// <summary>
	/// Gets the norm of the complex number
	/// </summary>
	/// <param name="c">
	/// Complex value
	/// </param>
	/// <returns>
	/// Norm of complex number
	/// </returns>
    template <>
    inline float abs(const complex<float>& c)
    {
        return sqrtf(c.real() * c.real() + c.imag() * c.imag());
    }

	/// <summary>
	/// Gets the norm of the complex number
	/// </summary>
	/// <param name="c">
	/// Complex value
	/// </param>
	/// <returns>
	/// Norm of complex number
	/// </returns>
    template <>
    inline double abs(const complex<double>& c)
    {
        return sqrt(c.real() * c.real() + c.imag() * c.imag());
    }

	/// <summary>
	/// Gets the norm of the complex number
	/// </summary>
	/// <param name="c">
	/// Complex value
	/// </param>
	/// <returns>
	/// Norm of complex number
	/// </returns>
    template <>
    inline long double abs(const complex<long double>& c)
    {
        return sqrtl(c.real() * c.real() + c.imag() * c.imag());
    }

	/// <summary>
	/// Gets the phase angle of the complex number
	/// </summary>
	/// <typeparam name="Type">
	/// Type of value in complex number
	/// </typeparam>
	/// <param name="c">
	/// Complex value
	/// </param>
	/// <returns>
	/// Phase angle of complex number
	/// </returns>
    template <typename Type>
    inline Type arg(const complex<Type>& c)
    {
        return static_cast<Type>(atan2(static_cast<double>(c.imag()), static_cast<double>(c.real())));
    }

	/// <summary>
	/// Gets the phase angle of the complex number
	/// </summary>
	/// <param name="c">
	/// Complex value
	/// </param>
	/// <returns>
	/// Phase angle of complex number
	/// </returns>
    template <>
    inline float arg(const complex<float>& c)
    {
        return atan2f(c.imag(), c.real());
    }

	/// <summary>
	/// Gets the phase angle of the complex number
	/// </summary>
	/// <param name="c">
	/// Complex value
	/// </param>
	/// <returns>
	/// Phase angle of complex number
	/// </returns>
    template <>
    inline double arg(const complex<double>& c)
    {
        return atan2(c.imag(), c.real());
    }

	/// <summary>
	/// Gets the phase angle of the complex number
	/// </summary>
	/// <param name="c">
	/// Complex value
	/// </param>
	/// <returns>
	/// Phase angle of complex number
	/// </returns>
    template <>
    inline long double arg(const complex<long double>& c)
    {
        return atan2l(c.imag(), c.real());
    }

	/// <summary>
	/// Gets the field norm of the complex number
	/// </summary>
	/// <typeparam name="Type">
	/// Type of value in complex number
	/// </typeparam>
	/// <param name="c">
	/// Complex value
	/// </param>
	/// <returns>
	/// Field norm of complex number
	/// </returns>
    template <typename Type>
    inline Type norm(const complex<Type>& c)
    {
        return c.real() * c.real() + c.imag() * c.imag();
    }

	/// <summary>
	/// Gets the complex conjugate of the complex number
	/// </summary>
	/// <typeparam name="Type">
	/// Type of value in complex number
	/// </typeparam>
	/// <param name="c">
	/// Complex value
	/// </param>
	/// <returns>
	/// Complex conjugate
	/// </returns>
    template <typename Type>
    inline complex<Type> conj(const complex<Type>& c)
    {
        return complex<Type>(c.real(), -c.imag());
    }

	/// <summary>
	/// Gets the complex number given by the polar values
	/// </summary>
	/// <typeparam name="Type">
	/// Type of value in complex number
	/// </typeparam>
	/// <param name="mag">
	/// Magnitude of polar coordinates
	/// </param>
	/// <param name="theta">
	/// Angle of polar coordinates
	/// </param>
	/// <returns>
	/// Complex number
	/// </returns>
    template <typename Type>
    inline complex<Type> polar(const Type& mag, const Type& theta = Type())
    {
        return complex<Type>(mag * static_cast<Type>(cos(static_cast<double>(theta))), mag * static_cast<Type>(sin(static_cast<double>(theta))));
    }

	/// <summary>
	/// Gets the complex number given by the polar values
	/// </summary>
	/// <param name="mag">
	/// Magnitude of polar coordinates
	/// </param>
	/// <param name="theta">
	/// Angle of polar coordinates
	/// </param>
	/// <returns>
	/// Complex number
	/// </returns>
    template <>
    inline complex<float> polar(const float& mag, const float& theta)
    {
        return complex<float>(mag * cosf(theta), mag * sinf(theta));
    }

	/// <summary>
	/// Gets the complex number given by the polar values
	/// </summary>
	/// <param name="mag">
	/// Magnitude of polar coordinates
	/// </param>
	/// <param name="theta">
	/// Angle of polar coordinates
	/// </param>
	/// <returns>
	/// Complex number
	/// </returns>
    template <>
    inline complex<double> polar(const double& mag, const double& theta)
    {
        return complex<double>(mag * cos(theta), mag * sin(theta));
    }

	/// <summary>
	/// Gets the complex number given by the polar values
	/// </summary>
	/// <param name="mag">
	/// Magnitude of polar coordinates
	/// </param>
	/// <param name="theta">
	/// Angle of polar coordinates
	/// </param>
	/// <returns>
	/// Complex number
	/// </returns>
    template <>
    inline complex<long double> polar(const long double& mag, const long double& theta)
    {
        return complex<long double>(mag * cosl(theta), mag * sinl(theta));
    }
}

#endif