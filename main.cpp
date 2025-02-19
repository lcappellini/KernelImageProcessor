#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include "Image.h"
#include "PNMFileHandler.h"
#include "ImageEditor.h"


enum class ImageEffect {
    Sharpen,
    GaussianBlur,
    EdgeDetection,
    Emboss,
    BrightnessChange,
    Crop,
    Inversion,
    ToGrayscale,
};

using namespace std;

void show_help() {
    cout << "Kernel Image Processor by Lorenzo Cappellini\n";
    cout << "Supported image formats: .pbm, .pgm, .ppm\n";
    cout << "Supported bit depth values: 8, 16\n";
    cout << "\nMain Arguments:\n";
    cout << "-i <input_file>         : Specify input file path\n";
    cout << "-o <output_file>        : Specify output file path (encoded as bytes)\n";
    cout << "-op <output_file>       : Specify output file path (encoded as plain ascii)\n";
    cout << "\nConvolution effects:\n";
    cout << "-sharpen                : Apply sharpening effect\n";
    cout << "-blur                   : Apply Gaussian blur effect\n";
    cout << "-edge                   : Apply \"edge detection\" effect\n";
    cout << "-emboss                 : Apply embossing effect\n";
    cout << "\nOther effects:\n";
    cout << "-brightness <factor>    : Change brightness by the given factor\n";
    cout << "-crop <coordinates>     : Crop the image with comma-separated coordinates for the vertices: \"x0,y0,x1,y1\"\n";
    cout << "-invert                 : Invert the colors of the image\n";
    cout << "-grayscale              : Convert the image to grayscale\n";
    cout << "\nOther commands:\n";
    cout << "-v                      : Enable verbose output for debugging information\n";
    cout << "-h                      : Show this help message\n";
}

void log_error(const string& message){
    cerr << "ERROR: " << message << endl;
    exit(1);
}

int main(int argc, char *argv[]) {
    string inputFile;
    string outputFile;
    bool inputFileProvided = false;
    bool outputFileProvided = false;
    bool outputPlain = false;

    bool verbose = false;

    float brightnessFactor;
    ImageEffect imageEffect;
    bool imageEffectProvided = false;
    vector<int> cropCoords;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-i") {
            if (inputFileProvided) {
                log_error("You can specify only 1 input file");
            }
            if (i + 1 >= argc){
                log_error("Bad argument format");
            }
            inputFile = argv[i + 1];
            i++;
            inputFileProvided = true;

        } else if (arg == "-o" || arg == "-op") {
            if (outputFileProvided) {
                log_error("You can specify only 1 output file");
            }
            if (i + 1 >= argc){
                log_error("Bad argument format");
            }
            outputFile = argv[i + 1];
            i++;
            outputFileProvided = true;
            outputPlain = arg == "-op";

        } else if (arg == "-sharpen") {
            if (imageEffectProvided) {
                log_error("You can specify only 1 effect argument");
            }
            imageEffect = ImageEffect::Sharpen;
            imageEffectProvided = true;

        } else if (arg == "-blur") {
            if (imageEffectProvided) {
                log_error("You can specify only 1 effect argument");
            }
            imageEffect = ImageEffect::GaussianBlur;
            imageEffectProvided = true;

        } else if (arg == "-edge") {
            if (imageEffectProvided) {
                log_error("You can specify only 1 effect argument");
            }
            imageEffect = ImageEffect::EdgeDetection;
            imageEffectProvided = true;

        } else if (arg == "-emboss") {
            if (imageEffectProvided) {
                log_error("You can specify only 1 effect argument");
            }
            imageEffect = ImageEffect::Emboss;
            imageEffectProvided = true;

        } else if (arg == "-brightness") {
            if (imageEffectProvided) {
                log_error("You can specify only 1 effect argument");
            }
            if (i + 1 >= argc){
                log_error("Bad argument format");
            }
            try {
                brightnessFactor = stof(argv[i + 1]);
                i++;
            } catch (const std::invalid_argument& e) {
                log_error("Invalid float value for argument: " + arg);
            }
            imageEffect = ImageEffect::BrightnessChange;
            imageEffectProvided = true;

        } else if (arg == "-crop") {
            if (imageEffectProvided) {
                log_error("You can specify only 1 effect argument");
            }
            if (i + 1 >= argc) {
                log_error("Missing value for argument: " + arg);
            }
            string values = argv[i + 1];
            stringstream ss(values);
            string item;
            while (std::getline(ss, item, ',')) {
                try {
                    cropCoords.push_back(std::stoi(item));
                } catch (const invalid_argument& e) {
                    log_error("Invalid number in -c argument: " + item);
                }
            }
            i++;
            imageEffect = ImageEffect::Crop;
            imageEffectProvided = true;

        } else if (arg == "-invert") {
            if (imageEffectProvided) {
                log_error("You can specify only 1 effect argument");
            }
            imageEffect = ImageEffect::Inversion;
            imageEffectProvided = true;

        } else if (arg == "-grayscale") {
            if (imageEffectProvided) {
                log_error("You can specify only 1 effect argument");
            }
            imageEffect = ImageEffect::ToGrayscale;
            imageEffectProvided = true;

        } else if (arg == "-v") {
            verbose = true;

        } else if (arg == "-h") {
            show_help();
            exit(1);

        } else {
            log_error("Bad argument format or unknown argument");
        }
    }

    if (!inputFileProvided){
        log_error("You must provide an input file");
    }

    if (!outputFileProvided){
        log_error("You must provide an output file");
    }

    if (!imageEffectProvided){
        log_error("No image effect provided");
    }

    if (verbose) {
        cout << "Loading file: " << inputFile << endl;
    }

    Image *img;
    try {
        img = PNMFileHandler::load(inputFile);
        if (img == nullptr) {
            log_error("There was an error while loading the input file");
        }
    } catch (const std::exception& e){
        log_error(e.what());
        exit(1);
    }

    if (verbose) {
        cout << "Loaded!" << endl;
        cout << "Properties:" << endl;
        cout << " - Size: " << img->get_width() << ", " << img->get_height() << endl;
        cout << " - Channels: " << (int)img->get_nChannels() << endl;
        cout << " - BitDepth: " << (int)img->get_bitDepth() << endl;
        cout << endl;
    }

    Image * new_img;

    try {
        switch (imageEffect) {
            case ImageEffect::Sharpen: {
                if (verbose) {
                    cout << "Applying sharpening...";
                }
                new_img = ImageEditor::sharpen(img);
                break;
            }
            case ImageEffect::GaussianBlur: {
                if (verbose) {
                    cout << "Applying Gaussian blur...";
                }
                new_img = ImageEditor::gaussian_blur3(img);
                break;
            }
            case ImageEffect::EdgeDetection: {
                if (verbose) {
                    cout << "Applying \"edge detection\" effect...";
                }
                new_img = ImageEditor::edge_detection_effect(img);
                break;
            }
            case ImageEffect::Emboss: {
                if (verbose) {
                    cout << "Applying embossing...";
                }
                new_img = ImageEditor::emboss(img);
                break;
            }
            case ImageEffect::BrightnessChange: {
                if (verbose) {
                    cout << "Changing brightness...";
                }
                new_img = ImageEditor::change_brightness(img, brightnessFactor);
                break;
            }
            case ImageEffect::Crop: {
                if (verbose) {
                    cout << "Cropping with coordinates: " << cropCoords.data() << "...";
                }
                new_img = ImageEditor::crop(img, cropCoords[0], cropCoords[1], cropCoords[2], cropCoords[3]);
                break;
            }
            case ImageEffect::Inversion: {
                if (verbose) {
                    cout << "Applying inversion...";
                }
                new_img = ImageEditor::invert(img);
                break;
            }
            case ImageEffect::ToGrayscale: {
                if (verbose) {
                    cout << "Converting to grayscale...";
                }
                new_img = ImageEditor::grayscale(img);
                break;
            }
        }
    } catch (const std::exception& e){
        if (verbose)
            cout << endl << endl;
        log_error(e.what());
        exit(1);
    }

    if (verbose) {
        cout << "Done" << endl;
    }

    int result;
    try {
        if (outputPlain) {
            result = PNMFileHandler::save_plain(new_img, outputFile);
        } else {
            result = PNMFileHandler::save(new_img, outputFile);
        }
    } catch (const std::exception& e) {
        log_error(e.what());
        exit(1);
    }

    if (result == 0){
        cout << "Success!" << endl;
    } else {
        log_error("There was an error while saving the output file");
    }

    return 0;
}