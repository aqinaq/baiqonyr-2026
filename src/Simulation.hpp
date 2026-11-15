#pragma once
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <vector>
#include <limits>
namespace bk {
constexpr float rocketScale = 1.8f;
constexpr float pi = 3.14159265358979323846f;
enum class Flight { Ascent, Choosing, Transit, Arrived, Destroyed };
struct Destination {
    const char *name;
    glm::vec3 center, color;
    float radius;
    const char *warning;
    bool dangerous;
};
inline const Destination destinations[] = {
    {"MOON", {-850, 5770, -1750}, {.65,.68,.72}, 55, "Controlled descent and landing on the lunar surface.", false},
    {"MARS", {1200, 5840, -3200}, {.72,.25,.12}, 85, "Controlled descent and landing on Mars.", false},
    {"SUN", {-3600, 6000, -6250}, {1,.55,.12}, 220, "High thermal exposure. Shield endurance uncertain.", true},
    {"VENUS", {2600, 5650, -4500}, {.86,.65,.34}, 105, "Dense atmosphere. Heat and pressure risk.", true},
    {"JUPITER", {-4500, 5900, 1250}, {.74,.57,.39}, 230, "Extreme pressure and turbulent cloud layers.", true},
    {"SATURN", {4750, 5950, 500}, {.82,.72,.48}, 175, "Stylized polar landing, approaching clear of the rings.", false},
    {"MERCURY", {-2100, 5650, -4250}, {.45,.42,.38}, 38, "Surface landing. Keep the sun shield aligned.", false},
    {"URANUS", {2500, 6200, 5000}, {.32,.72,.77}, 105, "Low-risk distant observation orbit.", false},
    {"NEPTUNE", {-2750, 6100, 5750}, {.15,.32,.8}, 110, "Low-risk distant observation orbit.", false}
};
constexpr int destinationCount = sizeof(destinations) / sizeof(destinations[0]);
// Simulation metres and seconds are deliberately compressed for a classroom demo.
struct Mission {
    bool running = false, paused = false, separated = false;
    float clock = -8.f, speed = 1.f, separationTime = 24.f;
    float boosterAge = 0.f;
    Flight flight = Flight::Ascent;
    int destination = -1, origin = -1;
    bool surfaceDeparture = false;
    glm::vec3 takeoffStart{0};
    glm::quat departureRotation{1,0,0,0};
    std::vector<glm::mat4> plantedFlags;
    static constexpr float crewWalkSpeed = 1.5f;
    static constexpr float boardingWalkEnd = 16.f/crewWalkSpeed;
    static constexpr float boardingDuration = boardingWalkEnd+2.f;
    static constexpr float flagPlantStart = 2.5f+14.5f/crewWalkSpeed;
    static constexpr float flagUnfurlStart = flagPlantStart+3.f;
    static constexpr float ceremonyDuration = flagUnfurlStart+2.f;
    static float boardingProgress(float elapsed,int i) {
        return std::clamp((elapsed*crewWalkSpeed-i*2.f)/8.f,0.f,1.f);
    }
    float boarding = 0, crewAnimationTime = 0;
    float closingHatch = 0;
    bool boardingCrew() const { return running && flight==Flight::Ascent && boarding<boardingDuration; }
    float crewBoardProgress(int i) const { return boardingProgress(boarding,i); }
    float crewExitProgress(int i) const { return surfaceLanded() ? std::clamp(((aftermath-2.5f)*crewWalkSpeed-i*1.5f)/8.f,0.f,1.f) : 0.f; }
    int crewAboard() const {
        int count=0;
        for(int i=0;i<5;++i) count+=crewBoardProgress(i)>=1.f;
        return count;
    }
    float travel = 0, aftermath = 0, resultDelay = 0, departure = 0;
    static float ease(float t) { t=std::clamp(t,0.f,1.f); return t*t*t*(t*(t*6-15)+10); }
    std::vector<glm::vec3> route;
    float routeLength = 0;
    static constexpr float orbitTime = 100.f, travelDuration = 18.f;
    bool inSpace() const { return flight != Flight::Ascent; }
    bool canContinue() const {
        return flight==Flight::Arrived && resultReady() && (!canLand() || aftermath>=ceremonyDuration);
    }
    bool selectDestination(int index) {
        bool onward=canContinue();
        if ((flight!=Flight::Choosing && !onward) || index<0 || index>=destinationCount ||
            (onward && index==destination)) return false;
        glm::vec3 start=position();
        glm::quat rotation=glm::quat_cast(glm::mat3(rocket()));
        bool lifting=onward && canLand();
        int previous=onward ? destination : -1;
        glm::vec3 routingStart=start;
        glm::mat4 flag(1);
        if(lifting) {
            flag=flagFrame();
            routingStart=destinations[previous].center+landingNormal()*(clearance(previous)+35.f);
        }
        if(!planRoute(index,routingStart)) return false;
        if(lifting) plantedFlags.push_back(flag);
        origin=previous; surfaceDeparture=lifting; takeoffStart=start;
        departureRotation=rotation; closingHatch=lifting ? 1.f : 0.f;
        destination=index; travel=0; aftermath=0; resultDelay=0;
        departure=lifting ? boardingDuration+3.f : 3.f; // Close hatch and hand off the camera before liftoff.
        flight=Flight::Transit;
        return true;
    }
    float cruiseProgress() const {
        return surfaceDeparture ? std::clamp((progress()-.2f)/.8f,0.f,1.f) : progress();
    }
    float progress() const { return std::clamp(travel / travelDuration, 0.f, 1.f); }
    float danger() const {
        return destination >= 0 && destinations[destination].dangerous
            ? std::clamp((progress() - .45f) / .55f, 0.f, 1.f) : 0.f;
    }
    glm::vec3 position() const {
        if (route.empty()) return {0, altitudeAt(std::min(clock, orbitTime)), 0};
        if(surfaceDeparture && progress()<.2f)
            return glm::mix(takeoffStart,route.front(),ease(progress()/.2f));
        float cruise=cruiseProgress();
        if (canLand() && cruise > .75f) {
            float t = (cruise - .75f) / .25f;
            t = ease(t);
            return glm::mix(route.back(), landingPoint(), t);
        }
        float distance = std::min(cruise / (canLand() ? .75f : 1.f), 1.f) * routeLength;
        for (std::size_t i=1; i<route.size(); ++i) {
            float length = glm::length(route[i]-route[i-1]);
            if (distance <= length) return glm::mix(route[i-1],route[i],ease(distance/std::max(length,.001f)));
            distance -= length;
        }
        return route.back();
    }
    static float clearance(int i) { return i == 5 ? 330.f : destinations[i].radius + 45.f; }
    static bool clearSegment(glm::vec3 a, glm::vec3 b) {
        glm::vec3 delta=b-a;
        float length2=glm::dot(delta,delta);
        for (int i=0; i<destinationCount; ++i) {
            float t=length2>0 ? std::clamp(glm::dot(destinations[i].center-a,delta)/length2,0.f,1.f) : 0;
            if (glm::length(a+t*delta-destinations[i].center) < clearance(i)) return false;
        }
        return true;
    }
    bool planRoute(int index, glm::vec3 start) {
        const auto &d=destinations[index];
        // Saturn descends over the pole, away from the equatorial ring mesh.
        glm::vec3 approach=index==5 ? glm::vec3(0,1,0) : glm::normalize(start-d.center);
        glm::vec3 end=d.center+approach*(clearance(index)+35.f);
        std::vector<glm::vec3> nodes{start,end};
        // Visibility graph: connect only segments outside every planet and ring envelope.
        for (int i=0; i<destinationCount; ++i)
            for (int axis=0; axis<3; ++axis)
                for (float sign : {-1.f,1.f}) {
                    auto point=destinations[i].center;
                    point[axis]+=sign*(clearance(i)*1.8f+100.f);
                    nodes.push_back(point);
                }
        std::vector<float> cost(nodes.size(),std::numeric_limits<float>::infinity());
        std::vector<int> previous(nodes.size(),-1);
        std::vector<bool> visited(nodes.size(),false);
        cost[0]=0;
        for (std::size_t step=0; step<nodes.size(); ++step) {
            int u=-1;
            for (std::size_t i=0; i<nodes.size(); ++i)
                if (!visited[i] && (u<0 || cost[i]<cost[u])) u=int(i);
            if (u<0 || !std::isfinite(cost[u])) break;
            if (u==1) break;
            visited[u]=true;
            for (std::size_t v=0; v<nodes.size(); ++v) {
                float next=cost[u]+glm::length(nodes[v]-nodes[u]);
                if (!visited[v] && next<cost[v] && clearSegment(nodes[u],nodes[v])) {
                    cost[v]=next; previous[v]=u;
                }
            }
        }
        if (!std::isfinite(cost[1])) return false;
        route.clear();
        for (int at=1; at>=0; at=previous[at]) route.push_back(nodes[at]);
        std::reverse(route.begin(),route.end()); routeLength=cost[1];
        return true;
    }
    bool canLand() const { return destination == 0 || destination == 1 || destination == 5 || destination == 6; }
    glm::vec3 landingNormal() const {
        return glm::normalize(route.back() - destinations[destination].center);
    }
    glm::vec3 landingPoint() const {
        return destinations[destination].center + landingNormal() * (destinations[destination].radius + .15f);
    }
    glm::mat4 flagFrame() const {
        auto pose=rocketModel(); const auto &body=destinations[destination];
        glm::vec3 desired=glm::vec3(pose*glm::vec4(-5.5f,0,1.5f,1));
        glm::vec3 normal=glm::normalize(desired-body.center);
        glm::vec3 x=glm::normalize(glm::vec3(pose[0])-normal*glm::dot(glm::vec3(pose[0]),normal));
        glm::mat4 frame(1);
        frame[0]=glm::vec4(x,0); frame[1]=glm::vec4(normal,0);
        frame[2]=glm::vec4(glm::cross(x,normal),0);
        frame[3]=glm::vec4(body.center+normal*(body.radius+.015f),1);
        return frame;
    }
    bool surfaceLanded() const { return flight==Flight::Arrived && canLand(); }
    float hatchOpen() const {
        if(flight==Flight::Ascent && (!running || boarding<boardingDuration))
            return !running ? 1.f : 1.f-ease((boarding-boardingWalkEnd)/2.f);
        return surfaceLanded() ? ease((aftermath-.5f)/2.f) : closingHatch*ease(departure/3.f);
    }
    float flagPlacement() const { return surfaceLanded() ? ease((aftermath-flagPlantStart)/3.f) : 0.f; }
    float flagUnfurl() const { return surfaceLanded() ? ease((aftermath-flagUnfurlStart)/2.f) : 0.f; }
    bool resultReady() const { return resultDelay >= 5.f; }
    void launch() {
        if (!running) {
            running = true;
            clock = -8.f;
        }
    }
    void reset() { *this = Mission{}; }
    void update(float dt) {
        if(paused) return;
        crewAnimationTime+=std::max(0.f,dt);
        if (!running) return;
        float elapsed=std::max(0.f,dt);
        if(boardingCrew()) {
            float held=std::min(elapsed,boardingDuration-boarding);
            boarding+=held; elapsed-=held;
        }
        float step = elapsed * speed;
        // Detached stages keep falling even while the core waits for a destination.
        if (separated) boosterAge += step;
        if (flight == Flight::Destroyed || flight == Flight::Arrived) {
            resultDelay += std::max(0.f, dt); // Real seconds, independent of playback speed.
            aftermath = std::min(aftermath + std::max(0.f, dt), 28.f);
            return;
        }
        if (flight == Flight::Choosing || flight == Flight::Arrived) return;
        if (flight == Flight::Transit) {
            if (departure>0) {
                float held=std::min(std::max(dt,0.f),departure);
                departure-=held;
                step=std::max(0.f,dt-held)*speed;
            }
            travel = std::min(travel + step, travelDuration);
            if (travel >= travelDuration)
                flight = destinations[destination].dangerous ? Flight::Destroyed : Flight::Arrived;
            return;
        }
        clock = std::min(clock + step, orbitTime);
        if (clock >= 24.f && !separated) {
            separated = true;
            separationTime = 24.f;
            boosterAge = std::max(0.f, clock-separationTime);
        }
        if (clock >= orbitTime) flight = Flight::Choosing;
    }

    void separate() {
        if (clock >= 5.f && !separated) {
            separated = true;
            separationTime = clock;
            boosterAge = 0.f;
        }
    }
    float altitudeAt(float t) const {
        t = std::max(0.f, t);
        if(t<=80.f) return .55f*t*t;
        // Hermite coast to the navigation altitude with zero arrival velocity.
        float u=std::clamp((t-80.f)/20.f,0.f,1.f);
        return (2*u*u*u-3*u*u+1)*3520.f+(u*u*u-2*u*u+u)*1760.f
             +(-2*u*u*u+3*u*u)*5500.f;
    }
    float altitude() const { return position().y; }
    float velocity() const {
        if(inSpace()) return 0;
        if(clock<=80) return 1.1f*std::max(0.f,clock);
        float u=std::clamp((clock-80)/20.f,0.f,1.f);
        return ((6*u*u-6*u)*3520+(3*u*u-4*u+1)*1760+(-6*u*u+6*u)*5500)/20;
    }
    bool ignition() const { return running && !boardingCrew() && !(surfaceDeparture && departure>0) && clock >= -2.f && (flight == Flight::Ascent || flight == Flight::Transit); }
    glm::mat4 rocket() const { auto transform = glm::translate(glm::mat4(1), position());
        if (destination >= 0) {
            glm::vec3 direction = glm::normalize(destinations[destination].center - route.front());
            if (canLand()) {
                float turn = ease((cruiseProgress()-.55f)/.20f);
                glm::vec3 outward = landingNormal();
                float cosine = std::clamp(glm::dot(direction,outward),-1.f,1.f);
                glm::vec3 tangent = outward-direction*cosine;
                if (glm::length(tangent)<.0001f)
                    tangent = glm::cross(direction, std::abs(direction.y)<.9f ? glm::vec3(0,1,0) : glm::vec3(1,0,0));
                tangent=glm::normalize(tangent);
                float angle=std::acos(cosine)*turn;
                direction=glm::normalize(direction*std::cos(angle)+tangent*std::sin(angle));
            }
            glm::vec3 cross = glm::cross(glm::vec3(0,1,0), direction);
            glm::vec3 axis = glm::length(cross) > .0001f ? glm::normalize(cross) : glm::vec3(1,0,0);
            auto desired=glm::angleAxis(std::acos(std::clamp(direction.y,-1.f,1.f)),axis);
            transform *= glm::mat4_cast(glm::slerp(departureRotation,desired,ease(cruiseProgress()*5.f)));
        }
        return transform; }
    glm::mat4 rocketModel() const { return glm::scale(rocket(),glm::vec3(rocketScale)); }
    glm::mat4 booster(int i) const {
        float a = float(i) * pi * .5f + pi*.25f;
        glm::vec3 radial(std::cos(a), 0, std::sin(a));
        auto local = glm::translate(glm::mat4(1), (radial * 1.72f + glm::vec3(0, 3.8f, 0))*rocketScale);
        if (!separated)
            return rocket() * local;
        float t = boosterAge;
        // Detach in world space, preserve position and initial vertical velocity.
        glm::vec3 p =
            radial * (1.72f*rocketScale + 4.f * t) +
            glm::vec3(
                0, 3.8f*rocketScale + altitudeAt(separationTime) + 1.1f * separationTime * t - 4.905f * t * t, 0);
        return glm::rotate(glm::translate(glm::mat4(1), p), t * .35f,
                           glm::vec3(radial.z, 0, -radial.x));
    }
    bool boosterVisible(int i) const {
        if (!separated) return true;
        auto pose=booster(i);
        // Bounds of the scaled stage along world Y, including its nose and bell.
        float extent=rocketScale*(4.f*std::abs(pose[1].y)+
            .7f*std::sqrt(pose[0].y*pose[0].y+pose[2].y*pose[2].y));
        return pose[3].y>extent;
    }
    std::string phase() const {
        if (flight == Flight::Choosing) return "AWAITING DESTINATION";
        if (flight == Flight::Transit && departure>0) return "ALIGNING FOR DEPARTURE";
        if (flight == Flight::Transit && canLand() && progress() > .75f) return "LANDING / CONTROLLED DESCENT";
        if (flight == Flight::Transit) return danger() > .4f ? "WARNING / HULL FAILURE" : "INTERPLANETARY TRANSFER";
        if (flight == Flight::Arrived) return resultReady() ? (canLand() ? "LANDED" : "ORBIT ACHIEVED") : "STABILIZING";
        if (flight == Flight::Destroyed) return resultReady() ? "SIGNAL LOST" : "RECONNECTING";
        if (!running)
            return "READY FOR LAUNCH";
        if (boardingCrew()) return crewAboard()==5 ? "CREW ABOARD / SEALING HATCH" : "CREW BOARDING / "+std::to_string(crewAboard())+" OF 5 ABOARD";
        if (clock < -2)
            return "COUNTDOWN";
        if (clock < 0)
            return "ENGINE IGNITION";
        if (separated)
            return "CORE STAGE ASCENT";
        return "POWERED ASCENT";
    }
};
inline float atmosphere(float altitude) {
    return std::clamp(altitude / 450.f, 0.f, 1.f);
}
} // namespace bk
