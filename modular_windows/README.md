## OTUS проектная работа
### Реализация архиватора на основе алгоритма LZW

### Версия для компиляции под windows

Проверено под win10<br>
Собиралось с использованием CMake 3.29.3 (cmake.org); CMake GUI<br>
Компилятор gcc (x86_64-mcf-seh-rev1, Built by MinGW-Builds project) 13.2.0 в составе MinGW-W64-builds-5.0.0<br>


```
Usage:
    lzw -[c|d] file-in file-out
    Commands:
    -c         -- compress
    -d         -- decompress
    file-in    -- input path/to/file/filename
    file-out   -- output path/to/file/filename
    compressed file extension (strictly): '.lzwbin'
    *lzw -- executable filename
```
