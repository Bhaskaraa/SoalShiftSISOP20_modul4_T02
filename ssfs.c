#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static  struct fuse_operations xmp_oper = {
.getattr = xmp_getattr,
.access = xmp_access,
.readlink = xmp_readlink,
.readdir = xmp_readdir,
.mknod = xmp_mknod,
.mkdir = xmp_mkdir,
.symlink = xmp_symlink,
.unlink = xmp_unlink,
.rmdir = xmp_rmdir,
.rename = xmp_rename,
.link = xmp_link,
.chmod = xmp_chmod,
.chown = xmp_chown,
.truncate = xmp_truncate,
.utimens = xmp_utimens,
.open = xmp_open,
.read = xmp_read,
.write = xmp_write,
.statfs = xmp_statfs,
.create = xmp_create,
.release = xmp_release,
.fsync = xmp_fsync,
.setxattr = xmp_setxattr,
.getxattr = xmp_getxattr,
.listxattr = xmp_listxattr,
.removexattr = xmp_removexattr,

static  int  xmp_getattr(const char *path, struct stat *stbuf) {
return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,

off_t offset, struct fuse_file_info *fi)

{

DIR *dp;
struct dirent *de;

(void) offset;

(void) ;

}

int (*getattr) (const char *, struct stat *);
//Get file attributes.

int (*readlink) (const char *, char *, size_t);
//Read the target of a symbolic link

int (*mknod) (const char *, mode_t, dev_t);
//Create a file node.

int (*mkdir) (const char *, mode_t);
//Create a directory.

int (*rename) (const char *, const char *);
//Rename a file

int (*chmod) (const char *, mode_t);
//Change the permission bits of a file

int (*chown) (const char *, uid_t, gid_t);
//Change the owner and group of a file

int (*truncate) (const char *, off_t);
//Change the size of a file

int (*open) (const char *, struct fuse_file_info *);
//File open operation.

int (*readdir) (const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info *);
//Read directory

int (*read) (const char *, char *, size_t, off_t, struct fuse_file_info *);
//Read data from an open file

int (*write) (const char *, const char *, size_t, off_t, struct fuse_file_info *);

int  main(int  argc, char *argv[])

{

umask(0)
return fuse_main();

}