/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <string>
#include <thread>
#include <algorithm>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/strings/str_format.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server_builder.h>
#include "fs_data_logger.grpc.pb.h"
#include "sine_data_provider.h"
#include "sensor_data_provider.h"

ABSL_FLAG(uint16_t, port, 8080, "Server port for the service");
ABSL_FLAG(float, amp, 1, "Amplitude of the sine wave if sine wave data provider is used.");
ABSL_FLAG(float, freq, 1, "Frequency of the sine wave if sine wave data provider is used.");
ABSL_FLAG(bool, use_sensor, false, "Read data from sfm3019 sensor");


using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerWriter;
using fs_data_logger::DataAcquisitionService;
using fs_data_logger::MeasurementRecord;
using fs_data_logger::StartDataAcquisitionRequest;
using fs_data_logger::StopDataAcquisitionRequest;
using fs_data_logger::DataLoggerStatus;
using fs_data_logger::ClientRegistrationRequest;
using fs_data_logger::Client;
using fs_data_logger::QueryRuntimeStatisticsRequest;
using fs_data_logger::RuntimeStatistics;


bool contains(std::vector<uint32_t> vec, uint32_t key) {
  return std::find(vec.begin(), vec.end(), key) != vec.end();
}


class ServiceImpl final : public DataAcquisitionService::Service {
private:
    DataProvider *_dataProvider;
    uint32_t _clientCount = 0;
    std::vector<uint32_t> _clients;
public:
    explicit ServiceImpl(DataProvider *dataProvider) : _dataProvider(dataProvider) {
      std::cout << "creating new service instance" << std::endl;
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#pragma ide diagnostic ignored "ConstantConditionsOC"
#pragma ide diagnostic ignored "UnreachableCode"

    [[maybe_unused]] Status StartDataAcquisition(ServerContext *context, const StartDataAcquisitionRequest *request,
                                                 ServerWriter<MeasurementRecord> *writer) override {
      uint32_t client_id = request->client_id();

      if(!contains(_clients, client_id)) {
        std::cout << "Unknown client will not be served: " << client_id << std::endl;
        return Status::CANCELLED;
      }

      std::cout << "starting data acquisition" << std::endl;

      uint64_t currentTimestamp = 0;
      _dataProvider->start();
      while (contains(_clients, client_id)) {
        MeasurementRecord record = _dataProvider->read();
        uint64_t timestamp = record.timestamp();
        bool isNew = timestamp > currentTimestamp;
        if (isNew) {
          currentTimestamp = timestamp;
          writer->Write(record);
        }
      }
      std::cout << "data acquisition stopped" << std::endl;
      return Status::OK;
    }

#pragma clang diagnostic pop

    [[maybe_unused]] Status StopDataAcquisition(ServerContext *context, const StopDataAcquisitionRequest *request,
                                                DataLoggerStatus *response) override {
      uint32_t client_id = request->client_id();
      auto it = std::find(_clients.begin(), _clients.end(), client_id);
      if(it == _clients.end()) {
        std::cout << "Ignoring unknown client " << client_id << std::endl;
        return Status::CANCELLED;
      }
      _dataProvider->stop();
      _clients.erase(it);
      std::cout << "stopping data acquisition" << std::endl;
      return Status::OK;
    }

    [[maybe_unused]] Status RegisterClient(ServerContext *context, const ClientRegistrationRequest *request,
                                           Client *response) override {
      uint32_t client_id = ++_clientCount;
      _clients.push_back(client_id); // add client_id to active clients
      response->set_client_id(client_id);
      response->set_signal_name("Air");
      response->set_sensor_name("SFM3019");
      response->set_signal_unit("sl/min");
      response->set_sampling_frequency_milli_hz(2000000);
      return Status::OK;
    }

    [[maybe_unused]] Status QueryRuntimeStatistics(ServerContext* context, const QueryRuntimeStatisticsRequest* request,
                                                   RuntimeStatistics* response) override {
      uint32_t client_id = request->client_id();
      auto it = std::find(_clients.begin(), _clients.end(), client_id);
      if(it == _clients.end()) {
        std::cout << "Ignoring unknown client " << client_id << std::endl;
        return Status::CANCELLED;
      }
      *response = _dataProvider->getStatistics();
      return Status::OK;
    }
};

void RunServer(uint16_t port, bool use_sensor, 
               float amplitude, float frequency) {
  std::string server_address = absl::StrFormat("0.0.0.0:%d", port);

  DataProvider *dataProvider = nullptr;
  if (use_sensor){
    dataProvider = new SensorDataProvider();
  }
  else{
    dataProvider = new SineDataProvider(amplitude, frequency);
  }
  ServiceImpl service(dataProvider);

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shut down. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);
  RunServer(absl::GetFlag(FLAGS_port), 
            absl::GetFlag(FLAGS_use_sensor),
            absl::GetFlag(FLAGS_amp), absl::GetFlag(FLAGS_freq));
  return 0;
}
