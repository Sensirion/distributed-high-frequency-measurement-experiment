from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class Status(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = ()
    OK: _ClassVar[Status]
    ERROR: _ClassVar[Status]
OK: Status
ERROR: Status

class ClientRegistrationRequest(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class StartDataAcquisitionRequest(_message.Message):
    __slots__ = ("client_id",)
    CLIENT_ID_FIELD_NUMBER: _ClassVar[int]
    client_id: int
    def __init__(self, client_id: _Optional[int] = ...) -> None: ...

class StopDataAcquisitionRequest(_message.Message):
    __slots__ = ("client_id",)
    CLIENT_ID_FIELD_NUMBER: _ClassVar[int]
    client_id: int
    def __init__(self, client_id: _Optional[int] = ...) -> None: ...

class QueryRuntimeStatisticsRequest(_message.Message):
    __slots__ = ("client_id",)
    CLIENT_ID_FIELD_NUMBER: _ClassVar[int]
    client_id: int
    def __init__(self, client_id: _Optional[int] = ...) -> None: ...

class MeasurementRecord(_message.Message):
    __slots__ = ("timestamp", "flow")
    TIMESTAMP_FIELD_NUMBER: _ClassVar[int]
    FLOW_FIELD_NUMBER: _ClassVar[int]
    timestamp: int
    flow: float
    def __init__(self, timestamp: _Optional[int] = ..., flow: _Optional[float] = ...) -> None: ...

class DataLoggerStatus(_message.Message):
    __slots__ = ("status",)
    STATUS_FIELD_NUMBER: _ClassVar[int]
    status: Status
    def __init__(self, status: _Optional[_Union[Status, str]] = ...) -> None: ...

class Client(_message.Message):
    __slots__ = ("client_id", "sensor_name", "signal_name", "signal_unit", "sampling_frequency_milli_hz")
    CLIENT_ID_FIELD_NUMBER: _ClassVar[int]
    SENSOR_NAME_FIELD_NUMBER: _ClassVar[int]
    SIGNAL_NAME_FIELD_NUMBER: _ClassVar[int]
    SIGNAL_UNIT_FIELD_NUMBER: _ClassVar[int]
    SAMPLING_FREQUENCY_MILLI_HZ_FIELD_NUMBER: _ClassVar[int]
    client_id: int
    sensor_name: str
    signal_name: str
    signal_unit: str
    sampling_frequency_milli_hz: int
    def __init__(self, client_id: _Optional[int] = ..., sensor_name: _Optional[str] = ..., signal_name: _Optional[str] = ..., signal_unit: _Optional[str] = ..., sampling_frequency_milli_hz: _Optional[int] = ...) -> None: ...

class RuntimeStatistics(_message.Message):
    __slots__ = ("max_readout_time_us", "min_readout_time_us", "avg_readout_time_us", "number_of_samples_since_start")
    MAX_READOUT_TIME_US_FIELD_NUMBER: _ClassVar[int]
    MIN_READOUT_TIME_US_FIELD_NUMBER: _ClassVar[int]
    AVG_READOUT_TIME_US_FIELD_NUMBER: _ClassVar[int]
    NUMBER_OF_SAMPLES_SINCE_START_FIELD_NUMBER: _ClassVar[int]
    max_readout_time_us: int
    min_readout_time_us: int
    avg_readout_time_us: float
    number_of_samples_since_start: int
    def __init__(self, max_readout_time_us: _Optional[int] = ..., min_readout_time_us: _Optional[int] = ..., avg_readout_time_us: _Optional[float] = ..., number_of_samples_since_start: _Optional[int] = ...) -> None: ...
