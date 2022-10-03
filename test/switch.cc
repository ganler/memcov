#include "check.hpp"

int main(int argc, char** argv)
{
    check_total_cov(10);
    check_bitmap_bytes(2);

    switch (argc) {
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    }

    check_hitbits(2);
}