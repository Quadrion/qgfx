#ifndef __fft_h_
#define __fft_h_

#include "qtl/complex.h"
#include "qtl/type_traits.h"
#include "qtl/utility.h"
#include "qtl/valarray.h"
#include "qtl/vector.h"

#include <cstddef>

namespace qtl
{
    const double PI		= 3.141592653589793238463;
    const float  PI_F	= 3.14159265358979f;
    const float  PI2_F	= 1.570796326794895f;

	/// <summary>
	/// Calculates the fourier transform of a 1D array of complex values
	/// </summary>
	/// <typeparam name="Type">
	/// Type of value in complex number array
	/// </typeparam>
	/// <param name="vals">
	/// Complex number array
	/// </param>
	/// <param name="count">
	/// Number of values
	/// </param>
    template <typename Type>
    inline void fft(qtl::complex<Type>* vals, const std::size_t count)
    {
        if (count <= 1)
        {
            return;
        }

        const std::size_t evenCount = (count + 1) / 2;
        const std::size_t oddCount = count / 2;

        qtl::complex<Type>* evens = new qtl::complex<Type>[evenCount];
        qtl::complex<Type>* odds = new qtl::complex<Type>[oddCount];

        for (std::size_t i = 0; i < count; i++)
        {
            if (i % 2 == 0)
            {
                evens[i / 2] = vals[i];
            }
            else
            {
                odds[i / 2] = vals[i];
            }
        }

        fft(evens, evenCount);
        fft(odds, oddCount);

        for (std::size_t k = 0; k < count; ++k)
        {
            qtl::complex<Type> t = qtl::polar(1.0, -2.0 * PI * k / count) * odds[k];
            vals[k] = evens[k] + t;
            vals[k + count / 2] = evens[k] - t;
        }
    }

	/// <summary>
	/// Calculates the fourier transform of a 1D array of complex values
	/// </summary>
	/// <param name="vals">
	/// Complex number array
	/// </param>
	/// <param name="count">
	/// Number of values
	/// </param>
	template<>
	void fft(qtl::complex<float>* vals, const std::size_t count);

	/// <summary>
	/// Calculates the fourier transform of a 1D array of complex values
	/// </summary>
	/// <param name="vals">
	/// Complex number array
	/// </param>
	/// <param name="count">
	/// Number of values
	/// </param>
	template<>
	void fft(qtl::complex<double>* vals, const std::size_t count);
	
	/// <summary>
	/// Calculates the fourier transform of a 1D array of complex values
	/// </summary>
	/// <param name="vals">
	/// Complex number array
	/// </param>
	/// <param name="count">
	/// Number of values
	/// </param>
	template<>
	void fft(qtl::complex<long double>* vals, const std::size_t count);

	/// <summary>
	/// Calculates the fourier transform of a 1D array of complex values
	/// </summary>
	/// <param name="vals">
	/// Complex number array
	/// </param>
	void fft(qtl::valarray<qtl::complex<float>>& vals);

	/// <summary>
	/// Calculates the fourier transform of a 1D array of complex values
	/// </summary>
	/// <param name="vals">
	/// Complex number array
	/// </param>
	void fft(qtl::valarray<qtl::complex<double>>& vals);

	/// <summary>
	/// Calculates the fourier transform of a 1D array of complex values
	/// </summary>
	/// <param name="vals">
	/// Complex number array
	/// </param>
	void fft(qtl::valarray<qtl::complex<long double>>& vals);
}

#endif
