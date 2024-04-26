import {loadPackageDefinition, credentials} from "@grpc/grpc-js";
import { loadSync } from "@grpc/proto-loader";


export class DataClient {
    readonly CACHE_SIZE = 2 * 10 * 60 * 1000; // maximum 10 minutes data are saved when sampling with 2 kHz
    readonly CACHE_DELETE_CHUNK = this.CACHE_SIZE / 10; 
    readonly proto_path = "./proto/fs_data_logger.proto";
    readonly proto_loading_options = {
        keepCase: true,
        longs: String,
        enums: String,
        defaults: true,
        oneofs: true,
    };
    readonly server = 'flow-demo-1.local:8080';
    //readonly server = 'localhost:50000';

    readonly dataLoggerClient: any;
    clientId: number = -1;
    // type is message Client in fs_data_logger protobuf description
    clientMetadata: any = null;

    timestamps: number[] = [];
    flowValues: number[] = [];

    constructor() {
        console.debug("creating data client");
        let packageDefinition = loadSync(this.proto_path, this.proto_loading_options);
        const fs_data_logger_proto: any = loadPackageDefinition(packageDefinition).fs_data_logger;
        this.dataLoggerClient = new fs_data_logger_proto.DataAcquisitionService(this.server, credentials.createInsecure());
    }

    // returns true if it could connect to the server, false otherwise
    async registerToServer() : Promise<boolean> {
        return new Promise<boolean>((res, rej) => {
            this.dataLoggerClient.registerClient({}, (error: any, client: any) => {
                if (error) {
                    console.log("error during registerClient: ");
                    console.error(error);
                    res(false);
                }
                this.clientId = client.client_id;
                this.clientMetadata = client;
                console.log("Register client with ID: ", this.clientId, " and metadata " , this.clientMetadata);
                res(true);
            });
        });
    }

    // you need to register and get a new client id before each call to startDataAcquisition
    async startDataAcquisition() : Promise<boolean>  {
        let registered = await this.registerToServer();
        if(!registered) {
            return false;
        }
        console.log("start data aquisition with client id ", this.clientId);
        this.timestamps = [];
        this.flowValues = [];
        try {
            var stream = this.dataLoggerClient.startDataAcquisition({client_id: this.clientId});
        } catch (error) {
            console.log("could not create data stream");
            console.error(error);
            return false;
        }
        console.log("started data aquisition");
        stream.on("data", (dataRecord: any) => {
            this.timestamps = this.timestamps.concat(Number(dataRecord.timestamp));
            this.flowValues = this.flowValues.concat(dataRecord.flow);
            this.cleanupData();
        });
        stream.on("error", (err: any) => {
            console.log(err);
        });
        stream.on('status', (status: any) => {
            if (status.metadata) {
                console.log('Received metadata');
                console.log(status.metadata);
            }
        });
        stream.on('end', () => {
            console.log('stream end signal received');
        });
        return true;
    }

    stopDataAcquisition() {
        this.dataLoggerClient.stopDataAcquisition({client_id: this.clientId}, (error: any, status: any) => {
            console.log("handler for stop aquisition")
            if (error != null) {
                console.log("error during StopDataAcquisition");
                console.error(error);
                return;
            }
            console.log(status.status);
        });
    }

    // tsFrom: data which is newer then the tsFrom is returend, not including tsFrom
    // subsampling: fraction between 0 and 1
    getDataFromTimestamp(tsFrom: number, subsampling: number) : number[][] {
        if(this.timestamps.length === 0) {
            return [[],[]];
        }
        let indexTsFrom = this.timestamps.findIndex((ts) => ts == tsFrom);
        if (indexTsFrom == -1) {
            console.log("fallback to send all data");
            indexTsFrom = 0;
        }
        let theData = [this.timestamps.slice(indexTsFrom+1, -1), 
            this.flowValues.slice(indexTsFrom+1, -1)];

        let subsampledData = subsample(theData, subsampling);
        console.log("returning data from timestamp with \t" + theData[0].length + " datapoints \nsubsampled with fraction " + subsampling + " to\t" + subsampledData[0].length + " datapoints");
        return subsampledData;
    }

    getDataInRange(tsStart: number, tsStop: number, subsampling: number) : number[][] {
        if(this.timestamps.length === 0) {
            return [[],[]];
        }
        let indexTsStart = this.timestamps.findIndex((ts) => ts >= tsStart);
        let indexTsStop = this.timestamps.findIndex((ts) => ts >= tsStop);
        if (indexTsStart == -1) {
            console.log("fallback to use first available datapoint as start");
            indexTsStart = 0;
        }
        if (indexTsStop == -1) {
            console.log("fallback to use last available datapoint as stop");
            indexTsStop = this.timestamps.length-1;
        }
        let theData = [this.timestamps.slice(indexTsStart, indexTsStop), 
            this.flowValues.slice(indexTsStart, indexTsStop)];
        

        let subsampledData = subsample(theData, subsampling);
        console.log("returning data range with \t\t" + theData[0].length + " datapoints \nsubsampled with fraction " + subsampling + " to\t" + subsampledData[0].length + " datapoints");
        return subsampledData;
    }

    cleanupData() {
        if(this.timestamps.length >= this.CACHE_SIZE) {
            console.log("before cleanup there are ", this.timestamps.length, " datapoints");
            this.timestamps.splice(0, this.CACHE_DELETE_CHUNK);
            this.flowValues.splice(0, this.CACHE_DELETE_CHUNK);
            console.log("after cleanup there are ", this.timestamps.length, " datapoints");
        }
    }
}


// written with kind assistance of ChatGPT
function subsample<T, X>(data: T[][], fraction: number): T[][] {
    if (fraction <= 0 || fraction > 1) {
        throw new Error("Fraction must be between 0 and 1");
    }

    const numSlices = Math.ceil(data[0].length * fraction);
    const sliceSize = Math.ceil(data[0].length / numSlices);
    const subsampledData: T[][] = [[], []];

    for (let i = 0; i < numSlices; i++) {
        const startIndex = i * sliceSize;
        // Ensure we don't exceed the array length
        const elementIndex = Math.min(startIndex, data[0].length - 1);
        // Always take the first element of the slice
        subsampledData[0].push(data[0][elementIndex]);
        subsampledData[1].push(data[1][elementIndex]);
    }

    return subsampledData;
}