//
// Created by Lorenzo on 31/01/2025.
//

#ifndef KERNELIMAGEPROCESSOR_PNMFILEHANDLER_H
#define KERNELIMAGEPROCESSOR_PNMFILEHANDLER_H

#include <string>
#include "Image.h"

using namespace std;

class PNMFileHandler {
public:
    static Image * load(const string& filename);
    bool save(Image * image, string filename);
};


#endif //KERNELIMAGEPROCESSOR_PNMFILEHANDLER_H
