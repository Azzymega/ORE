#include "math.hpp"

double fmod(double FirstValue, double SecondValue) {
    if (SecondValue != SecondValue || FirstValue != FirstValue) {
        return FirstValue;
    }
    if (SecondValue == 0) {
        return NAN;
    }
    return FirstValue - (int) (FirstValue / SecondValue) * SecondValue; // FIXME
    // TODO WTF??? NO X87? >>:(<<
}

