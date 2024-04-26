# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
"""Client and server classes corresponding to protobuf-defined services."""
import grpc

import fs_data_logger_pb2 as fs__data__logger__pb2


class DataAcquisitionServiceStub(object):
    """A simple data acquisition service.

    The service can be queried about the data it can acquire and it can be started and stopped
    """

    def __init__(self, channel):
        """Constructor.

        Args:
            channel: A grpc.Channel.
        """
        self.RegisterClient = channel.unary_unary(
                '/fs_data_logger.DataAcquisitionService/RegisterClient',
                request_serializer=fs__data__logger__pb2.ClientRegistrationRequest.SerializeToString,
                response_deserializer=fs__data__logger__pb2.Client.FromString,
                )
        self.StartDataAcquisition = channel.unary_stream(
                '/fs_data_logger.DataAcquisitionService/StartDataAcquisition',
                request_serializer=fs__data__logger__pb2.StartDataAcquisitionRequest.SerializeToString,
                response_deserializer=fs__data__logger__pb2.MeasurementRecord.FromString,
                )
        self.StopDataAcquisition = channel.unary_unary(
                '/fs_data_logger.DataAcquisitionService/StopDataAcquisition',
                request_serializer=fs__data__logger__pb2.StopDataAcquisitionRequest.SerializeToString,
                response_deserializer=fs__data__logger__pb2.DataLoggerStatus.FromString,
                )
        self.QueryRuntimeStatistics = channel.unary_unary(
                '/fs_data_logger.DataAcquisitionService/QueryRuntimeStatistics',
                request_serializer=fs__data__logger__pb2.QueryRuntimeStatisticsRequest.SerializeToString,
                response_deserializer=fs__data__logger__pb2.RuntimeStatistics.FromString,
                )


class DataAcquisitionServiceServicer(object):
    """A simple data acquisition service.

    The service can be queried about the data it can acquire and it can be started and stopped
    """

    def RegisterClient(self, request, context):
        """Register a new client for the data acquisition service

        The returned client ID must be supplied with each start/stop request
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def StartDataAcquisition(self, request, context):
        """A server-to-client streaming RPC.

        Obtains the Measurement data from the flow sensor.  Results are
        streamed rather than returned at once

        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def StopDataAcquisition(self, request, context):
        """Stop the running data acquisition
        returns true if the data acquisition is stopped
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def QueryRuntimeStatistics(self, request, context):
        """Query some runtime statistics since the last start of the sensor
        For the simulation the statistics will be empty
        """
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')


def add_DataAcquisitionServiceServicer_to_server(servicer, server):
    rpc_method_handlers = {
            'RegisterClient': grpc.unary_unary_rpc_method_handler(
                    servicer.RegisterClient,
                    request_deserializer=fs__data__logger__pb2.ClientRegistrationRequest.FromString,
                    response_serializer=fs__data__logger__pb2.Client.SerializeToString,
            ),
            'StartDataAcquisition': grpc.unary_stream_rpc_method_handler(
                    servicer.StartDataAcquisition,
                    request_deserializer=fs__data__logger__pb2.StartDataAcquisitionRequest.FromString,
                    response_serializer=fs__data__logger__pb2.MeasurementRecord.SerializeToString,
            ),
            'StopDataAcquisition': grpc.unary_unary_rpc_method_handler(
                    servicer.StopDataAcquisition,
                    request_deserializer=fs__data__logger__pb2.StopDataAcquisitionRequest.FromString,
                    response_serializer=fs__data__logger__pb2.DataLoggerStatus.SerializeToString,
            ),
            'QueryRuntimeStatistics': grpc.unary_unary_rpc_method_handler(
                    servicer.QueryRuntimeStatistics,
                    request_deserializer=fs__data__logger__pb2.QueryRuntimeStatisticsRequest.FromString,
                    response_serializer=fs__data__logger__pb2.RuntimeStatistics.SerializeToString,
            ),
    }
    generic_handler = grpc.method_handlers_generic_handler(
            'fs_data_logger.DataAcquisitionService', rpc_method_handlers)
    server.add_generic_rpc_handlers((generic_handler,))


 # This class is part of an EXPERIMENTAL API.
class DataAcquisitionService(object):
    """A simple data acquisition service.

    The service can be queried about the data it can acquire and it can be started and stopped
    """

    @staticmethod
    def RegisterClient(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/fs_data_logger.DataAcquisitionService/RegisterClient',
            fs__data__logger__pb2.ClientRegistrationRequest.SerializeToString,
            fs__data__logger__pb2.Client.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def StartDataAcquisition(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_stream(request, target, '/fs_data_logger.DataAcquisitionService/StartDataAcquisition',
            fs__data__logger__pb2.StartDataAcquisitionRequest.SerializeToString,
            fs__data__logger__pb2.MeasurementRecord.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def StopDataAcquisition(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/fs_data_logger.DataAcquisitionService/StopDataAcquisition',
            fs__data__logger__pb2.StopDataAcquisitionRequest.SerializeToString,
            fs__data__logger__pb2.DataLoggerStatus.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)

    @staticmethod
    def QueryRuntimeStatistics(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(request, target, '/fs_data_logger.DataAcquisitionService/QueryRuntimeStatistics',
            fs__data__logger__pb2.QueryRuntimeStatisticsRequest.SerializeToString,
            fs__data__logger__pb2.RuntimeStatistics.FromString,
            options, channel_credentials,
            insecure, call_credentials, compression, wait_for_ready, timeout, metadata)
