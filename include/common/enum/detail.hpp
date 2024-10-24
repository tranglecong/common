#include <algorithm>
#include <array>
#include <iostream>
#include <string_view>

namespace trlc
{

/**
 * @brief Represents an enumeration with a value and a name.
 *
 * @tparam T The type of the enum value.
 */
template<typename T>
struct Enum
{
    T value{};               ///< The enum value.
    std::string_view name{}; ///< The name associated with the enum value.

    /**
     * @brief Converts the Enum to its underlying value type.
     *
     * @return The underlying enum value.
     */
    constexpr operator T() const
    {
        return value;
    }

    /**
     * @brief Compare two enums using the == operator.
     *
     * @return `true` if both the value and name are equal, otherwise, return `false`
     */
    constexpr bool operator==(const Enum<T>& other)
    {
        if (value == other.value && name == other.name)
        {
            return true;
        }
        return false;
    }
};

namespace
{
/**
 * @brief Returns a default unknown enum value.
 *
 * @tparam T The type of the enum value.
 * @return An Enum of type T with default values.
 */
template<typename T>
constexpr Enum<T> default_unknown_enum()
{
    return Enum<T>{};
}
} // namespace

/**
 * @brief Holds an array of Enum entries and provides methods to retrieve them.
 *
 * @tparam T The type of the enum value.
 * @tparam N The number of enum entries.
 * @tparam EnumSearchPolicy The policy for searching by value.
 * @tparam StringSearchPolicy The policy for searching by name.
 * @tparam UnknownPolicy The policy for handling unknown enums.
 */
template<typename T, size_t N, class EnumSearchPolicy, class StringSearchPolicy, class UnknownPolicy>
struct EnumHolder
{
    /**
     * @brief Retrieves an Enum entry from a value.
     *
     * @param value The enum value to search for.
     * @return The corresponding Enum entry.
     */
    constexpr Enum<T> fromValue(T value) const
    {
        Enum<T> result{EnumSearchPolicy::template search<T, N>(value, m_entries)};
        if (result == default_unknown_enum<T>())
        {
            result = UnknownPolicy::template handle<T>(value, m_entries);
        }
        return result;
    }

    /**
     * @brief Retrieves an Enum entry from a string name.
     *
     * @param name The name to search for.
     * @return The corresponding Enum entry.
     */
    constexpr Enum<T> fromString(std::string_view name) const
    {
        Enum<T> result{StringSearchPolicy::template search<T, N>(name, m_entries)};
        if (result == default_unknown_enum<T>())
        {
            result = UnknownPolicy::template handle<T>(name, m_entries);
        }
        return result;
    }

    /**
     * @brief Retrieves all Enum values.
     *
     * @return An array of all Enum values.
     */
    constexpr const std::array<Enum<T>, N> allValues() const
    {
        return m_entries;
    }

    const std::array<Enum<T>, N>& m_entries; ///< The array of Enum entries.
};

namespace policy
{
// LinearSearchPolicy definition
/**
 * @brief Search policy that uses linear search to find an enum entry by value.
 */
struct LinearSearchPolicy
{
    /**
     * @brief Searches for an Enum entry by value.
     *
     * @param value The value to search for.
     * @param entries The array of Enum entries.
     * @return The corresponding Enum entry.
     */
    template<typename T, size_t N>
    static constexpr Enum<T> search(T value, const std::array<Enum<T>, N>& entries)
    {
        Enum<T> result{default_unknown_enum<T>()};
        for (const auto& entry : entries)
        {
            if (entry.value == value)
            {
                result = entry;
                break;
            }
        }
        return result;
    }
};

// SortedSearchPolicy definition
/**
 * @brief Search policy that uses binary search to find an enum entry by value.
 */
struct SortedSearchPolicy
{
    /**
     * @brief Searches for an Enum entry by value using binary search.
     *
     * @param value The value to search for.
     * @param entries The array of Enum entries.
     * @return The corresponding Enum entry.
     */
    template<typename T, size_t N>
    static constexpr Enum<T> search(T value, const std::array<Enum<T>, N>& entries)
    {
        // Perform binary search
        Enum<T> result{default_unknown_enum<T>()};
        size_t left = 0;
        size_t right = N - 1;

        while (left <= right)
        {
            size_t mid = left + (right - left) / 2;

            if (entries[mid].value == value)
            {
                result = entries[mid];
                break;
            }
            if (entries[mid].value < value)
            {
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
        }
        return result;
    }
};

// CaseSensitiveSearchPolicy definition
/**
 * @brief Search policy that uses case-sensitive comparison to find an enum entry by name.
 */
struct CaseSensitiveStringSearchPolicy
{
    /**
     * @brief Searches for an Enum entry by name.
     *
     * @param name The name to search for.
     * @param entries The array of Enum entries.
     * @return The corresponding Enum entry.
     */
    template<typename T, size_t N>
    static constexpr Enum<T> search(std::string_view name, const std::array<Enum<T>, N>& entries)
    {
        Enum<T> result{default_unknown_enum<T>()};
        for (const auto& entry : entries)
        {
            if (entry.name == name)
            {
                result = entry;
                break; // Added break here to stop on first match
            }
        }

        return result;
    }
};

// CaseInsensitiveSearchPolicy definition
/**
 * @brief Search policy that uses case-insensitive comparison to find an enum entry by name.
 */
struct CaseInsensitiveStringSearchPolicy
{
    /**
     * @brief Compares two characters for case-insensitive equality.
     *
     * @param a The first character.
     * @param b The second character.
     * @return True if the characters are equal, false otherwise.
     */
    static constexpr bool caseInsensitiveEqual(char a, char b)
    {
        return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b));
    }

    /**
     * @brief Searches for an Enum entry by name using case-insensitive comparison.
     *
     * @param name The name to search for.
     * @param entries The array of Enum entries.
     * @return The corresponding Enum entry.
     */
    template<typename T, size_t N>
    static constexpr Enum<T> search(std::string_view name, const std::array<Enum<T>, N>& entries)
    {
        Enum<T> result{default_unknown_enum<T>()};
        for (const auto& entry : entries)
        {
            if (name.size() == entry.name.size() && std::equal(entry.name.begin(), entry.name.end(), name.begin(), caseInsensitiveEqual))
            {
                result = entry;
                break; // Added break here to stop on first match
            }
        }
        return result;
    }
};

/**
 * @brief Policy for handling unknown enum values.
 */
struct UnknownPolicy
{
    /**
     * @brief Handles an unknown enum name.
     *
     * @param name The name of the unknown enum.
     * @param entries The array of Enum entries.
     * @return A default unknown Enum entry.
     */
    template<typename T, size_t N>
    static constexpr Enum<T> handle([[maybe_unused]] std::string_view name, [[maybe_unused]] const std::array<Enum<T>, N>& entries)
    {
        return default_unknown_enum<T>();
    }

    /**
     * @brief Handles an unknown enum value.
     *
     * @param value The unknown enum value.
     * @param entries The array of Enum entries.
     * @return A default unknown Enum entry.
     */
    template<typename T, size_t N>
    static constexpr Enum<T> handle([[maybe_unused]] T value, [[maybe_unused]] const std::array<Enum<T>, N>& entries)
    {
        return default_unknown_enum<T>();
    }
};
} // namespace policy
} // namespace trlc