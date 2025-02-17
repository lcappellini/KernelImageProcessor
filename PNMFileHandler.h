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
    static const string validExtensions[3];
    static bool hasValidExtension(const string& filename);
public:
    static string defaultComment;
    static Image * load(const string& filename);
    static int save(Image * image, const string& filename, const string& comments=defaultComment);

    static int save_plain(Image *image, const string &filename, const string& comments=defaultComment);
};

#endif //KERNELIMAGEPROCESSOR_PNMFILEHANDLER_H
