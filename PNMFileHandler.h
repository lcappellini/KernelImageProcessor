//
// Created by Lorenzo on 31/01/2025.
//

#ifndef KERNELIMAGEPROCESSOR_PNMFILEHANDLER_H
#define KERNELIMAGEPROCESSOR_PNMFILEHANDLER_H

#include <string>
#include "PixelImage.h"

using namespace std;

namespace PNMFileHandler {
    bool hasValidExtension(const string& filename);

    Image * load(const string& filename);
    int save(Image * image, const string& filename, bool plain=false, const string& comments="Made with KernelImageProcessor");
};

#endif //KERNELIMAGEPROCESSOR_PNMFILEHANDLER_H
