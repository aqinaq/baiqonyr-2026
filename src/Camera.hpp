#pragma once
#include "Simulation.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
namespace bk {
class Camera {
    bool initialized = false, previousFirstPerson = false, previousSite = false, previousLandmark = false;
    Flight previousFlight = Flight::Ascent;
    float lastTime = 0, blendTime = 0, blendDuration = 0, landingYawBase = 0;
    glm::vec3 previousAnchor{0}, offset{0}, blendStart{0};
    glm::quat orientation{1,0,0,0};
  public:
    float yaw = -.8f, pitch = .2f, distance = 65.f;
    float lookYaw = 0, lookPitch = 0, cockpitFov = 48.f;
    bool chase = true, shake = true, firstPerson = false, siteOverview = false, landmarkView = false;
    glm::vec3 eye{0}, target{0,9,0};
    glm::mat4 viewProjection{1}, viewMatrix{1}, projection{1};
    void returnHome() {
        yaw=-.8f; pitch=.2f; cockpitFov=48.f; lookYaw=0; lookPitch=0; distance=65; chase=true; firstPerson=false; siteOverview=false; landmarkView=false;
        previousAnchor=glm::vec3(0); blendStart=eye; offset=eye;
        blendTime=0; blendDuration=3.f;
    }
    void toggleSite() {
        siteOverview=!siteOverview; landmarkView=false; firstPerson=false;
        distance=siteOverview?240.f:65.f; pitch=siteOverview?.72f:.2f;
    }
    void toggleLandmark() {
        landmarkView=!landmarkView; siteOverview=false; firstPerson=false;
        distance=landmarkView?38.f:65.f; pitch=landmarkView?.10f:.2f; yaw=landmarkView?0.f:.8f;
    }
    void orbit(float x, float y) {
        if(firstPerson) {
            lookYaw=std::clamp(lookYaw-x*.006f,-1.45f,1.45f);
            lookPitch=std::clamp(lookPitch+y*.006f,-1.2f,1.2f);
            return;
        }
        yaw -= x * .006f;
        pitch = std::clamp(pitch + y * .006f, -.8f, 1.45f);
    }
    void zoom(float amount) {
        if(firstPerson) cockpitFov=std::clamp(cockpitFov*std::exp(-amount*.08f),20.f,75.f);
        else distance = std::clamp(distance * std::exp(-amount * .08f), 24.f, 12000.f);
    }
    glm::mat4 view(const Mission &m, float realTime) {
        if(m.running && (siteOverview || landmarkView)) { siteOverview=false; landmarkView=false; distance=65; pitch=.2f; yaw=-.8f; }
        float dt = initialized ? std::clamp(realTime-lastTime,0.f,.1f) : 0.f;
        lastTime=realTime;
        glm::vec3 anchor=(chase || firstPerson || m.inSpace()) ? m.position() : glm::vec3(0);
        glm::vec3 up(0,1,0), wantedTarget=anchor+glm::vec3(0,9*rocketScale,0);
        bool showingSite=siteOverview && !m.running;
        if(showingSite) wantedTarget={45,4,-20};
        bool showingLandmark=landmarkView && !m.running;
        if(showingLandmark) wantedTarget={26,3,28};
        glm::vec3 wantedEye=wantedTarget+distance*glm::vec3(std::sin(yaw)*std::cos(pitch),
            std::sin(pitch),std::cos(yaw)*std::cos(pitch));
        if (m.canLand() && !m.route.empty()) {
            float blend=glm::smoothstep(.55f,.75f,m.cruiseProgress());
            glm::vec3 normal=m.landingNormal();
            glm::vec3 tangent=glm::cross(normal,glm::vec3(0,1,0));
            if(glm::length(tangent)<.001f) tangent={1,0,0};
            tangent=glm::normalize(tangent);
            glm::vec3 side=glm::cross(tangent,normal);
            glm::vec3 surfaceTarget=anchor+normal*(8.f*rocketScale);
            glm::vec3 surfaceEye=surfaceTarget+distance*(normal*std::sin(std::max(pitch,.18f))+
                (tangent*std::sin(yaw)+side*std::cos(yaw))*std::cos(std::max(pitch,.18f)));
            wantedEye=glm::mix(wantedEye,surfaceEye,blend);
            wantedTarget=glm::mix(wantedTarget,surfaceTarget,blend);
            up=glm::normalize(glm::mix(up,normal,blend));
        }
        if(m.surfaceLanded() && !firstPerson) {
            auto pose=m.rocket();
            if(previousFlight!=Flight::Arrived) landingYawBase=yaw;
            float angle=yaw-landingYawBase;
            wantedTarget=glm::vec3(m.rocketModel()*glm::vec4(-4.5f,7,1,1));
            wantedEye=wantedTarget+glm::vec3(pose*glm::vec4(std::sin(angle),std::max(.18f,pitch),std::cos(angle),0))*distance*.85f;
            up=glm::normalize(glm::vec3(pose[1]));
        }
        if (m.flight==Flight::Choosing && !firstPerson) {
            wantedTarget={0,5800,0}; wantedEye={0,21500,10500}; up={0,1,0};
        }
        if(firstPerson) {
            auto rocket=m.rocket();
            wantedEye=glm::vec3(rocket*glm::vec4(0,15.15f*rocketScale,-.5f*rocketScale,1));
            // Crew eye level inside the cabin, looking through the forward window.
            glm::vec3 forward=glm::normalize(glm::vec3(rocket*glm::vec4(
                std::sin(lookYaw)*std::cos(lookPitch),std::sin(lookPitch),
                std::cos(lookYaw)*std::cos(lookPitch),0)));
            up=glm::normalize(glm::vec3(rocket*glm::vec4(0,1,0,0)));
            if(std::abs(glm::dot(forward,up))>.98f) up=glm::vec3(rocket[0]);
            wantedTarget=wantedEye+forward*100.f;
        }
        if(initialized && (previousFlight!=m.flight || previousFirstPerson!=firstPerson || previousSite!=showingSite || previousLandmark!=showingLandmark)) {
            blendStart=eye-previousAnchor;
            blendTime=0; blendDuration=previousFirstPerson!=firstPerson ? 1.4f : 3.f;
        }
        glm::vec3 wantedOffset=wantedEye-anchor;
        if(!initialized) offset=wantedOffset;
        else if(blendTime<blendDuration) {
            blendTime=std::min(blendTime+dt,blendDuration);
            float t=blendTime/blendDuration; t=t*t*t*(t*(t*6-15)+10);
            offset=glm::mix(blendStart,wantedOffset,t);
        } else offset=glm::mix(offset,wantedOffset,1-std::exp(-dt*8.f));
        eye=anchor+offset;
        // Prevent the camera passing below a destination's surface during a blend.
        for(const auto &body:destinations) {
            glm::vec3 radial=eye-body.center; float r=glm::length(radial);
            if(r<body.radius+2.f && r>.001f) eye=body.center+radial/r*(body.radius+2.f);
        }
        glm::vec3 forward=wantedTarget-eye;
        if(glm::length(forward)<.001f) forward={0,0,-1};
        forward=glm::normalize(forward);
        if(std::abs(glm::dot(forward,up))>.98f) up=glm::vec3(1,0,0);
        glm::quat desired=glm::quat_cast(glm::transpose(glm::mat3(glm::lookAt(glm::vec3(0),forward,up))));
        orientation=initialized ? glm::normalize(glm::slerp(orientation,desired,1-std::exp(-dt*7.f))) : desired;
        target=eye+orientation*glm::vec3(0,0,-100);
        glm::vec3 cameraUp=orientation*glm::vec3(0,1,0);
        glm::vec3 jitter(0);
        if(shake && m.ignition() && m.clock<4 && !m.paused)
            jitter=.045f*(1-std::clamp((m.clock+2)/6.f,0.f,1.f))*glm::vec3(std::sin(realTime*83),std::cos(realTime*97),0);
        viewMatrix=glm::lookAt(eye+jitter,target+jitter,cameraUp);
        previousAnchor=anchor; previousFlight=m.flight; previousFirstPerson=firstPerson; previousSite=showingSite; previousLandmark=showingLandmark; initialized=true;
        return viewMatrix;
    }
};
} // namespace bk
