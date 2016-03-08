#pragma once

#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>

class RecordableStats {
    public:

    protected:
        std::string getOutputFileName();

        virtual void writeToFile() = 0;

    private:
        static const std::string OUTPUT_FILE_PREFIX;
};