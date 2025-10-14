#include "sep_file.h"
#include "sep_log.h"
#include "sep_string.h"

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>


/**
 * Returns the index of the file extension in the given path.
 * If the path does not contain an extension, -1 is returned.
 * @param path The path to get the file extension from
 * @return The index of the file extension, or -1 if not found
 */
ssize_t get_file_extension_from(const char *path){
	ssize_t index = 0;

	while (path[index] != '\0'){
		if (path[index] == '.'){
			return index+1;
		}
		index++;
	}
	return -1;
}


bool is_relative_path(const char *path){
	return path[0] != '/';
}


/**
 * Opens the given file in read-only mode and returns a new SFile object.
 * The SFile object contains information about the file, including its path and
 * file descriptor. The file descriptor is used to read from the file.
 * The file is automatically closed when the SFile object is deleted using delete_s.
 * @param path The absolute path of the file to open
 * @return A new SFile object containing information about the file, or NULL if the file cannot be opened
 */
SFile* s_open(const char *path){

	if (is_relative_path(path)){s_log(ERROR,"Path must be absolute");return NULL;}

	SFile* file = malloc(sizeof(SFile));
	if (file == NULL){s_log(ERROR,"malloc  failed");return NULL;}

	file->file_descriptor = open(path, O_RDONLY);
	if (file->file_descriptor == -1){s_log(ERROR,"open  failed");free(file);return NULL;}


	file->path = new_s(path, strlen(path));
	if (file->path == NULL){s_log(ERROR,"malloc  failed"); close(file->file_descriptor);free(file);return NULL;}
        

	file->is_open = true;

	ssize_t index = get_file_extension_from(path);
	if (index == -1){s_log(ERROR,"get_file_extension_from  failed");close(file->file_descriptor);delete_s(file->path);free(file);return NULL;}

	file->extension = get_slice(file->path, index, file->path->size);

	return file;
}


size_t s_read(SFile* file){

	if (file == NULL){s_log(ERROR,"file is null");return 0;}
	if (file->is_open == false){s_log(ERROR,"file is not open");return 0;}

	ssize_t bytes_read = read(file->file_descriptor, file->buffer, 512);
	if (bytes_read == -1){s_log(ERROR,"read  failed");return 0;}
	return bytes_read;

}