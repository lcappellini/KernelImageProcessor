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
    if (!hasValidExtension(filename)){
        return nullptr;
    }

    ifstream input(filename);
    int n = 0;
    int i = 0;
    int k = 0;
    uint16_t width, height;
    uint16_t maxval; // 0 < maxval < 65536
    uint8_t * pixelData;
    uint8_t nChannels;
    for(string line; getline(input, line);)
    {
        if (line.at(0) == '#') {
            continue;
        } else if (n == 0) {
            if (line == "P1") {
                nChannels = 1;
            } else if (line == "P2") {
                nChannels = 2;
            } else if (line == "P3") {
                nChannels = 3;
            } else {
                return nullptr;
            }
            i++;
        } else if (i == 1) {
            istringstream iss(line);
            iss >> width >> height;
            pixelData = new uint8_t[width*height*nChannels];
            k = 0;
            i++;
        } else if (i == 2) { //TODO whitespace is also good, not only newline (add this possibility)
            if (nChannels != 1) {
                int val = stoi(line);
                if (!(0 < val && val < 65536)) {
                    return nullptr;
                }
                maxval = val;
            } else {
                maxval = 1;
            }
            i++;
        } else if (i > 2) {
            istringstream iss(line);
            string s;
            while (getline(iss, s, ' ')) { //TODO verify it works even with multiple whitespaces
                unsigned short val = stoi(s);
                uint8_t value = (val * (uint32_t)255) / maxval; //TODO IMAGES ARE "QUANTIZED" TO A 8 BITDEPTH, SHOULD I CHANGE THIS?
                pixelData[k++] = value;
            }
        }
        n++;
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

bool PNMFileHandler::save(Image * image, const string& filename) {
    ofstream outfile;
    outfile.open(filename, ios_base::trunc);

    outfile << "P" << (int)image->get_nChannels() << std::endl; //TODO check if P4 is a 4 channel image or not and if you can have 4 channels saved in this file format

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
    outfile << endl; //TODO CHECK IF OBLIGATORY OR NOT
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

