#include <iostream>

inline constexpr uint16_t swap(const uint16_t& number)
{
    return ((number << 8) & 0xFF00) + ((number >> 8) & 0x00FF);
}

int main()
{
    std::cout << "Enter number: ";
    uint16_t number{};
    std::cin >> number;
    std::cout << "Modified number: " << swap(number);
    return 0;
}
