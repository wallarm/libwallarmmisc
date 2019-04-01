#ifndef WALLARM_WORDMASK_H
#define WALLARM_WORDMASK_H

#include "utils.h"
#include <stdint.h>

#define W_INT_FOREACH_(pos, var, extra_decl_list, extra_chk, build_chk) \
    for (unsigned pos_, off_ = 0, var_ = (var) extra_decl_list;         \
         (build_chk), (pos_ = ffs(var_)) != 0 || (extra_chk);           \
         var_ &= (var_ - 1))                                            \
        if ((pos) = pos_ + off_ - 1, 1)

#define W_INT_FOREACH(pos, var) W_INT_FOREACH_(pos, var, , 0, 0)

#define W_UINT32_FOREACH_(pos, u32, extra_decl_list, extra_chk)         \
    W_INT_FOREACH_(                                                     \
        pos, (unsigned)((u32) & 0xffffffff),                            \
        W_SINGLE_ARG(extra_decl_list), extra_chk,                       \
        BUILD_BUG_ON(sizeof(uint32_t) > sizeof(unsigned)))

#define W_UINT32_FOREACH(pos, u32) W_UINT32_FOREACH_(pos, u32, , 0)

/* CAUTION! u64 is used twice in the macro */
#define W_UINT64_FOREACH(pos, u64)                                      \
    W_UINT32_FOREACH_(                                                  \
        pos,                                                            \
        (u64) & 0xffffffff,                                             \
        W_SINGLE_ARG(, hivar_inuse_ = 0),                               \
        !hivar_inuse_ &&                                                \
        (off_ = 32, hivar_inuse_ = 1, var_ = (uint64_t)(u64) >> 32,     \
         (pos_ = ffs(var_)) != 0))

#endif /* WALLARM_WORDMASK_H */
