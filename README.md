# SoalShiftSISOP20_modul4_T02
Repository Sebagai Laporan Resmi Soal Shift Modul 4 Praktikum Sistem Operasi 2020 \
Disusun oleh :
- I Gde Made Bhaskara Jala Dhananjaya (05311840000007)
- Azmi (05311840000047)

# Daftar Isi
## [Soal 1](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02#soal-1---enkripsi-versi-1)
- [Penyelesaian Soal 1](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02#penyelesaian-soal-1)
- [Dokumentasi Output yang Dihasilkan](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02#dokumentasi-penyelesaian-soal-1)

## [Soal 2](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02#soal-2---enkripsi-versi-2)
- [Penyelesaian Soal 2](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02#penyelesaian-soal-2)
- [Dokumentasi Output yang Dihasilkan](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02#dokumentasi-penyelesaian-soal-2)

## [Soal 3](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02#soal-3---sinkronisasi-direktori)
- [Penyelesaian Soal 3](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02#penyelesaian-soal-3)
- [Dokumentasi Output yang Dihasilkan](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02#dokumentasi-penyelesaian-soal-3)

## [Soal 4](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02#soal-4---log-system)
- [Penyelesaian Soal 4](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02#penyelesaian-soal-4)
- [Dokumentasi Output yang Dihasilkan](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02#dokumentasi-penyelesaian-soal-4)

## [Kendala yang Dialami Penulis](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02#kendala-yang-dialami)

# Pendahuluan Soal
Di suatu perusahaan, terdapat pekerja baru yang super jenius, ia bernama Jasir. Jasir baru bekerja selama seminggu di perusahan itu, dalam waktu seminggu tersebut ia selalu terhantui oleh ketidak amanan dan ketidak efisienan file system yang digunakan perusahaan tersebut. Sehingga ia merancang sebuah file system yang sangat aman dan efisien. Agar integritas filesystem tersebut lebih terjamin, maka setiap command yang dilakukan akan dicatat kedalam sebuah file log. File System yang dirancang oleh Jasir akan dijelaskan dari soal 1 hingga soal 4.

(***catatan*** : filesystem berfungsi normal layaknya linux pada umumnya) \
(***catatan*** : mount source (root) filesystem adalah direktori /home/[user]/Documents) 

# Soal 1 - Enkripsi Versi 1
Enkripsi versi 1 adalah sebagai berikut.
- Jika sebuah direktori dibuat dengan awalan “encv1_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v1.
- Jika sebuah direktori di-rename dengan awalan “encv1_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v1. 
- Apabila sebuah direktori terenkripsi di-rename menjadi tidak terenkripsi, maka isi adirektori tersebut akan terdekrip. 
- Setiap pembuatan direktori terenkripsi baru (mkdir ataupun rename) akan tercatat ke sebuah database/log berupa file.
- Semua file yang berada dalam direktori ter enkripsi menggunakan caesar cipher dengan key. 
- Metode enkripsi pada suatu direktori juga berlaku kedalam direktori lainnya yang ada didalamnya. 

Contoh implementasnya adalah sebagai berikut.
- Misal kan ada file bernama “kelincilucu.jpg” dalam directory FOTO_PENTING, dan key yang dipakai adalah 10 
- “encv1_rahasia/FOTO_PENTING/kelincilucu.jpg” => “encv1_rahasia/ULlL@u]AlZA(/g7D.|_.Da_a.jpg 

***Note : Dalam penamaan file ‘/’ diabaikan, dan ekstensi tidak perlu di encrypt.*** 

File penyelesaian soal ini dapat dilihat link berikut : [Source Code](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02/blob/master/ssfs.c) \
***Dibawah ini penulis akan menjelaskan source code berdasarkan fungsi yang digunakan pada soal, selengkapnya terdapat pada source code pada file ssfs.c .***

## Penyelesaian Soal 1
```
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
```
- sdasd
```
static int exception = 0;
static const char *dirpath = "/home/bhaskarajd/Documents";
char key[87] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}eTt$3J-zpc]lnh8,GwP_ND|jO";
```
- 
Berikut adalah fungsi untuk melakukan enkripsi :
```
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
```
- b
## Dokumentasi Penyelesaian Soal 1
![](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02/blob/master/Screenshot/1.png)
![](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02/blob/master/Screenshot/2.png)
![](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02/blob/master/Screenshot/3.png)
![](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02/blob/master/Screenshot/4.png)

# Soal 2 - Enkripsi Versi 2
Enkripsi versi 2 adalah sebagai berikut.
- Jika sebuah direktori dibuat dengan awalan “encv2_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v2. 
- Jika sebuah direktori di-rename dengan awalan “encv2_”, maka direktori tersebut akan menjadi direktori terenkripsi menggunakan metode enkripsi v2.
- Apabila sebuah direktori terenkripsi di-rename menjadi tidak terenkripsi, maka isi direktori tersebut akan terdekrip.
- Setiap pembuatan direktori terenkripsi baru (mkdir ataupun rename) akan tercatat ke sebuah database/log berupa file.
- Pada enkripsi v2, file-file pada direktori asli akan menjadi bagian-bagian kecil sebesar 1024 bytes dan menjadi normal ketika diakses melalui filesystem rancangan jasir. Sebagai contoh, file File_Contoh.txt berukuran 5 kB pada direktori asli akan menjadi 5 file kecil yakni : File_Contoh.txt.000, File_Contoh.txt.001, File_Contoh.txt.002, File_Contoh.txt.003, dan File_Contoh.txt.004.
- Metode enkripsi pada suatu direktori juga berlaku kedalam direktori lain yang ada didalam direktori tersebut (rekursif). 
 
File penyelesaian soal ini dapat dilihat link berikut : [Source Code](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02/blob/master/ssfs.c) \
***Dibawah ini penulis akan menjelaskan source code berdasarkan fungsi yang digunakan pada soal, selengkapnya terdapat pada source code pada file ssfs.c .***

## Penyelesaian Soal 2
Berikut merupakan fungsi untuk enkripsi :
```
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
```
- 
Berikut merupakan fungsi untuk dekripsi :
```
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
```
-
Berikut merupakan fungsi unntuk enkripsi direktori :
```
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
```
-

## Dokumentasi Penyelesaian Soal 2
![](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02/blob/master/Screenshot/5.png)
![](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02/blob/master/Screenshot/6.png)

# Soal 3 - Sinkronisasi Direktori
Tanpa mengurangi keumuman, misalkan suatu directory bernama dir akan tersinkronisasi dengan directory yang memiliki nama yang sama dengan awalan sync_ yaitu sync_dir. Persyaratan untuk sinkronisasi yaitu :
- Kedua directory memiliki parent directory yang sama.
- Kedua directory kosong atau memiliki isi yang sama. Dua directory dapat dikatakan memiliki isi yang sama jika memenuhi : \
***i.*** Nama dari setiap berkas di dalamnya sama. \
***ii.*** Modified time dari setiap berkas di dalamnya tidak berselisih lebih dari 0.1 detik.
- Sinkronisasi dilakukan ke seluruh isi dari kedua directory tersebut, tidak hanya di satu child directory saja. 
- Sinkronisasi mencakup pembuatan berkas/directory, penghapusan berkas/directory, dan pengubahan berkas/directory. 
 
Jika persyaratan di atas terlanggar, maka kedua directory tersebut tidak akan tersinkronisasi lagi. Implementasi dilarang menggunakan symbolic links dan thread. 
 
File penyelesaian soal ini dapat dilihat link berikut : [Source Code](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02/blob/master/ssfs.c) \
***Dibawah ini penulis akan menjelaskan source code berdasarkan fungsi yang digunakan pada soal, selengkapnya terdapat pada source code pada file ssfs.c .***

## Penyelesaian Soal 3
```
Define [Tidak mengerti maksud soalnya mas :(( ]
```
## Dokumentasi Penyelesaian Soal 3
![](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02/blob/master/Screenshot/S__11214850.jpg)

# Soal 4 - Log System
Log System bekerja sebagai berikut.
- Sebuah berkas nantinya akan terbentuk bernama "fs.log" di direktori *home* pengguna (/home/[user]/fs.log) yang berguna menyimpan daftar perintah system call yang telah dijalankan.
- Agar nantinya pencatatan lebih rapi dan terstruktur, log akan dibagi menjadi beberapa level yaitu ***INFO*** dan ***WARNING.*** 
- Untuk log level ***WARNING***, merupakan pencatatan log untuk syscall ***rmdir*** dan ***unlink***.
- Sisanya, akan dicatat dengan level ***INFO***.
- Format untuk logging yaitu : 
```
[LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC ...]
```
- LEVEL    : Level logging 
- yy       : Tahun dua digit 
- mm       : Bulan dua digit 
- dd       : Hari dua digit 
- HH       : Jam dua digit 
- MM       : Menit dua digit 
- SS       : Detik dua digit 
- CMD      : System call yang terpanggil 
- DESC     : Deskripsi tambahan (bisa lebih dari satu, dipisahkan dengan ::) 

Contoh format logging nantinya seperti : 
```
INFO::200419-18:29:28::MKDIR::/iz1  
INFO::200419-18:29:33::CREAT::/iz1/yena.jpg  
INFO::200419-18:29:33::RENAME::/iz1/yena.jpg::/iz1/yena.jpeg 
```
File penyelesaian soal ini dapat dilihat link berikut : [Source Code](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02/blob/master/ssfs.c) \
***Dibawah ini penulis akan menjelaskan source code berdasarkan fungsi yang digunakan pada soal, selengkapnya terdapat pada source code pada file ssfs.c .***

## Penyelesaian Soal 4
Berikut merupakan fungsi untuk melakukan Log :
```
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
```
- 
Berikut merupakan fungsi untuk mengambil atribut :
```
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
```
- 
Berikut merupakan fungsi untuk melakukan read directory :
```
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
```
-
Berikut merupakan fungsi untuk membuat direktori :
```
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
```
-
Berikut merupakan fungsi untuk menghapus direktori :
```
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
```
-
Berikut merupakan fungsi untuk mengganti nama :
```
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
```
-
Berikut merupakan fungsi untuk membuka file :
```
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
```
-
Berikut adalah fungsi untuk membaca file :
```
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
```
- 
Berikut merupakan fungsi untuk menulis di file :
```
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
```
-
Berikut daftar fungsi dan atribut/command yang ada dalam program :
```
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
```
-
Berikut adalah fungsi inti yang ada dalam program :
```
int main(int argc, char *argv[]) {
  umask(0);
  return fuse_main(argc, argv, &_func, NULL);
}
```
-

## Dokumentasi Penyelesaian Soal 4
![](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02/blob/master/Screenshot/7.png)
![](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02/blob/master/Screenshot/8.png)

## Kendala yang Dialami
***1.*** Banyak tugas yang lain yang membebani sehingga tidak fokus dalam mengerjakan soal shift. \
***2.*** Bobot soal shift yang sulit terutama nomor 1, 2, 3 dan 4 sehingga penulis diharuskan untuk mencari banyak referensi agar bisa menjawab soal. \
***3.*** Penulis masih menyesuaikan diri dengan sistem operasi LINUX/Ubuntu sehingga ada beberapa kendala dalam menggunakan terminal. \
***4.*** Sistem demo online sedikit rumit untuk dilakukan. \
***5.*** Karena disarankan untuk tidak keluar rumah dampak dari Covid-19, agak sulit untuk berkoordinasi dengan anggota kelompok. \
***6.*** Modul 4 cukup sulit untuk dimengerti, dan referensi yang beragam membuat penulis bingung.
