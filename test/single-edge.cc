#include "check.hpp"

int main()
{
    check_total_cov(1);
    check_bitmap_bytes(1);
    check_hitbits(0); // coverage received on exit.
}