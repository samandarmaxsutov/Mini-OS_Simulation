# MiniOS Simulyatsiyasi

## Umumiy ma’lumot
Ushbu loyiha C dasturlash tilida yozilgan mini operatsion tizim simulyatsiyasidir. Loyihada quyidagilar mavjud: **jarayonlar jadvali va scheduler**, **xotirada ishlaydigan fayl tizimi**, hamda **shell** orqali jarayonlar va fayllar bilan ishlash. Loyihani amalga oshirish maqsadi — o‘quvchilarga operatsion tizimlar bo‘yicha o‘rgangan nazariy va amaliy bilimlarini qo‘llash imkonini berish.

---

## Funksionallik

### Jarayonlarni boshqarish
- Jarayon yaratish va o‘chirish (`create <name> <burst>`, `kill <pid>`)
- Ishlayotgan va tayyor jarayonlarni ko‘rish (`ps`, `top`)
- Scheduler algoritmlari:
  - Round-Robin (RR)
  - First-Come First-Served (FCFS)
  - Shortest Job First (SJF)
- Jarayon holatlari: NEW, READY, RUNNING, FINISHED

### Fayl tizimi (Xotirada)
- Buyruqlar: `touch`, `ls`, `ls -l`, `cat`, `write`, `rm`
- Fayl nomi, mazmuni, o‘lchami, yaratilgan va o‘zgartirilgan vaqti saqlanadi
- Maksimal: 32 ta fayl, har biri 256 baytgacha mazmun bilan

### Shell
Interaktiv buyruq qobig‘i
##### Buyruqlar:
-  touch <file> - fayl yaratish
- ls - fayllarni ko‘rsatish
- ls -l - fayllarni metama’lumot bilan ko‘rsatish
- cat <file> - fayl mazmunini ko‘rsatish
- write <file> <text> - faylga yozish
- rm <file> - fayl o‘chirish
- create <name> <burst> - jarayon yaratish
- start - schedulerni ishga tushirish
- stop - schedulerni to‘xtatish
- top - jarayon jadvalini ko‘rsatish
- sched <rr|sjf|fcfs> [quantum] - scheduler algoritmini tanlash
- kill <pid> - jarayonni o‘chirish
- ps - jarayonlar holatini ko‘rsatish
- help - buyruqlar ro‘yxati
- exit - shell-dan chiqish


## Qanday ishlatish

### Talablar
- Linux (Ubuntu tavsiya qilinadi)
- Build vositalari: `gcc`, `make`, `meson`, `ninja`
- Ixtiyoriy: `gdb` — xatoliklarni aniqlash uchun

### Bosqichlar

```bash
# Loyihani build qilish
make build

# Shellni ishga tushirish
make run

# Build fayllarni tozalash
make clean
```

### Misol sessiya

```
mini-os> ls -l
NAME	SIZE	CREATED			MODIFIED
-------------------------------------------------------------
test.txt	0	08-12-2025 20:26:05	08-12-2025 20:26:05
mini-os> write test.txt samandar
mini-os> cat test.txt
samandar
mini-os> 
[Scheduler] Process t2 (PID=2) finished (Start=1 End=100)
mini-os> 
[Scheduler] Process t1 (PID=1) finished (Start=0 End=110)
mini-os> top
PID	Name		State		Burst	Remaining
---	----		-----		-----	---------
1	t1          	FINISHED    	60	0
2	t2          	FINISHED    	50	0
mini-os> exit   
Exiting shell...
```
### Loyiha tuzilishi

```
Mini-OS_Simulation/
├── build/             # Meson/Ninja build papkasi
├── src/               # Manba fayllar
│   ├── filesystem.c
│   ├── scheduler.c
│   ├── shell.c
│   └── main.c
├── include/           # Header fayllar
│   ├── process.h
│   ├── scheduler.h
│   ├── shell.h
│   └── filesystem.h
├── Makefile
└── README.md
```