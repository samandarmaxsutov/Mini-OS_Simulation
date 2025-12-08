## Arxitektura diagrammasi

```
+------------------+
|     Shell        |
| touch/ls/cat/... |
+--------+---------+
         |
         v
+--------+---------+
|  Process Manager |
| PCB, Scheduler   |
+--------+---------+
         |
         v
+--------+---------+
|  File System     |
| In-memory FS     |
+------------------+
```


## File system in memory

### 1. Strukturasi
```
typedef struct {
    char name[32];       // Fayl nomi
    char content[256];   // Fayl mazmuni
    int size;            // Fayl hajmi (baytda)
    time_t created_at;   // Yaratilgan vaqt
    time_t modified_at;  // Oxirgi o'zgartirilgan vaqt
} File;
```
files[32] – maksimal 32 ta fayl saqlash mumkin bo‘lgan massiv

file_count – hozirgi mavjud fayllar soni

### 2. Funksiyalar va ularning vazifasi

| Funksiya                                          | Vazifasi                                                                                             |
| ------------------------------------------------- | ---------------------------------------------------------------------------------------------------- |
| `fs_touch(const char* name)`                      | Yangi fayl yaratadi, bo‘sh mazmun bilan. Yaratilgan va o‘zgartirilgan vaqtni hozirgi vaqtga qo‘yadi. |
| `fs_write(const char* name, const char* content)` | Mavjud faylga matn yozadi, hajmini yangilaydi, `modified_at` ni hozirgi vaqtga o‘zgartiradi.         |
| `fs_cat(const char* name)`                        | Fayl mazmunini terminalga chiqaradi. Agar fayl topilmasa, “File not found” xabar beradi.             |
| `fs_ls()`                                         | Fayllar ro‘yxatini ko‘rsatadi: `name` va `size`.                                                     |
| `fs_ls_long()`                                    | Fayllar ro‘yxatini batafsil ko‘rsatadi: `name`, `size`, `created_at`, `modified_at`.                 |
| `fs_delete(const char* name)`                     | Faylni o‘chiradi va massivni chapga suradi, `file_count` ni kamaytiradi.                             |


## Jarayonlar (process) va Scheduler

### 1. Scheduler asosiy o‘zgaruvchilari:

```
static pthread_t scheduler_thread;   // Scheduler uchun alohida ip
static pthread_mutex_t lock;         // Scheduler va processlar o‘rtasida sinxronizatsiya
static int running;                  // Scheduler ish holati (1 = ishlamoqda)
static int scheduler_started;        // Scheduler ishga tushganligi
static int CURRENT_TIME;             // Virtual vaqt
static int SCHED_ALG;                // Ishlatilayotgan algoritm (RR/SJF/FCFS)
static int QUANTUM;                  // Round-robin kvant vaqti

static process_t* queue[MAX_PROCESSES]; // Processlar navbati
static int process_count;              // Navbatdagi processlar soni

```

### Scheduler API

| Funksiya                                             | Vazifasi                                                     |
| ---------------------------------------------------- | ------------------------------------------------------------ |
| `scheduler_init(sched_algorithm_t alg, int quantum)` | Scheduler algoritmi va RR kvantini sozlaydi                  |
| `scheduler_add(process_t* p)`                        | Navbatga yangi process qo‘shadi                              |
| `create_process_sched(const char* name, int burst)`  | Yangi process yaratadi va navbatga qo‘shadi                  |
| `scheduler_start()`                                  | Scheduler ipini ishga tushiradi                              |
| `scheduler_stop()`                                   | Scheduler ipini to‘xtatadi                                   |
| `scheduler_kill(int pid)`                            | PID bo‘yicha processni yo‘q qiladi                           |
| `scheduler_top()`                                    | Processlar jadvalini ko‘rsatadi (burst va qolgan vaqt bilan) |
| `scheduler_ps()`                                     | Processlar jadvalini ko‘rsatadi (faqat holat bilan)          |


### Shell

```shell_loop()```

```fgets``` orqali foydalanuvchi buyruqlarini o‘qiydi.

Har bir buyruq bo‘yicha:

| Buyruq                  | Vazifasi                                                             |                  |                                                               |
| ----------------------- | -------------------------------------------------------------------- | ---------------- | ------------------------------------------------------------- |
| `help`                  | Mavjud buyruqlar ro‘yxatini chiqaradi                                |                  |                                                               |
| `touch <file>`          | Yangi fayl yaratadi (`fs_touch`)                                     |                  |                                                               |
| `ls`                    | Fayllarni ro‘yxatlaydi (`fs_ls`)                                     |                  |                                                               |
| `ls -l`                 | Fayllarni batafsil (meta ma’lumot bilan) ro‘yxatlaydi (`fs_ls_long`) |                  |                                                               |
| `cat <file>`            | Fayl mazmunini chiqaradi (`fs_cat`)                                  |                  |                                                               |
| `write <file> <text>`   | Faylga matn yozadi (`fs_write`)                                      |                  |                                                               |
| `rm <file>`             | Faylni o‘chiradi (`fs_delete`)                                       |                  |                                                               |
| `create <name> <burst>` | Yangi jarayon yaratadi (`create_process_sched`)                      |                  |                                                               |
| `start`                 | Schedulerni ishga tushiradi (`scheduler_start`)                      |                  |                                                               |
| `stop`                  | Schedulerni to‘xtatadi (`scheduler_stop`)                            |                  |                                                               |
| `top`                   | Jarayonlar jadvalini chiqaradi (`scheduler_top`)                     |                  |                                                               |
| `sched <rr              | sjf                                                                  | fcfs> [quantum]` | Scheduler algoritmini tanlaydi va sozlaydi (`scheduler_init`) |
| `ps`                    | Jarayonlar holatini chiqaradi (`list_processes`)                     |                  |                                                               |
| `kill <pid>`            | Jarayonni PID bo‘yicha o‘ldiradi (`scheduler_kill`)                  |                  |                                                               |
| `exit`                  | Shellni yopadi                                                       |                  |                                                               |

