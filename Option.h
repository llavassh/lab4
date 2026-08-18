#ifndef OPTION_H
#define OPTION_H

#include <ostream>
#include <stdexcept>

template <typename T>
class Option {
private:
    T value;
    bool hasValue;

public:
    static Option<T> None() {
        return Option<T>();
    }

    static Option Some(const T &value) {
        return Option<T>(value);
    }

    Option(): value(T()), hasValue(false) {}
    explicit Option(const T &val): value(val), hasValue(true) {}

    bool isNone() const {
        return !hasValue;
    }

    bool isSome() const {
        return hasValue;
    }

    T Get() const {
        if (!hasValue) throw std::runtime_error("Option is None!");
        return value;
    }    

    T GetOrDefault(const T &defaultValue) const {
        return hasValue ? value : defaultValue;
    }
 
    operator bool() const {
        return hasValue;
    }

    bool operator==(const Option<T> &other) const {
        if (hasValue != other.hasValue) {
            return false;
        }
        if (!hasValue) {
            return true;
        }
        return value == other.value;
    }

};

template <typename T> 
std::ostream& operator<<(std::ostream& os, const Option<T> &opt) {
    if (opt.isSome()) {
        os << "Some(" << opt.Get() << ")";
    }
    if (opt.isNone()) {
        os << "None";
    }
    return os;
}


#endif