//
// Created by ian on 1/12/25.
//

#include "tests.h"

int main() {
    INIT_TESTING();

    TEST("ASSERT EQUALS TEST", []() -> bool {
        return ASSERT_EQUALS(2 + 2, 4);
    })

    TEST("ASSERT EQUALS FAIL TEST", []() -> bool {
        return ASSERT_EQUALS(1 + 2, 2);
    })

    START_TESTS()
}