#include <math.h>
#include <chrono>
#include "sensor_data_provider.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"
#include "sfm_sf06_i2c.h"
#include <thread>
#include <iostream>
#include <mutex>

// required to protect the creation of the measurement thread from race conditions
static std::mutex _mutex;

static std::thread* measurementThread;

static uint32_t maxReadoutTimeUs = 0;
static uint32_t minReadoutTimeUs = 10000;
static uint64_t sumReadoutTimeUs = 0;
static uint64_t numberOfSamplesSinceStart = 0;

uint64_t current_time_us(){
  auto current_time = std::chrono::system_clock::now();
  auto duration_in_seconds = std::chrono::duration<double>(current_time.time_since_epoch());
  double time = duration_in_seconds.count();
  return (uint64_t)(time * 1000000);
}

void measurementLoop(MeasurementRecord& lastMeasurement, 
                     bool& readMore){

    numberOfSamplesSinceStart = 0;
    maxReadoutTimeUs = 0;
    minReadoutTimeUs = 10000;
    sumReadoutTimeUs = 0;
    while( sfm_sf06_start_air_continuous_measurement() != NO_ERROR ){
      std::this_thread::sleep_for(std::chrono::duration<int64_t, std::micro>(1000));
      std::cout << "error starting measurement" << std::endl;
      sfm_sf06_stop_continuous_measurement();
    };
    // make sure that the sensor is ready!
    std::this_thread::sleep_for(std::chrono::duration<int64_t, std::micro>(20000));
    while(readMore){
      float flow = 0.0;
      float temperature = 0.0;
      uint16_t statusWord = 0u;
      auto timeStamp = current_time_us();
      uint16_t error = sfm_sf06_read_measurement_data(&flow, &temperature, &statusWord);
      if (error != NO_ERROR){
        std::cout << "error reading measurement values" << std::endl;
        continue;
      }
      auto timeAfterRead = current_time_us();
      lastMeasurement.set_timestamp(timeStamp);
      lastMeasurement.set_flow(flow);
      // compute some stats
      auto timeDelta = timeAfterRead - timeStamp;
      sumReadoutTimeUs += timeDelta;
      if (timeDelta > maxReadoutTimeUs){
        maxReadoutTimeUs = timeDelta;
      }
      if (timeDelta < minReadoutTimeUs){
        minReadoutTimeUs = timeDelta;
      }
      numberOfSamplesSinceStart++;
    }

}

SensorDataProvider::SensorDataProvider(): startCount(0){
  sensirion_i2c_hal_init();
  sfm_sf06_init(SFM3019_I2C_ADDR_2E);
}

void SensorDataProvider::start() {
  // start measurement
  std::lock_guard<decltype(_mutex)> lock(_mutex);
  startCount++;
  if (startCount == 1){
    isRunning = true;
    int16_t error = NO_ERROR;
    sfm_sf06_stop_continuous_measurement();
    measurementThread = new std::thread(measurementLoop,
                                        std::ref<MeasurementRecord>(lastMeasurement),
                                        std::ref<bool>(isRunning));
  }

}

void SensorDataProvider::stop() {
  // stop measurement
  std::lock_guard<decltype(_mutex)> lock(_mutex);
  startCount--;
  if(startCount == 0){
    isRunning = false;
    measurementThread->join();
    delete measurementThread;
    measurementThread = nullptr;
  }
}

MeasurementRecord SensorDataProvider::read() {
  return lastMeasurement;
}

RuntimeStatistics SensorDataProvider::getStatistics() {
  RuntimeStatistics statistics;
  statistics.set_max_readout_time_us(maxReadoutTimeUs);
  statistics.set_min_readout_time_us(minReadoutTimeUs);
  statistics.set_avg_readout_time_us(sumReadoutTimeUs/numberOfSamplesSinceStart);
  statistics.set_number_of_samples_since_start(numberOfSamplesSinceStart);
  return statistics;
}
