// =============================================================
//  big_integer.cpp -- BigInteger class implementation
//
//  TASK: Implement all methods declared in big_integer.h
//  This stub file exists only so the project structure is clear.
//  Replace its contents with your implementation.
// =============================================================

#include "big_integer.h"
#include <string>
#include <algorithm>

bool BigInteger::is_zero() const {
    return BigInteger::digits_.size() == 1 && BigInteger::digits_[0] == 0;
}
bool BigInteger::is_negative() const {
    return BigInteger::negative_;
}

std::string BigInteger::to_string() const {
    std::string str = (negative_ ) ? "-" : "";
    for (int i = digits_.size() - 1; i >= 0; --i) {
        char c = digits_[i] + '0';
        str.push_back(c);
}
    return str;

    }

BigInteger::BigInteger() { digits_.push_back(0); };
BigInteger::BigInteger(long long value) {
    if (value == 0) {
        digits_.push_back(0); 
        negative_ = false;
    } else {
        if (value < 0) {
            negative_ = true;
        }

        value = std::abs(value);
        while (value != 0) {
            digits_.push_back(value % 10);
            value /= 10;
        }
    }
}
BigInteger::BigInteger(int value) : BigInteger(static_cast<long long>(value)) {};

BigInteger::BigInteger(const std::string& str) {
    int start_index = 0;
    if (str[0] == '-' && str.size() > 1) {
        negative_ = true;
        start_index = 1;
    } else {
        for (int i = str.length() - 1; i >= start_index; --i) {
            digits_.push_back(str[i] - '0');
        }
        // очищаем наш вектор от ненужных нулей

        while (digits_.size() > 1 && digits_.back() == 0) {
            digits_.pop_back();
    }
    }
}

std::ostream& operator<<(std::ostream& os, const BigInteger& value) {
    if (value.negative_ && !(value.digits_.size() == 1 && value.digits_[0] == 0)) os << "-";
    for (int i = value.digits_.size() - 1; i >= 0; --i) {
        os << value.digits_[i];
}
    return os;
};

std::istream& operator>>(std::istream& is, BigInteger& value) {
    std::string temp;

    if (is >> temp) {
        value = BigInteger(temp);
    }
    return is;
}
bool BigInteger::operator==(const BigInteger& rhs) const {
    return (this->negative_ == rhs.negative_ && this->digits_ == rhs.digits_);
};

bool BigInteger::operator!=(const BigInteger& rhs) const {
    return !(*this == rhs);
};
bool BigInteger::operator<(const BigInteger& rhs) const {
    if (is_negative() != rhs.is_negative()) {
        return this->is_negative(); // если знаки не совпадают, то больше тот, кто положительный
    }

    if (is_negative()) {
        return rhs.abs_less(*this); // -x < -y if |x| > |y|
    } else {
        return this->abs_less(rhs); // x < y if |x| < |y|
    }
}
bool BigInteger::operator>(const BigInteger& other) const {
    return other < *this; 
}
bool BigInteger::operator<=(const BigInteger& other) const {
    return !(other < *this);
}
bool BigInteger::operator>=(const BigInteger& other) const {
    return !(*this < other);
}

BigInteger::operator bool() const {
    return !(is_zero());
}

BigInteger& BigInteger::operator+=(const BigInteger& rhs) {
    int carry = 0;
    for (size_t i=0; i<std::max(digits_.size(),rhs.digits_.size()) || carry; ++i) {
        if (i == digits_.size())
        digits_.push_back(0);
    digits_[i] += carry + (i < rhs.digits_.size() ? rhs.digits_[i] : 0);
    carry = digits_[i] >= 10;
    if (carry) digits_[i] -= 10;
}
    return *this;
}

BigInteger BigInteger::operator+(const BigInteger& rhs) const {
    BigInteger lhs = *this;
    lhs += rhs; 
    return lhs;
}

BigInteger& BigInteger::operator-=(const BigInteger& rhs) {
    int carry = 0;
    for (size_t i=0; i<rhs.digits_.size() || carry; ++i) {
        digits_[i] -= carry + (i < rhs.digits_.size() ? rhs.digits_[i] : 0);

	carry = digits_[i] < 0;
	if (carry)  digits_[i] += 10;
}
while (digits_.size() > 1 && digits_.back() == 0)
	digits_.pop_back();
    return *this;

}
BigInteger BigInteger::operator-(const BigInteger& rhs) const {
    BigInteger lhs = *this;
    lhs -= rhs; 
    return lhs;
}

BigInteger& BigInteger::operator*=(const BigInteger& rhs) {
    std::vector<int> c(digits_.size() + rhs.digits_.size(), 0);
        for (size_t i = 0; i < digits_.size(); ++i) {
            int carry = 0;
            for (size_t j = 0; j < rhs.digits_.size() || carry; ++j) {
                long long cur = c[i + j] + 
                                digits_[i] * 1LL * (j < rhs.digits_.size() ? rhs.digits_[j] : 0) + 
                                carry;
                c[i + j] = int(cur % 10);
                carry = int(cur / 10);
            }
        }

        while (c.size() > 1 && c.back() == 0) {
            c.pop_back();
        }

        digits_ = c; 
        return *this;
}
BigInteger BigInteger::operator*(const BigInteger& rhs) const {
    BigInteger lhs = *this;
    lhs *= rhs;
    return *this;
}

BigInteger BigInteger::operator/(const BigInteger& rhs) const {
    BigInteger result;
    result.negative_ = (this->negative_ != rhs.negative_);
    result.digits_.resize(this->digits_.size(), 0);
    BigInteger b = rhs;
    b.negative_ = false;
    BigInteger current;
    for (int i = this->digits_.size() - 1; i >= 0; --i) {
        if (current.is_zero()) {
            current.digits_[0] = this->digits_[i];
        } else {
            current.digits_.insert(current.digits_.begin(), this->digits_[i]);
        }

        int x = 0;
        while (current >= b) { 
            current -= b;      
            x++;
        }
        
        result.digits_[i] = x;
    }

    while (result.digits_.size() > 1 && result.digits_.back() == 0) {
        result.digits_.pop_back();
    }

    if (result.is_zero()) {
        result.negative_ = false;
    }

    return result;
}

BigInteger& BigInteger::operator/=(const BigInteger& rhs) {
    *this = *this / rhs;
    return *this;
}

BigInteger BigInteger::operator%(const BigInteger& rhs) const {
    BigInteger result = *this - (*this / rhs) * rhs;

    if (result.is_zero()) {
        result.negative_ = false;
    }

    return result;
}

BigInteger& BigInteger::operator%=(const BigInteger& other) {
    *this = *this % other;
    return *this;
}

BigInteger BigInteger::operator-() const {
    BigInteger result = *this; 
    
    if (!result.is_zero()) {
        result.negative_ = !result.negative_;
    }
    
    return result;
}
BigInteger& BigInteger::operator++() {
    *this += BigInteger(1);
    
    return *this; 
}
BigInteger BigInteger::operator++(int) {
    BigInteger old_value = *this; 
    ++(*this); 
    return old_value; 
}
BigInteger& BigInteger::operator--() {
    *this -= BigInteger(1);
    return *this;
}
BigInteger BigInteger::operator--(int) {
    BigInteger old_value = *this;
    --(*this);
    return old_value;
}
int main() {
    
}

