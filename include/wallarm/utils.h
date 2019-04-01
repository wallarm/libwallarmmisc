#ifndef WALLARM_UTILS_H
#define WALLARM_UTILS_H

#ifndef BUILD_BUG_ON
#define BUILD_BUG_ON(cond) ((void)(sizeof(struct{int:-!!(cond);})))
#endif

#define W_SAME_TYPE(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))

#define W_MUST_BE_ARRAY(a) BUILD_BUG_ON(W_SAME_TYPE(a, &(*(a))))

#define W_ARRAY_SIZE(a) (W_MUST_BE_ARRAY(a), (sizeof(a) / sizeof(*(a))))

#define W_SINGLE_ARG(...) __VA_ARGS__

#define W_BUILDIN_RVINT(name, value)                            \
    ({                                                          \
        int rv;                                                 \
                                                                \
        BUILD_BUG_ON(                                           \
            sizeof(value) != sizeof(unsigned int) &&            \
            sizeof(value) != sizeof(unsigned long) &&           \
            sizeof(value) != sizeof(unsigned long long));       \
                                                                \
        if (sizeof(value) == sizeof(unsigned int))              \
            rv = __builtin_##name(value);                       \
        else if (sizeof(value) == sizeof(unsigned long))        \
            rv = __builtin_##name##l(value);                    \
        else                                                    \
            rv = __builtin_##name##ll(value);                   \
        rv;                                                     \
    })

#endif /* WALLARM_UTILS_H */
