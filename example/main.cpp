#include "../include/memcov.hpp"

int main() {
    // # BB must be 3;
    // # BB entered must be 1;
    return !(memcov::now() == 1 && memcov::total() == 3);
}
