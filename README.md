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

File penyelesaian soal ini dapat dilihat link berikut : [Source Code]() \
***Dibawah ini penulis akan menjelaskan source code berdasarkan fungsi yang digunakan pada soal, selengkapnya terdapat pada source code pada file ***ssfs.c*** .***

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
 
File penyelesaian soal ini dapat dilihat link berikut : [Source Code]()

## Penyelesaian Soal 2
```

```
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
 
File penyelesaian soal ini dapat dilihat link berikut : [Source Code]()

## Penyelesaian Soal 3
```

```
## Dokumentasi Penyelesaian Soal 3
![](https://github.com/Bhaskaraa/SoalShiftSISOP20_modul4_T02/blob/master/Screenshot/coming-soon-9c717dcb8e.jpg)

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
File penyelesaian soal ini dapat dilihat link berikut : [Source Code]()

## Penyelesaian Soal 4
```

```
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
