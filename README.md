# Задание №1 — [asm](https://github.com/AlexeyShik/first-term/tree/master/asm)  

По адресу <http://github.com/sorokin/cpp-course> лежит пример двух программ на ассемблере. Программа [hello.asm](https://github.com/sorokin/cpp-course/blob/master/helloasm/hello.asm) — это программа выводящая строку "Hello, world". В ней подробно прокомментирована каждая строчка. Программа [add.asm](https://github.com/sorokin/cpp-course/blob/master/helloasm/add.asm) — это программа, которая выполняет сложение двух длинных чисел. 

Вам необходимо разобраться в этих примерах и написать на их основе программы выполняющие вычитание и умножение беззнаковых длинных чисел.

Обратите внимание, что приведенные примеры заточены на конкретную архитектуру процессора (x86-64), конкретный ассемблер (NASM) и операционную систему (Linux).

Разрешается писать программы под другие архитектуры, ассемблеры и операционные системы, при этом вам придется самим переписать код на нужную архитектуру и разобраться как сделать ввод-вывод.  

# Задание №2 — [bigint](https://github.com/AlexeyShik/first-term/tree/master/bigint)  
В данном задании необходимо написать класс длинное знаковое число. Требования к решению следующие:  
 1. Реализация должна содержаться в классе ```big_integer``` и находиться в файле ```big_integer.h```.  
 
 Класс должен содержать:  
 + Конструктор по умолчанию, инициализирующий число нулем.  
 + Конструктор копирования, после которого исходный объект и копию можно изменять независимо.  
 + Конструктор от int.  
 + Explicit конструктор от std::string.  
 + Оператор присваивания.  
 + Операторы сравнения.  
 + Арифметические операции: сложение, вычитание, изменение знака (унарный минус), унарный плюс.  
 + Умножение работающее за время O(nm), где n,m — длины множителей в битах.  
 + Деление и остаток от деления работающие за время O(nm), где n — длина делителя в битах, а m — длина частного в битах.  
 + Префиксный/постфиксный инкремент/декремент. (опционально)  
 + Битовые операции: и, или, исключающее или, не.  
 + Битовые сдвиги.  
 + Должна существовать глобальная функция std::string to_string(big_integer const&) возвращающая строковое представление числа.  
 
 2. Реализация функций класса должна быть расположена в файле big_integer.cpp.
 3. Пользоваться сторонними библиотеками длинных чисел при выполнении этого задания нельзя.  
 4. Разряды числа должны представляться 32-битными либо 64-битными числами. При этом необходимо, чтобы все биты в их представлении использовались. Например, нельзя хранить разряды как 64-битные числа, но использовать только младшие 32-бита в них.  
 5. Реализация должна использовать арифметические операции той битности, которая естественна для данного представления разрядов. Например, нельзя разбивать разряды на отдельные битики, и делать операции в системе счисления с основанием 2.  

По ссылке <https://github.com/sorokin/cpp-course/tree/master/bigint> можно найти:

[big_integer_testing.cpp](https://github.com/sorokin/cpp-course/blob/master/bigint/big_integer_testing.cpp) — набор модульных тестов для проверки корректности вашей программы. Прохождение этого набора тестов является необходимым, но не достаточным условием сдачи данного задания. При необходимости, этот набор тестов будет расширяться и дополняться.  
Тесты реализованы с помощью библиотеки gtest (Google Test). [gtest/](https://github.com/sorokin/cpp-course/tree/master/bigint/gtest) — библиотека [gtest](https://code.google.com/p/googletest/).  
[big_integer.h](https://github.com/sorokin/cpp-course/blob/master/bigint/big_integer.h), [big_integer.cpp](https://github.com/sorokin/cpp-course/blob/master/bigint/big_integer.cpp) — реализация класса длинного числа с использованием библиотеки GNU Multi-Precision, которая проходит приведенные тесты.  
Перед тем как писать код, возможно, стоит ознакомиться с книгой [Richard Brent, Paul Zimmermann — Modern Computer Arithmetic](https://members.loria.fr/PZimmermann/mca/mca-0.5.pdf). Про деление есть замечательная статья [Multiple-Length Division Revisited: A Tour of the Minefield](https://surface.syr.edu/cgi/viewcontent.cgi?article=1162&context=eecs_techreports).  
# Задание №3 — [bigint-optimized](https://github.com/AlexeyShik/first-term/tree/master/bigint-optimized)  
В данном задании необходимо реализовать класс длинного числа со small-object и copy-on-write оптимизациями.  

Длинное число должно удовлетворять всем требованиям задания 2 плюс:  

 1. Если в big_integer хранится число -2N ≤ a ≤ 2N-1, big_integer не должен выделять динамическую память. Конкретное значение N вы можете выбрать самостоятельно, допустимыми являются любые N ≥ 30.   
 2. Числа a < -2N и a > 2N-1 должны выделять не больше одного блока динамической памяти на каждый экземпляр big_integer.  
 3. Конструктор копирования и оператор присваивания должны работать за O(1) и удовлетворять гарантии безопасности исключений nothrow.  
 # Задание №4 — [vector](https://github.com/AlexeyShik/first-term/tree/master/vector)
 В данном задании необходимо реализовать шаблонный класс наподобие ```std::vector```, реализующий данные методы с требуемыми асимптотикой и гарантиями исключений.  
  ```vector() noexcept;```                                          // O(1) nothrow  
  ```vector(vector const &other);```                                // O(N) strong  
  ```vector &operator=(vector const &other);```                     // O(N) strong  

  ```~vector() noexcept;```                                         // O(N) nothrow  

  ```T &operator[](size_t i) noexcept;```                           // O(1) nothrow  
  ```T const &operator[](size_t i) const noexcept;```               // O(1) nothrow  

  ```T *data() noexcept;```                                         // O(1) nothrow  
  ```T const *data() const noexcept;```                             // O(1) nothrow  
  ```size_t size() const noexcept;```                               // O(1) nothrow  

  ```T &front() noexcept;```                                        // O(1) nothrow  
  ```T const &front() const noexcept;```                            // O(1) nothrow  

  ```T &back() noexcept;```                                         // O(1) nothrow  
  ```T const &back() const noexcept;```                             // O(1) nothrow  
  ```void push_back(T const &);```                                  // O(1)* strong  
  ```void pop_back() noexcept;```                                   // O(1) nothrow  

  ```bool empty() const noexcept;```                                // O(1) nothrow  

  ```size_t capacity() const noexcept;```                           // O(1) nothrow  
  ```void reserve(size_t);```                                       // O(N) strong  
  ```void shrink_to_fit();```                                       // O(N) strong  

  ```void clear() noexcept;```                                      // O(N) nothrow  

  ```void swap(vector &) noexcept;```                               // O(1) nothrow  

  ```iterator begin() noexcept;```                                  // O(1) nothrow  
  ```iterator end() noexcept;```                                    // O(1) nothrow  

  ```const_iterator begin() const noexcept;```                      // O(1) nothrow  
  ```const_iterator end() const noexcept;```                        // O(1) nothrow  

  ```iterator insert(const_iterator pos, T const &);```             // O(N) weak  
  ```iterator erase(const_iterator pos);```                         // O(N) weak  
  ```iterator erase(const_iterator first, const_iterator last);```  // O(N) weak  
  
 Тесты реализованы с помощью библиотеки gtest (Google Test). [gtest/](https://github.com/sorokin/vector-demo/tree/master/gtest)
