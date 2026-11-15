#include "Scene.hpp"
namespace bk {
static glm::mat4 transform(glm::vec3 p,glm::vec3 scale) {
    return glm::scale(glm::translate(glm::mat4(1),p),scale);
}
void Scene::buildRocketDetails() {
    glm::vec3 metal(.35,.39,.41), dark(.055,.065,.075), white(.84,.86,.82), orange(.72,.23,.08);
    auto add=[&](int mesh,glm::vec3 p,glm::vec3 scale,glm::vec3 color,int material=0) {
        rocketParts.push_back({mesh,transform(p,scale),color,material,70,0});
    };
    // Stage joining flanges, access covers and the payload separation band.
    for(float y:{3.f,5.2f,9.f,13.6f,15.7f}) {
        float r=y>14?1.27f:1.185f;
        add(1,{0,y,0},{r,.07f,r},metal);
        for(int i=0;i<12;++i) {
            float a=i*pi/6;
            add(3,{r*std::cos(a),y,r*std::sin(a)},{.035f,.035f,.035f},dark);
        }
    }
    // External raceways and pipes avoid the port-side airlock.
    for(float a:{0.f,pi*.5f,pi*1.5f}) {
        glm::vec3 radial(std::cos(a),0,std::sin(a));
        add(1,radial*1.19f+glm::vec3(0,8.3f,0),{.055f,8.3f,.055f},metal);
        for(float y:{5.f,7.f,9.f,11.f})
            add(3,radial*1.2f+glm::vec3(0,y,0),{.10f,.07f,.10f},dark);
    }
    // Four engine bells, dark throats and metallic cooling rings.
    for(int i=0;i<4;++i) {
        float a=pi*.25f+i*pi*.5f;
        glm::vec3 p(.45f*std::cos(a),1.05f,.45f*std::sin(a));
        add(14,p,{.38f,1.25f,.38f},metal);
        add(1,p+glm::vec3(0,.48f,0),{.14f,.025f,.14f},dark);
        for(int ring=0;ring<4;++ring)
            add(1,p+glm::vec3(0,-.52f+ring*.13f,0),{.35f-ring*.035f,.035f,.35f-ring*.035f},metal*.65f);
    }
    // Corrugated interstage skirt, small stabilizers and stronger landing feet.
    for(int i=0;i<24;++i) {
        float a=i*pi/12;
        add(1,{1.19f*std::cos(a),3.5f,1.19f*std::sin(a)},{.018f,1.15f,.018f},orange*.7f);
    }
    for(int i=0;i<4;++i) {
        float a=i*pi*.5f;
        auto frame=glm::rotate(glm::mat4(1),a,glm::vec3(0,1,0));
        rocketParts.push_back({0,frame*transform({1.42f,2.7f,0},{.65f,1.4f,.10f}),white,0,24,0});
        rocketParts.push_back({0,frame*transform({1.5f,.10f,0},{.95f,.20f,.70f}),metal,0,40,0});
    }
    for(float y:{-2.5f,-.6f,1.8f,2.65f})
        boosterParts.push_back({1,transform({0,y,0},{.66f,.065f,.66f}),metal,0,60,0});
    for(int i=0;i<12;++i) {
        float a=i*pi/6;
        boosterParts.push_back({1,transform({.65f*std::cos(a),-1.8f,.65f*std::sin(a)},
            {.018f,1.45f,.018f}),orange*.65f,0,32,0});
    }
}
void Scene::buildLaunchDetails() {
    const glm::vec3 steel(.16,.22,.24), concrete(.47,.48,.43), white(.88,.88,.78),
        teal(.025,.43,.49), yellow(.94,.67,.12), dark(.06,.075,.085);
    auto add=[&](int mesh,glm::vec3 p,glm::vec3 size,glm::vec3 color,int mat=0,float glow=0.f) {
        ground.push_back({mesh,transform(p,size),color,mat,28,glow});
    };
    // Raised AITU rooftop signs on the nearby offices and main assembly hangar.
    for(float x:{20.f,29.f,38.f}) {
        add(0,{x,4.34f,-17.8f},{6.5f,.18f,.55f},steel);
        add(15,{x-2.75f,4.55f,-17.8f},{.25f,.32f,.45f},teal,0,.3f);
    }
    add(0,{-51,12.65f,-73},{26,.3f,1.1f},steel);
    add(15,{-62,13.2f,-73},{1.f,.95f,1.f},white,0,.2f);
    // Visitor landmark: merged extruded letters and a curved red heart sculpture.
    add(0,{26,.35f,28},{30,.7f,5},concrete,1);
    add(0,{26,.76f,28},{29,.12f,4.7f},dark);
    add(11,{12,1.25f,28},{.45f,.6f,.8f},white,0,.16f);
    add(12,{16.3f,3.05f,28},{1.7f,2.0f,1.8f},{.85f,.025f,.06f},0,.2f);
    for(float x:{12.f,19.f,26.f,33.f,40.f}) {
        add(1,{x,.95f,30},{.12f,.22f,.12f},steel);
        add(3,{x,1.08f,30},{.16f,.06f,.16f},{1,.74f,.32f},0,2);
    }
    // Launch apron expansion, flame-trench walls and a ring of safety markings.
    add(0,{0,-.12f,0},{58,.25f,52},concrete,1);
    for(float x:{-5.f,5.f}) add(0,{x,.6f,-15},{.8f,1.2f,24},steel);
    add(0,{0,.15f,-24},{9,.20f,9},dark);
    for(int i=0;i<32;++i) {
        float a=i*pi/16;
        auto t=glm::translate(glm::mat4(1),{19*std::cos(a),.13f,19*std::sin(a)})*
            glm::rotate(glm::mat4(1),-a,glm::vec3(0,1,0));
        ground.push_back({0,t*glm::scale(glm::mat4(1),{1.8f,.025f,.5f}),i%2?yellow:dark,0,8,0});
    }
    // Tall service mast sized for the enlarged launch vehicle, with ladders and platforms.
    for(float x:{-13.f,-10.f}) for(float z:{-4.f,-1.f})
        add(0,{x,16,z},{.32f,32,.32f},steel);
    for(int level=0;level<10;++level) {
        float y=2+level*3.f;
        add(0,{-11.5f,y,-2.5f},{4,.18f,4},steel);
        for(float z:{-4.4f,-.6f}) {
            add(0,{-11.5f,y+1,z},{4,.07f,.07f},yellow);
            for(float x:{-13.4f,-9.6f}) add(0,{x,y+.5f,z},{.07f,1,.07f},yellow);
        }
    }
    for(int rung=0;rung<80;++rung) add(0,{-13.6f,.4f+rung*.38f,-2.5f},{.08f,.05f,.8f},steel);
    for(float z:{-2.95f,-2.05f}) add(1,{-13.6f,15.5f,z},{.04f,31,.04f},steel);
    add(0,{-7.5f,23,-2.5f},{7,.3f,1.8f},steel);
    add(3,{-11.5f,32.3f,-2.5f},{.22f,.22f,.22f},{1,.08f,.02f},0,3);
    // An airfield east of the launch complex, with threshold stripes and edge lights.
    add(0,{145,.04f,-15},{25,.2f,260},dark);
    add(0,{112,.03f,-55},{43,.18f,75},concrete,1);
    for(int i=0;i<24;++i) {
        float z=-135+i*10.f;
        add(0,{145,.155f,z},{.35f,.025f,4.5f},white);
        for(float x:{132.f,158.f}) add(3,{x,.3f,z},{.14f,.15f,.14f},{.55f,.75f,1},0,2);
    }
    for(float z:{-132.f,102.f}) for(int i=0;i<6;++i)
        add(0,{136.f+i*3.6f,.16f,z},{1.3f,.025f,12},white);
    for(float x:{132.7f,157.3f}) add(0,{x,.16f,-15},{.18f,.025f,240},white);
    // Tower, radar and ground support outside the runway clearance zone.
    add(0,{100,6,-112},{8,12,8},concrete,1);
    add(0,{100,12.7f,-112},{12,2.6f,12},steel);
    add(0,{100,13,-105.95f},{10,1.2f,.08f},{.045f,.25f,.32f},0,.2f);
    add(0,{100,14.2f,-112},{13,.3f,13},white);
    add(1,{100,16,-112},{.12f,3.5f,.12f},steel);
    add(0,{100,17.5f,-112},{4,.6f,.3f},teal);
    // Two parked transport aircraft, with tapered noses, wings, nacelles and gear.
    for(auto position:{glm::vec3(104,0,-40),glm::vec3(108,0,-76)}) {
        auto root=glm::translate(glm::mat4(1),position);
        auto part=[&](int mesh,glm::vec3 p,glm::vec3 size,glm::vec3 color,float rotation=0.f) {
            ground.push_back({mesh,root*glm::translate(glm::mat4(1),p)*
                glm::rotate(glm::mat4(1),rotation,glm::vec3(1,0,0))*glm::scale(glm::mat4(1),size),color,0,70,0});
        };
        part(3,{0,2.8f,0},{1.5f,1.5f,10},white);
        part(3,{0,3.4f,6.5f},{1.05f,.7f,2.2f},{.035f,.12f,.18f});
        part(13,{0,2.5f,-.8f},{1,.22f,1},white);
        part(0,{0,3.1f,-7.2f},{8,.15f,2.3f},teal);
        part(0,{0,4.6f,-7.5f},{.18f,4.5f,2.2f},teal);
        for(float x:{-4.4f,4.4f}) {
            part(1,{x,1.8f,1},{.65f,3.2f,.65f},steel,pi*.5f);
            part(1,{x,1.8f,2.62f},{.48f,.04f,.48f},dark,pi*.5f);
        }
        for(float x:{-1.f,1.f}) for(float z:{-2.f,5.5f}) {
            part(1,{x,1,z},{.07f,1.5f,.07f},steel);
            part(3,{x,.45f,z},{.28f,.45f,.45f},dark);
        }
        for(int i=0;i<9;++i) for(float x:{-1.48f,1.48f})
            part(0,{x,3.15f,-5.f+i*1.15f},{.035f,.32f,.45f},{.035f,.15f,.20f});
    }
    // Drainage channels, equipment cabinets, bollards and inspection paving.
    for(float x:{-30.f,30.f}) for(int i=0;i<15;++i) {
        float z=-23+i*3.f;
        add(0,{x,.03f,z},{.55f,.09f,2.7f},dark);
        for(int j=0;j<4;++j) add(0,{x,.09f,z-.9f+j*.6f},{.58f,.025f,.08f},steel);
    }
    for(int i=0;i<5;++i) {
        float x=12+i*3.f;
        add(0,{x,1.1f,-30},{1.7f,2.2f,.8f},steel);
        add(0,{x,1.5f,-29.57f},{.75f,.5f,.04f},dark);
        add(3,{x+.55f,1.65f,-29.52f},{.06f,.06f,.04f},{.2f,1,.5f},0,1);
    }
}
}
