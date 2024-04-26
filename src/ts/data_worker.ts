import * as dc from './data_client'

let port: any;

let myDataClient =  new dc.DataClient();

process.parentPort.once('message', (e: any) => {
    port = e.ports[0];
    port.on('message', async (e: any) => {
        console.log("got a messag in data worker - command is " + e.data.cmd);
        switch(e.data.cmd) {
            case "startDataAcquisition":
                const success = await myDataClient.startDataAcquisition();
                if(!success) {
                    port.postMessage(null);
                } else {
                    port.postMessage(myDataClient.clientMetadata);
                }
                break;
            case "stopDataAcquisition":
                myDataClient.stopDataAcquisition();
                break;
            case "getDataSinceTimestamp":
                const dataFromTs = myDataClient.getDataFromTimestamp(e.data.tsFrom, e.data.subsampling);
                port.postMessage(dataFromTs);
                break;
            case "getDataInRange":
                const dataRange = myDataClient.getDataInRange(e.data.tsStart, e.data.tsStop, e.data.subsampling);
                port.postMessage(dataRange);
                break;
            case "getHost":
                port.postMessage(myDataClient.server);
            default:
                console.log("invalid command " + e.data.cmd + " recieved in data worker");
                break;
        }
    });
    port.start();
  });
