#include "imbued_mushroom.h"

using namespace boost;
using namespace boost::assign;
using namespace GW2LIB;
using namespace std;

vector<GW2LIB::Vector3> ImbuedMushroom::POSITIONS = list_of
(Vector3(8225.500000f, 1006.359985f, -187.535004f))
(Vector3(6883.430176f, -357.003998f, -98.141998f))
(Vector3(7596.319824f, -1415.569946f, -31.462299f))
(Vector3(8856.080078f, -527.715027f, 15.985100f));

ImbuedMushroom::ImbuedMushroom() : elapsedMilliseconds(0) {

}

void ImbuedMushroom::updateState(int elapsedMilliseconds) {
    this->elapsedMilliseconds = elapsedMilliseconds;
}

void ImbuedMushroom::draw() {
    Vector3 pos = POSITIONS[getNextMushroomIndex()];

    float x, y;
    if (WorldToScreen(pos, &x, &y)) {
        stringstream ss;
        int nextSpawnTime = getNextSpawnTime();

        if (nextSpawnTime < 5000) {
            ss << format("%.1f") % (float(nextSpawnTime) / 1e3);
        }
        else {
            ss << format("%d") % (nextSpawnTime / 1000);
        }
        AssistDrawer::get().drawStreamToWindow(ss, x, y);
    }
}

int ImbuedMushroom::getNextSpawnTime() {
    return SPAWN_INTERVAL_MS - (elapsedMilliseconds % SPAWN_INTERVAL_MS);
}

int ImbuedMushroom::getNextMushroomIndex() {
    return (elapsedMilliseconds / SPAWN_INTERVAL_MS + 1) % 4;
}
