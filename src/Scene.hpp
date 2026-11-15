#pragma once
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <string>
namespace bk {
struct Object {
    int mesh;
    glm::mat4 transform;
    glm::vec3 color;
    int material = 0;
    float shine = 32, emission = 0;
};
class Scene {
    Mesh box{cube()}, tube{cylinder()}, cone{cylinder(0)}, ball{sphere()}, terrain{kazakhstan()};
    Mesh rock{sphere(5, 8)}, scrub{cylinder(0, 6)}, pipe{cylinder(1, 12)}, ridges{steppeRidges()};
    Mesh rings{saturnRings()}, fairing{noseFairing()}, sign{landmarkLetters()}, heart{landmarkHeart()}, wings{sweptWings()}, bell{engineBell()}, company{companyLetters()};
    Shader shader, sky;
    Texture concrete, flagTexture;
    GLuint skyVAO = 0;
    std::vector<Object> ground, rocketParts, boosterParts;
    void draw(const Object &o, const glm::mat4 &parent);
    void buildEnvironment();
    void buildLaunchDetails();
    void buildRocketDetails();
    void drawCrew(const Mission &m);
    void drawAstronaut(const glm::mat4 &frame, int member, float gait, float walk, float salute=0);
    void drawCockpit(const Mission &m, bool firstPerson);
    void drawLandingSequence(const Mission &m);
    void drawFlag(const glm::mat4 &frame, float extension, float unfurl);

  public:
    bool lights = true, night = false, wireframe = false;
    explicit Scene(const std::string &path);
    ~Scene();
    void render(const Mission &mission, Camera &camera, int width, int height, float realTime);
    size_t objects() const { return ground.size() + rocketParts.size() + 4 * boosterParts.size(); }
};
} // namespace bk
