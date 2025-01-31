#include <iostream>
#include "PixelImage.h"
#include "PNMFileHandler.h"

int main() {
    Image * img = PNMFileHandler::load(R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\image.ppm)");

    cout << img->get_width() << endl;
    cout << img->get_height() << endl;
    cout << (int)img->get_nChannels() << endl;

    uint8_t * pixel = img->get_at(3, 0);
    for (int i = 0; i < img->get_nChannels() ; i++) {
        cout << (int)pixel[i] << ",";
    }
    cout << endl;

    img->set_at(0, 0, new uint8_t[]{255, 255, 255});

    bool success = PNMFileHandler::save(img, "new_img.ppm");
    cout << success << endl;

    return 0;
}
