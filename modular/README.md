## OTUS проектная работа
### Реализация архиватора на основе алгоритма LZW

### Версия с использованием std::map

Сжатые файлы сохраняются из массива int<br>
Логирование в stdout

* /build/rebuild.sh -- скрипт для компиляции, исполняемый файл размещается в build/bin
* /build/tests_total.sh -- скрипт для запуска тестов
* /include -- заголовочные файлы
* /src -- файлы исходного кода *.cpp

```
Usage:
    lzw -[c|d] file-in file-out
    Commands:
    -c        -- compress
    -d        -- decompress
    file-in   -- input path/to/file/filename
    file-out  -- output path/to/file/filename
    compressed file extension (strictly): '.lzwbin'
    *lzw -- executable filename
```
