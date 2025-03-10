#include "gtest/gtest.h"
#include <filesystem>

#include "../PNMFileHandler.h"
#include "../ImageEditor.h"

using namespace std;

class ImageEditorSuite : public ::testing::Test {
protected:
    static string dataDir;

    static Image * img_1ch_8bit;
    static Image * img_1ch_16bit;
    static Image * img_3ch_8bit;
    static Image * img_3ch_16bit;
    static Kernel * edge_detection_kernel;

    static void SetUpTestSuite(){
        img_1ch_8bit = PNMFileHandler::load(dataDir + "P5_8.pgm");
        img_1ch_16bit = PNMFileHandler::load(dataDir + "P5_16.pgm");
        img_3ch_8bit = PNMFileHandler::load(dataDir + "P6_8.ppm");
        img_3ch_16bit = PNMFileHandler::load(dataDir + "P6_16.ppm");
        edge_detection_kernel = new Kernel(3, new float[9]{-1, -1, -1, -1, 8, -1, -1, -1, -1});
    }

    static void TearDownTestSuite() {
        delete img_1ch_8bit;
        delete img_1ch_16bit;
        delete img_3ch_8bit;
        delete img_3ch_16bit;
        delete edge_detection_kernel;
    }
};

string ImageEditorSuite::dataDir = "test/data/";
Image * ImageEditorSuite::img_1ch_8bit = nullptr;
Image * ImageEditorSuite::img_1ch_16bit = nullptr;
Image * ImageEditorSuite::img_3ch_8bit = nullptr;
Image * ImageEditorSuite::img_3ch_16bit = nullptr;
Kernel * ImageEditorSuite::edge_detection_kernel = nullptr;


// 1 channel, 8 bits
TEST_F(ImageEditorSuite, Test_1ch_8bit_invert) {
    Image * new_img = ImageEditor::invert(img_1ch_8bit);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 100);
    ASSERT_EQ(new_img->get_height(), 100);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 8);

    pixel = new_img->get_at(72, 8);
    ASSERT_EQ(pixel[0], 102);
    delete[] pixel;
    pixel = new_img->get_at(3745);
    ASSERT_EQ(pixel[0], 141);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_1ch_8bit_change_brightness) {
    Image * new_img = ImageEditor::change_brightness(img_1ch_8bit, 1.2);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 100);
    ASSERT_EQ(new_img->get_height(), 100);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 8);

    pixel = new_img->get_at(62, 7);
    ASSERT_EQ(pixel[0], 202);
    delete[] pixel;
    pixel = new_img->get_at(3700);
    ASSERT_EQ(pixel[0], 197);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_1ch_8bit_crop) {
    Image * new_img = ImageEditor::crop(img_1ch_8bit, 10, 10, 30, 30);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 20);
    ASSERT_EQ(new_img->get_height(), 20);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 8);

    pixel = new_img->get_at(5, 7);
    ASSERT_EQ(pixel[0], 195);
    delete[] pixel;
    pixel = new_img->get_at(67);
    ASSERT_EQ(pixel[0], 165);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_1ch_8bit_grayscale) {
    ASSERT_THROW({ImageEditor::grayscale(img_1ch_8bit);}, runtime_error);
}

TEST_F(ImageEditorSuite, Test_1ch_8bit_sharpen) {
    Image * new_img = ImageEditor::sharpen(img_1ch_8bit);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 100);
    ASSERT_EQ(new_img->get_height(), 100);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 8);

    pixel = new_img->get_at(50, 70);
    ASSERT_EQ(pixel[0], 51);
    delete[] pixel;
    pixel = new_img->get_at(607);
    ASSERT_EQ(pixel[0], 147);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_1ch_8bit_gaussian_blur) {
    Image * new_img = ImageEditor::gaussian_blur3(img_1ch_8bit);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 100);
    ASSERT_EQ(new_img->get_height(), 100);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 8);

    pixel = new_img->get_at(50, 7);
    ASSERT_EQ(pixel[0], 147);
    delete[] pixel;
    pixel = new_img->get_at(11);
    ASSERT_EQ(pixel[0], 127);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_1ch_8bit_convolve) {
    Image * new_img;
    uint16_t * pixel;
    
    new_img = ImageEditor::convolve(img_1ch_8bit, edge_detection_kernel, KernelMode::Wrap);    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 100);
    ASSERT_EQ(new_img->get_height(), 100);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 8);
    pixel = new_img->get_at(0, 0);
    ASSERT_EQ(pixel[0], 7);
    delete[] pixel;
    pixel = new_img->get_at(100);
    ASSERT_EQ(pixel[0], 0);
    delete[] pixel;

    new_img = ImageEditor::convolve(img_1ch_8bit, edge_detection_kernel, KernelMode::KernelCrop);
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 100);
    ASSERT_EQ(new_img->get_height(), 100);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 8);
    pixel = new_img->get_at(0, 0);
    ASSERT_EQ(pixel[0], 255);
    delete[] pixel;
    pixel = new_img->get_at(100);
    ASSERT_EQ(pixel[0], 255);
    delete[] pixel;

    new_img = ImageEditor::convolve(img_1ch_8bit, edge_detection_kernel, KernelMode::Extend);
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 100);
    ASSERT_EQ(new_img->get_height(), 100);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 8);
    pixel = new_img->get_at(0, 0);
    ASSERT_EQ(pixel[0], 9);
    delete[] pixel;
    pixel = new_img->get_at(100);
    ASSERT_EQ(pixel[0], 0);
    delete[] pixel;
    
    new_img = ImageEditor::convolve(img_1ch_8bit, edge_detection_kernel, KernelMode::Crop);
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 98);
    ASSERT_EQ(new_img->get_height(), 98);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 8);
    pixel = new_img->get_at(0, 0);
    ASSERT_EQ(pixel[0], 46);
    delete[] pixel;
    pixel = new_img->get_at(98);
    ASSERT_EQ(pixel[0], 0);
    delete[] pixel;
    delete new_img;
}


// 1 channel, 16 bits
TEST_F(ImageEditorSuite, Test_1ch_16bit_invert) {
    Image * new_img = ImageEditor::invert(img_1ch_16bit);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    pixel = new_img->get_at(72, 8);
    ASSERT_EQ(pixel[0], 49825);
    delete[] pixel;
    pixel = new_img->get_at(3745);
    ASSERT_EQ(pixel[0], 52631);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_1ch_16bit_change_brightness) {
    Image * new_img = ImageEditor::change_brightness(img_1ch_16bit, 1.2);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    pixel = new_img->get_at(62, 7);
    ASSERT_EQ(pixel[0], 29178);
    delete[] pixel;
    pixel = new_img->get_at(3700);
    ASSERT_EQ(pixel[0], 26320);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_1ch_16bit_crop) {
    Image * new_img = ImageEditor::crop(img_1ch_16bit, 10, 10, 30, 30);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 20);
    ASSERT_EQ(new_img->get_height(), 20);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    pixel = new_img->get_at(5, 7);
    ASSERT_EQ(pixel[0], 11458);
    delete[] pixel;
    pixel = new_img->get_at(67);
    ASSERT_EQ(pixel[0], 11526);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_1ch_16bit_grayscale) {
    ASSERT_THROW({ImageEditor::grayscale(img_1ch_16bit);}, runtime_error);
}

TEST_F(ImageEditorSuite, Test_1ch_16bit_sharpen) {
    Image * new_img = ImageEditor::sharpen(img_1ch_16bit);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    pixel = new_img->get_at(50, 70);
    ASSERT_EQ(pixel[0], 12112);
    delete[] pixel;
    pixel = new_img->get_at(607);
    ASSERT_EQ(pixel[0], 17188);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_1ch_16bit_gaussian_blur) {
    Image * new_img = ImageEditor::gaussian_blur3(img_1ch_16bit);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    pixel = new_img->get_at(50, 7);
    ASSERT_EQ(pixel[0], 15011);
    delete[] pixel;
    pixel = new_img->get_at(11);
    ASSERT_EQ(pixel[0], 12884);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_1ch_16bit_convolve) {
    Image * new_img;
    uint16_t * pixel;

    new_img = ImageEditor::convolve(img_1ch_16bit, edge_detection_kernel, KernelMode::Wrap);
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 16);
    pixel = new_img->get_at(0, 0);
    ASSERT_EQ(pixel[0], 0);
    delete[] pixel;
    pixel = new_img->get_at(249999);
    ASSERT_EQ(pixel[0], 6491);
    delete[] pixel;

    new_img = ImageEditor::convolve(img_1ch_16bit, edge_detection_kernel, KernelMode::KernelCrop);
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 16);
    pixel = new_img->get_at(0, 0);
    ASSERT_EQ(pixel[0], 65535);
    delete[] pixel;
    pixel = new_img->get_at(249999);
    ASSERT_EQ(pixel[0], 65535);
    delete[] pixel;

    new_img = ImageEditor::convolve(img_1ch_16bit, edge_detection_kernel, KernelMode::Extend);
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 16);
    pixel = new_img->get_at(0, 0);
    ASSERT_EQ(pixel[0], 1259);
    delete[] pixel;
    pixel = new_img->get_at(249999);
    ASSERT_EQ(pixel[0], 0);
    delete[] pixel;

    new_img = ImageEditor::convolve(img_1ch_16bit, edge_detection_kernel, KernelMode::Crop);
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 498);
    ASSERT_EQ(new_img->get_height(), 498);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 16);
    pixel = new_img->get_at(0, 0);
    ASSERT_EQ(pixel[0], 0);
    delete[] pixel;
    pixel = new_img->get_at(248003);
    ASSERT_EQ(pixel[0], 0);
    delete[] pixel;
    delete new_img;
}


// 3 channels, 8 bits
TEST_F(ImageEditorSuite, Test_3ch_8bit_invert) {
    Image * new_img = ImageEditor::invert(img_3ch_8bit);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 100);
    ASSERT_EQ(new_img->get_height(), 100);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 8);

    pixel = new_img->get_at(73, 40);
    ASSERT_EQ(pixel[0], 76);
    ASSERT_EQ(pixel[1], 109);
    ASSERT_EQ(pixel[2], 144);
    delete[] pixel;
    pixel = new_img->get_at(2675);
    ASSERT_EQ(pixel[0], 87);
    ASSERT_EQ(pixel[1], 122);
    ASSERT_EQ(pixel[2], 162);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_3ch_8bit_change_brightness) {
    Image * new_img = ImageEditor::change_brightness(img_3ch_8bit, 1.2);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 100);
    ASSERT_EQ(new_img->get_height(), 100);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 8);

    pixel = new_img->get_at(40, 1);
    ASSERT_EQ(pixel[0], 191);
    ASSERT_EQ(pixel[1], 179);
    ASSERT_EQ(pixel[2], 191);
    delete[] pixel;
    pixel = new_img->get_at(3490);
    ASSERT_EQ(pixel[0], 228);
    ASSERT_EQ(pixel[1], 206);
    ASSERT_EQ(pixel[2], 193);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_3ch_8bit_crop) {
    Image * new_img = ImageEditor::crop(img_3ch_8bit, 10, 10, 30, 30);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 20);
    ASSERT_EQ(new_img->get_height(), 20);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 8);

    pixel = new_img->get_at(6, 0);
    ASSERT_EQ(pixel[0], 185);
    ASSERT_EQ(pixel[1], 175);
    ASSERT_EQ(pixel[2], 173);
    delete[] pixel;
    pixel = new_img->get_at(227);
    ASSERT_EQ(pixel[0], 206);
    ASSERT_EQ(pixel[1], 196);
    ASSERT_EQ(pixel[2], 186);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_3ch_8bit_grayscale) {
    Image * new_img = ImageEditor::grayscale(img_3ch_8bit);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 100);
    ASSERT_EQ(new_img->get_height(), 100);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 8);

    pixel = new_img->get_at(7, 66);
    ASSERT_EQ(pixel[0], 117);
    delete[] pixel;
    pixel = new_img->get_at(9756);
    ASSERT_EQ(pixel[0], 75);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_3ch_8bit_sharpen) {
    Image * new_img = ImageEditor::sharpen(img_3ch_8bit);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 100);
    ASSERT_EQ(new_img->get_height(), 100);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 8);

    pixel = new_img->get_at(42, 50);
    ASSERT_EQ(pixel[0], 145);
    ASSERT_EQ(pixel[1], 110);
    ASSERT_EQ(pixel[2], 83);
    delete[] pixel;
    pixel = new_img->get_at(2058);
    ASSERT_EQ(pixel[0], 177);
    ASSERT_EQ(pixel[1], 153);
    ASSERT_EQ(pixel[2], 126);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_3ch_8bit_gaussian_blur) {
    Image * new_img = ImageEditor::gaussian_blur3(img_3ch_8bit);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 100);
    ASSERT_EQ(new_img->get_height(), 100);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 8);

    pixel = new_img->get_at(85, 49);
    ASSERT_EQ(pixel[0], 169);
    ASSERT_EQ(pixel[1], 143);
    ASSERT_EQ(pixel[2], 119);
    delete[] pixel;
    pixel = new_img->get_at(3425);
    ASSERT_EQ(pixel[0], 106);
    ASSERT_EQ(pixel[1], 83);
    ASSERT_EQ(pixel[2], 69);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_3ch_8bit_convolve) {
    Image * new_img;
    uint16_t * pixel;

    new_img = ImageEditor::convolve(img_3ch_8bit, edge_detection_kernel, KernelMode::Wrap);
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 100);
    ASSERT_EQ(new_img->get_height(), 100);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 8);
    pixel = new_img->get_at(0, 0);
    ASSERT_EQ(pixel[0], 0);
    ASSERT_EQ(pixel[1], 22);
    ASSERT_EQ(pixel[2], 95);
    delete[] pixel;
    pixel = new_img->get_at(100);
    ASSERT_EQ(pixel[0], 0);
    ASSERT_EQ(pixel[1], 0);
    ASSERT_EQ(pixel[2], 0);
    delete[] pixel;

    new_img = ImageEditor::convolve(img_3ch_8bit, edge_detection_kernel, KernelMode::KernelCrop);
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 100);
    ASSERT_EQ(new_img->get_height(), 100);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 8);
    pixel = new_img->get_at(0, 0);
    ASSERT_EQ(pixel[0], 255);
    ASSERT_EQ(pixel[1], 255);
    ASSERT_EQ(pixel[2], 255);
    delete[] pixel;
    pixel = new_img->get_at(100);
    ASSERT_EQ(pixel[0], 255);
    ASSERT_EQ(pixel[1], 255);
    ASSERT_EQ(pixel[2], 255);
    delete[] pixel;

    new_img = ImageEditor::convolve(img_3ch_8bit, edge_detection_kernel, KernelMode::Extend);
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 100);
    ASSERT_EQ(new_img->get_height(), 100);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 8);
    pixel = new_img->get_at(0, 0);
    ASSERT_EQ(pixel[0], 0);
    ASSERT_EQ(pixel[1], 12);
    ASSERT_EQ(pixel[2], 10);
    delete[] pixel;
    pixel = new_img->get_at(100);
    ASSERT_EQ(pixel[0], 0);
    ASSERT_EQ(pixel[1], 0);
    ASSERT_EQ(pixel[2], 0);
    delete[] pixel;

    new_img = ImageEditor::convolve(img_3ch_8bit, edge_detection_kernel, KernelMode::Crop);
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 98);
    ASSERT_EQ(new_img->get_height(), 98);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 8);
    pixel = new_img->get_at(0, 0);
    ASSERT_EQ(pixel[0], 41);
    ASSERT_EQ(pixel[1], 45);
    ASSERT_EQ(pixel[2], 44);
    delete[] pixel;
    pixel = new_img->get_at(100);
    ASSERT_EQ(pixel[0], 189);
    ASSERT_EQ(pixel[1], 189);
    ASSERT_EQ(pixel[2], 188);
    delete[] pixel;
    delete new_img;
}

// 3 channels, 16 bits
TEST_F(ImageEditorSuite, Test_3ch_16bit_invert) {
    Image * new_img = ImageEditor::invert(img_3ch_16bit);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    pixel = new_img->get_at(125, 117);
    ASSERT_EQ(pixel[0], 55769);
    ASSERT_EQ(pixel[1], 55092);
    ASSERT_EQ(pixel[2], 53779);
    delete[] pixel;
    pixel = new_img->get_at(73555);
    ASSERT_EQ(pixel[0], 59592);
    ASSERT_EQ(pixel[1], 58919);
    ASSERT_EQ(pixel[2], 58071);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_3ch_16bit_change_brightness) {
    Image * new_img = ImageEditor::change_brightness(img_3ch_16bit, 1.2);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    pixel = new_img->get_at(141, 4);
    ASSERT_EQ(pixel[0], 28080);
    ASSERT_EQ(pixel[1], 29585);
    ASSERT_EQ(pixel[2], 32162);
    delete[] pixel;
    pixel = new_img->get_at(39668);
    ASSERT_EQ(pixel[0], 19477);
    ASSERT_EQ(pixel[1], 20005);
    ASSERT_EQ(pixel[2], 20184);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_3ch_16bit_crop) {
    Image * new_img = ImageEditor::crop(img_3ch_16bit, 100, 100, 300, 300);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 200);
    ASSERT_EQ(new_img->get_height(), 200);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    pixel = new_img->get_at(104, 174);
    ASSERT_EQ(pixel[0], 6041);
    ASSERT_EQ(pixel[1], 6757);
    ASSERT_EQ(pixel[2], 7146);
    delete[] pixel;
    pixel = new_img->get_at(17633);
    ASSERT_EQ(pixel[0], 4806);
    ASSERT_EQ(pixel[1], 5134);
    ASSERT_EQ(pixel[2], 5035);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_3ch_16bit_grayscale) {
    Image * new_img = ImageEditor::grayscale(img_3ch_16bit);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    pixel = new_img->get_at(75, 108);
    ASSERT_EQ(pixel[0], 14959);
    delete[] pixel;
    pixel = new_img->get_at(87155);
    ASSERT_EQ(pixel[0], 6291);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_3ch_16bit_sharpen) {
    Image * new_img = ImageEditor::sharpen(img_3ch_16bit);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    pixel = new_img->get_at(373, 32);
    ASSERT_EQ(pixel[0], 19005);
    ASSERT_EQ(pixel[1], 22329);
    ASSERT_EQ(pixel[2], 6095);
    delete[] pixel;
    pixel = new_img->get_at(199020);
    ASSERT_EQ(pixel[0], 10769);
    ASSERT_EQ(pixel[1], 13044);
    ASSERT_EQ(pixel[2], 13648);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_3ch_16bit_gaussian_blur) {
    Image * new_img = ImageEditor::gaussian_blur3(img_3ch_16bit);
    uint16_t * pixel;
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    pixel = new_img->get_at(467, 247);
    ASSERT_EQ(pixel[0], 28816);
    ASSERT_EQ(pixel[1], 28532);
    ASSERT_EQ(pixel[2], 26960);
    delete[] pixel;
    pixel = new_img->get_at(141390);
    ASSERT_EQ(pixel[0], 32144);
    ASSERT_EQ(pixel[1], 32209);
    ASSERT_EQ(pixel[2], 30474);
    delete[] pixel;
    delete new_img;
}

TEST_F(ImageEditorSuite, Test_3ch_16bit_convolve) {
    Image * new_img;
    uint16_t * pixel;

    new_img = ImageEditor::convolve(img_3ch_16bit, edge_detection_kernel, KernelMode::Wrap);

    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 16);
    pixel = new_img->get_at(0, 0);
    ASSERT_EQ(pixel[0], 0);
    ASSERT_EQ(pixel[1], 0);
    ASSERT_EQ(pixel[2], 0);
    delete[] pixel;
    pixel = new_img->get_at(249999);
    ASSERT_EQ(pixel[0], 14535);
    ASSERT_EQ(pixel[1], 2350);
    ASSERT_EQ(pixel[2], 6735);
    delete[] pixel;

    new_img = ImageEditor::convolve(img_3ch_16bit, edge_detection_kernel, KernelMode::KernelCrop);
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 16);
    pixel = new_img->get_at(0, 0);
    ASSERT_EQ(pixel[0], 65535);
    ASSERT_EQ(pixel[1], 65535);
    ASSERT_EQ(pixel[2], 65535);
    delete[] pixel;
    pixel = new_img->get_at(249999);
    ASSERT_EQ(pixel[0], 65535);
    ASSERT_EQ(pixel[1], 65535);
    ASSERT_EQ(pixel[2], 65535);
    delete[] pixel;

    new_img = ImageEditor::convolve(img_3ch_16bit, edge_detection_kernel, KernelMode::Extend);
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 16);
    pixel = new_img->get_at(0, 0);
    ASSERT_EQ(pixel[0], 1513);
    ASSERT_EQ(pixel[1], 1255);
    ASSERT_EQ(pixel[2], 614);
    delete[] pixel;
    pixel = new_img->get_at(249999);
    ASSERT_EQ(pixel[0], 0);
    ASSERT_EQ(pixel[1], 0);
    ASSERT_EQ(pixel[2], 0);
    delete[] pixel;

    new_img = ImageEditor::convolve(img_3ch_16bit, edge_detection_kernel, KernelMode::Crop);
    
    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 498);
    ASSERT_EQ(new_img->get_height(), 498);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 16);
    pixel = new_img->get_at(0, 0);
    ASSERT_EQ(pixel[0], 20);
    ASSERT_EQ(pixel[1], 0);
    ASSERT_EQ(pixel[2], 0);
    delete[] pixel;
    pixel = new_img->get_at(248003);
    ASSERT_EQ(pixel[0], 0);
    ASSERT_EQ(pixel[1], 0);
    ASSERT_EQ(pixel[2], 205);
    delete[] pixel;
    delete new_img;
}



















