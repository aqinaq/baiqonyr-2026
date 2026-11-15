#version 330 core
in vec2 uv;out vec4 fragColor;
uniform mat4 invVP;uniform vec3 eye;uniform float altitude,night,time;
float hash(vec3 p){p=fract(p*.1031);p+=dot(p,p.yzx+33.33);return fract((p.x+p.y)*p.z);}
float cloudNoise(vec2 p){
 vec2 i=floor(p),f=fract(p);f=f*f*(3.0-2.0*f);
 return mix(mix(hash(vec3(i,7)),hash(vec3(i+vec2(1,0),7)),f.x),
            mix(hash(vec3(i+vec2(0,1),7)),hash(vec3(i+vec2(1,1),7)),f.x),f.y);
}
float galacticNoise(vec3 p){
 vec3 i=floor(p),f=fract(p); f=f*f*(3.0-2.0*f);
 return mix(mix(mix(hash(i),hash(i+vec3(1,0,0)),f.x),
                mix(hash(i+vec3(0,1,0)),hash(i+vec3(1,1,0)),f.x),f.y),
            mix(mix(hash(i+vec3(0,0,1)),hash(i+vec3(1,0,1)),f.x),
                mix(hash(i+vec3(0,1,1)),hash(i+vec3(1,1,1)),f.x),f.y),f.z);
}
vec3 starLayer(vec3 ray,float scale,float threshold){
 // Cube projection avoids the stretched star streaks of latitude/longitude poles.
 vec3 a=abs(ray); vec2 uv; float face;
 if(a.x>=a.y && a.x>=a.z){uv=ray.yz/a.x;face=ray.x>0.0?0.0:1.0;}
 else if(a.y>=a.z){uv=ray.xz/a.y;face=ray.y>0.0?2.0:3.0;}
 else {uv=ray.xy/a.z;face=ray.z>0.0?4.0:5.0;}
 vec2 p=(uv*.5+.5)*scale,cell=floor(p)+vec2(face*scale,0);

 float seed=hash(vec3(cell,scale));
 vec2 center=vec2(hash(vec3(cell,12)),hash(vec3(cell,31)))*.6+.2;
 float radius=mix(.035,.12,pow(seed,16.0));
 float aa=max(length(fwidth(p))*.5,.025);
 float point=1.0-smoothstep(max(0.0,radius-aa),radius+aa,length(fract(p)-center));
 vec3 temperature=mix(vec3(1.0,.73,.48),vec3(.65,.80,1.0),hash(vec3(cell,51)));
 return temperature*point*step(threshold,seed)*mix(.3,1.4,pow(seed,24.0))*min(1.0,radius/aa);
}
void main(){
 vec4 w=invVP*vec4(uv*2-1,1,1);vec3 ray=normalize(w.xyz/w.w-eye);
 float h=pow(max(ray.y+.06,0.0),.5);
 vec3 day=mix(vec3(.91,.61,.34),vec3(.18,.34,.52),clamp(h,0,1));
 vec3 dark=mix(vec3(.032,.045,.09),vec3(.003,.007,.024),clamp(h,0,1));
 vec3 col=mix(day,dark,night*.9);
 col=mix(col,vec3(.0008,.0011,.0022),altitude);
 vec3 sun=normalize(vec3(-.6,.22,-.8));float s=max(dot(ray,sun),0);
 col+=vec3(1,.7,.35)*pow(s,1400)*(1-altitude)*(1-night);
 col+=vec3(.28,.12,.04)*pow(s,16)*(1-altitude)*(1-night);
 // Thin wind-stretched clouds on a distant horizontal layer, fading into space.
 if(ray.y>.025){
  vec2 p=ray.xz/(ray.y+.12)*vec2(1.6,3.8)+vec2(time*.002,0);
  float cloud=cloudNoise(p)*.57+cloudNoise(p*2.13)*.28+cloudNoise(p*4.3)*.15;
  float density=smoothstep(.49,.70,cloud)*smoothstep(.025,.16,ray.y);
  vec3 cloudColor=mix(vec3(.94,.78,.59),vec3(.11,.14,.20),night);
  col=mix(col,cloudColor,density*.62*(1-altitude));
 }
 // Fixed celestial directions: no screen-space noise or animated star flicker.
 float visibility=smoothstep(.25,.85,max(altitude,night));
 vec3 galacticNormal=normalize(vec3(-.32,1.0,.18));
 float latitude=dot(ray,galacticNormal);
 float band=exp(-latitude*latitude*85.0);
 float clouds=galacticNoise(ray*7.0)*.50+galacticNoise(ray*19.0)*.30+galacticNoise(ray*53.0)*.20;
 float rift=smoothstep(.42,.68,galacticNoise(ray*26.0+vec3(19)))*exp(-latitude*latitude*360.0);
 vec3 milkyWay=mix(vec3(.025,.031,.048),vec3(.105,.085,.062),clouds);
 col+=milkyWay*band*clouds*(1.0-rift*.85)*visibility;
 col+=(starLayer(ray,450.0,.987)+starLayer(ray,180.0,.995)*1.8)*visibility;
 fragColor=vec4(col,1);
}
