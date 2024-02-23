#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char* ptr;
	size_t len, cap;
} str;

str str_new(void) {
	return (str){NULL, 0, 0};
}

void str_grow(str* s, const size_t extra) {
	const size_t required_cap = s->len + extra;
	if (required_cap <= s->cap) {
		return;
	}

	s->cap = s->cap == 0 ? 4 : s->cap / 2 * 3;
	if (required_cap > s->cap) {
		s->cap = required_cap;
	}
	s->ptr = realloc(s->ptr, s->cap);
}

void str_concat(str* s, const char* c, const size_t len) {
	str_grow(s, len);
	memcpy(s->ptr + s->len, c, len);
	s->len += len;
}

bool is_bit_set(uint16_t num, size_t bit) {
	assert(bit >= 0 && bit <= 15);
	const uint16_t mask = (uint16_t)1 << bit;
	return (num & mask) == mask;
}

void append_binary_repr(uint16_t num, str* out) {
	const size_t num_written = 8 * sizeof num;

	str_grow(out, num_written);

	for (size_t i = 0; i < num_written; i++) {
		out->ptr[i + out->len] = '0' + is_bit_set(num, num_written - i - 1);
	}

	out->len += num_written;
}

const char* find_any(const char* s, const char* const char_set) {
	for (const char* c = s; *c != '\0'; c++) {
		if (strchr(char_set, *c) != NULL) {
			return c;
		}
	}
	return NULL;
}

char* replace_number_in_string_bin(const char* input) {
	static const char* const DIGITS = "0123456789";

	str out = str_new();

	const char* c;
	const char* pc = input;
	while ((c = find_any(pc, DIGITS)) != NULL) {
		str_concat(&out, pc, c - pc);

		const long num = strtol(c, (char**)&c, 10);
		assert(num >= 0 && num <= 65000 && errno == 0);

		append_binary_repr(num, &out);

		pc = c;
	}

	str_concat(&out, pc, strlen(pc));
	str_concat(&out, "", 1); // for null byte

	return out.ptr;
}

int main(void) {
	char input[256];
	if (fgets(input, sizeof input, stdin) == NULL) {
		return 1;
	}
	input[strcspn(input, "\r\n")] = '\0';

	char* with_replacements = replace_number_in_string_bin(input);
	printf("%s\n", with_replacements);

	free(with_replacements);

	return 0;
}

