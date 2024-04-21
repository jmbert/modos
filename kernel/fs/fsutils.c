
#include <fs.h>
#include <stdint.h>
#include <string.h>
#include <mm/vm.h>


char *get_basename(char *path) {
	char *basename = memrchr(path, '/', strlen(path) - 1); // Do not count any trailing slash
	if (!basename) {
		basename = path;
	}
	return basename;
}

char *get_dirname(char *path) {
	char *basename = get_basename(path);
	size_t prebase_size = basename - path;
	char *prebase = kmalloc(prebase_size+1);
	memcpy(prebase, path, prebase_size);
	prebase[prebase_size] = '\0';
	return prebase;
}