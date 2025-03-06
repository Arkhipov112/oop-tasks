#include "BitArray.hpp"

BitArray::BitArray() {
	bit_array = nullptr;
	size_array = 0;
}

BitArray::~BitArray() {
	delete[] bit_array;
}

BitArray::BitArray(int num_bits, unsigned long value) {
	size_array = num_bits;

	value %= static_cast<unsigned long>(pow(2, num_bits));

	bit_array = new int[num_bits];
	memset(bit_array, 0, sizeof(int) * size_array);

	for (int i = num_bits - 1; value > 0; --i) {
		bit_array[i] = value % 2;
		value /= 2;
	}
}

BitArray::BitArray(const BitArray& b) {
	size_array = b.size_array;

	bit_array = new int[size_array];
	memcpy(bit_array, b.bit_array, sizeof(int) * size_array);
}

void BitArray::swap(BitArray& b) {
	if (size_array != b.size_array) { return; }
	int* temp = new int[size_array];

	memcpy(temp, bit_array, sizeof(int) * size_array);
	memcpy(bit_array, b.bit_array, sizeof(int) * size_array);
	memcpy(b.bit_array, temp, sizeof(int) * size_array);

	delete[] temp;
}

void BitArray::resize(int num_bits, bool value) {
	if (size_array == num_bits) { return; }

	int* temp = new int[num_bits];
	memset(temp, 0, sizeof(int) * num_bits);

	if (num_bits > size_array) {
		// memcpy(temp + (num_bits - size_array), bit_array, sizeof(int) * size_array);
		for (int i = 0; i < num_bits; ++i) { temp[i] = value; }
	}

	else {
		memcpy(temp, bit_array + (size_array - num_bits), sizeof(int) * num_bits);
	}

	delete[] bit_array;

	bit_array = temp;
	size_array = num_bits;
}

void BitArray::clear() {
	delete[] bit_array;
	bit_array = nullptr;
	size_array = 0;
}

void BitArray::push_back(bool bit) {
	int* temp = new int[size_array + 1];
	memcpy(temp, bit_array, sizeof(int) * size_array);
	temp[size_array] = bit;

	delete[] bit_array;
	bit_array = temp;
	size_array++;
}

const BitArray& BitArray::operator=(const BitArray& b) {
	if (&b == this) { return *this; }
	delete[] bit_array;

	size_array = b.size_array;
	bit_array = new int[size_array];
	memcpy(bit_array, b.bit_array, sizeof(int) * size_array);

	return *this;
}

BitArray& BitArray::operator&=(const BitArray& b) {
	if (size_array != b.size_array) { return *this; }

	for (int i = 0; i < size_array; ++i) {
		bit_array[i] *= b.bit_array[i];
	}

	return *this;
}

BitArray& BitArray::operator|=(const BitArray& b) {
	if (size_array != b.size_array) { return *this; }

	bool carry = 0;
	for (int i = size_array - 1; i >= 0; --i) {
		bit_array[i] += b.bit_array[i] + carry;
		if (bit_array[i] > 1) { carry = 1; }
		else { carry = 0; }
		bit_array[i] %= 2;
	}

	return *this;
}

BitArray& BitArray::operator^=(const BitArray& b) {
	if (size_array != b.size_array) { return *this; }

	for (int i = 0; i < size_array; ++i) {
		if (bit_array[i] == b.bit_array[i]) { bit_array[i] = 0; }
		else { bit_array[i] = 1; }
	}

	return *this;
}

BitArray& BitArray::operator<<=(int n) {
	if (n < 1 || n > size_array) { return *this; }

	for (int i = 0; i < size_array - n; ++i) {
		bit_array[i] = bit_array[i + n];
	}
	memset(bit_array + (size_array - n), 0, sizeof(int) * n);

	return *this;
}

BitArray& BitArray::operator>>=(int n) {
	if (n < 1 || n > size_array) { return *this; }

	for (int i = 1; i < size_array - n; ++i) {
		bit_array[size_array - i] = bit_array[size_array - (i + n)];
	}
	memset(bit_array, 0, sizeof(int) * n);

	return *this;
}

BitArray BitArray::operator<<(int n) const {
	BitArray result(*this);
	result <<= n;
	return result;
}

BitArray BitArray::operator>>(int n) const {
	BitArray result(*this);
	result >>= n;
	return result;
}

BitArray& BitArray::set(int n, bool val) {
	if (n < 0 || n >= size_array) { return *this; }

	bit_array[n] = val;
	return *this;
}

BitArray& BitArray::set() {
	for (int i = 0; i < size_array; ++i) {
		set(i, 1);
	}

	return *this;
}

BitArray& BitArray::reset(int n) {
	set(n, 0);
	return *this;
}

BitArray& BitArray::reset() {
	memset(bit_array, 0, sizeof(int) * size_array);
	return *this;
}

bool BitArray::any() const {
	for (int i = 0; i < size_array; ++i) {
		if (bit_array[i] == 1) { return true; }
	}

	return false;
}

bool BitArray::none() const {
	if (any()) { return false; }
	return true;
}

BitArray BitArray::operator~() {
	BitArray temp(*this);

	for (int i = 0; i < size_array; ++i) {
		if (temp.bit_array[i] == 0) { temp.bit_array[i] = 1; }
		else { temp.bit_array[i] = 0; }
	}

	return temp;
}

int BitArray::count() const {
	int count = 0;
	for (int i = 0; i < size_array; ++i) {
		if (bit_array[i] == 1) { ++count; }
	}

	return count;
}

bool BitArray::operator[](int i) const {
	if (i < 0 || i >= size_array) { return false; }
	return bit_array[i];
}

int BitArray::size() const {
	return size_array;
}

bool BitArray::empty() const {
	if (size_array == 0) { return true; }
	return false;
}

std::string BitArray::to_string() {
	std::string bit_array_str;
	for (int i = 0; i < size_array; ++i) {
		if (bit_array[i] == 1) { bit_array_str.append("1"); }
		else { bit_array_str.append("0"); }
	}

	return bit_array_str;
}

bool operator==(const BitArray& a, const BitArray& b) {
	if (a.size_array != b.size_array) { return false; }

	for (int i = 0; i < a.size_array; ++i) {
		if (a[i] != b[i]) { return false; }
	}

	return true;
}

bool operator!=(const BitArray& a, const BitArray& b) {
	if (a == b) { return false; }
	return true;
}

BitArray operator&(const BitArray& b1, const BitArray& b2) {
	BitArray result(b1);
	result &= b2;

	return result;
}

BitArray operator|(const BitArray& b1, const BitArray& b2) {
	BitArray result(b1);
	result |= b2;

	return result;
}

BitArray operator^(const BitArray& b1, const BitArray& b2) {
	BitArray result(b1);
	if (b1.size_array != b2.size_array) { return result; }

	for (int i = 1; i <= b1.size_array; ++i) {
		if (b1[i] == b2[i]) { result.set(i, 0); }
		else { result.set(i, 1); }
	}

	return result;
}
