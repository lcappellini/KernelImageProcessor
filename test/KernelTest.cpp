#include "gtest/gtest.h"
#include <filesystem>

#include "../Kernel.h"

using namespace std;

TEST(Kernel, KernelConstructor) {
    float matrix[9] = {1.1, 6.0, -2.8, 0, 8.1, -3.3, 0.6, 0.9, -5.2};
    Kernel * ker = new Kernel(3, matrix);

    ASSERT_EQ(ker->get_size(), 3);

    for (int i = 0; i < 9; i++) {
        ASSERT_EQ(matrix[i], ker->get_at(i));
    }
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            ASSERT_EQ(matrix[x+y*3], ker->get_at(x, y));
        }
    }
}