#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <wchar.h>
#include <stdlib.h>

long get_size(char* filename) {
    FILE* source = fopen(filename, "rb");
    
    if (!source)
        return 0;
    
    wchar_t symbol;
    long size = 0;

    while(!feof(source)) {
        symbol = fgetwc(source);
        if (symbol=='\n')
            size++;
    }

    fclose(source);

    return size;
}

int compare(const void * a, const void * b) {
    return wcscmp((const wchar_t*) a, (const wchar_t*) b);
}

wchar_t* delete_spaces(wchar_t* string) {
    wchar_t* result = string;
    while (((*result) == L' ') || ((*result) == L'\t') || ((*result) == L'\r')) result++;
    
    if ((*result) == '\n') 
        return NULL;

    return result;
}

int sort_file(long filesize, char* filename) {
    const int length = 3000;
    int strings_number = 0;

    FILE* source = fopen(filename, "r");
    FILE* output = fopen("sortfile.txt", "w");

    if (!(source && output)) 
        return 0;

    wchar_t* strings = (wchar_t*)calloc((filesize+10)*length, sizeof(wchar_t));
    wchar_t buffer[length];
    wchar_t* temp = NULL;

    long i = 0;
    wprintf(L"%s\n", filename);

    while (fgetws(buffer, length, source)) {
        temp = delete_spaces(buffer);

        if (!temp)
            continue;

        wcscpy(&strings[length*i], temp);
        wprintf(L"%d: ", i);
        fputws(&strings[length*i], stdout);
        i++;
    }

    wprintf(L"OK\n");
    
    strings_number = i;
    
    wprintf(L"NUMBER OF STRINGS: %d\nSIZEOF: %d\nLENGTH: %d\n", strings_number, sizeof(wchar_t), length);

    qsort(&strings[0], strings_number, sizeof(wchar_t)*length, compare);

    for (i = 0; i < strings_number; i++) {
        fputws(&strings[i*length], output);
    }

    fclose(source);
    fclose(output);
    
    free(strings);

    return 1;
}


int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");
    wprintf(L"OK\n");   

    if (argc!=2) {
        wprintf(L"USING: sortfile %%filename\n");
        return 0;
    }
    
    long  filesize = get_size(argv[1]);
    wprintf(L"FILE: %s; SIZE: %ld\n", argv[1], filesize);
    
    if ((!filesize) || (!sort_file(filesize, argv[1])))
        wprintf(L"Wrong input file or filename");

    return 0;
}
