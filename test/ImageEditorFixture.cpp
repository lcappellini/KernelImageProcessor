#include "gtest/gtest.h"
#include <filesystem>

#include "../PNMFileHandler.h"
#include "../ImageEditor.h"

using namespace std;

class ImageEditorSuite : public ::testing::Test {
protected:
    static string outputDir;
    static string dataDir;

    static Image * img_1ch_8bit;
    static Image * img_1ch_16bit;
    static Image * img_3ch_8bit;
    static Image * img_3ch_16bit;

    static void SetUpTestSuite(){
        img_1ch_8bit = PNMFileHandler::load(dataDir + "P5_8.pgm");
        img_1ch_16bit = PNMFileHandler::load(dataDir + "P5_16.pgm");
        img_3ch_8bit = PNMFileHandler::load(dataDir + "P6_8.ppm");
        img_3ch_16bit = PNMFileHandler::load(dataDir + "P6_16.ppm");
    }

    void SetUp() override {
        filesystem::create_directories(outputDir);
    }

    void TearDown() override {
    }

    static void TearDownTestSuite() {
        filesystem::remove_all(outputDir);
    }
};

string ImageEditorSuite::outputDir = "test/output/";
string ImageEditorSuite::dataDir = "test/data/";
Image * ImageEditorSuite::img_1ch_8bit = nullptr;
Image * ImageEditorSuite::img_1ch_16bit = nullptr;
Image * ImageEditorSuite::img_3ch_8bit = nullptr;
Image * ImageEditorSuite::img_3ch_16bit = nullptr;

/*
 - tutti gli effetti per immagini a 1 canale e 3 canali, 8 e 16 bitDepth
 */

// 1 channel, 8 bits
TEST_F(ImageEditorSuite, Test_1ch_8bit_invert) {
    Image * new_img = ImageEditor::invert(img_1ch_8bit);

    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 100);
    ASSERT_EQ(new_img->get_height(), 100);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 8);

    ASSERT_EQ(new_img->get_at(72, 8)[0], 102);
    ASSERT_EQ(new_img->get_at(3745)[0], 141);
}

TEST_F(ImageEditorSuite, Test_1ch_8bit_change_brightness) {
    Image * new_img = ImageEditor::change_brightness(img_1ch_8bit, 1.2);

    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 100);
    ASSERT_EQ(new_img->get_height(), 100);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 8);

    ASSERT_EQ(new_img->get_at(62, 7)[0], 202);
    ASSERT_EQ(new_img->get_at(3700)[0], 197);
}

TEST_F(ImageEditorSuite, Test_1ch_8bit_crop) {
    Image * new_img = ImageEditor::crop(img_1ch_8bit, 10, 10, 30, 30);

    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 20);
    ASSERT_EQ(new_img->get_height(), 20);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 8);

    ASSERT_EQ(new_img->get_at(5, 7)[0], 195);
    ASSERT_EQ(new_img->get_at(67)[0], 165);
}

TEST_F(ImageEditorSuite, Test_1ch_8bit_grayscale) {
    ASSERT_THROW({ImageEditor::grayscale(img_1ch_8bit);}, runtime_error);
}

TEST_F(ImageEditorSuite, Test_1ch_8bit_sharpen) {
    Image * new_img = ImageEditor::sharpen(img_1ch_8bit);

    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 100);
    ASSERT_EQ(new_img->get_height(), 100);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 8);

    ASSERT_EQ(new_img->get_at(50, 70)[0], 51);
    ASSERT_EQ(new_img->get_at(607)[0], 147);
}

TEST_F(ImageEditorSuite, Test_1ch_8bit_gaussian_blur) {
    Image * new_img = ImageEditor::gaussian_blur3(img_1ch_8bit);

    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 100);
    ASSERT_EQ(new_img->get_height(), 100);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 8);

    ASSERT_EQ(new_img->get_at(50, 7)[0], 147);
    ASSERT_EQ(new_img->get_at(11)[0], 127);
}


// 1 channel, 16 bits
TEST_F(ImageEditorSuite, Test_1ch_16bit_invert) {
    Image * new_img = ImageEditor::invert(img_1ch_16bit);

    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    EXPECT_EQ(new_img->get_at(72, 8)[0], 49825);
    EXPECT_EQ(new_img->get_at(3745)[0], 52631);
}

TEST_F(ImageEditorSuite, Test_1ch_16bit_change_brightness) {
    Image * new_img = ImageEditor::change_brightness(img_1ch_16bit, 1.2);

    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    EXPECT_EQ(new_img->get_at(62, 7)[0], 29178);
    EXPECT_EQ(new_img->get_at(3700)[0], 26320);
}

TEST_F(ImageEditorSuite, Test_1ch_16bit_crop) {
    Image * new_img = ImageEditor::crop(img_1ch_16bit, 10, 10, 30, 30);

    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 20);
    ASSERT_EQ(new_img->get_height(), 20);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    EXPECT_EQ(new_img->get_at(5, 7)[0], 11458);
    EXPECT_EQ(new_img->get_at(67)[0], 11526);
}

TEST_F(ImageEditorSuite, Test_1ch_16bit_grayscale) {
    ASSERT_THROW({ImageEditor::grayscale(img_1ch_16bit);}, runtime_error);
}

TEST_F(ImageEditorSuite, Test_1ch_16bit_sharpen) {
    Image * new_img = ImageEditor::sharpen(img_1ch_16bit);

    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    EXPECT_EQ(new_img->get_at(50, 70)[0], 12112);
    EXPECT_EQ(new_img->get_at(607)[0], 17188);
}

TEST_F(ImageEditorSuite, Test_1ch_16bit_gaussian_blur) {
    Image * new_img = ImageEditor::gaussian_blur3(img_1ch_16bit);

    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    EXPECT_EQ(new_img->get_at(50, 7)[0], 15011);
    EXPECT_EQ(new_img->get_at(11)[0], 12884);
}

// 3 channels, 16 bits
TEST_F(ImageEditorSuite, Test_3ch_16bit_invert) {
    Image * new_img = ImageEditor::invert(img_3ch_16bit);

    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    uint16_t * pixel;
    pixel = new_img->get_at(125, 117);
    EXPECT_EQ(pixel[0], 55769);
    EXPECT_EQ(pixel[1], 55092);
    EXPECT_EQ(pixel[2], 53779);
    pixel = new_img->get_at(73555);
    EXPECT_EQ(pixel[0], 59592);
    EXPECT_EQ(pixel[1], 58919);
    EXPECT_EQ(pixel[2], 58071);
}

TEST_F(ImageEditorSuite, Test_3ch_16bit_change_brightness) {
    Image * new_img = ImageEditor::change_brightness(img_3ch_16bit, 1.2);

    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    uint16_t * pixel;
    pixel = new_img->get_at(141, 4);
    EXPECT_EQ(pixel[0], 28080);
    EXPECT_EQ(pixel[1], 29585);
    EXPECT_EQ(pixel[2], 32162);
    pixel = new_img->get_at(39668);
    EXPECT_EQ(pixel[0], 19477);
    EXPECT_EQ(pixel[1], 20005);
    EXPECT_EQ(pixel[2], 20184);
}

TEST_F(ImageEditorSuite, Test_3ch_16bit_crop) {
    Image * new_img = ImageEditor::crop(img_3ch_16bit, 100, 100, 300, 300);

    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 200);
    ASSERT_EQ(new_img->get_height(), 200);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    uint16_t * pixel;
    pixel = new_img->get_at(104, 174);
    EXPECT_EQ(pixel[0], 6041);
    EXPECT_EQ(pixel[1], 6757);
    EXPECT_EQ(pixel[2], 7146);
    pixel = new_img->get_at(17633);
    EXPECT_EQ(pixel[0], 4806);
    EXPECT_EQ(pixel[1], 5134);
    EXPECT_EQ(pixel[2], 5035);
}

TEST_F(ImageEditorSuite, Test_3ch_16bit_grayscale) {
    Image * new_img = ImageEditor::grayscale(img_3ch_16bit);

    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 1);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    EXPECT_EQ(new_img->get_at(75, 108)[0], 14959);
    EXPECT_EQ(new_img->get_at(87155)[0], 6291);
}

TEST_F(ImageEditorSuite, Test_3ch_16bit_sharpen) {
    Image * new_img = ImageEditor::sharpen(img_3ch_16bit);

    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    uint16_t * pixel;
    pixel = new_img->get_at(373, 32);
    EXPECT_EQ(pixel[0], 19005);
    EXPECT_EQ(pixel[1], 22329);
    EXPECT_EQ(pixel[2], 6095);
    pixel = new_img->get_at(199020);
    EXPECT_EQ(pixel[0], 10769);
    EXPECT_EQ(pixel[1], 13044);
    EXPECT_EQ(pixel[2], 13648);
}

TEST_F(ImageEditorSuite, Test_3ch_16bit_gaussian_blur) {
    Image * new_img = ImageEditor::gaussian_blur3(img_3ch_16bit);

    ASSERT_NE(new_img, nullptr);
    ASSERT_EQ(new_img->get_width(), 500);
    ASSERT_EQ(new_img->get_height(), 500);
    ASSERT_EQ(new_img->get_nChannels(), 3);
    ASSERT_EQ(new_img->get_bitDepth(), 16);

    uint16_t * pixel;
    pixel = new_img->get_at(467, 247);
    EXPECT_EQ(pixel[0], 28816);
    EXPECT_EQ(pixel[1], 28532);
    EXPECT_EQ(pixel[2], 26960);
    pixel = new_img->get_at(141390);
    EXPECT_EQ(pixel[0], 32144);
    EXPECT_EQ(pixel[1], 32209);
    EXPECT_EQ(pixel[2], 30474);
}



















