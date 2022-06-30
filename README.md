# Dead-By-Daylight - Chance to unhook

Программа для расчёта вероятности спрыгивания выживших с крюка в игре Dead by Daylight методом Монте-Карло.

## ОСОБЕННОСТИ

- Вычисление вероятности спрыгнуть с крюка каждому из выживших
- Можно менять колличество доступных попыток
- Можно менять процент удачи
- Можно указать количество проводимых экспериментов (игр)
- Оптимизирована скорость вычислений

## ИСПОЛЬЗОВАНИЕ

Скачать двоичный файл программы из релиза или скомпилировать самостоятельно из исходного файла

Запуск с параметрами по умолчанию:

```sh
.\DbD_Unhook.exe
```

Или с другими параметрами:

```sh
.\DbD_Unhook.exe -t 6 -l 8 -n 1000000
```

## ОПЦИИ

```sh
-h                                   Распечатать текст справки и выйти.

-t COUNT                             COUNT попыток спрыгнуть с крюка. 
                                     По умолчанию COUNT = 3.

-l PERCENT                           PERCENT процентов удачи у выжившего
                                     По умолчанию PERCENT = 4.
                                     
-n COUNT                             COUNT испытаний (или игр). Чем больше, 
                                     тем точнее результаты. 
                                     По умолчанию COUNT = 100000000.
```

## ЛИЦЕНЗИЯ

MIT

**Free Software, Hell Yeah!**
