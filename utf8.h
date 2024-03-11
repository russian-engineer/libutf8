#include <stdint.h>

typedef int32_t rune_t;

int utf8_to_rune(const char *utf8_str, rune_t *rune_str);
int rune_to_utf8(const rune_t *rune_str, char *utf8_str, int max_len);
int rune_strcmp(rune_t *utf8_text1, int32_t *utf8_text2);
int rune_strlen(const rune_t *runes);
void print_binary(int num);
