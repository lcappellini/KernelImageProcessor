#include <iostream>
#include "PixelImage.h"
#include "PNMFileHandler.h"
#include "ImageEditor.h"

int main() {
    //Image *img = PNMFileHandler::load(R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\animal.ppm)");
    Image *img = PNMFileHandler::load(R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\animal_bytes.ppm)");

    cout << img->get_width() << endl;
    cout << img->get_height() << endl;
    cout << (int) img->get_nChannels() << endl;

    uint8_t *pixel = img->get_at(3, 0);
    for (int i = 0; i < img->get_nChannels(); i++) {
        cout << (int) pixel[i] << ",";
    }
    cout << endl;

    Kernel *ker = new Kernel(3, new float[9]{-1, -1, -1, -1, 8, -1, -1, -1, -1});

    Image * newimg = ImageEditor::convolve(img, ker, KernelMode::Wrap, new uint8_t[]{255, 0, 0});
    bool success = PNMFileHandler::save(newimg, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\mod_animal.ppm)");

    Image * invimg = ImageEditor::invert(img);
    success = PNMFileHandler::save(invimg, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\inv_animal.ppm)");
    cout << success << endl;

    Image * brimg = ImageEditor::change_brightness(img, 100);
    success = PNMFileHandler::save(brimg, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\br_animal.ppm)");
    cout << success << endl;

    Image * grimg = ImageEditor::grayscale(img);
    success = PNMFileHandler::save(grimg, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\gr_animal.ppm)");
    cout << success << endl;

    Image * shimg = ImageEditor::sharpen(img);
    success = PNMFileHandler::save(shimg, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\sh_animal.ppm)");
    cout << success << endl;

    Image * blimg = ImageEditor::gaussian_blur3(img);
    success = PNMFileHandler::save(blimg, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\bl_animal.ppm)");
    cout << success << endl;

    Image * crimg = ImageEditor::crop(img, 14, 33, 61, 74);
    success = PNMFileHandler::save(crimg, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\cr_animal.ppm)");
    cout << success << endl;

    return 0;
}
