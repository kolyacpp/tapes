# tapes

## Сборка

```Bash
git clone --recursive https://github.com/kolyacpp/tapes.git
cd tapes
cmake --preset Release
cmake --build cmake-build-Release -j 10
```

## Консольное приложение

```Bash
./cmake-build-Release/sorter/sorter --help
```

```
Tapes cli
Usage: ./cmake-build-Release/sorter/sorter [OPTIONS] SUBCOMMAND

Options:
  -h,--help                   Print this help message and exit
  -c,--config [config.conf]   Configuration file

Subcommands:
  create                      Creates a new tape file with values
  print                       Prints values from tape file
  sort                        Sorts values from input tape file to output tape file
  gen-config                  Generate a default configuration file
```

### create

Создаёт новый файл для ленты, который заполняется из `stdin`, завершается при отрицательном числе или `EOF`.
```Bash
sorter create ./input.tape
```

### print

Выводит ленту из файла в `stdout`.
```Bash
sorter print ./input.tape
```

### sort

Читает ленту из `input` файла, сортирует и записывает ответ в `output` файл. Файл `input` открывается только в режиме чтения.
```Bash
sorter sort ./input.tape ./output.tape
```

Возможные опции, вывод `--help`:

```
sorter sort [OPTIONS] input output

Positionals:
  input TEXT REQUIRED         Path to the input tape file
  output TEXT REQUIRED        Path to the output tape file

Options:
  -h,--help                   Print this help message and exit
  -b,--buffer-size UINT [65536] 
                              Buffer size in bytes
  -t,--tape-count UINT [2]    Tape size in bytes
  -o,--order-asc BOOLEAN [1]  Order asceding
  -p,--print-result BOOLEAN [1] 
                              Print the result to stdout
  -r,--read-delay UINT [0]    Read delay in milliseconds
  -w,--write-delay UINT [0]   Write delay in milliseconds
  -m,--move-delay UINT [0]    Move delay in milliseconds
  -R,--rewind-delay UINT [0]  Rewind delay in milliseconds
  -c,--clear-delay UINT [0]   Clear delay in milliseconds
  -d,--temp-directory TEXT [system-default] 
                              Temp directory
```

В скобках `[]` указаны значения по умолчанию. Есть возможность переопределить значения по умолчанию с помощью `config` файла:

```Bash
sorter -c config.conf sort ...
sorter --config config.conf sort ...
```

Затем можно переопределить опции ещё раз с помощью аргументов из списка выше.

### gen-config 

Генерирует конфиг, который содержит значения опций по умолчанию:

```Bash
sorter gen-config ./config.conf
```

#### config.conf:

```
sort.buffer-size=65536
sort.tape-count=2
sort.order-asc=1
sort.print-result=1
sort.read-delay=0
sort.write-delay=0
sort.move-delay=0
sort.rewind-delay=0
sort.clear-delay=0
sort.temp-directory="system-default"
```

## Опции

### buffer-size

Максимальное количество чисел в буфферах и кучах, которые используются во время сортировки.

### tape-count

Количество временных лент для сортировки.


### order-asc

Порядок сортировки.

### print-result

Вывести результат сортировки в `stdout` аналогично `sorter print ./output.tape`.

### delay
 Задержки перемещения ленты, чтения, записи, перемотки в начало и очистки ленты:
- read
- write
- move
- rewind
- clear

### temp-directory

Папка для временных файлов лент. По умолчанию используется системная `/tmp` или `...AppData/Local/Temp`.

## Алгоритм

За основу взят алгоритм https://people.cs.aau.dk/~simas/aalg04/esort.pdf

Этапы сортировки:

1) Чтение `input` ленты блоками размера буффера, их сортировка в оперативной памяти и запись на `output` ленту.

2) Пока сортировка не закончена:
 - Чтение `output` ленты блоками и запись блоков на временные ленты циклически
 - Чтение временных лент по блокам и их `merge` с помощью кучи, после этого размер отсортированных блоков увеличивается в `tape-count` раз


## Использованные библиотеки

Фреймворк для тестов – [googletest](https://github.com/google/googletest) | [3-clause BSD](https://github.com/google/googletest/blob/main/LICENSE)

Парсер аргументов командой строки – [CLI11](https://github.com/CLIUtils/CLI11) | [License](https://github.com/CLIUtils/CLI11/blob/main/LICENSE)

## TODO

- улучшить способ генерации имён для временных файлов, чтобы каждый раз не указывать разный `prefix` имени во время создания

- написать и собрать `doxygen`, настроить деплой в `github pages`
