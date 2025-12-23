#include "BitArray.h"
#include <stdexcept>

BitArray::BitArray() {
    num_bits = 0;
}

BitArray::~BitArray() = default;

BitArray::BitArray(int num_bits, unsigned long value) {
    this->num_bits = num_bits;
    ensure_capacity(num_bits);
    for (int i = 0; i < num_bits; i++) {
        bool bit_value = (value >> i) & 1;
        set_bit(i, bit_value);
    }
}

BitArray::BitArray(const BitArray& b) {
    num_bits = b.num_bits;
    data = b.data;
}

BitArray& BitArray::operator=(const BitArray& b) {
    if (this != &b) {
        num_bits = b.num_bits;
        data = b.data;
    }
    return *this;
}

void BitArray::swap(BitArray& b) {
    std::swap(num_bits, b.num_bits);
    std::swap(data, b.data);
}

void BitArray::resize(int num_bits, bool value) {
    ensure_capacity(num_bits);
    for (int i = this->num_bits; i < num_bits; i++) {
        set_bit(i, value);
    }
    this->num_bits = num_bits;
}

void BitArray::clear() {
    num_bits = 0;
    data.clear();
}

void BitArray::push_back(bool bit) {
    resize(num_bits + 1, bit);
}

BitArray& BitArray::operator&=(const BitArray& b) { // побитовое И с присваиванием
    if (num_bits != b.num_bits) {
        throw std::invalid_argument("BitArray sizes must be equal");
    }
    for (int i = 0; i < data.size(); i++) {
        data[i] &= b.data[i];
    }
    return *this;
}

BitArray& BitArray::operator|=(const BitArray& b) { // побитовое ИЛИ с присваиванием
    if (num_bits != b.num_bits) {
        throw std::invalid_argument("BitArray sizes must be equal");
    }
    for (int i = 0; i < data.size(); i++) {
        data[i] |= b.data[i];
    }
    return *this;
}

BitArray& BitArray::operator^=(const BitArray& b) { // исключающее ИЛИ с присваиванием
    if (num_bits != b.num_bits) {
        throw std::invalid_argument("BitArray sizes must be equal");
    }
    for (int i = 0; i < data.size(); i++) {
        data[i] ^= b.data[i];
    }
    return *this;
}

BitArray& BitArray::operator<<=(int n) {
    for (int i = num_bits - 1; i >= n; i--) {
        set_bit(i, get_bit(i - n)); // переносим биты
    }
    for (int i = 0; i < n; i++) {
        set_bit(i, false); // обнуляем младшие n битов
    }
    return *this;
}

BitArray& BitArray::operator>>=(int n) {
    for (int i = 0; i < num_bits - n; i++) {
        set_bit(i, get_bit(i + n));
    }
    for (int i = num_bits - n; i < num_bits; i++) {
        set_bit(i, false);
    }
    return *this;
}

BitArray BitArray::operator<<(int n) const { // новый сдвинутый массив
    BitArray result(*this);
    result <<= n;
    return result;
}

BitArray BitArray::operator>>(int n) const {
    BitArray result(*this);
    result >>= n;
    return result;
}

BitArray& BitArray::set(int n, bool value) {
    set_bit(n, value);
    return *this;
}

BitArray& BitArray::set() {
    for (int i = 0; i < num_bits; i++) {
        set_bit(i, true);
    }
    return *this;
}

BitArray& BitArray::reset(int n) {
    return set(n, false);
}

BitArray& BitArray::reset() {
    for (int i = 0; i < num_bits; i++) {
        set_bit(i, false);
    }
    return *this;
}

bool BitArray::any() const {
    for (int i = 0; i < num_bits; i++) {
        if (get_bit(i)) {
            return true;
        }
    }
    return false;
}

bool BitArray::none() const {
    return !any();
}

BitArray BitArray::operator~() const {
    BitArray result(*this);
    for (int i = 0; i < num_bits; i++) {
        result.set_bit(i, !get_bit(i));
    }
    return result;
}

int BitArray::count() const {
    int count = 0;
    for (int i = 0; i < num_bits; i++) {
        if (get_bit(i)) {
            ++count;
        }
    }
    return count;
}

bool BitArray::operator[](int i) const {
    return get_bit(i);
}

int BitArray::size() const {
    return num_bits;
}

bool BitArray::empty() const {
    return num_bits == 0;
}

std::string BitArray::to_string() const {
    std::string result;
    for (int i = num_bits - 1; i >= 0; i--) {
        if (get_bit(i) == true) {
            result += '1';
        } else {
            result += '0';
        }
    }
    return result;
}

int BitArray::count_index_array(int n) {
    return n / (sizeof(unsigned long) * 8);
}

int BitArray::count_index_bit(int n) {
    return n % (sizeof(unsigned long) * 8);
}

void BitArray::ensure_capacity(int num_bits) {
    int required_size = num_bits / (sizeof(unsigned long) * 8) + 1;
    if (static_cast<int>(data.size()) < required_size) {
        data.resize(required_size, 0);
    }
}

void BitArray::set_bit(int n, bool value) {
    int index = count_index_array(n);
    int bit = count_index_bit(n);
    if (value) {
        data[index] |= (1UL << bit);
    }
    else {
        data[index] &= ~(1UL << bit);
    }
}

bool BitArray::get_bit(int n) const {
    int index = count_index_array(n);
    int bit = count_index_bit(n);
    return (data[index] >> bit) & 1;
}

bool operator==(const BitArray& a, const BitArray& b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const BitArray& a, const BitArray& b) {
    return !(a == b);
}

BitArray operator&(const BitArray& b1, const BitArray& b2) {
    if (b1.size() != b2.size()) {
        throw std::invalid_argument("BitArray sizes must be equal");
    }
    BitArray result(b1);
    result &= b2;
    return result;
}

BitArray operator|(const BitArray& b1, const BitArray& b2) {
    if (b1.size() != b2.size()) {
        throw std::invalid_argument("BitArray sizes must be equal");
    }
    BitArray result(b1);
    result |= b2;
    return result;
}

BitArray operator^(const BitArray& b1, const BitArray& b2) {
    if (b1.size() != b2.size()) {
        throw std::invalid_argument("BitArray sizes must be equal");
    }
    BitArray result(b1);
    result ^= b2;
    return result;
}
