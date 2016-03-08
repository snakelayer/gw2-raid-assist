#include "recordable_stats.h"

using namespace std;

const string RecordableStats::OUTPUT_FILE_PREFIX = "GW2-RaidAssist-";

string RecordableStats::getOutputFileName() {
    string localTime = boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());
    return OUTPUT_FILE_PREFIX + localTime + ".txt";
}