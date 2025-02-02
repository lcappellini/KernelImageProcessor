//
// Created by Lorenzo on 31/01/2025.
//

#ifndef KERNELIMAGEPROCESSOR_PNMFILEHANDLER_H
#define KERNELIMAGEPROCESSOR_PNMFILEHANDLER_H

#include <string>
#include "PixelImage.h"

using namespace std;

class PNMFileHandler {
private:
    static const string validExts[3];
    static bool hasValidExtension(const string& filename);
public:
    static Image * load(const string& filename);
    static bool save(Image * image, const string& filename);

    static bool save_plain(Image *image, const string &filename);
};


#endif //KERNELIMAGEPROCESSOR_PNMFILEHANDLER_H
