#ifndef WALLARM_TREE_H
#define WALLARM_TREE_H

#include <wallarm/config.h>
#ifdef HAVE_SYS_TREE_H
#include <sys/tree.h>
#else
#include <wallarm/sys/tree.h>
#endif

#define	WRB_PROTOTYPE(name, type, keytype)	\
	WRB_PROTOTYPE_INTERNAL(name, type, keytype,)
#define	WRB_PROTOTYPE_STATIC(name, type, keytype)	\
	WRB_PROTOTYPE_INTERNAL(name, type, keytype, __attribute__((__unused__)) static)
#define WRB_PROTOTYPE_INTERNAL(name, type, keytype, attr)		\
	WRB_PROTOTYPE_PDFS_START(name, type, attr);			\
	WRB_PROTOTYPE_PDFS_NEXT(name, type, attr);			\
	WRB_PROTOTYPE_CMP(name, type, attr);				\
	WRB_PROTOTYPE_FIND(name, type, keytype, attr);			\
	WRB_PROTOTYPE_FIND_GE(name, type, keytype, attr);		\
	WRB_PROTOTYPE_FIND_GE_TEST(name, type, keytype, attr);		\
	WRB_PROTOTYPE_FIND_FIRST(name, type, keytype, attr);		\
	WRB_PROTOTYPE_FIND_NEXT(name, type, keytype, attr);		\
	RB_PROTOTYPE_INTERNAL(name, type, , name##_WRB_CMP, attr)

#define WRB_PROTOTYPE_CMP(name, type, attr)	\
	attr int name##_WRB_CMP(const struct type *a, const struct type *b)
#define WRB_PROTOTYPE_FIND(name, type, keytype, attr)	\
	attr struct type *name##_WRB_FIND(const struct name *, const keytype)
#define WRB_PROTOTYPE_FIND_GE(name, type, keytype, attr)	\
	attr struct type *name##_WRB_FIND_GE(const struct name *, const keytype)
#define WRB_PROTOTYPE_FIND_GE_TEST(name, type, keytype, attr)	\
	attr struct type *name##_WRB_FIND_GE_TEST(const struct name *, const keytype, int *)
#define WRB_PROTOTYPE_FIND_FIRST(name, type, keytype, attr)	\
	attr struct type *name##_WRB_FIND_FIRST(const struct name *, struct type **, const keytype)
#define WRB_PROTOTYPE_FIND_NEXT(name, type, keytype, attr)	\
	attr struct type *name##_WRB_FIND_NEXT(struct type **, const keytype)
#define WRB_PROTOTYPE_PDFS_START(name, type, attr)	\
	attr struct type *name##_WRB_PDFS_START(struct type *)
#define WRB_PROTOTYPE_PDFS_NEXT(name, type, attr)	\
	attr struct type *name##_WRB_PDFS_NEXT(struct type *)

#define WRB_GENERATE(name, type, keytype, field, keycmp, elm2key)	\
	WRB_GENERATE_INTERNAL(name, type, keytype, field, keycmp, elm2key,)
#define WRB_GENERATE_STATIC(name, type, keytype, field, keycmp, elm2key)	\
	WRB_GENERATE_INTERNAL(name, type, keytype, field, keycmp, elm2key, __attribute__((__unused__)) static)
#define WRB_GENERATE_INTERNAL(name, type, keytype, field, keycmp, elm2key, attr)	\
	WRB_GENERATE_PDFS_START(name, type, field, attr);		\
	WRB_GENERATE_PDFS_NEXT(name, type, field, attr);		\
	WRB_GENERATE_CMP(name, type, keycmp, elm2key, attr);		\
	WRB_GENERATE_FIND(name, type, keytype, field, keycmp, elm2key, attr)	\
	WRB_GENERATE_FIND_GE_TEST(name, type, keytype, field, keycmp, elm2key, attr)	\
	WRB_GENERATE_FIND_GE(name, type, keytype, field, keycmp, elm2key, attr)	\
	WRB_GENERATE_FIND_FIRST(name, type, keytype, field, keycmp, elm2key, attr)	\
	WRB_GENERATE_FIND_NEXT(name, type, keytype, field, keycmp, elm2key, attr)	\
	RB_GENERATE_INTERNAL(name, type, field, name##_WRB_CMP, attr)

#define WRB_GENERATE_FIND_GE_TEST(name, type, keytype, field, keycmp, elm2key, attr)	\
/* Finds the first node greater than or equal to the search key */	\
attr struct type *							\
name##_WRB_FIND_GE_TEST(const struct name *head, const keytype key, int *pfound)	\
{									\
	struct type *tmp = RB_ROOT(head);				\
	struct type *res = NULL;					\
	while (tmp) {							\
		int comp = keycmp(key, elm2key(tmp));			\
		if (comp < 0) {						\
			res = tmp;					\
			tmp = RB_LEFT(tmp, field);			\
		}							\
		else if (comp > 0)					\
			tmp = RB_RIGHT(tmp, field);			\
		else {							\
			if (pfound != NULL)				\
				*pfound = 1;				\
			return (tmp);					\
		}							\
	}								\
	if (pfound != NULL)						\
		*pfound = 0;						\
	return (res);							\
}

#define WRB_GENERATE_FIND_GE(name, type, keytype, field, keycmp, elm2key, attr)	\
/* Finds the first node greater than or equal to the search key */	\
attr struct type *							\
name##_WRB_FIND_GE(const struct name *head, const keytype key)		\
{									\
	return (name##_WRB_FIND_GE_TEST(head, key, NULL));		\
}

#define WRB_GENERATE_FIND(name, type, keytype, field, keycmp, elm2key, attr)	\
/* Finds the node with the same key as elm */				\
attr struct type *							\
name##_WRB_FIND(const struct name *head, const keytype key)		\
{									\
	struct type *tmp = RB_ROOT(head);				\
	while (tmp) {							\
		int comp = keycmp(key, elm2key(tmp));			\
		if (comp < 0)						\
			tmp = RB_LEFT(tmp, field);			\
		else if (comp > 0)					\
			tmp = RB_RIGHT(tmp, field);			\
		else							\
			return (tmp);					\
	}								\
	return (NULL);							\
}

#define WRB_GENERATE_CMP(name, type, keycmp, elm2key, attr)		\
attr int								\
name##_WRB_CMP(const struct type *a, const struct type *b)		\
{									\
    return (keycmp(elm2key(a), elm2key(b)));				\
}

#define WRB_GENERATE_FIND_FIRST(name, type, keytype, field, keycmp, elm2key, attr)	\
/* 									\
 * Start search of an ordered set of keys. Returns a node by the	\
 * first key or NULL if the node is not found.				\
 * Also setups pstate variable, to use by WRB_FIND_NEXT().  		\
 */									\
attr struct type *							\
name##_WRB_FIND_FIRST(const struct name *head, struct type **pstate, const keytype key)	\
{									\
	struct type *first;						\
	int lfound;							\
									\
	first = name##_WRB_FIND_GE_TEST(head, key, &lfound);		\
	*pstate = first;						\
	return (lfound ? first : NULL);					\
}

#define WRB_GENERATE_FIND_NEXT(name, type, keytype, field, keycmp, elm2key, attr)	\
/* 									\
 * Continue search of an ordered set of keys started by 		\
 * WRB_FIND_FIRST(). Returns a node by the next key or NULL if the node	\
 * is not found.							\
 * Also updates pstate variable.					\
 */									\
attr struct type *							\
name##_WRB_FIND_NEXT(struct type **pstate, const keytype key)		\
{									\
	struct type *from = *pstate;					\
	int comp;							\
	int lfound;							\
	struct type *next;						\
	struct name head;						\
									\
	if (from == NULL)						\
		return (NULL);						\
	comp = keycmp(key, elm2key(from));				\
	if (comp == 0) {						\
		return (from);						\
	} else if (comp < 0) {						\
		return (NULL);						\
	}								\
	/* comp > 0: from < key */					\
	for (next = RB_PARENT(from, field); next != NULL;		\
		from = next, next = RB_PARENT(from, field)) {		\
									\
		comp = keycmp(key, elm2key(next));			\
		if (comp > 0)						\
			continue;					\
		if (comp == 0) {					\
			*pstate = next;					\
			return (next);					\
		}							\
		/* comp < 0 */						\
		break;							\
	}								\
	/* now next is NULL or next > key; still from < key */		\
	RB_ROOT(&head) = RB_RIGHT(from, field);				\
									\
	from = name##_WRB_FIND_GE_TEST(&head, key, &lfound);		\
	*pstate = (from != NULL) ? from : next;				\
	return (lfound ? from : NULL);					\
}

#define WRB_GENERATE_PDFS_START(name, type, field, attr)		\
attr struct type *							\
name##_WRB_PDFS_START(struct type *from)				\
{									\
	struct type *parent = NULL;					\
	while (from) {							\
		parent = from;						\
		if ((from = RB_LEFT(parent, field)) == NULL)		\
			from = RB_RIGHT(parent, field);			\
	}								\
	return (parent);						\
}

#define WRB_GENERATE_PDFS_NEXT(name, type, field, attr)			\
attr struct type *							\
name##_WRB_PDFS_NEXT(struct type *from)					\
{									\
	struct type *next = RB_PARENT(from, field);			\
									\
	if (next != NULL && RB_LEFT(next, field) == from &&		\
	    RB_RIGHT(next, field) != NULL)				\
            next = name##_WRB_PDFS_START(RB_RIGHT(next, field));	\
	return (next);							\
}

/* Post-order Depth-First Search, usable for tree destruction */
#define WRB_FOREACH_PDFS(x, name, head, y)				\
for ((x) = WRB_PDFS_START(name, RB_ROOT(head));				\
     (x) != NULL && ((y) = WRB_PDFS_NEXT(name, (x)), 1);		\
     (x) = (y))

#define WRB_NEXT(name, y)	name##_RB_NEXT(y)
#define WRB_PREV(name, y)	name##_RB_PREV(y)
#define WRB_FIND(name, x, y)	name##_WRB_FIND(x, y)
#define WRB_FIND_GE(name, x, y)	name##_WRB_FIND_GE(x, y)
#define WRB_FIND_GE_TEST(name, x, y, pfound)	name##_WRB_FIND_GE_TEST(x, y, pfound)
#define WRB_FIND_FIRST(name, head, pstate, x)	name##_WRB_FIND_FIRST(head, pstate, x)
#define WRB_FIND_NEXT(name, pstate, x)	name##_WRB_FIND_NEXT(pstate, x)
#define WRB_PDFS_START(name, y)	name##_WRB_PDFS_START(y)
#define WRB_PDFS_NEXT(name, y)	name##_WRB_PDFS_NEXT(y)

#endif /* WALLARM_TREE_H */
