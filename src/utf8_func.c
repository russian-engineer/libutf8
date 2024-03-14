#include "../include/utf8.h"
#include <stdio.h>
#include <stdint.h>

rune_t rune_getc()
{
    char buffer[6] = "VENOM"; // Массив для хранения введенных символов
    rune_t result = 0;

    fgets(buffer, 6, stdin); // Считываем ввод пользователя в массив

    if ((buffer[0] & 0x80) == 0)
    {
        result = buffer[0];
        if ((result != '\n') && (buffer[1] != '\n'))
            CleanBufferUtf8();
    }
    else if ((buffer[0] & 0xE0) == 0xC0)
    {
        result = ((buffer[0] & 0x1F) << 6) | (buffer[1] & 0x3F);
        if (buffer[2] != '\n')
            CleanBufferUtf8();
    }
    else if ((buffer[0] & 0xF0) == 0xE0)
    {
        result = ((buffer[0] & 0x0F) << 12) | ((buffer[1] & 0x3F) << 6) | (buffer[2] & 0x3F);
        if (buffer[3] != '\n')
            CleanBufferUtf8();
    }
    else if ((buffer[0] & 0xF8) == 0xF0)
    {
        result = ((buffer[0] & 0x07) << 18) | ((buffer[1] & 0x3F) << 12) | ((buffer[2] & 0x3F) << 6) | (buffer[3] & 0x38);
        if (buffer[4] != '\n')
            CleanBufferUtf8();
    }

    return result;
}

int utf8_to_rune(const char *utf8_str, rune_t *rune_str)
{
    int i = 0;
    int j = 0;
    while (utf8_str[i] != '\0')
    {
        if ((utf8_str[i] & 0x80) == 0)
        {
            rune_str[j++] = utf8_str[i];
            i += 1;
        }
        else if ((utf8_str[i] & 0xE0) == 0xC0)
        {
            rune_str[j++] = ((utf8_str[i] & 0x1F) << 6) | (utf8_str[i + 1] & 0x3F);
            i += 2;
        }
        else if ((utf8_str[i] & 0xF0) == 0xE0)
        {
            rune_str[j++] = ((utf8_str[i] & 0x0F) << 12) | ((utf8_str[i + 1] & 0x3F) << 6) | (utf8_str[i + 2] & 0x3F);
            i += 3;
        }
        else if ((utf8_str[i] & 0xF8) == 0xF0)
        {
            rune_str[j++] = ((utf8_str[i] & 0x07) << 18) | ((utf8_str[i + 1] & 0x3F) << 12) | ((utf8_str[i + 2] & 0x3F) << 6) | (utf8_str[i + 3] & 0x38);
            i += 4;
        }
        else
        {
            return -1; // Неверный формат UTF-8
        }
    }
    rune_str[j] = 0;
    
    return j; // Возвращаем количество рун
}

int rune_to_utf8(const rune_t *rune_str, char *utf8_str, int max_len)
{
    int i = 0;
    int j = 0;
    while ((rune_str[i] != 0) && (j < max_len - 1))
    {
        if (rune_str[i] < 0x80)
        {
            utf8_str[j++] = (char) rune_str[i];
        }
        else if (rune_str[i] < 0x800)
        {
            utf8_str[j++] = (char) ((rune_str[i] >> 6) | 0xC0);
            utf8_str[j++] = (char) ((rune_str[i] & 0x3F) | 0x80);
        }
        else if (rune_str[i] < 0x10000)
        {
            utf8_str[j++] = (char) ((rune_str[i] >> 12) | 0xE0);
            utf8_str[j++] = (char) (((rune_str[i] >> 6) & 0x3F) | 0x80);
            utf8_str[j++] = (char) ((rune_str[i] & 0x3F) | 0x80);
        }
        else if (rune_str[i] < 0x110000)
        {
            utf8_str[j++] = (char) ((rune_str[i] >> 18) | 0xF0);
            utf8_str[j++] = (char) (((rune_str[i] >> 12) & 0x3F) | 0x80);
            utf8_str[j++] = (char) (((rune_str[i] >> 6) & 0x3F) | 0x80);
            utf8_str[j++] = (char) ((rune_str[i] & 0x3F) | 0x80);
        }
        else
        {
            return -1; // Неверная руна Unicode
        }
        i++;
    }
    utf8_str[j] = '\0'; // Завершаем строку
    
    return j; // Возвращаем количество байтов в UTF-8 строке
}

int rune_strcmp(rune_t *utf8_text1, int32_t *utf8_text2)
{
    int i = 0;

    // Проходим по массивам и сравниваем руны
    while (utf8_text1[i] != 0 && utf8_text2[i] != 0)
    {
        if (utf8_text1[i] != utf8_text2[i])
            return 1; // Массивы не равны
        i++;
    }

    // Проверяем, достигли ли конца обоих массивов
    if (utf8_text1[i] == 0 && utf8_text2[i] == 0)
        return 0; // Массивы равны
    else
        return 1; // Массивы имеют разные длины
}

int rune_strlen(const rune_t *runes)
{
    int length = 0;
    while (runes[length] != 0)
        length++;
    
    return length;
}

void print_binary(int num)
{
    int i;
    for (i = 31; i >= 0; i--)
    {
        printf("%d", (num & (1 << i)) ? 1 : 0);
        if (i % 8 == 0)
            printf(" "); // Добавляем пробел после каждых 8 бит
    }
    printf("\n");
}

void CleanBufferUtf8() { for(int c = getchar (); c != '\n' && c != EOF; c = getchar ()); }

// int main()
// {
//     const char *utf8_str = "Пример строки\" UTF-8";
//     rune_t runes[100];
//     int num_runes = utf8_to_rune(utf8_str, runes);
//     printf("Number of runes: %d\n", rune_strlen(runes));
//     for (int i = 0; i < num_runes; i++) {
//         printf("%d\n", runes[i]);
//     }
//     
//     for (int i = 0; runes[i] != 0; i++)
//     {
//         if (runes[i] == '\\')
//             printf("Кажется победа\n");
//     }
//
//     char utf8_str2[100];
//     int num_bytes = rune_to_utf8(runes, utf8_str2, 100);
//     printf("UTF-8 string: %s\n", utf8_str);
//     return 0;
// }

