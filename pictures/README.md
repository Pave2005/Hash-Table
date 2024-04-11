# Hash-таблица и ее оптимизация.
## Описание поставленной задачи.
Была поставлена задача написать проект, состоящий из двух частей.
### Задача первой:
Написать популярную структуру данных - Hash-таблицу, используя 7 разных Hash-функций (см. [hash_funcs.cpp](https://github.com/Pave2005/Hash-Table/blob/main/no_optimized/hash_funcs.cpp)).
Для хранения данных в таблице используется так же известная структура данных - двусвязный список.
Далее нужно было протестировать таблицу на количество коллизий для каждой hash-функции. Для этого я поместил в таблицу роман Булгакова "Мастер и Маргарита", содержащий 11 тыс. слов.

### Задача второй части:
С помощью Perf проанализировать время работы программы и тремя способами оптимизировать функции, которые больше всего влияют на время работы.
Первый способ оптимизации - оптимизация с помощью ассемблерной вставки.
Второй способ оптимизации - оптимизация в помощью написания своей функции на языке ассемблера и применение ее в программе на C.
Третий способ оптимизации - оптимизация в помощью SIMD-функций.
Основной упор делаем на оптимизацию функции поиска элемента в Hash-таблице.

## Тест Hash-функции на коллизии.
### Hash-функции.
Hash-таблицу можно использовать для исследования Hash-функций. По "заселенности" Hash-таблицы можно определить качество Hash-функций.
### Тест на коллизии.
Коллизия - совпадений значений hash двух или более объектов.

[Наглядная иллюстрация коллизии](https://www.geeksforgeeks.org/java-program-to-implement-hash-tables-chaining-with-doubly-linked-lists/)
![](collision.png)

Я тестировал следующие Hash-функции:
- HashRetZero () -  возвращает 0.
- HashFirstAscii () - возвращает ASCII-код первого символа слова.
- HashWordLen () - возвращает длину слова.
- HashAsciiSum () - возвращает сумму всех ASCII-кодов слова.
- HashOriginal () - возвращает сумму всех ASCII-кодов слова, деленную на длину слова.
- HashRorAscii ()
- MurmurHash ()

При тестировании я использовал Hash-таблицу вместительностью 2000 Hash-значений.

#### 1. HashRetZero ()

![](ret_0.jpg)

На гистограмме можно увидеть, что все элементы таблицы размещены в первом Hash-значении таблицы. Очевидно, что эта функция обладает низким качеством, так как все преимущества поиска в Hash-таблицы сводятся на ноль и такой поиск сравним с поиском элемента в массиве.

#### 2. HashFirstAscii ()

![](first_ascii.jpg)

Очевидно, что это не самый хороший пример Hash-функции, пик коллизий приходится на начало таблицы, так как ASCII-коды символов обычно не превосходят 255.

#### 3. HashWordLen ()

![](word_len.jpg)

Также является плохим примером использования Hash-таблицы.
Данный результат получается из-за того, что Hash-значение - длина слова, но длины слов обычно не превосходят даже 50 элементов, при этом размер таблицы 2000, из-за чего пик коллизий приходится на начало таблицы.

#### 4. HashAsciiSum ()

##### Размер 256:
![](ascii_sum_256.jpg)

На первый взгляд Hash-функция кажется хорошей, так как распределение более менее равномерной, однако, неправильно подобран размер таблицы.

##### Размер 2000:
![](ascii_sum.jpg)

Как видно здесь уже возникают проблемы с коллизиями, все из-за того, что сумма ASCII кодов ограничена из-за чего первый вариант показывает неплохое распределение, но при больших размерах таблицы возникает пик коллизий и распределение перестает быть равномерным.

#### 5. HashOriginal ()

![](original.jpg)

По гистограмме видно, что распределение не равномерное и пик приходится на начало таблицы, все это из-за того, что в функции используется сумма ASCII-кодов элементов и длина элементов, а эти величины ограничены.

#### 6. HashRorAscii ()

![](ror.jpg)

Данная функция так же как и представленные выше функции имеет пики, но при этом они распределены по всей таблице, что делает ее неплохой для хранения данных, но при этом нельзя сказать, что она качественно хэширует значения.

#### 7. MurmurHash ()

![](murmur.jpg)

MurmurHash - достаточно популярная Hash-функция. Как видно из гистограммы, распределение достаточно равномерное и не имеет четко выделяющихся пиков.
В дальнейшем эта функция будет использоваться по умолчанию.


### Вывод:
Протестировав все данные Hash-функции самой качественной оказалась MurmurHash (), так как при ее использовании распределение элементов по всей таблице достаточно равномерное.