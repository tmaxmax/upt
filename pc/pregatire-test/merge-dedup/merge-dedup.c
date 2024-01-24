#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *val;
    size_t len, cap;
} Vec;

Vec vec_new(void) { return (Vec){.val = NULL, .len = 0, .cap = 0}; }

// Allocates new memory so that the vector can hold exactly new_cap elements.
// Expects the vector to be in a valid state (i.e. vec->vals is either NULL or
// points to allocated memory). The new capacity must accomodate
// the elements already present in the vector (i.e. new_cap >= vec->len).
void vec_alloc(Vec *v, const size_t new_cap) {
    // Check that we don't truncate the vector by mistake.
    // vec_trunc should be used for truncation.
    assert(new_cap >= v->len);
    v->val = realloc(v->val, new_cap * sizeof *v->val);
    // Check that the allocation succeeded.
    assert(v->val != NULL);
    v->cap = new_cap;
}

// Sets the vector length, effectively removing the elements
// starting at index new_len. The new length must be smaller or equal to the old
// length.
void vec_trunc(Vec *v, const size_t new_len) {
    assert(new_len <= v->len);
    v->len = new_len;
}

void vec_free(Vec *v) {
    // The parameter doesn't need to be a pointer. I chose to make it a pointer
    // so that the interface is consistent.
    free(v->val);
    // We could also set v->vals to NULL and len and cap to 0, but we expect the
    // caller to not use the vector after free anyway, making this unnecessary.
    // Even for debugging it may be worse to set the pointer to NULL – valgrind
    // could not point out use-after-free errors.
}

void vec_push(Vec *v, int elem) {
    // We wrap the reallocation logic inside push so we don't have to care about
    // size and capacity when using the vector. This cleans up the code a lot,
    // prevents mistakes and isolates the internals.
    if (v->len == v->cap) {
        vec_alloc(v, v->cap == 0 ? 4 : v->cap / 2 * 3);
    }

    v->val[v->len++] = elem;
}

size_t vec_len(const Vec *v) { return v->len; }

// Get a pointer to the element at index. This is useful for modifying the
// element at that index.
int *vec_at(Vec *v, const size_t index) {
    // Do some bounds checking to help debugging. See
    // https://en.cppreference.com/w/cpp/container/vector/at.
    assert(index < v->len);
    return v->val + index;
}

// Get the value of the element at index.
// NOTE: We need this because in const contexts (i.e. when we have a const Vec*)
// we can't use vec_at. It's also nicer to not have to always dereference when
// you just need to read the value, not write to it.
int vec_get(const Vec *v, const size_t index) {
    assert(index < v->len);
    return v->val[index];
}

// Check if the last element of the vec is equal to the given value.
bool vec_is_last(const Vec *v, const int elem) {
    // This function exists for convenience: writing the below code multiple
    // times is unconfortably verbose.
    return v->len != 0 && v->val[v->len - 1] == elem;
}

Vec solve_task(Vec *a, Vec *b) {
    // This task is very specific, so I chose not to split it into smaller,
    // more generic functions for multiple reasons:
    // 1. It's shorter this way
    // 2. It's more performant
    // The algorithm used to solve this problem is a combination of
    // the merging algorithm used in merge sort and a linear complexity
    // remove algorithm. The merge algorithm is described here:
    //   https://www.pbinfo.ro/articole/5588/interclasarea-tablourilor;
    // and the removal algorithm has an explanation and an implementation
    // example here:
    //   https://en.cppreference.com/w/cpp/algorithm/remove.
    // The merge algorithm is modified so that it only pushes values present
    // in both input vectors.

    Vec dupes = vec_new();
    size_t a_new_len = 0, b_new_len = 0;

    size_t i = 0, j = 0;
    while (i < vec_len(a) && j < vec_len(b)) {
        const int a_val = vec_get(a, i), b_val = vec_get(b, j);

        if (a_val == b_val) {
            // Values are equal. Add the value to dupes and drop it from both
            // inputs.
            i++;
            j++;
            vec_push(&dupes, a_val);
        } else if (a_val < b_val) {
            i++;
            // This is an unique value, add back to the array. Why is it unique?
            // Here the value in a is smaller than the value in b, which means
            // that after this value from b only larger values follow. An
            // example:
            //   a = 1, 2, 3, 4, 5
            //   b = 1, 5, 6, 7, 8
            // Consider that we are at element 2 in a. This means that we are
            // at element 5 in b. Given the sorted property, we are sure there
            // is no 2 in b, neither before 5, nor after. At this point, 2 will
            // be pushed into dupes and kept also in a, and the input will
            // progress to 3. In the same manner 3 is handled, then 4, and then
            // we are at 5. Now the algorithm is at 5 in both a and b, which
            // means that the first branch of this if will be chosen, and 5 will
            // be pushed to dupes but not kept in a or b.
            //
            // We add the value back only if it's unique to a. For example, if
            // the input looks like:
            //   a = 1, 3, 3
            //   b = 1, 3, 5
            // the first 3 would be added to dupes, and the second should be
            // dropped. To drop it, we must check if it is present in dupes
            // The property of the input that it is sorted reduces this check
            // to just checking if the last element in dupes is equal to this
            // one.
            if (!vec_is_last(&dupes, a_val)) {
                *vec_at(a, a_new_len++) = a_val;
            }
        } else {
            j++;
            // Same here. The logic above can be equally applied to this branch.
            if (!vec_is_last(&dupes, b_val)) {
                *vec_at(b, b_new_len++) = b_val;
            }
        }
    }

    // Just as in the normal merge algorithm, we need to add the remaining
    // elements from each array. We still need to check if the elements are not
    // a duplicate. Consider the following input:
    //   a = 1, 5, 5, 5
    //   b = 1, 5, 5
    // That last 5 in a is also present in b, so it must not be appended.
    // It will be at the end of dupes, so we check for that.

    while (i < vec_len(a)) {
        const int v = vec_get(a, i);
        if (!vec_is_last(&dupes, v)) {
            *vec_at(a, a_new_len++) = v;
        }
        // We must increment even if we not push, so that we skip the duplicate
        // and push the following unique values, if they exist. Otherwise we
        // end up in an infinite loop.
        i++;
    }
    vec_trunc(a, a_new_len);

    while (j < vec_len(b)) {
        const int v = vec_get(b, j);
        if (!vec_is_last(&dupes, v)) {
            *vec_at(b, b_new_len++) = v;
        }
        j++;
    }
    vec_trunc(b, b_new_len);

    return dupes;
}

// The read and print functions are not part of the Vec API – notice that they
// don't start with vec_ – because reading and printing is something specific
// to the task solved, not something generalisable to all use cases of Vec.

bool read_ints_vec(Vec *v) {
    size_t len;
    if (scanf("%zu", &len) != 1) {
        return false;
    }

    // Allocate once beforehand so we don't do multiple allocations with push.
    // We make use of an implementation detail, though – this is an abstraction
    // leak. It is fine here, as we use this to optimize performance, but it
    // opens quite the can of worms when it comes to writing code – see below.
    vec_alloc(v, len);

    for (size_t i = 0; i < len; i++) {
        int elem;
        // What would happen if we would do scanf("%d", vec_at(v, i)) directly
        // instead of scanning into an int and pushing it afterwards?
        // In theory the memory is already allocated, so this would be legal
        // from a memory usage standpoint. What about the Vec API contract?
        // If we didn't allocate above – if we did not make use of
        // implementation details – we wouldn't be thinking about this. Avoid
        // relying on implementation details as much as possible and design APIs
        // and functions which allow you to do so.
        if (scanf("%d", &elem) != 1) {
            vec_free(v);
            return false;
        }

        vec_push(v, elem);
    }

    return true;
}

void print_result(const Vec *r, const char *name) {
    printf("%s: ", name);
    for (size_t i = 0; i < vec_len(r); i++) {
        printf("%d ", vec_get(r, i));
    }
    printf("\n");
}

int main(void) {
    Vec a = vec_new(), b = vec_new();
    if (!read_ints_vec(&a) || !read_ints_vec(&b)) {
        return 1;
    }

    // We could also validate that the input is sorted if we don't trust the
    // input data – this is what you'll do in real life, when retrieving values
    // from users.

    Vec c = solve_task(&a, &b);

    print_result(&a, "a");
    print_result(&b, "b");
    print_result(&c, "c");

    vec_free(&a);
    vec_free(&b);
    vec_free(&c);

    return 0;
}

// NOTE: Why did we implement all these functions to work with Vec instead of
// just accessing the struct members directly (i.e. vec_get, vec_at, vec_trunc
// and so on)?
// A good abstraction is one that completely hides the implementation details.
// For example, another viable implementation of Vec would be the following:
//  typedef struct {
//     int *start, *end;
//     size_t cap;
//  } Vec;
// Instead of an integer for the length, we store a pointer to the end of the
// vector. The implementations of vec_push, vec_len would have to change. If
// outside code would depend on implementation details of Vec, then all code
// must be changed. Like this, with external functions, we don't have to change
// the code that uses Vec anymore.
// Besides this advantage, we can use assertions inside these functions (see
// vec_trunc, vec_alloc). This helps with debugging and catching bugs. Without
// such functions we would have to either write the assertions each time we
// use the vector or not write them at all. As a last note, it is true that
// asserts slow down code. There are ways to disable them for optimized/release
// builds – see https://en.wikipedia.org/wiki/Assert.h.
