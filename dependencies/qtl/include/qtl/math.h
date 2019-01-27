#ifndef math_h__
#define math_h__

/// <summary>
/// Gets the maximum value between two values
/// </summary>
/// <param name="left">
/// Left hand value
/// </param>
/// <param name="right">
/// Right hand value
/// </param>
/// <returns>
/// Maximum between left and right values
/// </returns>
template <typename T>
inline T max(const T left, const T right)
{
	return left < right ? right : left;
}

/// <summary>
/// Gets the minimum value between two values
/// </summary>
/// <param name="left">
/// Left hand value
/// </param>
/// <param name="right">
/// Right hand value
/// </param>
/// <returns>
/// Minimum between left and right values
/// </returns>
template <typename T>
inline T min(const T left, const T right)
{
	return left > right ? right : left;
}

#endif