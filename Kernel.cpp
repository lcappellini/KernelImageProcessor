//
// Created by Lorenzo on 01/02/2025.
//

#include <stdexcept>
#include "Kernel.h"

using namespace std;

Kernel::Kernel(uint16_t size, float *matrix) : size(size), matrix(matrix) {

}

uint16_t Kernel::get_size() const {
    return size;
}

float Kernel::get_at_unsafe(int index) {
    return matrix[index];
}

float Kernel::get_at(int index) {
    if (index < 0 || index > size*size)
        throw out_of_range("Index out of bounds");
    return matrix[index];
}

float Kernel::get_at(int x, int y) {
    if (x < 0 || x >= size || y < 0 || y >= size)
        throw out_of_range("Indexes out of bounds");
    return get_at(x+y*size);
}
