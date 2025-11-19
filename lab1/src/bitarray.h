#pragma once
#include <string>
#include <vector>

class BitArray {
public:
    //Конструктор: создает пустой массив (0 битов).
    BitArray();
    //Деструктор: освобождает память.
    ~BitArray();
    //Конструирует массив, хранящий заданное количество бит.
    //Первые sizeof(long) бит можно инициализировать с помощью параметра value.
    explicit BitArray(int num_bits, unsigned long value = 0);
    //Конструктор копирования.
    BitArray(const BitArray& b);
    //Обменивает значения двух битовых массивов.
    void swap(BitArray& b);

    BitArray& operator=(const BitArray& b);
    //Изменяет размер массива. В случае расширения, новые элементы
    //инициализируются значением value.
    void resize(int num_bits, bool value = false);
    //Очищает массив.
    void clear();
    //Добавляет новый бит в конец массива. В случае необходимости
    //происходит перераспределение памяти.
    void push_back(bool bit);

    //Битовые операции над массивами.
    //Работают только на массивах одинакового размера.
    //Обоснование реакции на параметр неверного размера входит в задачу.
    BitArray& operator&=(const BitArray& b); // and
    BitArray& operator|=(const BitArray& b); // or
    BitArray& operator^=(const BitArray& b); // xor

    //Битовый сдвиг с заполнением нулями.
    BitArray& operator<<=(int n); // влево
    BitArray& operator>>=(int n); // вправо
    BitArray operator<<(int n) const; // новый сдвинутый влево массив
    BitArray operator>>(int n) const; // новый сдвинутый вправо массив

    //Устанавливает бит с индексом n в значение val.
    BitArray& set(int n, bool val = true);
    //Заполняет массив истиной.
    BitArray& set();
    //Устанавливает бит с индексом n в значение false.
    BitArray& reset(int n);
    //Заполняет массив ложью.
    BitArray& reset();
    //true, если массив содержит истинный бит.
    bool any() const;
    //true, если все биты массива ложны.
    bool none() const;
    //Битовая инверсия
    BitArray operator~() const;
    //Подсчитывает количество единичных бит.
    int count() const;
    //Возвращает значение бита по индексу i.
    bool operator[](int i) const;

    int size() const;
    bool empty() const;

    //Возвращает строковое представление массива.
    std::string to_string() const;
private:
    int num_bits; // текущее кол-во битов
    std::vector<unsigned long> data; // хранилище битов

    static int count_index_array(int n); // вычисляет в каком элементе вектора data находится бит n
    static int count_index_bit(int n) ; // вычисляет позицию бита внутри элемента вектора

    void ensure_capacity(int num_bits); // точно ли хватает памяти
    void set_bit(int n, bool value); // установка бита
    bool get_bit(int n) const; // чтение бита
};

bool operator==(const BitArray& a, const BitArray& b);
bool operator!=(const BitArray& a, const BitArray& b);

BitArray operator&(const BitArray& b1, const BitArray& b2);
BitArray operator|(const BitArray& b1, const BitArray& b2);
BitArray operator^(const BitArray& b1, const BitArray& b2);