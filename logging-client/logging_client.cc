#include <iostream>
#include <memory>
#include <string>
#include <csignal>
#include <fstream>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

#include <grpcpp/grpcpp.h>
#include <thread>

#include "fs_data_logger.grpc.pb.h"

ABSL_FLAG(std::string, server_addr, "flow-demo-1.local:8080", "Server address");
ABSL_FLAG(std::string, log_file, "flow_data.csv", "CSV Log file prefix");

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using fs_data_logger::Client;
using fs_data_logger::ClientRegistrationRequest;
using fs_data_logger::DataAcquisitionService;
using fs_data_logger::DataLoggerStatus;
using fs_data_logger::MeasurementRecord;
using fs_data_logger::StartDataAcquisitionRequest;
using fs_data_logger::StopDataAcquisitionRequest;

typedef std::unique_ptr<grpc::ClientReader<MeasurementRecord>> Reader;

class LoggingClient
{
public:
  explicit LoggingClient(const std::shared_ptr<Channel> &channel)
      : stub_(DataAcquisitionService::NewStub(channel)) {}

  /*
   * Register client with the server to retrieve sensor info
   */
  int RegisterClient(Client &client)
  {
    // Register at server and receive client and sensor information.
    ClientRegistrationRequest request;

    ClientContext context;

    // Get client info from server
    Status status = stub_->RegisterClient(&context, request, &client);

    // Act upon its status.
    if (status.ok())
    {
      return 0;
    }
    else
    {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return -1;
    }
  }

  /*
   * Start Data Acquisition
   */
  void StartDataAcquisition(Client &client, const std::string &log_file_name)
  {
    StartDataAcquisitionRequest request;
    ClientContext context;
    MeasurementRecord record;

    // request reader for data
    request.set_client_id(client.client_id());
    Reader reader = stub_->StartDataAcquisition(&context, request);

    // Open log file
    std::ofstream log_file;

    // Open logfile in append mode
    log_file.open(log_file_name, std::ios_base::app);
    // write headers
    log_file << "Epoch_UTC," << client.signal_name() << "_" << client.sensor_name() << std::endl;

    while (!shouldStop && reader->Read(&record))
    {
      log_file << record.timestamp() << "," << record.flow() << std::endl;
    }
    log_file.close();
    StopDataAcquisition(client.client_id());
  }

  /*
   * Stop Data Acquisition
   */
  void StopDataAcquisition(uint32_t client_id)
  {
    StopDataAcquisitionRequest request;
    ClientContext context;
    DataLoggerStatus response;

    request.set_client_id(client_id);

    Status status = stub_->StopDataAcquisition(&context, request, &response);
    if (!status.ok())
    {
      std::cout << "Failed to stop data acquisition" << std::endl;
    }
  }

  void startLogging(Client &client, const std::string &log_file_name)
  {
    shouldStop = false;
    StartDataAcquisition(client, log_file_name);
  }

  void stopLogging()
  {
    shouldStop = true;
  }

private:
  std::unique_ptr<DataAcquisitionService::Stub> stub_;
  volatile bool shouldStop = false;
};

static LoggingClient *loggingClient;

void exiting(int signal)
{
  std::cout << "Logging stopped, exiting with signal " << signal << std::endl;
  loggingClient->stopLogging();
}

int main(int argc, char **argv)
{
  absl::ParseCommandLine(argc, argv);
  std::string server_addr = absl::GetFlag(FLAGS_server_addr);
  std::string log_file_name = absl::GetFlag(FLAGS_log_file);
  loggingClient = new LoggingClient(grpc::CreateChannel(server_addr, grpc::InsecureChannelCredentials()));
  static Client client;
  int status = loggingClient->RegisterClient(client);
  if (status < 0)
  {
    std::cout << "Error registering client, exiting." << std::endl;
    return -1;
  }
  std::cout << "Successfully registered with client ID: " << client.client_id() << std::endl;

  std::signal(SIGTERM, exiting);
  loggingClient->startLogging(client, log_file_name);

  return 0;
}
