# RF project

- [Learning test project](#learning-test-project)
    - [Список Ссылок на информацию о протоколе KeeLoq](#%D1%81%D0%BF%D0%B8%D1%81%D0%BE%D0%BA-%D1%81%D1%81%D1%8B%D0%BB%D0%BE%D0%BA-%D0%BD%D0%B0-%D0%B8%D0%BD%D1%84%D0%BE%D1%80%D0%BC%D0%B0%D1%86%D0%B8%D1%8E-%D0%BE-%D0%BF%D1%80%D0%BE%D1%82%D0%BE%D0%BA%D0%BE%D0%BB%D0%B5-keeloq)
    - [STM32 Flash info](#stm32-flash-info)
    - [STM32f103c8t6 info](#stm32f103c8t6-info)
    - [Code snippets](#code-snippets)


[![N|logic](image/3.png)](http://gimran.ru)





## Список Ссылок на информацию о протоколе KeeLoq

- [Описание протокола на форуме](http://phreakerclub.com/forum/showthread.php?t=1094)


## Фильтры

- [модуляция в Proteus](https://utmiit-201.jimdo.com/модуляция/)
- [расчет фильтров от TI](http://www.ti.com/design-tools/signal-chain-design/webench-filters.html)

## Recivers

- [RR3-433](https://spb.terraelectronica.ru/product/163566) ([PDF](https://spb.terraelectronica.ru/pdf/show?pdf_file=%252Fds%252Fpdf%252FR%252FRR3_8.pdf))
- [AM detector схемы](https://www.edn.com/design/analog/4442773/AM-detector-more-sensitive-than-simple-diode)
- [Прецизионные выпрямители на ОУ](http://www.gaw.ru/html.cgi/txt/doc/op/funop_13_2.htm)

## STM32 Flash info
- [общее описание на CMSIS easystm32.ru](http://easystm32.ru/for-beginners/38-flash-stm32)
- [опиание тонкостей и косяков с хабра](https://habr.com/post/213771/comments/)
- [реализация на HAL MCU.by](http://www.mcu.by/%D1%81%D1%82%D0%B0%D1%80%D1%82-arm-%D1%8D%D1%82%D0%B0-%D1%83%D0%B6%D0%BD%D0%B0%D1%81%D0%BD%D0%B0%D1%8F-flah-%D0%BE%D1%82-stm32-hal/)
- [Sappise youtube flash (+подключение библиотеки)](https://www.youtube.com/watch?v=R5KMEz4g9EU)

## STM32f103c8t6 info

-[bluepill wiki](https://wiki.stm32duino.com/index.php?title=Blue_Pill)
## Code snippets
```

```



**GPIO**
```
LL_GPIO_SetOutputPin(GPIOx, GPIO_Pin_x);
```