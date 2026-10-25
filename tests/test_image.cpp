// Fichier : test_image.cpp
// Rôle : squelette des tests du buffer et de l'export PPM.
// Responsable : Personne C
// Dépendances : Image.hpp, GoogleTest
// Tests des dimensions, du buffer de pixels et de l'export PPM.

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "render/Image.hpp"

namespace {

std::filesystem::path testPath(const std::string& name) {
    return std::filesystem::temp_directory_path() / ("miniRTX_" + name + ".ppm");
}

std::vector<unsigned char> readBinary(const std::filesystem::path& path) {
    std::ifstream input(path, std::ios::binary);
    return std::vector<unsigned char>(
        std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
}

void removeTestFile(const std::filesystem::path& path) {
    std::error_code error;
    std::filesystem::remove(path, error);
}

void expectColor(const rt::Color& actual, double red, double green, double blue) {
    EXPECT_DOUBLE_EQ(actual.x(), red);
    EXPECT_DOUBLE_EQ(actual.y(), green);
    EXPECT_DOUBLE_EQ(actual.z(), blue);
}

} // namespace

TEST(ImageTest, AcceptsPositiveDimensions) {
    const rt::Image image(3, 4);

    EXPECT_EQ(image.width(), 3);
    EXPECT_EQ(image.height(), 4);
}

TEST(ImageTest, RejectsNonPositiveDimensions) {
    EXPECT_THROW(rt::Image(0, 2), std::invalid_argument);
    EXPECT_THROW(rt::Image(2, 0), std::invalid_argument);
    EXPECT_THROW(rt::Image(-1, 2), std::invalid_argument);
    EXPECT_THROW(rt::Image(2, -1), std::invalid_argument);
}

TEST(ImageTest, StartsWithBlackPixels) {
    const rt::Image image(2, 2);

    expectColor(image.pixel(0, 0), 0.0, 0.0, 0.0);
    expectColor(image.pixel(1, 0), 0.0, 0.0, 0.0);
    expectColor(image.pixel(0, 1), 0.0, 0.0, 0.0);
    expectColor(image.pixel(1, 1), 0.0, 0.0, 0.0);
}

//TEST(ImageTest, StoresPixelsIndependently) {
//    rt::Image image(2, 2);
//    const rt::Color red(1.0, 0.0, 0.0);
//    const rt::Color blue(0.0, 0.0, 1.0);
//
//    image.setPixel(0, 0, red);
//    image.setPixel(1, 1, blue);
//
//    expectColor(image.pixel(0, 0), 1.0, 0.0, 0.0);
//    expectColor(image.pixel(1, 1), 0.0, 0.0, 1.0);
//    expectColor(image.pixel(1, 0), 0.0, 0.0, 0.0);
//    expectColor(image.pixel(0, 1), 0.0, 0.0, 0.0);
//}

TEST(ImageTest, AcceptsBoundaryCoordinates) {
    rt::Image image(2, 2);
    const rt::Color color(0.25, 0.5, 0.75);

    EXPECT_NO_THROW(image.setPixel(0, 0, color));
    EXPECT_NO_THROW(image.setPixel(1, 1, color));
    EXPECT_NO_THROW(image.pixel(0, 0));
    EXPECT_NO_THROW(image.pixel(1, 1));
}

TEST(ImageTest, RejectsOutOfBoundsCoordinates) {
    rt::Image image(2, 2);
    const rt::Color color(1.0, 1.0, 1.0);

    EXPECT_THROW(image.setPixel(-1, 0, color), std::out_of_range);
    EXPECT_THROW(image.setPixel(2, 0, color), std::out_of_range);
    EXPECT_THROW(image.setPixel(0, -1, color), std::out_of_range);
    EXPECT_THROW(image.setPixel(0, 2, color), std::out_of_range);
    EXPECT_THROW(image.pixel(-1, 0), std::out_of_range);
    EXPECT_THROW(image.pixel(2, 0), std::out_of_range);
    EXPECT_THROW(image.pixel(0, -1), std::out_of_range);
    EXPECT_THROW(image.pixel(0, 2), std::out_of_range);
}

//TEST(ImageTest, ExportsPpmHeaderAndPixelOrder) {
//    const auto path = testPath("header");
//    removeTestFile(path);
//    rt::Image image(2, 2);
//    image.setPixel(0, 0, rt::Color(1.0, 0.0, 0.0));
//    image.setPixel(1, 0, rt::Color(0.0, 1.0, 0.0));
//    image.setPixel(0, 1, rt::Color(0.0, 0.0, 1.0));
//    image.setPixel(1, 1, rt::Color(1.0, 1.0, 1.0));
//
//    ASSERT_NO_THROW(image.savePPM(path.string()));
//    const auto bytes = readBinary(path);
//    removeTestFile(path);
//
//    const std::string header = "P6\n2 2\n255\n";
//    ASSERT_GE(bytes.size(), header.size() + 2 * 2 * 3);
//    EXPECT_EQ(std::string(bytes.begin(), bytes.begin() + header.size()), header);
//    const std::vector<unsigned char> expectedPixels{
//        255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 255, 255};
//    EXPECT_TRUE(std::equal(expectedPixels.begin(), expectedPixels.end(),
//                           bytes.begin() + header.size()));
//}

//TEST(ImageTest, AppliesGammaAndSaturatesPpmComponents) {
//    const auto path = testPath("gamma");
//    removeTestFile(path);
//    rt::Image image(2, 2);
//    image.setPixel(0, 0, rt::Color(0.5, 0.0, 1.0));
//    image.setPixel(1, 0, rt::Color(-1.0, 2.0, 0.5));
//
//    ASSERT_NO_THROW(image.savePPM(path.string()));
//    const auto bytes = readBinary(path);
//    removeTestFile(path);
//
//    const std::string header = "P6\n2 2\n255\n";
//
//    ASSERT_GE(bytes.size(), header.size() + 2 * 2 * 3);
//    const auto pixels = bytes.begin() + header.size();
//    EXPECT_EQ(pixels[0], 186);
//    EXPECT_EQ(pixels[1], 0);
//    EXPECT_EQ(pixels[2], 255);
//    EXPECT_EQ(pixels[3], 0);
//    EXPECT_EQ(pixels[4], 255);
//    EXPECT_EQ(pixels[5], 186);
//}

//TEST(ImageTest, ExportsNonSquareImageInRowMajorOrder) {
//    const auto path = testPath("rect");
//    removeTestFile(path);
//    rt::Image image(3, 2);
//    image.setPixel(2, 0, rt::Color(1.0, 0.0, 0.0));  // fin de la 1re ligne
//    image.setPixel(0, 1, rt::Color(0.0, 0.0, 1.0));  // début de la 2e ligne
//
//    ASSERT_NO_THROW(image.savePPM(path.string()));
//    const auto bytes = readBinary(path);
//    removeTestFile(path);
//
//    const std::string header = "P6\n3 2\n255\n";
//    ASSERT_EQ(bytes.size(), header.size() + 3 * 2 * 3);
//    EXPECT_EQ(std::string(bytes.begin(), bytes.begin() + header.size()), header);
//    const auto pixels = bytes.begin() + header.size();
//    EXPECT_EQ(pixels[6], 255);   // pixel (2,0) : R
//    EXPECT_EQ(pixels[9], 0);     // pixel (0,1) : R
//    EXPECT_EQ(pixels[11], 255);  // pixel (0,1) : B
//}

TEST(ImageTest, ExportsEmptyImageAsBlackPixels) {
    const auto path = testPath("empty");
    removeTestFile(path);
    const rt::Image image(2, 2);

    ASSERT_NO_THROW(image.savePPM(path.string()));
    const auto bytes = readBinary(path);
    removeTestFile(path);

    const std::string header = "P6\n2 2\n255\n";
    ASSERT_EQ(bytes.size(), header.size() + 12);
    EXPECT_TRUE(std::all_of(bytes.begin() + header.size(), bytes.end(),
                            [](unsigned char value) { return value == 0; }));
}

TEST(ImageTest, ReportsPpmOpenFailure) {
    const rt::Image image(1, 1);
    const auto path = std::filesystem::temp_directory_path() / "missing" / "image.ppm";

    EXPECT_THROW(image.savePPM(path.string()), std::runtime_error);
}

