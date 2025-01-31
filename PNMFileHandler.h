//
// Created by Lorenzo on 31/01/2025.
//

#ifndef KERNELIMAGEPROCESSOR_PNMFILEHANDLER_H
#define KERNELIMAGEPROCESSOR_PNMFILEHANDLER_H

#include <string>
#include "PixelImage.h"

using namespace std;

class PNMFileHandler {
public:
    static Image * load(const string& filename);
    static bool save(Image * image, const string& filename);
};


#endif //KERNELIMAGEPROCESSOR_PNMFILEHANDLER_H
