# Usage

## Install dependencies

pip install -r requirements.txt

## Run the logger

python python-logging-client.py --server-addr localhost:8080 --log-file data.csv


# Development

Update generated files if protocol changes. 

```
python -m grpc_tools.protoc -I../proto --python_out=. --pyi_out=. --grpc_python_out=. ../proto/fs_data_logger.proto
```
