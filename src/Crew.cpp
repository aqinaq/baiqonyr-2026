#include "Scene.hpp"
namespace bk {
namespace {
glm::mat4 at(glm::vec3 p) { return glm::translate(glm::mat4(1),p); }
const glm::vec3 accents[]={{.42f,.16f,.66f},{.91f,.24f,.49f},{.96f,.65f,.08f},{.12f,.36f,.78f},{.42f,.22f,.20f}};
}
// Hierarchical rigid joints keep every accessory attached to its animated limb.
// Reference order: violet braids, pink bun/glasses, gold waves/stars, blue curls, brown bob/tablet.
void Scene::drawAstronaut(const glm::mat4 &frame,int member,float gait,float walk,float salute) {
    const auto accent=accents[member];
    const glm::vec3 suit(.91f,.87f,.84f), hair(.065f,.042f,.038f), skin(.77f,.51f,.37f), dark(.035f,.045f,.06f);
    auto part=[&](int mesh,glm::vec3 p,glm::vec3 scale,glm::vec3 color,const glm::mat4 &joint=glm::mat4(1),int material=0) {
        draw({mesh,joint*glm::scale(at(p),scale),color,material,48,0},frame);
    };
    float bob=walk*.025f*std::abs(std::sin(gait)) + .008f*std::sin(gait*.35f);
    auto torso=at({0,bob,0});
    part(3,{0,1.14f,0},{member==3?.35f:.30f,.41f,.23f},suit,torso);
    part(0,{0,1.23f,-.27f},{.43f,.60f,.22f},suit,torso); // life support
    part(0,{0,1.23f,-.39f},{.30f,.42f,.035f},accent,torso);
    part(1,{0,.91f,0},{.29f,.09f,.235f},dark,torso);
    part(0,{0,.91f,.242f},{.11f,.09f,.035f},{.7f,.72f,.74f},torso);
    for(float side:{-1.f,1.f}) {
        part(0,{side*.18f,1.18f,.215f},{.045f,.52f,.03f},accent,torso);
        auto hip=torso*at({side*.155f,.86f,0})*glm::rotate(glm::mat4(1),side*std::sin(gait)*.42f*walk,glm::vec3(1,0,0));
        part(3,{0,-.20f,0},{.15f,.26f,.16f},suit,hip);
        part(0,{side*.13f,-.18f,.03f},{.09f,.22f,.17f},accent,hip);
        auto knee=hip*at({0,-.39f,0})*glm::rotate(glm::mat4(1),std::max(0.f,-side*std::sin(gait))*.55f*walk,glm::vec3(1,0,0));
        part(3,{0,-.14f,0},{.135f,.23f,.14f},suit,knee);
        part(3,{0,.01f,.12f},{.115f,.10f,.035f},accent,knee);
        part(0,{0,-.32f,.055f},{.26f,.17f,.40f},suit,knee);
        part(0,{0,-.40f,.055f},{.27f,.045f,.42f},accent,knee);
        part(0,{0,-.25f,.19f},{.08f,.10f,.025f},accent,knee);
        float raised=(side>0?salute:0);
        auto shoulder=torso*at({side*.31f,1.39f,0})*
            glm::rotate(glm::mat4(1),side*(.13f+raised*1.8f),glm::vec3(0,0,1))*
            glm::rotate(glm::mat4(1),-side*std::sin(gait)*.36f*walk,glm::vec3(1,0,0));
        part(3,{0,-.14f,0},{.14f,.22f,.15f},suit,shoulder);
        part(0,{0,-.06f,.145f},{.18f,.12f,.018f},{.02f,.62f,.75f},shoulder);
        part(3,{0,-.06f,.16f},{.035f,.035f,.008f},{1,.75f,.12f},shoulder);
        auto elbow=shoulder*at({0,-.30f,0})*glm::rotate(glm::mat4(1),-.2f-raised*.6f,glm::vec3(1,0,0));
        part(3,{0,-.12f,0},{.12f,.19f,.125f},suit,elbow);
        part(1,{0,-.23f,0},{.12f,.075f,.125f},accent,elbow);
        part(3,{0,-.32f,.025f},{.105f,.12f,.10f},suit,elbow);
        if(member==4 && side<0) part(0,{0,-.28f,.13f},{.27f,.36f,.035f},dark,elbow);
    }
    part(0,{0,1.24f,.243f},{.25f,.19f,.065f},accent,torso);
    part(0,{0,1.25f,.28f},{.18f,.105f,.015f},dark,torso);
    for(int i=0;i<3;++i) part(3,{-.06f+i*.06f,1.25f,.295f},{.018f,.025f,.01f},i==0?glm::vec3(.05f,.8f,1):glm::vec3(1,.5f,.2f),torso);
    part(1,{0,1.54f,0},{.28f,.10f,.26f},accent,torso);
    part(1,{0,1.58f,0},{.29f,.055f,.27f},suit,torso);
    auto head=torso*at({0,1.82f,0})*glm::rotate(glm::mat4(1),.035f*std::sin(gait*.3f),glm::vec3(0,1,0));
    part(3,{0,0,0},{.26f,.29f,.235f},skin,head);
    part(3,{0,.17f,-.05f},{.28f,.19f,.23f},hair,head);
    for(float side:{-1.f,1.f}) {
        part(3,{side*.255f,-.015f,0},{.05f,.075f,.055f},skin,head);
        part(3,{side*.10f,.015f,.213f},{.067f,.062f,.025f},{1,.96f,.91f},head);
        part(3,{side*.10f,.015f,.237f},{.033f,.042f,.012f},hair,head);
        part(3,{side*.09f,.03f,.249f},{.011f,.013f,.006f},{1,1,1},head);
        part(3,{side*.10f,.102f,.21f},{.075f,.018f,.018f},hair,head);
        part(3,{side*.16f,-.09f,.19f},{.045f,.025f,.018f},{.83f,.39f,.34f},head);
        if(member==0) for(int b=0;b<6;++b)
            part(3,{side*(.23f+.015f*std::sin(b*2.f)),.09f-b*.10f,.01f},{.075f,.075f,.075f},b%2?accent:hair,head);
        if(member==2 || member==4) for(int b=0;b<4;++b)
            part(3,{side*(.23f+.025f*std::sin(b*2.f)),.15f-b*.10f,-.04f},{.085f,.11f,.15f},hair,head);
        if(member==1) {
            // Four thin bars leave the eyes visible through the spectacles.
            for(float y:{-.06f,.09f}) part(0,{side*.105f,y,.255f},{.17f,.018f,.018f},dark,head);
            for(float x:{side*.025f,side*.185f}) part(0,{x,.015f,.255f},{.018f,.16f,.018f},dark,head);
        }
    }
    part(3,{0,-.035f,.235f},{.033f,.045f,.04f},skin,head);
    part(3,{0,-.135f,.209f},{.075f,.022f,.017f},{.38f,.12f,.12f},head);
    part(3,{0,-.127f,.222f},{.052f,.009f,.008f},{1,.95f,.88f},head);
    if(member==1) {
        part(3,{0,.35f,-.11f},{.16f,.15f,.15f},hair,head);
        part(0,{0,.018f,.257f},{.055f,.018f,.018f},dark,head);
    }
    if(member==3) for(int i=0;i<9;++i) {
        float a=i*2.4f;
        part(3,{.21f*std::cos(a),.22f+.045f*std::sin(i*1.8f),.15f*std::sin(a)},{.095f,.09f,.095f},hair,head);
    }
    if(member==2) for(int i=0;i<3;++i) {
        auto star=head*at({-.22f+i*.03f,.10f+i*.065f,.17f});
        for(int r=0;r<5;++r) part(0,{0,.02f,0},{.018f,.065f,.018f},{1,.72f,.12f},star*glm::rotate(glm::mat4(1),r*2*pi/5,glm::vec3(0,0,1)));
    }
    // Open clear visor: structural rim and highlights, no opaque sphere hiding the face.
    for(int i=0;i<24;++i) {
        float a=i*2*pi/24;
        part(3,{.33f*std::cos(a),.02f+.35f*std::sin(a),.035f},{.022f,.022f,.025f},suit,head);
    }
    for(int i=0;i<5;++i) part(3,{-.27f-i*.008f,.15f+i*.027f,.14f},{.012f,.025f,.012f},{.65f,.85f,.94f},head);
}

void Scene::drawCrew(const Mission &m) {
    bool home=m.flight==Flight::Ascent && m.clock<=-8;
    bool returning=m.surfaceDeparture && m.departure>0;
    if(!home && !m.surfaceLanded() && !returning) return;
    auto parent=m.rocketModel();
    // Project endpoints onto curved planet surfaces, including onward boarding.
    auto floor=[&](glm::vec3 p) {
        int body=returning?m.origin:m.destination;
        if(!home && body>=0) {
            glm::vec3 w=glm::vec3(parent*glm::vec4(p,1));
            w=destinations[body].center+glm::normalize(w-destinations[body].center)*(destinations[body].radius+.03f);
            p=glm::vec3(glm::inverse(parent)*glm::vec4(w,1));
        } else p.y=.25f;
        return p;
    };
    glm::vec3 bottom=floor({-11,0,0}), top(-1.3f,5.6f,0);
    if(m.hatchOpen()>0) {
        // Retractable gangway: oriented along its true slope, with visible treads.
        glm::vec3 rampBottom=glm::mix(top,bottom,m.hatchOpen());
        glm::vec3 d=top-rampBottom;
        auto ramp=parent*at((rampBottom+top)*.5f)*glm::rotate(glm::mat4(1),std::atan2(d.y,d.x),glm::vec3(0,0,1));
        draw({0,glm::scale(glm::mat4(1),{glm::length(d),.12f,1.3f}),{.23f,.28f,.30f}},ramp);
        for(int i=0;i<25;++i) {
            glm::vec3 p=glm::mix(rampBottom,top,i/24.f);
            draw({0,glm::scale(at(p),{.10f,.055f,1.25f}),{.78f,.64f,.22f}},parent);
        }
    }
    for(int i=0;i<5;++i) {
        float t=home?m.crewBoardProgress(i):returning?Mission::boardingProgress(Mission::boardingDuration+3-m.departure,i):m.crewExitProgress(i);
        if((home||returning) && t>=1) continue;
        if(m.surfaceLanded() && t<=0) continue;
        glm::vec3 waiting=home?floor({-9.f+i*1.5f,0,6.f}):floor({-6.6f-i*.8f,0,2.5f+i*.65f});
        if(!home && i==2) waiting=floor({-5.8f,0,1.5f});
        float path=(home||returning)?t:1-t;
        glm::vec3 p,dir;
        if(path<.35f) { p=glm::mix(waiting,bottom,path/.35f); dir=bottom-waiting; p=floor(p); }
        else if(path<.92f) { p=glm::mix(bottom,top,(path-.35f)/.57f); dir=top-bottom; }
        else {p=glm::mix(top,glm::vec3(-.6f,5.6f,0),(path-.92f)/.08f);dir={1,0,0};}
        bool walking=t>0 && t<1;
        if(m.surfaceLanded()) dir=-dir;
        float yaw=walking?std::atan2(dir.x,dir.z):0;
        float time=home?m.boarding:returning?Mission::boardingDuration+3-m.departure:m.aftermath;
        auto frame=parent*at(p)*glm::rotate(glm::mat4(1),yaw,glm::vec3(0,1,0));
        float salute=m.surfaceLanded() && t>=1 ? .65f+.15f*std::sin(time*2+i) : 0;
        if(i==2 && m.surfaceLanded() && m.aftermath>Mission::flagPlantStart && m.aftermath<Mission::flagUnfurlStart) salute=.35f;
        // Gold crew member bends to plant the pole once everyone has disembarked.
        if(i==2 && m.surfaceLanded() && m.aftermath>Mission::flagPlantStart && m.aftermath<Mission::flagUnfurlStart)
            frame=frame*glm::rotate(glm::mat4(1),-.25f*std::sin((m.aftermath-Mission::flagPlantStart)*pi/3),glm::vec3(0,0,1));
        drawAstronaut(frame,i,m.crewAnimationTime*7*(walking?Mission::crewWalkSpeed:1.f)+i,walking?1.f:0.f,salute);
    }
}

void Scene::drawCockpit(const Mission &m,bool firstPerson) {
    auto parent=m.rocketModel();
    auto part=[&](glm::vec3 p,glm::vec3 size,glm::vec3 color,float glow=0) {
        draw({0,glm::scale(at(p),size),color,0,64,glow},parent);
    };
    glm::vec3 trim(.12f,.18f,.22f);
    // Structural window surround matches the physical aperture in material 22.
    for(float x:{-.99f,.99f}) part({x,15.01f,.84f},{.09f,1.35f,.10f},trim);
    for(float y:{14.36f,15.66f}) part({0,y,1.08f},{1.94f,.08f,.12f},trim);
    if(!firstPerson) return;
    part({0,14.95f,-1.12f},{1.9f,1.8f,.09f},trim);
    for(float x:{-1.12f,1.12f}) part({x,14.95f,0},{.09f,1.8f,1.9f},trim);
    if(m.crewAboard()==5 && !m.surfaceLanded()) for(int i=0;i<4;++i) {
        auto seat=parent*at({i%2?-.66f:.66f,14.03f,i<2?-.22f:-.78f})*glm::scale(glm::mat4(1),glm::vec3(.58f));
        drawAstronaut(seat,i,m.clock*.4f,0);
    }
    part({0,14.04f,0},{1.95f,.12f,1.9f},trim);
    part({0,14.56f,.77f},{1.75f,.17f,.46f},trim);
    for(float x:{-.55f,0.f,.55f}) {
        part({x,14.665f,.74f},{.42f,.025f,.25f},{.02f,.26f,.38f},.7f);
        for(int j=0;j<4;++j) part({x-.13f+j*.08f,14.685f,.73f},{.035f,.008f,.12f},{.1f,.9f,.8f},.7f);
    }
    for(float x:{-.31f,.31f}) {
        part({x,14.77f,.57f},{.05f,.24f,.05f},trim);
        draw({3,glm::scale(at({x,14.79f,.55f}),{.09f,.07f,.11f}),{.92f,.87f,.82f}},parent);
    }
}
} // namespace bk
