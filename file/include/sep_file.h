#ifndef FILE_H
#define FILE_H

#include "../../string/include/sep_string.h"

typedef struct{
    String *path;
    bool is_open;
    String* extension;
    int file_descriptor;
    char buffer[512];
}SFile;




SFile *s_open(const char *path);
size_t s_read(SFile* file);








#endif