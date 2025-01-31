//
// Created by Lorenzo on 31/01/2025.
//

#include <fstream>
#include <sstream>
#include <vector>
#include "PNMFileHandler.h"

using namespace std;

Image * PNMFileHandler::load(const string& filename) {
    ifstream input(filename);
    int n = 0;
    int i = 0;
    unsigned short width, height;
    unsigned short maxval; // 0 < maxval < 65536
    vector<unsigned short> pixelData;
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
            i++;
        } else if (i == 2) { //TODO whitespace is also good, not only newline (add this possibility)
            if (nChannels != 1) {
                int val = stoi(line);
                if (!(0 < val && val < 65536)) {
                    return nullptr;
                }
                maxval = val;
            }
            i++;
        } else if (i > 3) {
            istringstream iss(line);
            string s;
            while (getline(iss, s, ' ')) { //TODO verify it works even with multiple whitespaces
                unsigned short val = stoi(s);
                pixelData.push_back(val);
            }
        }
        n++;
    }

    if (pixelData.size() % 3 != 0){
        return nullptr;
    }

    vector<vector<uint8_t>> chs(nChannels);
    unsigned int c = 0;
    for (unsigned short val : pixelData){
        uint8_t value = (val * (uint32_t)255) / maxval;
        chs[c].push_back(value);
        c = (c + 1) % nChannels;
    }

    vector<Channel> channels;
    for (int k = 0; k < nChannels; k++) {
        channels.emplace_back(width, height, chs[k].data());
    }

    auto img = new Image(width, height, nChannels, channels.data());
    return img;
}

bool PNMFileHandler::save(Image * image, string filename) {
    return false;
}

