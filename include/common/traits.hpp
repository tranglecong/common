#pragma once
#include <type_traits>

#define CREATE_HAS_COMPARISON_OPERATOR_TRAIT(op, name)             \
    template<typename T, typename... Args>                         \
    auto t_class_support_##name##_operator(int)->decltype(         \
        std::declval<T>() op std::declval<T>(), std::true_type{}); \
    template<typename, typename...>                                \
    auto t_class_support_##name##_operator(...)->std::false_type;  \
    template<typename T, typename... Args>                         \
    inline constexpr bool has_##name##_operator_for_t_class =      \
        decltype(t_class_support_##name##_operator<T, Args...>(0))::value;

CREATE_HAS_COMPARISON_OPERATOR_TRAIT(==, equal);
CREATE_HAS_COMPARISON_OPERATOR_TRAIT(!=, not_equal);
CREATE_HAS_COMPARISON_OPERATOR_TRAIT(<, less);
CREATE_HAS_COMPARISON_OPERATOR_TRAIT(<=, less_or_equal);
CREATE_HAS_COMPARISON_OPERATOR_TRAIT(>, greater);
CREATE_HAS_COMPARISON_OPERATOR_TRAIT(>=, greater_or_equal);

#define CREATE_HAS_MEMBER_FUNCTION_TRAIT(funcname)                                                           \
    template<typename, typename T>                                                                           \
    struct has_##funcname##_member_function : std::false_type                                                \
    {                                                                                                        \
    };                                                                                                       \
    template<typename C, typename Ret, typename... Args>                                                     \
    struct has_##funcname##_member_function<C, Ret(Args...)>                                                 \
    {                                                                                                        \
    private:                                                                                                 \
        template<typename T>                                                                                 \
        static auto test(T*)                                                                                 \
            -> decltype(std::declval<T>().funcname(std::declval<Args>()...),                                 \
                        std::is_same<Ret, decltype(std::declval<T>().funcname(std::declval<Args>()...))>{}); \
        template<typename>                                                                                   \
        static std::false_type test(...);                                                                    \
                                                                                                             \
    public:                                                                                                  \
        static constexpr bool value = decltype(test<C>(nullptr))::value;                                     \
    };

#define SFINAE_STATIC_POLYMOROHISM_IMPL(derived, funcname, ret, ...)                                                             \
    auto funcname(__VA_ARGS__)->::std::enable_if_t<Traits::has_##funcname##_member_function<derived, ret(__VA_ARGS__)>()>, ret > \
    {                                                                                                                            \
        return static_cast<T>(this)->funcname();                                                                                 \
    }

#define SFINAE_STATIC_POLYMOROHISM_DEFAULT_IMPL(derived, funcname, ret, ...)                                                      \
    auto funcname(__VA_ARGS__)->::std::enable_if_t<!Traits::has_##funcname##_member_function<derived, ret(__VA_ARGS__)>()>, ret > \
    {                                                                                                                             \
        return;                                                                                                                   \
    }
