// Fichier : main.cpp
// Rôle : point d'entrée CLI minimal ; le parsing et le rendu seront branchés ici.
// Responsable : Personne C
// Dépendances : SceneParser.hpp, Renderer.hpp, Image.hpp
// TODO : TODO(C): parser les options CLI, mesurer le temps et sauvegarder l'image.
// CHAÎNE : options CLI -> SceneParser -> Renderer -> Image::savePPM.

#include "render/Renderer.hpp"
#include "scene/SceneParser.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

namespace {

/// Affiche la syntaxe et les options de la ligne de commande.
void printUsage(const char* program) {
    std::cout
        << "Usage: " << program
        << " <scene> -o <output.ppm> [-w <width>] [-h <height>]"
           " [-s <samples>] [-d <max-depth>] [-t <threads>]\n"
        << "Options:\n"
        << "  -o <path>       PPM output path (required)\n"
        << "  -w <width>      Image width (default: 800)\n"
        << "  -h <height>     Image height (default: 450)\n"
        << "  -s <samples>    Samples per pixel (default: 1)\n"
        << "  -d <depth>      Maximum ray depth (default: 5)\n"
        << "  -t <threads>    Number of threads (default: 1)\n"
        << "  --help          Show this help\n";
}

/// Convertit une chaîne en entier et signale l'option fautive.
int parseInt(const std::string& value, const char* option) {
    std::size_t parsed = 0;
    int result = 0;
    try {
        result = std::stoi(value, &parsed);
    } catch (const std::exception&) {
        throw std::invalid_argument(std::string(option) + " expects an integer, got '" +
                                    value + "'");
    }
    if (parsed != value.size())
        throw std::invalid_argument(std::string(option) + " expects an integer, got '" +
                                    value + "'");
    return result;
}

/// Convertit une chaîne en entier strictement positif.
unsigned parseUnsigned(const std::string& value, const char* option) {
    const int result = parseInt(value, option);
    if (result <= 0)
        throw std::invalid_argument(std::string(option) + " must be positive");
    return static_cast<unsigned>(result);
}

} // namespace

/// Charge une scène, lance le rendu et enregistre l'image produite.
int main(int argc, char** argv) {
    if (argc == 1) {
        printUsage(argv[0]);
        return 1;
    }

    if (std::string(argv[1]) == "--help") {
        printUsage(argv[0]);
        return 0;
    }

    try {
        if (argc < 2)
            throw std::invalid_argument("a scene file is required");

        //// PARSING CLI
        std::string scenePath;
        std::string outputPath;
        rt::RenderSettings settings;

        scenePath = argv[1];
        for (int i = 2; i < argc; ++i) {
            const std::string option = argv[i];
            if (option == "--help") {
                printUsage(argv[0]);
                return 0;
            }
            if (i + 1 >= argc)
                throw std::invalid_argument("missing value for " + option);

            const std::string value = argv[++i];
            if (option == "-o") {
                outputPath = value;
            } else if (option == "-w") {
                settings.width = parseInt(value, "-w");
            } else if (option == "-h") {
                settings.height = parseInt(value, "-h");
            } else if (option == "-s") {
                settings.samplesPerPixel = parseInt(value, "-s");
            } else if (option == "-d") {
                settings.maxDepth = parseInt(value, "-d");
            } else if (option == "-t") {
                settings.threadCount = parseUnsigned(value, "-t");
            } else {
                throw std::invalid_argument("unknown option '" + option + "'");
            }
        }

        if (outputPath.empty())
            throw std::invalid_argument("missing required option -o");

        //// PARSING
        const rt::Scene scene = rt::SceneParser::parseFile(scenePath);

        //// RENDERING
        const rt::Image image = rt::Renderer(settings).render(scene);

        //// SAVE
        image.savePPM(outputPath);


    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
