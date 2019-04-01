#include <wallarm/tree.h>
#include <wallarm/bsearch.h>
#include <wallarm/wordmask.h>
#include <CUnit/Basic.h>
#include <stdlib.h>
#include <stdbool.h>

struct test_tree_entry {
    int value;
    int key;
    RB_ENTRY(test_tree_entry) link;
};

RB_HEAD(test_tree, test_tree_entry);

#define TEST_TREE_KEYCMP(a, b) ((a) - (b))
#define TEST_TREE_ELM2KEY(elm) ((elm)->key)

WRB_PROTOTYPE_STATIC(
    test_tree, test_tree_entry, int);
WRB_GENERATE_STATIC(
    test_tree, test_tree_entry, int, link,
    TEST_TREE_KEYCMP, TEST_TREE_ELM2KEY);

static int
test_intcmp(const void *pa, const void *pb)
{
    const int *a = pa, *b = pb;

    return (TEST_TREE_KEYCMP(*a, *b));
}

static void
test_rbtree(void)
{
    int inserts[] =
        {10, 5, 8, 15, -5, 2, 7, 11, 12, 13, 14, 16};
    static const int nonexists[] =
        {-6, 4, 3, 17};
    static const struct {
        int key;
        int fkey;
        bool fail;
    } nfinds[] = {
        {.key = -6, .fkey = -5},
        {.key = -5, .fkey = -5},
        {.key = -4, .fkey = 2},
        {.key = 4, .fkey = 5},
        {.key = 5, .fkey = 5},
        {.key = 15, .fkey = 15},
        {.key = 16, .fkey = 16},
        {.key = 17, .fail = true},
    };
    struct test_tree tree = RB_INITIALIZER(tree);
    struct test_tree_entry *e, *e2;
    unsigned i;

    for (i = 0; i != sizeof(inserts) / sizeof(inserts[0]); i++) {
        CU_ASSERT_FATAL((e = malloc(sizeof(*e))) != NULL);
        e->key = inserts[i];
        e->value = inserts[i] + 10;
        CU_ASSERT_FATAL(RB_INSERT(test_tree, &tree, e) == NULL);
        e = WRB_FIND(test_tree, &tree, inserts[i]);
        CU_ASSERT_FATAL(e != NULL);
        CU_ASSERT_FATAL(e->key == inserts[i]);
    }
    for (i = 0; i != sizeof(nonexists) / sizeof(nonexists[0]); i++) {
        e = WRB_FIND(test_tree, &tree, nonexists[i]);
        CU_ASSERT_FATAL(e == NULL);
    }
    for (i = 0; i != sizeof(inserts) / sizeof(inserts[0]); i++) {
        e = WRB_FIND(test_tree, &tree, inserts[i]);
        e2 = RB_INSERT(test_tree, &tree, e);
        CU_ASSERT_FATAL(e != NULL);
        CU_ASSERT_FATAL(e2 == e);
        CU_ASSERT_FATAL(e->key == inserts[i]);
        CU_ASSERT_FATAL(e->value == e->key + 10);
    }
    for (i = 0; i != sizeof(nfinds) / sizeof(nfinds[0]); i++) {
        e = WRB_FIND_GE(test_tree, &tree, nfinds[i].key);
        if (nfinds[i].fail) {
            CU_ASSERT_FATAL(e == NULL);
        } else {
            CU_ASSERT_FATAL(e != NULL);
            CU_ASSERT_FATAL(e->key == nfinds[i].fkey);
        }
    }

    qsort(inserts, sizeof(inserts) / sizeof(inserts[0]), sizeof(inserts[0]),
          test_intcmp);

    for (i = 0, e = RB_MIN(test_tree, &tree);
         i != sizeof(inserts) / sizeof(inserts[0]); i++,
             e = WRB_NEXT(test_tree, e)) {
        CU_ASSERT_FATAL(e != NULL);
        CU_ASSERT_FATAL(e->key == inserts[i]);
    }
    CU_ASSERT_FATAL(e == NULL);

    WRB_FOREACH_PDFS(e, test_tree, &tree, e2)
        free(e);
}

static int
s_cunit_run_tests(int test_arg, int argc, char **argv)
{
    CU_pTestRegistry registry = CU_get_registry();
    char *suite_name, *test_name;
    CU_ErrorCode rv;
    CU_pSuite suite;
    CU_pTest test;

    if (test_arg >= argc) {
        rv = CU_basic_run_tests();
        goto done;
    }
    suite_name = argv[test_arg];
    if ((test_name = strchr(suite_name, '.')) != NULL) {
        *test_name = '\0';
        test_name++;
    }
    if ((suite = CU_get_suite_by_name(suite_name, registry)) == NULL)
        return (EXIT_FAILURE);
    if (test_name == NULL) {
        rv = CU_basic_run_suite(suite);
        goto done;
    }
    if ((test = CU_get_test_by_name(test_name, suite)) == NULL)
        return (EXIT_FAILURE);
    rv = CU_basic_run_test(suite, test);
  done:
    if (rv != CUE_SUCCESS)
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

static void
test_bsearch(void)
{
#define IARR(...) \
    .arr = (int []){__VA_ARGS__}, \
    .len = sizeof((int []){__VA_ARGS__}) / sizeof(int)

    struct {
        int *arr;
        unsigned len;
        int key;
        unsigned res;
        bool found;
    } tests[] = {
        {IARR(), .key = 5},
        {IARR(5), .key = 4},
        {IARR(5), .key = 5, .found = true},
        {IARR(5), .key = 6, .res = 1},
        {IARR(5, 7), .key = 4},
        {IARR(5, 7), .key = 5, .found = true},
        {IARR(5, 7), .key = 6, .res = 1},
        {IARR(5, 7), .key = 7, .found = true, .res = 1},
        {IARR(5, 7), .key = 8, .res = 2},
        {IARR(5, 7, 9), .key = 4},
        {IARR(5, 7, 9), .key = 5, .found = true},
        {IARR(5, 7, 9), .key = 6, .res = 1},
        {IARR(5, 7, 9), .key = 7, .found = true, .res = 1},
        {IARR(5, 7, 9), .key = 8, .res = 2},
        {IARR(5, 7, 9), .key = 9, .found = true, .res = 2},
        {IARR(5, 7, 9), .key = 10, .res = 3},
        {IARR(5, 7, 9, 11), .key = 4},
        {IARR(5, 7, 9, 11), .key = 5, .found = true},
        {IARR(5, 7, 9, 11), .key = 6, .res = 1},
        {IARR(5, 7, 9, 11), .key = 7, .found = true, .res = 1},
        {IARR(5, 7, 9, 11), .key = 8, .res = 2},
        {IARR(5, 7, 9, 11), .key = 9, .found = true, .res = 2},
        {IARR(5, 7, 9, 11), .key = 10, .res = 3},
        {IARR(5, 7, 9, 11), .key = 11, .found = true, .res = 3},
        {IARR(5, 7, 9, 11), .key = 12, .res = 4},
        {IARR(5, 7, 9, 11, 13), .key = 4},
        {IARR(5, 7, 9, 11, 13), .key = 5, .found = true},
        {IARR(5, 7, 9, 11, 13), .key = 6, .res = 1},
        {IARR(5, 7, 9, 11, 13), .key = 7, .found = true, .res = 1},
        {IARR(5, 7, 9, 11, 13), .key = 8, .res = 2},
        {IARR(5, 7, 9, 11, 13), .key = 9, .found = true, .res = 2},
        {IARR(5, 7, 9, 11, 13), .key = 10, .res = 3},
        {IARR(5, 7, 9, 11, 13), .key = 11, .found = true, .res = 3},
        {IARR(5, 7, 9, 11, 13), .key = 12, .res = 4},
        {IARR(5, 7, 9, 11, 13), .key = 13, .found = true, .res = 4},
        {IARR(5, 7, 9, 11, 13), .key = 14, .res = 5},
        {IARR(5, 7, 9, 11, 13, 15), .key = 4},
        {IARR(5, 7, 9, 11, 13, 15), .key = 5, .found = true},
        {IARR(5, 7, 9, 11, 13, 15), .key = 6, .res = 1},
        {IARR(5, 7, 9, 11, 13, 15), .key = 7, .found = true, .res = 1},
        {IARR(5, 7, 9, 11, 13, 15), .key = 8, .res = 2},
        {IARR(5, 7, 9, 11, 13, 15), .key = 9, .found = true, .res = 2},
        {IARR(5, 7, 9, 11, 13, 15), .key = 10, .res = 3},
        {IARR(5, 7, 9, 11, 13, 15), .key = 11, .found = true, .res = 3},
        {IARR(5, 7, 9, 11, 13, 15), .key = 12, .res = 4},
        {IARR(5, 7, 9, 11, 13, 15), .key = 13, .found = true, .res = 4},
        {IARR(5, 7, 9, 11, 13, 15), .key = 14, .res = 5},
        {IARR(5, 7, 9, 11, 13, 15), .key = 15, .found = true, .res = 5},
        {IARR(5, 7, 9, 11, 13, 15), .key = 16, .res = 6},
    };
#undef IARR

    for (unsigned i = 0; i != sizeof(tests) / sizeof(tests[0]); i++) {
        bool found;
        unsigned res;

        found = WALLARM_BSEARCH(
            &res, tests[i].arr, tests[i].len, *wbsearch_elm - tests[i].key);
        CU_ASSERT_EQUAL(found, tests[i].found);
        CU_ASSERT_EQUAL(res, tests[i].res);
    }
}

static void
test_uint64_foreach(void)
{
    struct {
        uint64_t input;
        uint64_t output;
        unsigned break_pos;
    } tests[] = {
        {.input = 0x0, .output = 0x0, .break_pos = -1},
        {.input = 0x1, .output = 0x1, .break_pos = -1},
        {.input = 0xff0f0f0f, .output = 0xff0f0f0f, .break_pos = -1},
        {.input =  0xff0f0f0f0f0f0f0fULL,
         .output = 0xff0f0f0f0f0f0f0fULL, .break_pos = -1},
        {.input =  0xff0f0f0f0f0f0f0fULL,
         .output = 0x1, .break_pos = 0},
        {.input =  0xff0f0f0f0f0f0f0fULL,
         .output = 0x3, .break_pos = 1},
        {.input =  0xff0f0f0f0f0f0f0fULL,
         .output = 0x7f0f0f0f0f0f0f0fULL, .break_pos = 62},
        {.input =  0xff0f0f0f0f0f0f0fULL,
         .output = 0xff0f0f0f0f0f0f0fULL, .break_pos = 63},
        {.input = 0x80000000ULL, .output = 0x80000000ULL, .break_pos = -1},
    };

    for (unsigned i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
        uint64_t rv = 0;
        unsigned pos;
        unsigned break_pos = -1;

        W_UINT64_FOREACH(pos, tests[i].input) {
            rv |= ((uint64_t)0x1) << pos;
            if (pos == tests[i].break_pos) {
                break_pos = pos;
                break;
            }
        }
        CU_ASSERT_EQUAL(break_pos, tests[i].break_pos);
        CU_ASSERT_EQUAL(rv, tests[i].output);
    }
}

int
main(int argc, char **argv)
{
    CU_pRunSummary sum;
    CU_TestInfo all_array[] = {
        {"rbtree", test_rbtree},
        {"bsearch", test_bsearch},
        {"uint64_foreach", test_uint64_foreach},
        CU_TEST_INFO_NULL
    };
    CU_SuiteInfo suites[] = {
        {.pName = "all", .pTests = all_array},
        CU_SUITE_INFO_NULL,
    };
    int rc;

    if (CU_initialize_registry() != CUE_SUCCESS)
        return (EXIT_FAILURE);

    CU_register_suites(suites);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    rc = s_cunit_run_tests(1, argc, argv);
    if (rc != EXIT_SUCCESS || (sum = CU_get_run_summary()) == NULL ||
        sum->nSuitesFailed || sum->nTestsFailed || sum->nAssertsFailed) {

        CU_cleanup_registry();
        return (EXIT_FAILURE);
    }
    CU_cleanup_registry();
    return (EXIT_SUCCESS);
}
