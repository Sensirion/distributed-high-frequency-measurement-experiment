#ifndef DATA_PROVIDER_H
#define DATA_PROVIDER_H

#include "fs_data_logger.pb.h"
using fs_data_logger::MeasurementRecord;
using fs_data_logger::RuntimeStatistics;

class DataProvider {
public:
    virtual ~DataProvider() = default;

    virtual void start() = 0;

    virtual void stop() = 0;

    virtual MeasurementRecord read() = 0;

    virtual RuntimeStatistics getStatistics() = 0;
};

#endif // DATA_PROVIDER_H