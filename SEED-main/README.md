# 안녕하세요! 나는 SEED입니다 (Добрый день! Я - SEED)
verified_user
 
SEED - симметричный блочный криптоалгоритм разработанный [KISA](https://seed.kisa.or.kr/kisa/algorithm/EgovSeedInfo.do) (Корейским агентством информационной безопасности) в 1998 году.

## Теоретическое описание

С официального сайта Корейского агенства информационной безопасности были взяты следующие источники:

![alt_text](https://sun9-22.userapi.com/impf/lblQ2qZP882obTdLVVHGqv1_45Q6ISWnQi_S5Q/RA923AaKfWU.jpg?size=2540x708&quality=96&proxy=1&sign=0ac4b64baacaf064a9333112e5f9dfe7&type=album)

  1)SEED 128 Algorithm Self Evaluation
  
  2)SEED 128 Algorithm Specification
  
Описания  RFC:

  1) [RFC4009](https://tools.ietf.org/html/rfc4009) - The SEED Encryption Algorithm (устаревшая версия)
  2) [RFC4269](https://tools.ietf.org/html/rfc4269) - The SEED Encryption Algorithm

## Библиотека LibTomMath

Алгоритмы шифрования и дешифрования реальзованы с использованием функций из указанной библиотеки и в стилистике бибилиотеки LibTomMath. Функции возвращают значение типа mp_err - сообщение об ошибке. Также в случае если возвращаемое значение не присваивается ни одной переменной, будет реализовано предупреждение MP_WUR - warn unused result.

## Сборка кода

CmakeLists.txt

Необходимо изменить путь к библиотеке LibTomMath.

## Содержание файлов курсовой работы

### const.h

Данный файл содежит все необходимые для работы алгоритма константные значения S-Boxes (четыре расширенных S-массива)
```C
static const u_int32_t SS0 [256] =
{ ... }
```

### kisa_seed.h

Основнаая реальзация алгоритмов SEED, без использования библиотеки для больших чисел

### seed.h

Добавлены инициализации алгоритмов шифрования, дешифрования, а так же вспомогательные алгоритмы и структуры данных для SEED с использовнаие библиотеки LibTomMath.

Структура для хранения данных (по 4 блока), например, открытый текст, шифртекст и дешифрованный текст:
```C
struct mp_seed_text {
    mp_int text[4];
};
```

Структура для хранения и работы с ключом:
```C
struct mp_seed_key {
    u_int32_t ks[32];
};
```

Инициализированы основные функции шифрования и дешифрования:
```C
mp_err mp_seed_encrypt(struct mp_seed_key *key, struct mp_seed_text *in_text, struct mp_seed_text *out_text) MP_WUR;

mp_err mp_seed_decrypt(struct mp_seed_key *key, struct mp_seed_text *in_text, struct mp_seed_text *out_text) MP_WUR;
```

### seed.c

Реализации алгоритмов инициализированных ранее. Выполнена для с использованием библиотеки LibTomMath.

## Описание тестов
См. файл main.c

### Тест №1
Для теста бралось следующее начальное значение.
```C
{0x0001000, 0x04050607, 0x08090a0b, 0x0c0d0e0f}
```
Далее с помощью алгоритмов рассмотренных в документации (см. раздел Теоретическое описание) производилось шифрование и дешифрование начальных значений. Как видно из результатов тестирования 1 оба алгоритма работают корректно.
![alt_text](https://sun9-33.userapi.com/impf/vmRZBPZcFA_6-u5kmT18bJ37e5ArJTzNZGMHxw/_xCIKjKzY0o.jpg?size=644x182&quality=96&proxy=1&sign=98dff3f23a2df9b97923e84063afc2c4&type=album)

### Тест №2
Для теста бралось аналогичные начальные значения как и для Теста №1.
```C
{0x0001000, 0x04050607, 0x08090a0b, 0x0c0d0e0f}
```
Дальше использовались аналогичные алгоритмы шифрования и дешифрования, но уже написаных для библиотеки больших чисел LibTommath. В данном случае полученные значения несколько отличаются от результатов первого теста, что скорее всего связано с особенностью работы используемой сторонней библиотеки. 
![alt_text](https://sun9-12.userapi.com/impf/KfMYlnCUDDPGok0u34iMYmuK0pEFE84vU2Gprw/xxbgJBn5qDo.jpg?size=840x128&quality=96&proxy=1&sign=38149dd83f48ca5bbea74262a4fd3806&type=album)

## Встраивание в библиотеку LibTomMath.

Для встраивания полученных алгоритмов и новых структур необходимо добавить их инициализацию в файл <tommath.h>

![alt_text](https://sun9-72.userapi.com/impf/2_Y9bMigOk_nWzVcZO0qnHIE7qRmY8vfSelhCg/3JbfissnE5g.jpg?size=1848x976&quality=96&proxy=1&sign=0f78314e669a9c954f4eb48a10e0c756&type=album)
