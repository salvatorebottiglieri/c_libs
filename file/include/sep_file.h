#ifndef FILE_H
#define FILE_H

#include <sep_string.h>

typedef struct{
    String *path;
    bool is_open;
    String* extension;
    int file_descriptor;
    char buffer[512];
}SFile;




SFile *s_open(const char *path);
bool s_close(SFile* file);
size_t s_read(SFile* file);
SFile** get_files_from(const char *dir);








#endif