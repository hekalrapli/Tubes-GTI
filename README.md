# 3D Dodge Car Modular dengan Folder objek

Struktur ini memisahkan komponen objek 3D ke dalam folder `include/objek` dan `src/objek`.

Compile dari folder utama project:

```powershell
g++ .\src\*.cpp .\src\objek\*.cpp -I .\include -I ..\dependencies\include -L ..\dependencies\lib -lfreeglut -lopengl32 -lglu32 -o .\game.exe
```

Jalankan:

```powershell
.\game.exe
```

