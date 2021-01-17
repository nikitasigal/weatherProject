#include <stdio.h>
#include <locale.h>
#include <windows.h>

int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    char s[100] = {0};
    scanf("%s", s);
    for (int i = 0; i < strlen(s); ++i) {
        printf("%c", s[i]);
    }
    printf("Привет, %s", s);
    return 0;
}
