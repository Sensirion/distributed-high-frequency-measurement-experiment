#ifndef SERVER_SINE_DATA_PROVIDER_H
#define SERVER_SINE_DATA_PROVIDER_H

#include "data_provider.h"

using fs_data_logger::MeasurementRecord;
using fs_data_logger::RuntimeStatistics;

class SineDataProvider : public DataProvider {
public:
    SineDataProvider() : amplitude(1.0), frequency(1.0) {}

    SineDataProvider(float amplitude, float frequency) : amplitude(amplitude), frequency(frequency) {}

    void start() override;

    void stop() override;

    MeasurementRecord read() override;

    RuntimeStatistics getStatistics() override;

private:
    float amplitude;
    float frequency;
};


#endif //SERVER_SINE_DATA_PROVIDER_H
