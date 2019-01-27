#ifndef __algorithm_h_
#define __algorithm_h_

namespace qtl
{
    /// <summary>
    /// Searches through a set of iterators to find a value.  Complexity O(1)
    /// </summary>
    /// <typeparam name="IteratorType">
    /// Type of iterator
    /// </typeparam>
    /// <typeparam name="Type">
    /// Type to search for
    /// </typeparam>
    /// <param name="first">
    /// Starting search position
    /// </param>
    /// <param name="last">
    /// Ending search position (exclusive)
    /// </param>
    /// <param name="value">
    /// Value to search for
    /// </param>
    /// <returns>
    /// Iterator containing value.  If value not found, returns last
    /// </returns>
    template <typename IteratorType, typename Type>
    IteratorType find(IteratorType first, IteratorType last, const Type& value) // works best for unordered structures
    {
        while (first != last)
        {
            if (*first == value)
            {
                return first;
            }
            ++first;
        }
        return last;
    }
}

#endif