
#include <fs.h>
#include <log.h>

struct file *gfids[GFID_MAX] = {0};
gfid n_gfids = 1;

struct file *resolve_gfid(gfid id) {
	return gfids[id];
}
gfid add_file(struct file *file) {
	gfid new_gfid = n_gfids;
	gfids[new_gfid] = file;
	n_gfids++;
	return new_gfid;
}

void remove_file(gfid id) {
	gfids[id] = NULL;
}