#include "../include/sep_file.h"
#include "../../log/include/sep_log.h"

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include <dirent.h>



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



SFile** get_files_from(const char *dir_path){
    DIR *dir;
    struct dirent *entry;
	// preallocate some space beacuse i don't know how many file will be in the directory and 
	// at the same time i don't want to do too much different mem allocation
	size_t NUM_FILES= 100;
	SFile** files = (SFile**) malloc(sizeof(SFile*)*NUM_FILES);

	if (files == NULL){s_log(ERROR,"malloc  failed");return NULL;}
	size_t files_count = 0;


    dir = opendir(dir_path);
    if (dir == NULL) {perror("Error opening directory");return NULL;}


    // Read the directory entries
    while ((entry = readdir(dir)) != NULL) {
		// Skip "." and ".." entries
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
			continue;
		}

		// if the number of files is bigger than the allocated space
		// double the allocated space and reallocate
		if (files_count == NUM_FILES){
			NUM_FILES *= 2;
			files = realloc(files, sizeof(SFile*) * NUM_FILES);
			if (files == NULL){s_log(ERROR,"realloc  failed");return NULL;}
		}

        files[files_count] = s_open(entry->d_name);

		// free resources in case of error
		if (files[files_count] == NULL){
			s_log(ERROR,"error reading file %s", entry->d_name);
			for (size_t i = 0; i < files_count; i++){s_close(files[i]);}
			free(files);
			return NULL;}
		files_count++;
    }

	// if the number of files is smaller than the allocated space
	// reallocate the memory to fit the number of files and don't waste space
	if (NUM_FILES > files_count){
		files = realloc(files, sizeof(SFile*) * files_count);
		if (files == NULL){s_log(ERROR,"realloc  failed");return NULL;}
	}

    // Close the directory
    closedir(dir);

	return files;


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
	if (index == -1){s_log(ERROR,"get_file_extension_from failed");close(file->file_descriptor);delete_s(file->path);free(file);return NULL;}

	file->extension = get_slice(file->path, index, file->path->size);

	return file;
}


/**
 * Closes the given file and frees all associated memory.
 * @param file The SFile object to close
 * @return true if the file was closed successfully, false otherwise
 */
bool s_close(SFile* file){
	if (file == NULL){s_log(ERROR,"file is null");return false;}
	if (file->is_open == false){s_log(ERROR,"file is not open");return false;}
	close(file->file_descriptor);
	delete_s(file->path);
	delete_s(file->extension);
	file->is_open = false;
	free(file);
	return true;
}



/**
 * Reads up to 512 bytes from the given file into the file's buffer.
 * If the file is null or not open, an error message is printed and 0 is returned.
 * If the read operation fails, an error message is printed and 0 is returned.
 * @param file The SFile object to read from
 * @return The number of bytes read from the file, or 0 if an error occurred
 */
size_t s_read(SFile* file){

	if (file == NULL){s_log(ERROR,"file is null");return 0;}
	if (file->is_open == false){s_log(ERROR,"file is not open");return 0;}

	ssize_t bytes_read = read(file->file_descriptor, file->buffer, 512);
	if (bytes_read == -1){s_log(ERROR,"read  failed");return 0;}
	return bytes_read;

}