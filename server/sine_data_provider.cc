#include <cmath>
#include <chrono>

#include "sine_data_provider.h"
#include <thread>


void SineDataProvider::start() {
  // start measurement
}

void SineDataProvider::stop() {
  // stop measurement
  const auto start = std::chrono::high_resolution_clock::now();
}

MeasurementRecord SineDataProvider::read() {
  auto current_time = std::chrono::system_clock::now();
  auto duration_in_seconds = std::chrono::duration<double>(current_time.time_since_epoch());

  double time = duration_in_seconds.count();

  double value = amplitude * sin(2.0 * M_PI * time * frequency);
  MeasurementRecord record;
  record.set_timestamp((uint64_t)(time * 1000000));
  record.set_flow((float)value);
  std::this_thread::sleep_for(std::chrono::duration<int64_t, std::micro>(250));
  return record;
}

RuntimeStatistics SineDataProvider::getStatistics(){
  // having runtime statistics is not meaningful for the sine wave data provider
  return RuntimeStatistics();  
}
