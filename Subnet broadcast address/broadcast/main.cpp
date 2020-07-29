#include <iostream>
#include <limits>
using namespace std;

constexpr uint32_t filterInit = 0x80000000;

uint32_t getBroadcast(const uint32_t ip, const uint32_t mask) {
    //if ip == 255.255.255.255
    if (ip == UINT32_MAX) {
        return 0;
    }

    bool isPrevBitZero = ~mask & filterInit;
    for(uint32_t filter = filterInit >> 1; filter != 0; filter >>= 1) {
        //if previous bit zero and current bit one
        if (isPrevBitZero && (mask & filter))
            return 0;
        isPrevBitZero = ~mask & filter;
    }

    return ip | ~mask;
}

int main()
{
    uint32_t ip, mask;
    cout << "Enter IP-address number: ";
    cin >> ip;
    cout << "Enter subnetwork mask number: ";
    cin >> mask;
    auto result = getBroadcast(ip, mask);
    if (result == 0) {
        cout << "Incorrect data";
    } else {
        cout << "Broadcast address: " << result;
    }
    return 0;
}
