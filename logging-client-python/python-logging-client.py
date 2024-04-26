import argparse
import logging
import signal

import grpc

import fs_data_logger_pb2
import fs_data_logger_pb2_grpc


class LoggingClient:
    should_stop = False

    def __init__(self, server_address: str, log_file_name: str):
        self.channel = grpc.insecure_channel(server_address)
        self.stub = fs_data_logger_pb2_grpc.DataAcquisitionServiceStub(self.channel)
        self.log_file_name = log_file_name
        signal.signal(signal.SIGTERM, self.set_stop_flag)
        signal.signal(signal.SIGINT, self.set_stop_flag)

    def start_logging(self) -> None:
        client: fs_data_logger_pb2.Client = self.stub.RegisterClient(fs_data_logger_pb2.ClientRegistrationRequest())
        data_stream = self.stub.StartDataAcquisition(
            fs_data_logger_pb2.StartDataAcquisitionRequest(client_id=client.client_id))
        with open(self.log_file_name, 'a') as f:
            f.write(f'Epoch_UTC,{client.signal_name}_{client.sensor_name}\n')
            while not self.should_stop:
                record: fs_data_logger_pb2.MeasurementRecord = next(data_stream)
                f.write(f'{record.timestamp},{record.flow}\n')

        self.stub.StopDataAcquisition(fs_data_logger_pb2.StopDataAcquisitionRequest(client_id=client.client_id))
        self.channel.close()

    def set_stop_flag(self, _signum, _frame) -> None:
        self.should_stop = True


def main() -> None:
    parser = argparse.ArgumentParser(description="Start flow sensor data logging from gRPC server")
    parser.add_argument('-s', '--server-addr', default="localhost:8080",
                        help='Address of the gRPC server')
    parser.add_argument('-f', '--log-file', default="flow-data.csv",
                        help="Name of the CSV log file")

    args = parser.parse_args()
    logging_client = LoggingClient(args.server_addr, args.log_file)
    logging_client.start_logging()


if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)
    main()
