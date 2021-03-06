#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <sys/xattr.h>
#include <sys/wait.h>
#include <pthread.h>

static int exception = 0;
static const char *dirpath = "/home/bhaskarajd/Documents";
char key[87] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}eTt$3J-zpc]lnh8,GwP_ND|jO";

void logging(int warn, char *cmd, const char *desc) {
  char buffer[80], mode[8];
  FILE *logFile = fopen("/home/bhaskarajd/fs.log", "a");
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);

  if (!warn) strcpy(mode, "INFO");
  else strcpy(mode, "WARNING");
  strftime(buffer, 80, "%y%m%d-%H:%M:%S", tm);
  fprintf(logFile, "%s::%s::%s::%s\n", mode, buffer, cmd, desc);
  fclose(logFile);
}

void encv1(int encrypt, char *a, int len) {
  if (!strcmp(a, ".") || !strcmp(a, "..") || (!strchr(a, '/') && !encrypt)) return;
  int index = -1, end = -1;
  sscanf(a, "%*[^/]/%n%*[^./]%n", &index, &end);

  if (!(a[len - 1] == '/')) {
    char *ext = strrchr(a, '.');
    if (ext) end = ext - a;
    else end = len;
  }

  if (encrypt) {
    for (int i = index; i < end; i++) {
      for (int j = 0; j < 87; j++) {
        if (a[i] == key[j]) {
            a[i] = key[(j + 10) % 87];
            break;
        }
      }
    }
  } else {
    for (int i = index; i < end; i++) {
      for (int j = 0; j < 87; j++) {
        if (a[i] == key[j]) {
            a[i] = key[(j + 77) % 87];
            break;
        }
      }
    }
  }
}

void encv2(char *path) {
  FILE *file = fopen(path, "rb");
  int count = 0;
  char topath[1000];
  sprintf(topath, "%s.%03d", path, count);
  void *buffer = malloc(1024);

  while(1) {
    size_t readSize = fread(buffer, 1, 1024, file);
    if (!readSize)break;
    FILE *op = fopen(topath, "w");
    fwrite(buffer, 1, readSize, op);
    fclose(op);
    count++;
    sprintf(topath, "%s.%03d", path, count);
  }
  free(buffer);
  fclose(file);
  remove(path);
}

void decv2(char *path) {
  FILE *check = fopen(path, "r");
  if (check)return;

  FILE *file = fopen(path, "w");
  int count = 0;
  char topath[1000];
  sprintf(topath, "%s.%03d", path, count);
  void *buffer = malloc(1024);
  while(1) {
    FILE *op = fopen(topath, "rb");
    if (!op)break;
    size_t readSize = fread(buffer, 1, 1024, op);
    fwrite(buffer, 1, readSize, file);
    fclose(op);
    remove(topath);
    count++;
    sprintf(topath, "%s.%03d", path, count);
  }
  free(buffer);
  fclose(file);
}

void direncv2(int encrypt, char *path) {
  struct dirent *de;
  DIR *d = opendir(path);
  if (!d) return;
  char dir[1000], file[1000];

  while ((de = readdir(d))) {
    if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")) continue;
    if (de->d_type == DT_DIR) {
      sprintf(dir, "%s/%s", path, de->d_name);
      encrypt ? direncv2(1, dir) : direncv2(0, dir);
    } else if (de->d_type == DT_REG) {
      sprintf(file, "%s/%s", path, de->d_name);
      if (encrypt) {
        encv2(file);
      } else {
        file[strlen(file) - 4] = '\0';
        decv2(file);
      }
    }
  }
  closedir(d);
}

static int _getattr(const char *path, struct stat *stbuf) {
  char *encrypted1 = strstr(path, "encv1_"), *encrypted2 = strstr(path, "encv2_");
  if (!exception && encrypted1) encv1(0, encrypted1, strlen(encrypted1));

  int res;
  char fpath[1000];
  sprintf(fpath, "%s%s", dirpath, path);
  res = lstat(fpath, stbuf);

  if (res == -1) {
    if (!encrypted2 || !strstr(encrypted2, "/")) {
      return -errno;
    } else {
      sprintf(fpath, "%s%s.000", dirpath, path);
      lstat(fpath, stbuf);
      int count = 0, sizeCount = 0;
      struct stat st;
      while(!stat(fpath, &st)) {
        count++;
        sprintf(fpath, "%s%s.%03d", dirpath, path, count);
        sizeCount += st.st_size;
      }
      stbuf->st_size = sizeCount;
    }
  }
  return 0;
}

static int _readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t ofdirpathet, struct fuse_file_info *fi) {
  char *encrypted1 = strstr(path, "encv1_"), *encrypted2 = strstr(path, "encv2_");
  if (encrypted1) encv1(0, encrypted1, strlen(encrypted1));

  char fpath[1000];
  if (!strcmp(path, "/")) {
    path = dirpath;
    sprintf(fpath, "%s", path);
  } else sprintf(fpath, "%s%s", dirpath, path);
  int res = 0;

  DIR *d;
  struct dirent *de;
  (void) ofdirpathet;
  (void) fi;
  d = opendir(fpath);
  if (!d) return -errno;

  while ((de = readdir(d))) {
    struct stat st;
    memset(&st, 0, sizeof(st));
    st.st_ino = de->d_ino;
    st.st_mode = de->d_type << 12;

    int d_len = strlen(de->d_name);
    if (encrypted2 && de->d_type == DT_REG) {
      if (!strcmp(de->d_name+(d_len-4), ".000")) {
        de->d_name[d_len-4] = '\0';
        res = (filler(buf, de->d_name, &st, 0));
      }
    } else {
      if (encrypted1) encv1(1, de->d_name, d_len);
      res = (filler(buf, de->d_name, &st, 0));
    } if (res != 0) break;
  }

  closedir(d);
  return 0;
}

static int _mkdir(const char *path, mode_t mode) {
  char *encrypted1 = strstr(path, "encv1_");
  if (encrypted1) encv1(0, encrypted1, strlen(encrypted1) - 1);

  char fpath[1000];
  sprintf(fpath, "%s%s", dirpath, path);
  int res;

  res = mkdir(fpath, mode);
  if (res == -1) return -errno;
  logging(0, "MKDIR", path);
  exception = 2;
  return 0;
}

static int _mknod(const char *path, mode_t mode, dev_t rdev) {
  char *encrypted1 = strstr(path, "encv1_");
  if (encrypted1) {
    encv1(0, encrypted1, strlen(encrypted1) - 1);
    exception = 1;
  }

  char fpath[1000];
  if (!strcmp(path, "/")) {
    path = dirpath;
    sprintf(fpath, "%s", path);
  } else sprintf(fpath, "%s%s", dirpath, path);
  int res;

  if (S_ISREG(mode)) {
    res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
    res != -1 ? res = close(res) : (res = -1);
  } else if (S_ISFIFO(mode)) res = mkfifo(fpath, mode);
  else res = mknod(fpath, mode, rdev);

  if (res == -1) return -errno;
  logging(0, "CREATE", path);
  return 0;
}

static int _unlink(const char *path) {
  char *encrypted1 = strstr(path, "encv1_");
  if (encrypted1) encv1(0, encrypted1, strlen(encrypted1));

  char fpath[1000];
  if (!strcmp(path, "/")) {
    path = dirpath;
    sprintf(fpath, "%s", path);
  } else sprintf(fpath, "%s%s", dirpath, path);
  int res;

  res = unlink(fpath);
  if (res == -1) return -errno;
  logging(1, "REMOVE", path);
  exception = 0;
  return 0;
}

static int _rmdir(const char *path) {
  char *encrypted1 = strstr(path, "encv1_");
  if (encrypted1) encv1(0, encrypted1, strlen(encrypted1));

  char fpath[1000];
  sprintf(fpath, "%s%s", dirpath, path);
  int res;

  res = rmdir(fpath);
  if (res == -1) return -errno;
  logging(1, "RMDIR", path);
  exception = 0;
  return 0;
}

static int _rename(const char *from, const char *to) {
  char *encrypted2from = strstr(from, "encv2_"), *encrypted2to = strstr(to, "encv2_");
  char ffrom[1000], fto[1000], str[100];
  sprintf(ffrom, "%s%s", dirpath, from);
  sprintf(fto, "%s%s", dirpath, to);
  int res;

  res = rename(ffrom, fto);
  if (res == -1) return -errno;
  if (!encrypted2from && encrypted2to) direncv2(1, fto);
  else if (encrypted2from && !encrypted2to) direncv2(0, fto);

  sprintf(str, "%s::%s", from, to);
  logging(0, "RENAME", str);
  exception = 0;
  return 0;
}

static int _truncate(const char *path, off_t size) {
  char *encrypted1 = strstr(path, "encv1_");
  if (encrypted1) encv1(0, encrypted1, strlen(encrypted1));

  char fpath[1000];
  sprintf(fpath, "%s%s", dirpath, path);
  int res;

  res = truncate(fpath, size);
  if (res == -1) return -errno;
  exception = 0;
  return 0;
}

static int _open(const char *path, struct fuse_file_info *fi) {
  char *encrypted1 = strstr(path, "encv1_"), *encrypted2 = strstr(path, "encv2_");
  if (encrypted1 && exception == 1) encv1(0, encrypted1, strlen(encrypted1) - 1);
  else if (encrypted1) encv1(0, encrypted1, strlen(encrypted1));

  char fpath[1000];
  if (encrypted2) sprintf(fpath, "%s%s.000", dirpath, path);
  else sprintf(fpath, "%s%s", dirpath, path);
  int res;

  res = open(fpath, fi->flags);
  if (res == -1) return -errno;
  exception = 0;
  close(res);
  return 0;
}

static int _read(const char *path, char *buf, size_t size, off_t ofdirpathet, struct fuse_file_info *fi) {
  char *encrypted1 = strstr(path, "encv1_");
  if (encrypted1 && exception == 1) encv1(0, encrypted1, strlen(encrypted1) - 1);
  else if (encrypted1) encv1(0, encrypted1, strlen(encrypted1));

  char fpath[1000];
  sprintf(fpath, "%s%s", dirpath, path);
  int fd, res;

  (void) fi;
  fd = open(fpath, O_RDONLY);
  if (fd == -1) return -errno;

  res = pread(fd, buf, size, ofdirpathet);
  if (res == -1) res = -errno;
  exception = 0;
  close(fd);
  return res;
}

static int _write(const char *path, const char *buf, size_t size, off_t ofdirpathet, struct fuse_file_info *fi) {
  char *encrypted1 = strstr(path, "encv1_");
  if (encrypted1 && exception == 1) encv1(0, encrypted1, strlen(encrypted1) - 1);
  else if (encrypted1) encv1(0, encrypted1, strlen(encrypted1));

  char fpath[1000];
  sprintf(fpath, "%s%s", dirpath, path);
  int fd, res;

  (void) fi;
  fd = open(fpath, O_WRONLY);
  if (fd == -1) return -errno;

  res = pwrite(fd, buf, size, ofdirpathet);
  if (res == -1) res = -errno;
  logging(0, "WRITE", path);
  close(fd);
  return res;
}

static struct fuse_operations _func = {
  .getattr  = _getattr,
  .readdir  = _readdir,
  .read     = _read,
  .mkdir    = _mkdir,
  .mknod    = _mknod,
  .unlink   = _unlink,
  .rmdir    = _rmdir,
  .rename   = _rename,
  .truncate = _truncate,
  .open     = _open,
  .read     = _read,
  .write    = _write,
};

int main(int argc, char *argv[]) {
  umask(0);
  return fuse_main(argc, argv, &_func, NULL);
}
