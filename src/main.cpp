#include <iostream>
#include <assert.h>

#include "node.hpp"
#include "driver.hpp"
#include "parser.hpp"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        assert(0);
    }
    Driver drv(argv[1]);
    drv.parse();
    std::cout << *drv.root_;
}