#include "gtest/gtest.h"
#include <filesystem>

#include "../PNMFileHandler.h"

using namespace std;

class PNMFileHandlerSuite : public ::testing::Test {
protected:
    static string outputDir;
    static string dataDir;

    static void SetUpTestSuite(){
        filesystem::create_directories(outputDir);
    }

    static void TearDownTestSuite() {
        filesystem::remove_all(outputDir);
    }
};

string PNMFileHandlerSuite::outputDir = "test/output/";
string PNMFileHandlerSuite::dataDir = "test/data/";


TEST_F(PNMFileHandlerSuite, TestFileNotFound) {
    ASSERT_THROW({
        PNMFileHandler::load(dataDir + "notfound.ppm");
        }, runtime_error);
}

TEST_F(PNMFileHandlerSuite, TestP1) {
    Image * img = PNMFileHandler::load(dataDir + "P1.pbm");

    ASSERT_NE(img, nullptr);
    ASSERT_EQ(img->get_width(), 100);
    ASSERT_EQ(img->get_height(), 100);
    ASSERT_EQ(img->get_nChannels(), 1);
    ASSERT_EQ(img->get_bitDepth(), 8);

    uint16_t * pixel;
    pixel = img->get_at(56, 89);
    ASSERT_EQ(pixel[0], 0);
    delete[] pixel;
    pixel = img->get_at(4725);
    ASSERT_EQ(pixel[0], 0);
    delete[] pixel;

    uint16_t value;
    value = 255;
    img->set_at(90, 12, &value);
    pixel = img->get_at(10, 12);
    ASSERT_EQ(pixel[0], 255);
    delete[] pixel;
    value = 0;
    img->set_at(109, &value);
    pixel = img->get_at(109);
    ASSERT_EQ(pixel[0], 0);
    delete[] pixel;

    ASSERT_EQ(PNMFileHandler::save(img, outputDir + "P1bytes.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P1bytes.pgm"));
    ASSERT_EQ(PNMFileHandler::save_plain(img, outputDir + "P1plain.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P1plain.pgm"));
    delete img;
}

TEST_F(PNMFileHandlerSuite, TestP4) {
    Image * img = PNMFileHandler::load(dataDir + "P4.pbm");

    ASSERT_NE(img, nullptr);
    ASSERT_EQ(img->get_width(), 100);
    ASSERT_EQ(img->get_height(), 100);
    ASSERT_EQ(img->get_nChannels(), 1);
    ASSERT_EQ(img->get_bitDepth(), 8);

    ASSERT_THROW({img->get_at(60, 110);}, out_of_range);
    ASSERT_THROW({img->get_at(10003);}, out_of_range);

    uint16_t * pixel;
    pixel = img->get_at(50, 60);
    ASSERT_EQ(pixel[0], 0);
    delete[] pixel;
    pixel = img->get_at(6179);
    ASSERT_EQ(pixel[0], 255);
    delete[] pixel;

    uint16_t value;
    value = 255;
    img->set_at(88, 53, &value);
    pixel = img->get_at(88, 53);
    ASSERT_EQ(pixel[0], 255);
    delete[] pixel;
    value = 0;
    img->set_at(2, &value);
    pixel = img->get_at(2);
    ASSERT_EQ(pixel[0], 0);
    delete[] pixel;

    ASSERT_EQ(PNMFileHandler::save(img, outputDir + "P4bytes.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P4bytes.pgm"));
    ASSERT_EQ(PNMFileHandler::save_plain(img, outputDir + "P4plain.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P4plain.pgm"));
    delete img;
}

TEST_F(PNMFileHandlerSuite, TestP2_8) {
    Image * img = PNMFileHandler::load(dataDir + "P2_8.pgm");

    ASSERT_NE(img, nullptr);
    ASSERT_EQ(img->get_width(), 100);
    ASSERT_EQ(img->get_height(), 100);
    ASSERT_EQ(img->get_nChannels(), 1);
    ASSERT_EQ(img->get_bitDepth(), 8);

    ASSERT_THROW({img->get_at(60, 110);}, out_of_range);
    ASSERT_THROW({img->get_at(10003);}, out_of_range);

    uint16_t * pixel;
    pixel = img->get_at(0, 0);
    ASSERT_EQ(pixel[0], 131);
    delete[] pixel;
    pixel = img->get_at(1);
    ASSERT_EQ(pixel[0], 126);
    delete[] pixel;

    uint16_t value;
    value = 255;
    img->set_at(56, 93, &value);
    pixel = img->get_at(56, 93);
    ASSERT_EQ(pixel[0], 255);
    delete[] pixel;
    value = 0;
    img->set_at(1452, &value);
    pixel = img->get_at(1452);
    ASSERT_EQ(pixel[0], 0);
    delete[] pixel;

    ASSERT_EQ(PNMFileHandler::save(img, outputDir + "P2_8bytes.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P2_8bytes.pgm"));
    ASSERT_EQ(PNMFileHandler::save_plain(img, outputDir + "P2_8plain.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P2_8plain.pgm"));
    delete img;
}

TEST_F(PNMFileHandlerSuite, TestP2_16) {
    Image * img = PNMFileHandler::load(dataDir + "P2_16.pgm");

    ASSERT_NE(img, nullptr);
    ASSERT_EQ(img->get_width(), 500);
    ASSERT_EQ(img->get_height(), 500);
    ASSERT_EQ(img->get_nChannels(), 1);
    ASSERT_EQ(img->get_bitDepth(), 16);

    ASSERT_THROW({img->get_at(501, 0);}, out_of_range);
    ASSERT_THROW({img->get_at(-1);}, out_of_range);

    uint16_t * pixel;
    pixel = img->get_at(300, 200);
    ASSERT_EQ(pixel[0], 13191);
    delete[] pixel;
    pixel = img->get_at(1780);
    ASSERT_EQ(pixel[0], 14438);
    delete[] pixel;

    uint16_t value;
    value = 10000;
    img->set_at(120, 420, &value);
    pixel = img->get_at(120, 420);
    ASSERT_EQ(pixel[0], 10000);
    delete[] pixel;
    value = 0;
    img->set_at(13753, &value);
    pixel = img->get_at(13753);
    ASSERT_EQ(pixel[0], 0);
    delete[] pixel;

    ASSERT_EQ(PNMFileHandler::save(img, outputDir + "P2_16bytes.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P2_16bytes.pgm"));
    ASSERT_EQ(PNMFileHandler::save_plain(img, outputDir + "P2_16plain.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P2_16plain.pgm"));
    delete img;
}

TEST_F(PNMFileHandlerSuite, TestP5_8) {
    Image * img = PNMFileHandler::load(dataDir + "P5_8.pgm");

    ASSERT_NE(img, nullptr);
    ASSERT_EQ(img->get_width(), 100);
    ASSERT_EQ(img->get_height(), 100);
    ASSERT_EQ(img->get_nChannels(), 1);
    ASSERT_EQ(img->get_bitDepth(), 8);

    ASSERT_THROW({img->get_at(29, 100);}, out_of_range);
    ASSERT_THROW({img->get_at(-10);}, out_of_range);

    uint16_t * pixel;
    pixel = img->get_at(67, 93);
    ASSERT_EQ(pixel[0], 42);
    delete[] pixel;
    pixel = img->get_at(4346);
    ASSERT_EQ(pixel[0], 121);
    delete[] pixel;

    uint16_t value;
    value = 187;
    img->set_at(76, 21, &value);
    pixel = img->get_at(76, 21);
    ASSERT_EQ(pixel[0], 187);
    delete[] pixel;
    value = 10;
    img->set_at(487, &value);
    pixel = img->get_at(487);
    ASSERT_EQ(pixel[0], 10);
    delete[] pixel;

    ASSERT_EQ(PNMFileHandler::save(img, outputDir + "P5_8bytes.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P5_8bytes.pgm"));
    ASSERT_EQ(PNMFileHandler::save_plain(img, outputDir + "P5_8plain.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P5_8plain.pgm"));
    delete img;
}

TEST_F(PNMFileHandlerSuite, TestP5_16) {
    Image * img = PNMFileHandler::load(dataDir + "P5_16.pgm");

    ASSERT_NE(img, nullptr);
    ASSERT_EQ(img->get_width(), 500);
    ASSERT_EQ(img->get_height(), 500);
    ASSERT_EQ(img->get_nChannels(), 1);
    ASSERT_EQ(img->get_bitDepth(), 16);

    ASSERT_THROW({img->get_at(509, 499);}, out_of_range);
    ASSERT_THROW({img->get_at(352323);}, out_of_range);

    uint16_t * pixel;
    pixel = img->get_at(350, 162);
    ASSERT_EQ(pixel[0], 7847);
    delete[] pixel;
    pixel = img->get_at(34875);
    ASSERT_EQ(pixel[0], 17674);
    delete[] pixel;

    uint16_t value;
    value = 5632;
    img->set_at(30, 154, &value);
    pixel = img->get_at(30, 154);
    ASSERT_EQ(pixel[0], 5632);
    delete[] pixel;
    value = 3515;
    img->set_at(4124, &value);
    pixel = img->get_at(4124);
    ASSERT_EQ(pixel[0], 3515);
    delete[] pixel;

    ASSERT_EQ(PNMFileHandler::save(img, outputDir + "P5_16bytes.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P5_16bytes.pgm"));
    ASSERT_EQ(PNMFileHandler::save_plain(img, outputDir + "P5_16plain.pgm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P5_16plain.pgm"));
    delete img;
}

TEST_F(PNMFileHandlerSuite, TestP3_8) {
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
    delete[] pixel;
    uint16_t new_pixel2[3] = {70, 128, 255};
    img->set_at(8343, new_pixel2);
    pixel = img->get_at(8343);
    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(pixel[i], new_pixel2[i]);
    }
    delete[] pixel;

    ASSERT_EQ(PNMFileHandler::save(img, outputDir + "P3_8bytes.ppm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P3_8bytes.ppm"));
    ASSERT_EQ(PNMFileHandler::save_plain(img, outputDir + "P3_8plain.ppm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P3_8plain.ppm"));
    delete img;
}

TEST_F(PNMFileHandlerSuite, TestP3_16) {
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
    delete[] pixel;
    uint16_t new_pixel2[3] = {0, 65535, 12800};
    img->set_at(43236, new_pixel2);
    pixel = img->get_at(43236);
    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(pixel[i], new_pixel2[i]);
    }
    delete[] pixel;

    ASSERT_EQ(PNMFileHandler::save(img, outputDir + "P3_16bytes.ppm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P3_16bytes.ppm"));
    ASSERT_EQ(PNMFileHandler::save_plain(img, outputDir + "P3_16plain.ppm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P3_16plain.ppm"));
    delete img;
}

TEST_F(PNMFileHandlerSuite, TestP6_8) {
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
    delete[] pixel;
    uint16_t new_pixel2[3] = {43, 31, 0};
    img->set_at(7888, new_pixel2);
    pixel = img->get_at(7888);
    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(pixel[i], new_pixel2[i]);
    }
    delete[] pixel;

    ASSERT_EQ(PNMFileHandler::save(img, outputDir + "P6_8bytes.ppm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P6_8bytes.ppm"));
    ASSERT_EQ(PNMFileHandler::save_plain(img, outputDir + "P6_8plain.ppm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P6_8plain.ppm"));
    delete img;
}

TEST_F(PNMFileHandlerSuite, TestP6_16) {
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
    delete[] pixel;
    uint16_t new_pixel2[3] = {255, 678, 44444};
    img->set_at(8932, new_pixel2);
    pixel = img->get_at(8932);
    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(pixel[i], new_pixel2[i]);
    }
    delete[] pixel;

    ASSERT_EQ(PNMFileHandler::save(img, outputDir + "P6_16bytes.ppm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P6_16bytes.ppm"));
    ASSERT_EQ(PNMFileHandler::save_plain(img, outputDir + "P6_16plain.ppm"), 0);
    ASSERT_TRUE(filesystem::exists(outputDir + "P6_16plain.ppm"));
    delete img;
}
