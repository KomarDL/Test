#include <iostream>

#define POWER_OF_2(power) \
{\
    decltype(power) tmp = power;\
    power = 1;\
    power = power << (tmp % (sizeof(power) * CHAR_BIT));\
}

int main()
{
    std::cout << "Enter number: ";
    uint32_t number{};
    std::cin >> number;
    POWER_OF_2(number);
    std::cout << "Modified number: " << number;
    return 0;
}
