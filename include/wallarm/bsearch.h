#ifndef WALLARM_BSEARCH_H
#define WALLARM_BSEARCH_H

#include <stddef.h>
#include <stdbool.h>

/*
 * Array binary search algorithm working with objects of known sizes.
 *
 * comparemacro should use wbsearch_elm variable as the current
 * element pointer, and should return:
 * < 0, if wbsearch_elm < compared_value
 * > 0, if wbsearch_elm > compared_value
 * 0, if wbsearch_elm == compared_value
 *
 * The macro returns a boolean value:
 * true, if the element is found
 *     *(pres) is set to the index of the found element
 * false, if the element is not found
 *     *(pres) is set to the index the new element should be inserted to
 */
#define WALLARM_BSEARCH(pres, array, len, comparemacro)         \
    ({                                                          \
        __label__ done;                                         \
        typeof(&array[0]) wbsearch_elm;                         \
        typeof(&array[0]) wbsearch_array = &(array)[0];         \
        typeof(len) wbsearch_from, wbsearch_to;                 \
        typeof(len) wbsearch_len = (len);                       \
        ptrdiff_t wbsearch_test;                                \
        bool wbsearch_found;                                    \
                                                                \
        if ((len) == 0) {                                       \
            *(pres) = 0;                                        \
            wbsearch_found = false;                             \
            goto done;                                          \
        }                                                       \
        wbsearch_elm = &wbsearch_array[0];                      \
        wbsearch_test = comparemacro;                           \
        if (wbsearch_test == 0) {                               \
            *(pres) = 0;                                        \
            wbsearch_found = true;                              \
            goto done;                                          \
        } else if (wbsearch_test > 0) {                         \
            *(pres) = 0;                                        \
            wbsearch_found = false;                             \
            goto done;                                          \
        }                                                       \
        wbsearch_from = 0;                                      \
        wbsearch_elm = &wbsearch_array[wbsearch_len - 1];       \
        wbsearch_test = comparemacro;                           \
        if (wbsearch_test == 0) {                               \
            *(pres) = wbsearch_len - 1;                         \
            wbsearch_found = true;                              \
            goto done;                                          \
        } else if (wbsearch_test < 0) {                         \
            *(pres) = wbsearch_len;                             \
            wbsearch_found = false;                             \
            goto done;                                          \
        }                                                       \
        wbsearch_to = wbsearch_len - 1;                         \
        while (wbsearch_to - wbsearch_from > 1) {               \
            typeof(len) wbsearch_idx;                           \
                                                                \
            wbsearch_idx = wbsearch_from +                      \
                (wbsearch_to - wbsearch_from) / 2;              \
            wbsearch_elm = &wbsearch_array[wbsearch_idx];       \
            wbsearch_test = comparemacro;                       \
            if (wbsearch_test == 0) {                           \
                *(pres) = wbsearch_idx;                         \
                wbsearch_found = true;                          \
                goto done;                                      \
            } else if (wbsearch_test > 0)                       \
                wbsearch_to = wbsearch_idx;                     \
            else                                                \
                wbsearch_from = wbsearch_idx;                   \
        }                                                       \
        *(pres) = wbsearch_from + 1;                            \
        wbsearch_found = false;                                 \
      done:                                                     \
        wbsearch_found;                                         \
    })

#endif /* WALLARM_BSEARCH_H */
