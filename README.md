# Dodge Car Modular dengan Folder objek

Struktur ini memisahkan komponen objek 3D ke dalam folder `include/objek` dan `src/objek`.

Compile dari folder utama project:

```powershell
g++ .\dodge_car_modular\src\*.cpp .\dodge_car_modular\src\objek\*.cpp -I .\dodge_car_modular\include -I .\dependencies\include -L .\dependencies\lib -lfreeglut -lopengl32 -lglu32 -o .\dodge_car_modular\game.exe
```

Jalankan:

```powershell
Copy-Item .\dependencies\bin\freeglut.dll .\dodge_car_modular\ -Force
.\dodge_car_modular\game.exe
```
