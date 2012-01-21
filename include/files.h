#ifndef __FILES_H_
#define __FILES_H_

// int is_dir(char *path);
// int is_file(char *filename);
int check_source_dir(char *path);
int check_dest_dir(char *path);
int remove_extension(char *filename, char *sub_filename);
int get_current_dir(char *filename, char *current_dir);

int is_dir(const char *path);
int is_file(const char *filename);
int is_markdown_file(char *filename);
int is_html_file(char *filename);
int is_image_file(char *filename);

int create_dir(const char *dirname);

int cp_file(const char *source, const char *dest);
int cp_dir(const char *source, const char *dest);

int get_short_filename(char *filename, char *short_filename);
int load_file_content(char *filename, char **buffer);

#endif
