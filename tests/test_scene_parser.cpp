#include <gtest/gtest.h>

#include <atomic>
#include <filesystem>
#include <fstream>
#include <string>

#include "scene/SceneParser.hpp"

namespace {

class SceneFile {
public:
    explicit SceneFile(const std::string& contents) {
        static std::atomic<unsigned> nextId{0};
        path_ = std::filesystem::temp_directory_path() /
                ("minirtx-scene-parser-" + std::to_string(nextId++) + ".scene");
        std::ofstream output(path_);
        if (!output) throw std::runtime_error("cannot create scene fixture");
        output << contents;
        if (!output) throw std::runtime_error("cannot write scene fixture");
    }

    ~SceneFile() { std::filesystem::remove(path_); }

    const std::filesystem::path& path() const { return path_; }

private:
    std::filesystem::path path_;
};

std::string parseError(const std::string& contents) {
    SceneFile file(contents);
    try {
        (void)rt::SceneParser::parseFile(file.path().string());
    } catch (const std::exception& error) {
        return error.what();
    }
    return {};
}

} // namespace

TEST(SceneParserTest, ParsesEverySupportedDirective) {
    SceneFile file(
        "# header\n"
        "camera 0 1 5 0 0 0 45\n"
        "background 0.1 0.2 0.3\n"
        "material matte 0.8 0.2 0.1 0.1 0.7 0.2 16 0.0\n"
        "light 2 4 1 1 0.9 0.8 2\n"
        "sphere 0 1 0 1 matte\n"
        "plane 0 0 0 0 1 0 matte\n"
        "triangle -1 0 0 1 0 0 0 1 0 matte\n");

    const rt::Scene scene = rt::SceneParser::parseFile(file.path().string());

    EXPECT_EQ(scene.objects().size(), 3U);
    EXPECT_EQ(scene.lights().size(), 1U);
}

TEST(SceneParserTest, IgnoresEmptyLinesCommentsAndInlineComments) {
    SceneFile file(
        "\n"
        "# ignored\n"
        "camera 0 0 5 0 0 0 45 # trailing comment\n"
        "\n"
        "light 0 2 0 1 1 1 1 # another comment\n");

    const rt::Scene scene = rt::SceneParser::parseFile(file.path().string());

    EXPECT_TRUE(scene.objects().empty());
    EXPECT_EQ(scene.lights().size(), 1U);
}

TEST(SceneParserTest, ReportsUnknownDirectiveWithLineNumber) {
    const std::string error = parseError(
        "# line 1\n"
        "camera 0 0 5 0 0 0 45\n"
        "wat 1 2 3\n");

    EXPECT_EQ(error, "line 3: unknown directive 'wat'");
}

TEST(SceneParserTest, ReportsMissingMaterialWithLineNumber) {
    const std::string error = parseError(
        "camera 0 0 5 0 0 0 45\n"
        "sphere 0 0 0 1 missing\n");

    EXPECT_EQ(error, "line 2: unknown material 'missing'");
}

TEST(SceneParserTest, ReportsWrongArityWithLineNumber) {
    const std::string error = parseError(
        "camera 0 0 5 0 0 0\n");

    EXPECT_EQ(error, "line 1: camera expects 7 arguments, got 6");
}

TEST(SceneParserTest, RejectsMalformedAndNonFiniteNumbers) {
    EXPECT_EQ(parseError("camera 0 0 5 0 0 0 nan\n"),
              "line 1: invalid number 'nan'");
    EXPECT_EQ(parseError("camera 0 0 5 0 0 0 inf\n"),
              "line 1: invalid number 'inf'");
    EXPECT_EQ(parseError("camera 0 0x 5 0 0 0 45\n"),
              "line 1: invalid number '0x'");
}

TEST(SceneParserTest, RejectsMissingFile) {
    const std::filesystem::path path =
        std::filesystem::temp_directory_path() / "minirtx-scene-parser-does-not-exist.scene";
    std::filesystem::remove(path);

    try {
        (void)rt::SceneParser::parseFile(path.string());
        FAIL() << "expected an error";
    } catch (const std::runtime_error& error) {
        EXPECT_EQ(std::string(error.what()), "cannot open scene file '" + path.string() + "'");
    }
}

TEST(SceneParserTest, RequiresExactlyOneCamera) {
    EXPECT_EQ(parseError("background 0 0 0\n"),
              "line 1: missing camera directive");
    EXPECT_EQ(parseError(
                  "camera 0 0 5 0 0 0 45\n"
                  "camera 0 0 5 0 0 0 45\n"),
              "line 2: duplicate camera");
}

TEST(SceneParserTest, RejectsDuplicateBackgroundAndMaterial) {
    EXPECT_EQ(parseError(
                  "camera 0 0 5 0 0 0 45\n"
                  "background 0 0 0\n"
                  "background 1 1 1\n"),
              "line 3: duplicate background");
    EXPECT_EQ(parseError(
                  "camera 0 0 5 0 0 0 45\n"
                  "material m 1 1 1 0 1 0 8 0\n"
                  "material m 1 1 1 0 1 0 8 0\n"),
              "line 3: duplicate material 'm'");
}

TEST(SceneParserTest, RejectsOutOfRangeValues) {
    EXPECT_EQ(parseError("camera 0 0 5 0 0 0 0\n"),
              "line 1: fov must be in ]0,180[");
    EXPECT_EQ(parseError("camera 0 0 5 0 0 0 180\n"),
              "line 1: fov must be in ]0,180[");
    EXPECT_EQ(parseError(
                  "camera 0 0 5 0 0 0 45\n"
                  "material m 1 1 1 -1 1 1 8 0\n"),
              "line 2: ambient must be in [0,1]");
    EXPECT_EQ(parseError(
                  "camera 0 0 5 0 0 0 45\n"
                  "material m 1 1 1 0 1 1 8 1.1\n"),
              "line 2: reflectivity must be in [0,1]");
    EXPECT_EQ(parseError(
                  "camera 0 0 5 0 0 0 45\n"
                  "light 0 1 0 1 1 1 -1\n"),
              "line 2: intensity must be >= 0");
}

TEST(SceneParserTest, RejectsDegenerateCamera) {
    EXPECT_EQ(parseError("camera 1 2 3 1 2 3 45\n"),
              "line 1: camera lookAt must differ from position");
}

TEST(SceneParserTest, RejectsNegativeShininessAndNonPositiveRadius) {
    EXPECT_EQ(parseError("camera 0 0 5 0 0 0 45\n"
                         "material m 1 1 1 0 1 0 -1 0\n"),
              "line 2: shininess must be >= 0");
    EXPECT_EQ(parseError("camera 0 0 5 0 0 0 45\n"
                         "material m 1 1 1 0 1 0 8 0\n"
                         "sphere 0 0 0 0 m\n"),
              "line 3: radius must be > 0");
}

TEST(SceneParserTest, RejectsMaterialDeclaredAfterUse) {
    EXPECT_EQ(parseError("camera 0 0 5 0 0 0 45\n"
                         "sphere 0 0 0 1 m\n"
                         "material m 1 1 1 0 1 0 8 0\n"),
              "line 2: unknown material 'm'");
}
