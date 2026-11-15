#include "Mesh.hpp"
#include "Simulation.hpp"
#include <array>
#include <string>
namespace bk {
Geometry engineBell() {
    Geometry g; constexpr int bands=12,slices=48;
    for(int side=0;side<2;++side) {
        unsigned start=unsigned(g.vertices.size());
        for(int j=0;j<=bands;++j) {
            float t=float(j)/bands,r=.42f+.58f*(1-t)*(1-t)-(side?.08f:0.f);
            for(int i=0;i<=slices;++i) {
                float a=2*pi*i/slices;
                glm::vec3 normal=glm::normalize(glm::vec3(std::cos(a),1.16f*(1-t),std::sin(a)))*(side?-1.f:1.f);
                g.vertices.push_back({{r*std::cos(a),t-.5f,r*std::sin(a)},normal,{float(i)/slices,t}});
            }
        }
        for(int j=0;j<bands;++j) for(int i=0;i<slices;++i) {
            unsigned a=start+j*(slices+1)+i,b=a+slices+1;
            if(side) for(unsigned n:{a,a+1,b,a+1,b+1,b}) g.indices.push_back(n);
            else for(unsigned n:{a,b,a+1,a+1,b,b+1}) g.indices.push_back(n);
        }
    }
    return g;
}
Geometry sweptWings() {
    Geometry g;
    auto triangle=[&](glm::vec3 a,glm::vec3 b,glm::vec3 c,glm::vec3 n) {
        if(glm::dot(glm::cross(b-a,c-a),n)<0) std::swap(b,c);
        for(auto p:{a,b,c}) {g.indices.push_back(unsigned(g.vertices.size()));g.vertices.push_back({p,n,{p.x,p.z}});}
    };
    for(float side:{-1.f,1.f}) {
        std::array<glm::vec2,4> p={glm::vec2(0,2),{10*side,-1.8f},{9*side,-3.f},{0,-2}};
        for(float y:{-.5f,.5f}) {
            glm::vec3 n(0,y>0?1.f:-1.f,0);
            triangle({p[0].x,y,p[0].y},{p[1].x,y,p[1].y},{p[2].x,y,p[2].y},n);
            triangle({p[0].x,y,p[0].y},{p[2].x,y,p[2].y},{p[3].x,y,p[3].y},n);
        }
        for(int i=0;i<4;++i) {
            auto a=p[i],b=p[(i+1)%4]; glm::vec3 n=glm::normalize(glm::vec3(b.y-a.y,0,a.x-b.x))*side;
            triangle({a.x,-.5f,a.y},{b.x,-.5f,b.y},{a.x,.5f,a.y},n);
            triangle({b.x,-.5f,b.y},{b.x,.5f,b.y},{a.x,.5f,a.y},n);
        }
    }
    return g;
}
Geometry noseFairing() {
    Geometry g;
    constexpr int bands=24, slices=96;
    for(int j=0;j<=bands;++j) {
        float t=float(j)/bands, angle=t*pi*.5f;
        float r=std::cos(angle), y=std::sin(angle)*1.0f-.5f;
        for(int i=0;i<=slices;++i) {
            float a=2*pi*i/slices;
            g.vertices.push_back({{r*std::cos(a),y,r*std::sin(a)},
                glm::normalize(glm::vec3(r*std::cos(a),std::sin(angle),r*std::sin(a))),{float(i)/slices,t}});
        }
    }
    for(int j=0;j<bands;++j) for(int i=0;i<slices;++i) {
        unsigned a=j*(slices+1)+i,b=a+slices+1;
        for(unsigned n:{a,b,a+1,a+1,b,b+1}) g.indices.push_back(n);
    }
    return g;
}
static void block(Geometry &g,glm::vec3 p,glm::vec3 scale) {
    auto box=cube(); unsigned base=unsigned(g.vertices.size());
    for(auto v:box.vertices) {v.position=v.position*scale+p;g.vertices.push_back(v);}
    for(auto i:box.indices) g.indices.push_back(base+i);
}
static Geometry lettering(const std::string &text) {
    Geometry g;
    float x=0;
    for(char c:text) {
        std::array<int,7> rows{};
        switch(c) {
        case 'I': rows={31,4,4,4,4,4,31};break;
        case 'B': rows={30,17,17,30,17,17,30};break;
        case 'A': rows={14,17,17,31,17,17,17};break;
        case 'Q': rows={14,17,17,17,21,18,13};break;
        case 'O': rows={14,17,17,17,17,17,14};break;
        case 'N': rows={17,25,25,21,19,19,17};break;
        case 'Y': rows={17,17,10,4,4,4,4};break;
        case 'T': rows={31,4,4,4,4,4,4};break;
        case 'U': rows={17,17,17,17,17,17,14};break;
        case 'R': rows={30,17,17,30,20,18,17};break;
        default:x+=8;continue;
        }
        for(int row=0;row<7;++row) for(int col=0;col<5;++col)
            if(rows[row]&(1<<(4-col))) block(g,{x+col,6.f-row,0},{1,1,.7f});
        x+=6;
    }
    return g;
}
Geometry landmarkLetters() { return lettering("I BAIQONYR"); }
Geometry companyLetters() { return lettering("AITU"); }
Geometry landmarkHeart() {
    Geometry g; constexpr int n=96;
    std::array<glm::vec2,n> points;
    for(int i=0;i<n;++i) {
        float t=2*pi*i/n;
        points[i]={std::pow(std::sin(t),3.f),
            (13*std::cos(t)-5*std::cos(2*t)-2*std::cos(3*t)-std::cos(4*t))/16.f};
    }
    auto triangle=[&](glm::vec3 a,glm::vec3 b,glm::vec3 c,glm::vec3 normal){
        if(glm::dot(glm::cross(b-a,c-a),normal)<0) std::swap(b,c);
        for(auto p:{a,b,c}) {g.indices.push_back(unsigned(g.vertices.size()));g.vertices.push_back({p,normal,{0,0}});}
    };
    for(int i=0;i<n;++i) {
        auto a=points[i],b=points[(i+1)%n];
        for(float z:{-.18f,.18f}) triangle({0,0,z},{a.x,a.y,z},{b.x,b.y,z},{0,0,z>0?1.f:-1.f});
        glm::vec3 normal=glm::normalize(glm::vec3(a.y-b.y,b.x-a.x,0));
        triangle({a.x,a.y,-.18f},{b.x,b.y,-.18f},{a.x,a.y,.18f},normal);
        triangle({b.x,b.y,-.18f},{b.x,b.y,.18f},{a.x,a.y,.18f},normal);
    }
    return g;
}
}
