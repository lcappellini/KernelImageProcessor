//
// Created by Lorenzo on 31/01/2025.
//

#include <fstream>
#include <sstream>
#include <vector>
#include "PNMFileHandler.h"
#include "PixelImage.h"

using namespace std;

Image * PNMFileHandler::load(const string& filename) {
    if (!hasValidExtension(filename)) {
        return nullptr;
    }

    int n = 0;
    uint16_t width, height;
    uint16_t maxval = 1; // 0 < maxval < 65536
    uint8_t nChannels;
    bool binary;
    uint8_t bitDepth;
    string line;
    string magic;

    ifstream file(filename, std::ios::binary);
    if (!file) {
        throw runtime_error("File not found");
    }

    file >> magic;

    while (file.peek() == '\n' || file.peek() == '\r' || file.peek() == '#'){
        getline(file, line);
    }

    file >> width >> height;

    if (magic != "P1" && magic != "P4") {
        file >> maxval;
    }

    file.ignore(1);


    bitDepth = maxval > 255 ? 16 : 8;
    uint32_t maxBitValue = (uint32_t)(pow(2, bitDepth)-1);

    if (magic == "P1" || magic == "P2") {
        binary = false;
        nChannels = 1;
    } else if (magic == "P3") {
        binary = false;
        nChannels = 3;
    } else if (magic == "P4" || magic == "P5") {
        binary = true;
        nChannels = 1;
    } else if (magic == "P6") {
        binary = true;
        nChannels = 3;
    } else {
        return nullptr;
    }

    void *pixelData;

    if (bitDepth == 8) {
        pixelData = new uint8_t[width * height * nChannels];
    }
    else if (bitDepth == 16) {
        pixelData = new uint16_t[width * height * nChannels];
    }

    if (binary) {
        if (maxval == 1) {
            char data;
            while (file.read(&data, 1)) {
                for (int b = 7; b >= 0; b--) {
                    reinterpret_cast<uint8_t*>(pixelData)[n] = (((data >> b) & 1) ^ 1) * 255; //xor because 0=255 and 1=0
                    n++;
                    if (n % width == 0){
                        break;
                    }
                }
            }
        } else if (maxval < 256) {
            char data;
            while (file.read(&data, 1)) {
                if (maxval == 255)
                    reinterpret_cast<uint8_t*>(pixelData)[n] = (uint8_t)data;
                else
                    reinterpret_cast<uint8_t*>(pixelData)[n] = (data * maxBitValue) / maxval;
                n++;
            }
        } else {
            uint8_t dataPtr[2];
            uint16_t data;
            while (file.read((char *)dataPtr, 2)) {
                data = ((dataPtr[0]) << 8) | dataPtr[1];
                if (maxval == 65535)
                    reinterpret_cast<uint16_t*>(pixelData)[n] = (uint16_t)data;
                else
                    reinterpret_cast<uint16_t*>(pixelData)[n] = (data * maxBitValue) / maxval;
                n++;
            }
        }
    } else {
        while (getline(file, line)) {
            istringstream iss(line);
            string s;
            while (getline(iss, s, ' ')) {
                if (s.empty())
                    continue;
                unsigned short val = stoi(s);
                if (magic == "P1") {
                    val = val ? 0 : 1;
                }
                if (bitDepth == 8) {
                    reinterpret_cast<uint8_t *>(pixelData)[n] = (val * maxBitValue) / maxval;
                } else {
                    reinterpret_cast<uint16_t*>(pixelData)[n] = (val * maxBitValue) / maxval;
                }
                n++;
            }
        }
    }

    if (nChannels == 1){
        if (bitDepth == 8) {
            return new PixelImage<1, 8>(width, height, (uint8_t *)pixelData);
        } else if (bitDepth == 16) {
            return new PixelImage<1, 16>(width, height, (uint16_t *)pixelData);
        }
    } else if (nChannels == 3){
        if (bitDepth == 8) {
            return new PixelImage<3, 8>(width, height, (uint8_t *)pixelData);
        } else if (bitDepth == 16) {
            return new PixelImage<3, 16>(width, height, (uint16_t *)pixelData);
        }
    }
    return nullptr;
}

bool PNMFileHandler::hasValidExtension(const string &filename) {
    string ext = filename.substr(filename.find_last_of('.') + 1);
    for (const auto & validExt : {"ppm", "pgm", "pbm"}) {
        if (ext == validExt) {
            return true;
        }
    }
    return false;
}

int PNMFileHandler::save(Image *image, const string &filename, bool plain, const string &comments) {
    ofstream outfile;
    if (plain)
        outfile.open(filename, ios_base::trunc);
    else
        outfile.open(filename, ios_base::trunc | ios_base::binary);

    uint8_t magic;
    if (plain) {
        if (image->get_nChannels() == 1){
            magic = 2;
        } else {
            magic = 3;
        }
    } else {
        if (image->get_nChannels() == 1) {
            magic = 5;
        } else {
            magic = 6;
        }
    }

    outfile << "P" << (int)magic << std::endl;

    stringstream commentsStream(comments);
    string line;
    while (getline(commentsStream, line)) {
        outfile << "# " << line << endl;
    }

    outfile << image->get_width() << " " << image->get_height() << endl;

    uint16_t maxval = (uint16_t)(pow(2, image->get_bitDepth())-1);
    outfile << maxval << endl;

    uint8_t bitDepth = image->get_bitDepth();
    uint8_t chs = image->get_nChannels();
    if (plain) {
        for (int i = 0; i < image->get_width() * image->get_height(); i++){
            uint16_t * pixel = image->get_at(i);
            for (int n = 0; n < chs; n++){
                if (n != 0){
                    outfile << " ";
                }
                outfile << (int)pixel[n];
            }
            outfile << endl;
        }
    } else {
        for (int i = 0; i < image->get_width() * image->get_height(); i++) {
            uint16_t *pixel = image->get_at(i);
            if (bitDepth == 8) {
                for (int j = 0; j < chs; j++) {
                    outfile.put((char) pixel[j]);
                }
            } else {
                for (int j = 0; j < chs; j++) {
                    uint16_t value = pixel[j];
                    outfile.put((char) ((value >> 8) & 0xFF));
                    outfile.put((char) (value & 0xFF));
                }
            }
        }
    }
    outfile.close();
    return 0;
}

