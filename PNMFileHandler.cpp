//
// Created by Lorenzo on 31/01/2025.
//

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "PNMFileHandler.h"
#include "PixelImage.h"

using namespace std;


const string PNMFileHandler::validExts[3] = {"ppm", "pgm", "pbm"};

Image * PNMFileHandler::load(const string& filename) {
    if (!hasValidExtension(filename)) {
        return nullptr;
    }

    ifstream file(filename, std::ios::binary);

    int n = 0;
    uint16_t width, height;
    uint16_t maxval; // 0 < maxval < 65536
    uint8_t *pixelData;
    uint8_t nChannels;
    bool binary;
    string line;

    file >> line;
    if (line == "P1") {
        binary = false;
        nChannels = 1;
    } else if (line == "P2") {
        binary = false;
        nChannels = 2;
    } else if (line == "P3") {
        binary = false;
        nChannels = 3;
    } else if (line == "P4") {
        binary = true;
        nChannels = 1;
    } else if (line == "P5") {
        binary = true;
        nChannels = 2;
    } else if (line == "P6") {
        binary = true;
        nChannels = 3;
    } else {
        return nullptr;
    }

    file.get(); //goto next line

    do {
        getline(file, line);
    } while (line[0] == '#');


    istringstream sizeStream(line);
    cout << line << endl;
    sizeStream >> width >> height;

    if (nChannels != 1) {
        int val;
        file >> val;
        if (!(0 < val && val < 65536)) {
            return nullptr;
        }
        maxval = val;
    } else {
        maxval = 1;
    }

    file.get(); //goto next line

    pixelData = new uint8_t[width * height * nChannels];

    if (binary) {
        char data;
        if (maxval == 1) {
            while (file.read(&data, 1)) {
                for (int b = 7; b >= 0; b--) {
                    pixelData[n] = (data >> b) & 1;
                    n++;
                }
            }
        } else if (maxval < 256) {
            while (file.read(&data, 1)) {
                pixelData[n] = data;
                n++;
            }
        } else {
            while (file.read(&data, 2)) {
                pixelData[n] = (data * (uint32_t) 255) / maxval; //TODO IMAGES ARE "QUANTIZED" TO A 8 BITDEPTH, SHOULD I CHANGE THIS?
                n++;
            }
        }
    } else {
        while (getline(file, line)) {
            istringstream iss(line);
            string s;
            while (getline(iss, s, ' ')) { //TODO verify it works even with multiple whitespaces
                unsigned short val = stoi(s);
                uint8_t value = (val * (uint32_t) 255) / maxval; //TODO IMAGES ARE "QUANTIZED" TO A 8 BITDEPTH, SHOULD I CHANGE THIS?
                pixelData[n++] = value;
            }
        }
    }

    /*cout << "Width:" << width << endl;
    cout << "Height:" << height << endl;
    cout << "Maxval:" << maxval << endl;*/

    if (nChannels == 1){
        return new PixelImage<1>(width, height, pixelData);
    } else if (nChannels == 2){
        return new PixelImage<2>(width, height, pixelData);
    } else if (nChannels == 3){
        return new PixelImage<3>(width, height, pixelData);
    } else if (nChannels == 4){
        return new PixelImage<4>(width, height, pixelData);
    }
    return nullptr;
}

bool PNMFileHandler::save_plain(Image * image, const string& filename) {
    ofstream outfile;
    outfile.open(filename, ios_base::trunc);

    outfile << "P" << (int)image->get_nChannels() << endl;

    outfile << "# This is a comment" << endl; //TODO remove, just for testing purpose, consider if keeping the original comments or not

    outfile << image->get_width() << " " << image->get_height() << endl;

    outfile << 255 << endl; //TODO images are saved with bitdepth=8, should i consider adding more options?? (i'll need to edit the pixel array to handle arbitrary precision ints)

    for (int i = 0; i < image->get_width()*image->get_height(); i++){
        uint8_t * pixel = image->get_at(i);
        for (int n = 0; n < image->get_nChannels(); n++){
            if (n != 0){
                outfile << " ";
            }
            outfile << (int)pixel[n];
        }
        outfile << endl;
    }
    outfile.close();
    return true;
}

bool PNMFileHandler::save(Image * image, const string& filename) {
    ofstream outfile;
    outfile.open(filename, ios_base::trunc | ios_base::binary);

    outfile << "P" << 3 + (int)image->get_nChannels() << std::endl;

    outfile << "# This is a comment" << endl; //TODO remove, just for testing purpose, consider if keeping the original comments or not

    outfile << image->get_width() << " " << image->get_height() << endl;

    outfile << 255 << endl; //TODO images are saved with bitdepth=8, should i consider adding more options?? (i'll need to edit the pixel array to handle arbitrary precision ints)

    for (int i = 0; i < image->get_width()*image->get_height(); i++){
        char * pixel = (char *)image->get_at(i);
        outfile.write(pixel, image->get_nChannels());
    }
    outfile.close();
    return true;
}

bool PNMFileHandler::hasValidExtension(const string &filename) {
    string ext = filename.substr(filename.find_last_of('.') + 1);
    for (const auto & validExt : validExts) {
        if (ext == validExt) {
            return true;
        }
    }
    return false;
}

