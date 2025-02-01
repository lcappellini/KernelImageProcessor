#include <iostream>
#include "PixelImage.h"
#include "PNMFileHandler.h"
#include "ImageEditor.h"

int main() {
    Image *img = PNMFileHandler::load(R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\desert.ppm)");

    cout << img->get_width() << endl;
    cout << img->get_height() << endl;
    cout << (int) img->get_nChannels() << endl;

    uint8_t *pixel = img->get_at(3, 0);
    for (int i = 0; i < img->get_nChannels(); i++) {
        cout << (int) pixel[i] << ",";
    }
    cout << endl;

    Kernel *ker = new Kernel(3, new float[9]{-1, -1, -1, -1, 8, -1, -1, -1, -1});

    Image * newimg = ImageEditor::convolve(img, ker, KernelMode::Extend);
    bool success = PNMFileHandler::save(newimg, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\mod_desert.ppm)");
    cout << success << endl;

    return 0;
}
