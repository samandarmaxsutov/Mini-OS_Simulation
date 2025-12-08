### Bosqichlar

```bash
# Loyihani build qilish
make build

# Shellni ishga tushirish
make run

# Build fayllarni tozalash
make clean
```

### Shell
### Buyruqlar:
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
