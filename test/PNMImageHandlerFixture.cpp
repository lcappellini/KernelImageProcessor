#include "gtest/gtest.h"
#include <filesystem>

#include "../PNMFileHandler.h"

using namespace std;

class PNMImageHandlerSuite : public ::testing::Test {
protected:
    static string outputDir;
    static string dataDir;

    static void SetUpTestSuite(){
        filesystem::create_directories(outputDir);
    }

    void SetUp() override {
    }

    void TearDown() override {
    }

    static void TearDownTestSuite() {
        filesystem::remove_all(outputDir);
    }
};

string PNMImageHandlerSuite::outputDir = "test/output/";
string PNMImageHandlerSuite::dataDir = "test/data/";


TEST_F(PNMImageHandlerSuite, TestFileNotFound) {
    ASSERT_THROW({
        PNMFileHandler::load(dataDir + "notfound.ppm");
        }, runtime_error);
}

TEST_F(PNMImageHandlerSuite, TestP1) {
    Image * img = PNMFileHandler::load(dataDir + "P1.pbm");

    ASSERT_NE(img, nullptr);
    ASSERT_EQ(img->get_width(), 100);
    ASSERT_EQ(img->get_height(), 100);
    ASSERT_EQ(img->get_nChannels(), 1);
    ASSERT_EQ(img->get_bitDepth(), 8);

    ASSERT_EQ(img->get_at(56, 89)[0], 0);
    ASSERT_EQ(img->get_at(4725)[0], 0);

    uint16_t value;
    value = 255;
    img->set_at(90, 12, &value);
    ASSERT_EQ(img->get_at(10, 12)[0], 255);
    value = 0;
    img->set_at(109, &value);
    ASSERT_EQ(img->get_at(109)[0], 0);

    //TODO SHOULD I TEST "SAVE" METHOD? P1/P4 IMAGES ARE LOADED AS P2/P5
}

TEST_F(PNMImageHandlerSuite, TestP4) {
    Image * img = PNMFileHandler::load(dataDir + "P4.pbm");

    ASSERT_NE(img, nullptr);
    ASSERT_EQ(img->get_width(), 100);
    ASSERT_EQ(img->get_height(), 100);
    ASSERT_EQ(img->get_nChannels(), 1);
    ASSERT_EQ(img->get_bitDepth(), 8);

    ASSERT_THROW({img->get_at(60, 110);}, out_of_range);
    ASSERT_THROW({img->get_at(10003);}, out_of_range);

    ASSERT_EQ(img->get_at(50, 60)[0], 0);
    ASSERT_EQ(img->get_at(6179)[0], 255);

    uint16_t value;
    value = 255;
    img->set_at(88, 53, &value);
    ASSERT_EQ(img->get_at(88, 53)[0], 255);
    value = 0;
    img->set_at(2, &value);
    ASSERT_EQ(img->get_at(2)[0], 0);

    //TODO SHOULD I TEST "SAVE" METHOD? P1/P4 IMAGES ARE LOADED AS P2/P5
}

TEST_F(PNMImageHandlerSuite, TestP2_8) {
    Image * img = PNMFileHandler::load(dataDir + "P2_8.pgm");

    ASSERT_NE(img, nullptr);
    ASSERT_EQ(img->get_width(), 100);
    ASSERT_EQ(img->get_height(), 100);
    ASSERT_EQ(img->get_nChannels(), 1);
    ASSERT_EQ(img->get_bitDepth(), 8);

    ASSERT_THROW({img->get_at(60, 110);}, out_of_range);
    ASSERT_THROW({img->get_at(10003);}, out_of_range);

    ASSERT_EQ(img->get_at(0, 0)[0], 131);
    ASSERT_EQ(img->get_at(1)[0], 126);

    uint16_t value;
    value = 255;
    img->set_at(56, 93, &value);
    ASSERT_EQ(img->get_at(56, 93)[0], 255);
    value = 0;
    img->set_at(1452, &value);
    ASSERT_EQ(img->get_at(1452)[0], 0);

    ASSERT_EQ(PNMFileHandler::save(img, outputDir + "P2_8bytes.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P2_8bytes.pgm"));
    ASSERT_EQ(PNMFileHandler::save_plain(img, outputDir + "P2_8plain.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P2_8plain.pgm"));
}

TEST_F(PNMImageHandlerSuite, TestP2_16) {
    Image * img = PNMFileHandler::load(dataDir + "P2_16.pgm");

    ASSERT_NE(img, nullptr);
    ASSERT_EQ(img->get_width(), 500);
    ASSERT_EQ(img->get_height(), 500);
    ASSERT_EQ(img->get_nChannels(), 1);
    ASSERT_EQ(img->get_bitDepth(), 16);

    ASSERT_THROW({img->get_at(501, 0);}, out_of_range);
    ASSERT_THROW({img->get_at(-1);}, out_of_range);

    ASSERT_EQ(img->get_at(300, 200)[0], 13191);
    ASSERT_EQ(img->get_at(1780)[0], 14438);

    uint16_t value;
    value = 10000;
    img->set_at(120, 420, &value);
    ASSERT_EQ(img->get_at(120, 420)[0], 10000);
    value = 0;
    img->set_at(13753, &value);
    ASSERT_EQ(img->get_at(13753)[0], 0);

    ASSERT_EQ(PNMFileHandler::save(img, outputDir + "P2_16bytes.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P2_16bytes.pgm"));
    ASSERT_EQ(PNMFileHandler::save_plain(img, outputDir + "P2_16plain.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P2_16plain.pgm"));
}

TEST_F(PNMImageHandlerSuite, TestP5_8) {
    Image * img = PNMFileHandler::load(dataDir + "P5_8.pgm");

    ASSERT_NE(img, nullptr);
    ASSERT_EQ(img->get_width(), 100);
    ASSERT_EQ(img->get_height(), 100);
    ASSERT_EQ(img->get_nChannels(), 1);
    ASSERT_EQ(img->get_bitDepth(), 8);

    ASSERT_THROW({img->get_at(29, 100);}, out_of_range);
    ASSERT_THROW({img->get_at(-10);}, out_of_range);

    ASSERT_EQ(img->get_at(67, 93)[0], 42);
    ASSERT_EQ(img->get_at(4346)[0], 121);

    uint16_t value;
    value = 187;
    img->set_at(76, 21, &value);
    ASSERT_EQ(img->get_at(76, 21)[0], 187);
    value = 10;
    img->set_at(487, &value);
    ASSERT_EQ(img->get_at(487)[0], 10);

    ASSERT_EQ(PNMFileHandler::save(img, outputDir + "P5_8bytes.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P5_8bytes.pgm"));
    ASSERT_EQ(PNMFileHandler::save_plain(img, outputDir + "P5_8plain.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P5_8plain.pgm"));

    //TODO RELOAD THE SAVED FILE TO CHECK IF CORRECT
}

TEST_F(PNMImageHandlerSuite, TestP5_16) {
    Image * img = PNMFileHandler::load(dataDir + "P5_16.pgm");

    ASSERT_NE(img, nullptr);
    ASSERT_EQ(img->get_width(), 500);
    ASSERT_EQ(img->get_height(), 500);
    ASSERT_EQ(img->get_nChannels(), 1);
    ASSERT_EQ(img->get_bitDepth(), 16);

    ASSERT_THROW({img->get_at(509, 499);}, out_of_range);
    ASSERT_THROW({img->get_at(352323);}, out_of_range);

    ASSERT_EQ(img->get_at(350, 162)[0], 7847);
    ASSERT_EQ(img->get_at(34875)[0], 17674);

    uint16_t value;
    value = 5632;
    img->set_at(30, 154, &value);
    ASSERT_EQ(img->get_at(30, 154)[0], 5632);
    value = 3515;
    img->set_at(4124, &value);
    ASSERT_EQ(img->get_at(4124)[0], 3515);

    ASSERT_EQ(PNMFileHandler::save(img, outputDir + "P5_16bytes.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P5_16bytes.pgm"));
    ASSERT_EQ(PNMFileHandler::save_plain(img, outputDir + "P5_16plain.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P5_16plain.pgm"));
}

TEST_F(PNMImageHandlerSuite, TestP3_8) {
    Image * img = PNMFileHandler::load(dataDir + "P3_8.ppm");

    ASSERT_NE(img, nullptr);
    ASSERT_EQ(img->get_width(), 100);
    ASSERT_EQ(img->get_height(), 100);
    ASSERT_EQ(img->get_nChannels(), 3);
    ASSERT_EQ(img->get_bitDepth(), 8);

    ASSERT_THROW({img->get_at(110, 90);}, out_of_range);
    ASSERT_THROW({img->get_at(-10);}, out_of_range);

    uint16_t * pixel;
    pixel = img->get_at(30, 25);
    ASSERT_EQ(pixel[0], 179);
    ASSERT_EQ(pixel[1], 148);
    ASSERT_EQ(pixel[2], 122);
    delete[] pixel;
    pixel = img->get_at(1200);
    ASSERT_EQ(pixel[0], 185);
    ASSERT_EQ(pixel[1], 176);
    ASSERT_EQ(pixel[2], 170);
    delete[] pixel;

    uint16_t new_pixel1[3] = {105, 120, 220};
    img->set_at(67, 86, new_pixel1);
    pixel = img->get_at(67, 86);
    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(pixel[i], new_pixel1[i]);
    }
    uint16_t new_pixel2[3] = {70, 128, 255};
    img->set_at(8343, new_pixel2);
    pixel = img->get_at(8343);
    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(pixel[i], new_pixel2[i]);
    }

    ASSERT_EQ(PNMFileHandler::save(img, outputDir + "P3_8bytes.ppm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P3_8bytes.ppm"));
    ASSERT_EQ(PNMFileHandler::save_plain(img, outputDir + "P3_8plain.ppm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P3_8plain.ppm"));
}

TEST_F(PNMImageHandlerSuite, TestP3_16) {
    Image * img = PNMFileHandler::load(dataDir + "P3_16.ppm");

    ASSERT_NE(img, nullptr);
    ASSERT_EQ(img->get_width(), 500);
    ASSERT_EQ(img->get_height(), 500);
    ASSERT_EQ(img->get_nChannels(), 3);
    ASSERT_EQ(img->get_bitDepth(), 16);

    ASSERT_THROW({img->get_at(500, 90);}, out_of_range);
    ASSERT_THROW({img->get_at(250000);}, out_of_range);

    uint16_t * pixel;
    pixel = img->get_at(283, 498);
    ASSERT_EQ(pixel[0], 11058);
    ASSERT_EQ(pixel[1], 11211);
    ASSERT_EQ(pixel[2], 12063);
    delete[] pixel;
    pixel = img->get_at(100000);
    ASSERT_EQ(pixel[0], 7888);
    ASSERT_EQ(pixel[1], 8360);
    ASSERT_EQ(pixel[2], 9298);
    delete[] pixel;

    uint16_t new_pixel1[3] = {46573, 45325, 29420};
    img->set_at(234, 90, new_pixel1);
    pixel = img->get_at(234, 90);
    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(pixel[i], new_pixel1[i]);
    }
    uint16_t new_pixel2[3] = {0, 65535, 12800};
    img->set_at(43236, new_pixel2);
    pixel = img->get_at(43236);
    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(pixel[i], new_pixel2[i]);
    }

    ASSERT_EQ(PNMFileHandler::save(img, outputDir + "P3_16bytes.ppm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P3_16bytes.ppm"));
    ASSERT_EQ(PNMFileHandler::save_plain(img, outputDir + "P3_16plain.ppm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P3_16plain.ppm"));
}

TEST_F(PNMImageHandlerSuite, TestP6_8) {
    Image * img = PNMFileHandler::load(dataDir + "P6_8.ppm");

    ASSERT_NE(img, nullptr);
    ASSERT_EQ(img->get_width(), 100);
    ASSERT_EQ(img->get_height(), 100);
    ASSERT_EQ(img->get_nChannels(), 3);
    ASSERT_EQ(img->get_bitDepth(), 8);

    ASSERT_THROW({img->get_at(110, 90);}, out_of_range);
    ASSERT_THROW({img->get_at(-10);}, out_of_range);

    uint16_t * pixel;
    pixel = img->get_at(34, 59);
    ASSERT_EQ(pixel[0], 44);
    ASSERT_EQ(pixel[1], 24);
    ASSERT_EQ(pixel[2], 13);
    delete[] pixel;
    pixel = img->get_at(2200);
    ASSERT_EQ(pixel[0], 206);
    ASSERT_EQ(pixel[1], 197);
    ASSERT_EQ(pixel[2], 188);
    delete[] pixel;

    uint16_t new_pixel1[3] = {68, 200, 111};
    img->set_at(80, 16, new_pixel1);
    pixel = img->get_at(80, 16);
    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(pixel[i], new_pixel1[i]);
    }
    uint16_t new_pixel2[3] = {43, 31, 0};
    img->set_at(7888, new_pixel2);
    pixel = img->get_at(7888);
    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(pixel[i], new_pixel2[i]);
    }

    ASSERT_EQ(PNMFileHandler::save(img, outputDir + "P6_8bytes.ppm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P6_8bytes.ppm"));
    ASSERT_EQ(PNMFileHandler::save_plain(img, outputDir + "P6_8plain.ppm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P6_8plain.ppm"));
}

TEST_F(PNMImageHandlerSuite, TestP6_16) {
    Image * img = PNMFileHandler::load(dataDir + "P6_16.ppm");

    ASSERT_NE(img, nullptr);
    ASSERT_EQ(img->get_width(), 500);
    ASSERT_EQ(img->get_height(), 500);
    ASSERT_EQ(img->get_nChannels(), 3);
    ASSERT_EQ(img->get_bitDepth(), 16);

    ASSERT_THROW({img->get_at(500, 90);}, out_of_range);
    ASSERT_THROW({img->get_at(250000);}, out_of_range);

    uint16_t * pixel;
    pixel = img->get_at(282, 98);
    ASSERT_EQ(pixel[0], 11724);
    ASSERT_EQ(pixel[1], 11662);
    ASSERT_EQ(pixel[2], 11975);
    delete[] pixel;
    pixel = img->get_at(200000);
    ASSERT_EQ(pixel[0], 9686);
    ASSERT_EQ(pixel[1], 10480);
    ASSERT_EQ(pixel[2], 12120);
    delete[] pixel;

    uint16_t new_pixel1[3] = {7892, 0, 11111};
    img->set_at(78, 490, new_pixel1);
    pixel = img->get_at(78, 490);
    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(pixel[i], new_pixel1[i]);
    }
    uint16_t new_pixel2[3] = {255, 678, 44444};
    img->set_at(8932, new_pixel2);
    pixel = img->get_at(8932);
    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(pixel[i], new_pixel2[i]);
    }

    ASSERT_EQ(PNMFileHandler::save(img, outputDir + "P6_16bytes.ppm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P6_16bytes.ppm"));
    ASSERT_EQ(PNMFileHandler::save_plain(img, outputDir + "P6_16plain.ppm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P6_16plain.ppm"));
}

/*
 X load di file inesistente
 X load di file di ogni tipo (P1, P2, P3, P4, P5, P6) con 2 bitdepth diverse [12]
 X save di file di ogni tipo (P1, P2, P3, P4, P5, P6) con 2 bitdepth diverse [12]
 - Applica ogni effetto dell'editor a un'immagine a 8 e 16 bitdepth, a 1 e 3 canali
 X costructor di kernel
 X get_at e set_at di immagini a 8 e 16 bitdepth, a 1 e 3 canali
*/