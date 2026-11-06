// Fichier : SceneParser.cpp
// Rôle : parseur ligne par ligne et diagnostics contextualisés.
// Responsable : Personne C
// Dépendances : SceneParser.hpp, <fstream>
// CHAÎNE : fichier texte -> tokens -> validations -> constructeurs -> Scene.
// Le flux inverse est Renderer -> Scene ; le parser ne rend jamais l'image.

#include "scene/SceneParser.hpp"
#include "geometry/Sphere.hpp"
#include "geometry/Plane.hpp"
#include "geometry/Triangle.hpp"

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <sstream>
#include <vector>

namespace rt {
namespace {

using Tokens = std::vector<std::string>;

// Nombre fini sur tout le token ("1.5x", "nan", "inf" sont rejetés)
double toNumber(const std::string& tok) {
    char* end = nullptr;
    const double v = std::strtod(tok.c_str(), &end);
    if (tok.empty() || *end != '\0' || !std::isfinite(v))
        throw std::invalid_argument("invalid number '" + tok + "'");
    return v;
}

void expectArity(const Tokens& t, std::size_t n) {
    if (t.size() - 1 != n)
        throw std::invalid_argument(t[0] + " expects " + std::to_string(n) +
                                    " arguments, got " + std::to_string(t.size() - 1));
}

double num(const Tokens& t, std::size_t i) { return toNumber(t[i]); }
Vec3 vec(const Tokens& t, std::size_t i) { return Vec3{num(t, i), num(t, i + 1), num(t, i + 2)}; }
Color col(const Tokens& t, std::size_t i) { return Color{num(t, i), num(t, i + 1), num(t, i + 2)}; }

void expectUnitInterval(double value, const char* name) {
    if (value < 0.0 || value > 1.0)
        throw std::invalid_argument(std::string(name) + " must be in [0,1]");
}

} // namespace

Scene SceneParser::parseFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("cannot open scene file '" + path + "'");

    Scene scene;
    bool hasCamera = false, hasBackground = false;
    int lineNo = 0;
    std::string line;

    while (std::getline(in, line)) {
        ++lineNo;
        try {
            // Commentaires et lignes vides.
            const auto hash = line.find('#');
            if (hash != std::string::npos) line.erase(hash);
            std::istringstream iss(line);
            Tokens t;
            for (std::string tok; iss >> tok;) t.push_back(tok);
            if (t.empty()) continue;

            const std::string& d = t[0];

            if (d == "camera") {
                expectArity(t, 7);
                if (hasCamera) throw std::invalid_argument("duplicate camera");
                CameraParams cp;                       // up = (0,1,0) par défaut
                cp.position = vec(t, 1);
                cp.lookAt = vec(t, 4);
                cp.verticalFovDegrees = num(t, 7);
                if (!(cp.verticalFovDegrees > 0.0 && cp.verticalFovDegrees < 180.0))
                    throw std::invalid_argument("fov must be in ]0,180[");
                if (cp.position.x() == cp.lookAt.x() && cp.position.y() == cp.lookAt.y() &&
                    cp.position.z() == cp.lookAt.z())
                    throw std::invalid_argument("camera lookAt must differ from position");
                scene.setCameraParams(cp);
                hasCamera = true;

            } else if (d == "background") {
                expectArity(t, 3);
                if (hasBackground) throw std::invalid_argument("duplicate background");
                scene.setBackground(col(t, 1));
                hasBackground = true;

            } else if (d == "material") {
                expectArity(t, 9);   // name r g b ambient diffuse specular shininess reflectivity
                auto m = std::make_shared<Material>();
                m->name = t[1];
                m->albedo = col(t, 2);
                m->ambient = num(t, 5);
                m->diffuse = num(t, 6);
                m->specular = num(t, 7);
                m->shininess = num(t, 8);
                m->reflectivity = num(t, 9);
                for (const double coefficient : {num(t, 2), num(t, 3), num(t, 4)})
                    expectUnitInterval(coefficient, "albedo");
                expectUnitInterval(m->ambient, "ambient");
                expectUnitInterval(m->diffuse, "diffuse");
                expectUnitInterval(m->specular, "specular");
                if (m->shininess < 0.0) throw std::invalid_argument("shininess must be >= 0");
                if (m->reflectivity < 0.0 || m->reflectivity > 1.0)
                    throw std::invalid_argument("reflectivity must be in [0,1]");
                
                auto inUnit = [](double v) { return v >= 0.0 && v <= 1.0; };
                if (!inUnit(m->ambient))
                    throw std::invalid_argument("ambient must be in [0,1]");
                if (!inUnit(m->diffuse))
                    throw std::invalid_argument("diffuse must be in [0,1]");
                if (!inUnit(m->specular))
                    throw std::invalid_argument("specular must be in [0,1]");
                if (m->shininess < 0.0)
                    throw std::invalid_argument("shininess must be >= 0");
                if (!inUnit(m->reflectivity))
                    throw std::invalid_argument("reflectivity must be in [0,1]");
                scene.addMaterial(m);   // rejette les doublons

            } else if (d == "light") {
                expectArity(t, 7);   // x y z r g b intensity
                PointLight l;
                l.position = vec(t, 1);
                l.color = col(t, 4);
                l.intensity = num(t, 7);
                if (l.intensity < 0.0) throw std::invalid_argument("intensity must be >= 0");
                scene.addLight(l);

            } else if (d == "sphere") {
                expectArity(t, 5);   // cx cy cz radius material
                const double radius = num(t, 4);
                if (!(radius > 0.0)) throw std::invalid_argument("radius must be > 0");
                const auto& mat = scene.findMaterial(t[5]);
                scene.addObject(std::make_unique<Sphere>(vec(t, 1), radius, mat));

            } else if (d == "plane") {
                expectArity(t, 7);   // px py pz nx ny nz material
                const auto& mat = scene.findMaterial(t[7]);
                scene.addObject(std::make_unique<Plane>(vec(t, 1), vec(t, 4), mat));

            } else if (d == "triangle") {
                expectArity(t, 10);  // a(3) b(3) c(3) material
                const auto& mat = scene.findMaterial(t[10]);
                scene.addObject(std::make_unique<Triangle>(vec(t, 1), vec(t, 4), vec(t, 7), mat));

            } else {
                throw std::invalid_argument("unknown directive '" + d + "'");
            }

        } catch (const std::invalid_argument& e) {
            throw std::runtime_error("line " + std::to_string(lineNo) + ": " + e.what());
        }
    }

    if (in.bad()) throw std::runtime_error("read error on '" + path + "'");
    if (!hasCamera)
        throw std::runtime_error("line " + std::to_string(lineNo) + ": missing camera directive");

    return scene;
}

} // namespace rt
