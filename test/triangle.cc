#include "check.hpp"

int main(int argc, char** argv)
{
    /*
       ====================
       [check_total_cov]
       [check_bitmap_bytes]
       ====================
       |                   \
       |               [check_hitbits]
       |                   /
       [         ret       ]
    */
    check_total_cov(3);
    check_bitmap_bytes(1);
    check_hitbits(0);

    if (argc > 100) {
        check_hitbits(1);
    }
}