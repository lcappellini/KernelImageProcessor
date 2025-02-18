#include <iostream>
#include "PNMFileHandler.h"
#include "ImageEditor.h"

int main() {

    //Image * img = PNMFileHandler::load(R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\animal.ppm)");
    //Image * img = PNMFileHandler::load(R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\animal_bytes.ppm)");
    //Image * img = PNMFileHandler::load(R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\animal_bytescomm.ppm)");
    Image * img = PNMFileHandler::load(R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\statue_bytes16.ppm)");

    //PNMFileHandler::save_plain(img, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\animal_comm.ppm)");

    cout << img->get_width() << endl;
    cout << img->get_height() << endl;
    cout << (int)img->get_nChannels() << endl;
    cout << (int)img->get_bitDepth() << endl;


    /*Image * grimg = ImageEditor::grayscale(img);

    PNMFileHandler::save_plain(img, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\KernelImageProcessor\test\data\P3_16.ppm)");
    PNMFileHandler::save(img, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\KernelImageProcessor\test\data\P6_16.ppm)");

    PNMFileHandler::save_plain(grimg, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\KernelImageProcessor\test\data\P2_16.ppm)");
    PNMFileHandler::save(grimg, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\KernelImageProcessor\test\data\P5_16.ppm)");*/

    /*uint16_t *pixel = img->get_at(51, 77);
    for (int i = 0; i < img->get_nChannels(); i++) {
        cout << (int)pixel[i] << ",";
    }
    cout << endl;*/

    /*Kernel *ker = new Kernel(3, new float[9]{-1, -1, -1, -1, 8, -1, -1, -1, -1});

    Image * newimg = ImageEditor::convolve(img, ker, KernelMode::Crop, new uint8_t[]{255, 0, 0});
    PNMFileHandler::save_plain(newimg, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\mod_animal.ppm)");
    PNMFileHandler::save(newimg, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\mod_animal_bytes.ppm)");

    Image * invimg = ImageEditor::invert(img);
    PNMFileHandler::save(invimg, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\inv_animal.ppm)");

    Image * brimg = ImageEditor::change_brightness(img, 0.5);
    PNMFileHandler::save(brimg, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\br_animal.ppm)");

    Image * grimg = ImageEditor::grayscale(img);
    PNMFileHandler::save(grimg, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\gr_animal.pgm)");

    Image * shimg = ImageEditor::sharpen(img);
    PNMFileHandler::save(shimg, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\sh_animal.ppm)");

    Image * blimg = ImageEditor::gaussian_blur3(img);
    PNMFileHandler::save(blimg, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\bl_animal.ppm)");

    Image * crimg = ImageEditor::crop(img, 14, 33, 61, 74);
    PNMFileHandler::save(crimg, R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\cr_animal.ppm)");


    Image *p1 = PNMFileHandler::load(R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\tests\p1.pbm)");
    cout << "p1 loaded" << endl;
    Image *p2 = PNMFileHandler::load(R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\tests\p2.pgm)");
    cout << "p2 loaded" << endl;
    Image *p3 = PNMFileHandler::load(R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\tests\p3.ppm)");
    cout << "p3 loaded" << endl;
    Image *p4 = PNMFileHandler::load(R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\tests\p4.pbm)");
    cout << "p4 loaded" << endl;
    Image *p5 = PNMFileHandler::load(R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\tests\p5.pgm)");
    cout << "p5 loaded" << endl;
    Image *p6 = PNMFileHandler::load(R"(C:\Users\Lorenzo\Desktop\UNI\LAB PROG\example files\tests\p6.ppm)");
    cout << "p6 loaded" << endl;*/



    return 0;
}
