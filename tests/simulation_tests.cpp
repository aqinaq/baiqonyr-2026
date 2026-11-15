#include "Simulation.hpp"
#include "Camera.hpp"
#include <iostream>
#include <stdexcept>
using namespace bk;
void require(bool value, const char *name) {
    if (!value)
        throw std::runtime_error(name);
    std::cout << "PASS " << name << '\n';
}
float distance(const glm::mat4 &a, const glm::mat4 &b) {
    float d = 0;
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            d += std::abs(a[c][r] - b[c][r]);
    return d;
}
int main() {
    try {
        Mission boarding;
        require(boarding.hatchOpen()==1 && boarding.crewAboard()==0,"five crew wait beside open hatch");
        boarding.launch(); boarding.speed=20; boarding.update(15/Mission::crewWalkSpeed);
        require(boarding.clock==-8 && boarding.crewAboard()==4 && !boarding.ignition(),"fast playback cannot ignite before fifth crew boards");
        boarding.paused=true; boarding.update(10);
        require(boarding.boarding==15/Mission::crewWalkSpeed,"pause freezes crew animation");
        boarding.paused=false; boarding.update(Mission::boardingWalkEnd-boarding.boarding);
        require(boarding.crewAboard()==5 && boarding.hatchOpen()==1,"all five enter before hatch starts closing");
        boarding.update(2);
        require(boarding.hatchOpen()==0 && boarding.clock==-8,"hatch seals before countdown");
        boarding.reset();
        require(boarding.boarding==0 && boarding.crewAboard()==0,"reset restores crew to ground");
        Camera zoomCamera;
        zoomCamera.zoom(2);
        require(zoomCamera.distance<65,"exterior zoom moves closer");
        zoomCamera.firstPerson=true;
        auto zoomDistance=zoomCamera.distance;
        zoomCamera.zoom(2);
        require(zoomCamera.cockpitFov<48 && zoomCamera.distance==zoomDistance,"cockpit zoom changes lens without leaving crew seat");
        zoomCamera.zoom(-1000);
        require(zoomCamera.cockpitFov==75,"cockpit zoom out stays bounded");
        zoomCamera.returnHome();
        require(zoomCamera.cockpitFov==48,"reset restores cockpit lens");
        Camera camera;
        require(camera.chase && !camera.firstPerson, "default camera follows rocket in third person");
        Mission cameraMission; cameraMission.launch(); cameraMission.update(Mission::boardingDuration); cameraMission.update(107.9f);
        camera.view(cameraMission,0);
        auto priorEye=camera.eye;
        cameraMission.update(.1f); camera.view(cameraMission,.016f);
        require(glm::length(camera.eye-priorEye)<1.f, "navigation handoff has no camera cut");
        for(int i=2;i<240;++i) camera.view(cameraMission,i/60.f);
        require(glm::length(camera.eye-glm::vec3(0,21500,10500))<1.f, "camera settles into navigation overview");
        camera.firstPerson=true;
        for(int i=240;i<480;++i) camera.view(cameraMission,i/60.f);
        require(glm::length(camera.eye-glm::vec3(cameraMission.rocket()*glm::vec4(0,15.15f*rocketScale,-.5f*rocketScale,1)))<1.f,
                "first person settles at crew eye level inside cabin");
        require(glm::dot(glm::normalize(camera.target-camera.eye),glm::vec3(0,0,1))>.99f,"crew view looks through forward cabin window");
        for(int col=0;col<4;++col) for(int row=0;row<4;++row)
            require(std::isfinite(camera.viewMatrix[col][row]), "crew camera matrix remains finite");
        require(std::abs(cameraMission.altitudeAt(100)-cameraMission.altitudeAt(99.999f))<.01f,
                "ascent decelerates continuously at navigation altitude");
        auto beforeReset=camera.eye;
        cameraMission.reset(); camera.returnHome(); camera.view(cameraMission,8.f);
        require(glm::length(camera.eye-beforeReset)<2.f,"return home starts without teleporting camera");
        Mission m;
        require(!m.running && m.altitude() == 0, "idle is grounded");
        m.launch(); m.update(Mission::boardingDuration);
        m.update(6);
        require(m.clock == -2 && m.ignition(), "ignition begins at T minus 2");
        m.paused = true;
        m.update(30);
        require(m.clock == -2, "pause preserves mission time");
        m.paused = false;
        m.update(12);
        require(m.altitude() > 0, "ascent after countdown");
        auto before = m.booster(0);
        m.separate();
        require(distance(before, m.booster(0)) < .001f, "separation preserves world transform");
        m.update(10);
        require(m.booster(0)[3].x > before[3].x, "detached boosters move outward");
        Mission stages;
        stages.launch(); stages.update(Mission::boardingDuration); stages.update(32.f);
        require(stages.separated && stages.boosterAge==0, "automatic release starts at T plus 24");
        auto released=stages.booster(0);
        stages.update(.001f);
        require(std::abs((stages.booster(0)[3].y-released[3].y)/.001f-26.4f)<.1f,
                "released stage inherits core upward velocity");
        stages.update(5.f);
        for(int i=0;i<4;++i) {
            auto falling=stages.booster(i);
            auto later=stages; later.update(.1f);
            require(later.booster(i)[3].y<falling[3].y && stages.boosterVisible(i),
                    "each separated stage falls under gravity before impact");
        }
        stages.paused=true; auto frozen=stages.booster(0); stages.update(10);
        require(distance(frozen,stages.booster(0))==0,"pause freezes detached stage motion");
        stages.paused=false; stages.update(10);
        for(int i=0;i<4;++i) require(!stages.boosterVisible(i),"impacted stages leave the scene");
        stages.update(100);
        auto age=stages.boosterAge; stages.update(1);
        require(stages.flight==Flight::Choosing && stages.boosterAge>age,
                "detached stage clock continues at navigation gate");
        stages.reset();
        require(stages.boosterAge==0 && stages.boosterVisible(0),"reset restores attached stages");
        Mission a, b;
        a.launch(); a.update(Mission::boardingDuration);
        b.launch(); b.update(Mission::boardingDuration);
        for (int i = 0; i < 600; i++)
            a.update(1.f / 60);
        for (int i = 0; i < 300; i++)
            b.update(1.f / 30);
        require(std::abs(a.altitude() - b.altitude()) < .01f,
                "time integration is frame-rate independent");
        m.reset();
        require(!m.separated && !m.running && m.clock == -8, "reset clears mission state");
        m.separate();
        require(!m.separated, "premature separation rejected");
        m.launch(); m.update(Mission::boardingDuration);
        m.update(33);
        require(m.separated, "automatic separation occurs");
        require(atmosphere(900) == 1 && atmosphere(-1) == 0, "atmosphere factor bounded");
        for (int i = 0; i < 4; i++)
            require(std::isfinite(m.booster(i)[3].y), "booster transform finite");
        Mission trip;
        require(!trip.selectDestination(0), "cannot choose destination before orbit");
        trip.launch(); trip.update(Mission::boardingDuration);
        trip.update(1000);
        require(trip.flight == Flight::Choosing && trip.clock == Mission::orbitTime, "ascent stops at navigation gate");
        auto holding = trip.position();
        trip.update(1000);
        require(glm::length(trip.position()-holding) == 0, "waiting does not drift indefinitely");
        require(!trip.selectDestination(-1) && !trip.selectDestination(destinationCount), "invalid destinations rejected");
        for (int i = 0; i < destinationCount; ++i) {
            trip.reset(); trip.launch(); trip.update(Mission::boardingDuration); trip.update(Mission::orbitTime+8);
            require(trip.selectDestination(i), "destination accepted at gate");
            for (std::size_t j=1; j<trip.route.size(); ++j)
                require(Mission::clearSegment(trip.route[j-1],trip.route[j]),
                        "entire route clears every planet and ring envelope");
            trip.paused = true; trip.update(50);
            require(trip.progress() == 0, "pause freezes transfer");
            trip.paused = false;
            trip.update(3);
            require(trip.progress()==0 && glm::length(trip.position()-holding)<.001f,
                    "departure camera handoff holds rocket before acceleration");
            trip.update(9);
            require(trip.progress() == .5f && glm::length(trip.position()-holding) > 1, "ship travels toward destination");
            require(!trip.selectDestination(0), "cannot change route mid-flight");
            if(trip.canLand()) {
                auto sample=trip;
                sample.travel=Mission::travelDuration*.75f;
                auto join=sample.position();
                sample.travel-=.001f; auto left=sample.position();
                sample.travel+=.002f; auto right=sample.position();
                require(glm::length(join-left)<.01f && glm::length(right-join)<.01f,
                        "cruise and landing join with zero-speed continuous position");
            }
            trip.update(9);
            require(trip.flight == (destinations[i].dangerous ? Flight::Destroyed : Flight::Arrived), "route resolves to correct outcome");
            if(i==5) require(trip.canLand() && trip.landingNormal().y>.99f,"Saturn lands over the pole clear of its rings");
            if (trip.canLand()) {
                require(std::abs(glm::length(trip.position()-destinations[i].center) - destinations[i].radius - .15f)<.002f,
                        "touchdown rests on the selected surface");
                glm::vec3 up = glm::normalize(glm::vec3(trip.rocket()[1]));
                require(glm::dot(up,trip.landingNormal())>.999f, "landed rocket stands upright");
            }
            require(trip.hatchOpen()==0 && trip.flagPlacement()==0 && trip.flagUnfurl()==0,
                    "touchdown starts with closed hatch and stowed flag");
            if(trip.canLand()) {
                auto crew=trip; crew.update(Mission::flagPlantStart-.1f);
                for(int member=0;member<5;++member) require(crew.crewExitProgress(member)==1,"all five reach surface before flag planting");
                require(crew.flagPlacement()==0 && !crew.canContinue(),"flag and departure wait for disembarkation");
            }
            auto ceremony=trip;
            ceremony.speed=20; ceremony.update(2.5f);
            require(ceremony.hatchOpen()==(ceremony.canLand()?1.f:0.f) && ceremony.flagPlacement()==0,
                    "hatch opens before flag deployment only on landed worlds");
            ceremony.paused=true; ceremony.update(20);
            require(ceremony.flagPlacement()==0,"pause freezes landing ceremony");
            ceremony.paused=false; ceremony.update(Mission::ceremonyDuration-2.5f);
            require(ceremony.flagPlacement()==(ceremony.canLand()?1.f:0.f) &&
                    ceremony.flagUnfurl()==(ceremony.canLand()?1.f:0.f),
                    "flag plants and unfurls in real time only after surface landing");
            ceremony.reset();
            require(ceremony.hatchOpen()==1 && ceremony.flagUnfurl()==0,"reset opens boarding hatch and stows flag");
            require(!trip.ignition(), "engines stop at terminal outcome");
            require(!trip.resultReady(), "outcome is not announced immediately");
            trip.speed=20;
            trip.update(2);
            require(!trip.resultReady(), "20x playback does not skip result delay");
            trip.paused=true; trip.update(20);
            require(!trip.resultReady(), "pause freezes result delay");
            trip.paused=false; trip.update(3);
            require(trip.resultReady(), "result appears after five real seconds");
            auto end = trip.position(); trip.update(100);
            require(glm::length(trip.position()-end) == 0, "terminal position stays bounded");
        }
        // Exercise every onward destination from every survivable first stop.
        for(int from : {0,1,5,6,7,8}) {
            for(int to=0;to<destinationCount;++to) {
                if(from==to) continue;
                Mission tour; tour.launch(); tour.update(Mission::boardingDuration); tour.update(108);
                require(tour.selectDestination(from),"first tour destination accepted");
                tour.update(21); tour.update(Mission::ceremonyDuration);
                require(tour.canContinue(),"successful arrival unlocks onward travel");
                auto oldPosition=tour.position(); auto oldPose=tour.rocket();
                require(!tour.selectDestination(from),"cannot travel to current destination");
                require(tour.selectDestination(to),"onward destination has a clear route");
                require(glm::length(tour.position()-oldPosition)<.002f && distance(tour.rocket(),oldPose)<.01f,
                        "onward departure preserves position and orientation");
                require(tour.aftermath==0 && tour.resultDelay==0,"new leg clears arrival timers");
                require(tour.plantedFlags.size()==(tour.surfaceDeparture?1u:0u),"completed flag remains at previous landing");
                for(std::size_t k=1;k<tour.route.size();++k)
                    require(Mission::clearSegment(tour.route[k-1],tour.route[k]),"onward cruise clears planets and rings");
                tour.paused=true; tour.update(10);
                require(tour.departure==(tour.surfaceDeparture?Mission::boardingDuration+3:3) && tour.progress()==0,"pause holds onward departure");
                tour.paused=false; tour.update(tour.surfaceDeparture?Mission::boardingDuration+3:3);
                require(glm::length(tour.position()-oldPosition)<.002f && tour.hatchOpen()==0,"hatch closes before liftoff");
                bool outsideBodies=true;
                for(int step=0;step<180;++step) {
                    tour.update(.1f);
                    for(int body=0;body<destinationCount;++body)
                        outsideBodies &= glm::length(tour.position()-destinations[body].center)>=destinations[body].radius;
                }
                require(outsideBodies,"multi-leg flight remains outside solid planets");
                tour.update(.01f);
                require(tour.flight==(destinations[to].dangerous?Flight::Destroyed:Flight::Arrived),
                        "onward leg reaches its intended outcome");
                if(tour.flight==Flight::Destroyed) require(!tour.selectDestination(from),"destroyed rocket cannot continue");
            }
        }
        Mission chain; chain.launch(); chain.update(Mission::boardingDuration); chain.update(108);
        for(int stop : {0,1,6,5,8,0}) {
            require(chain.selectDestination(stop),"six-stop journey accepts next destination");
            chain.update(21+Mission::boardingDuration); chain.update(Mission::ceremonyDuration);
        }
        require(chain.plantedFlags.size()==4,"flags persist across repeated surface and orbital visits");
        chain.reset();
        require(chain.plantedFlags.empty() && !chain.canContinue(),"return home clears tour state");
        trip.reset();
        require(trip.destination == -1 && trip.aftermath == 0 && trip.flight == Flight::Ascent, "restart clears space mission");
        return 0;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
