#include "enum/detail.hpp"

namespace trlc
{

template<typename T>
struct Enum;

template<typename T, size_t N, class EnumSearchPolicy, class StringSearchPolicy, class UnknownPolicy>
struct EnumHolder;

using DefaultEnum = trlc::Enum<uint64_t>;

template<size_t N>
using DefaultEnumHolder = trlc::EnumHolder<uint64_t, N, trlc::policy::LinearSearchPolicy, trlc::policy::CaseSensitiveStringSearchPolicy, trlc::policy::UnknownPolicy>;

} // namespace trlc