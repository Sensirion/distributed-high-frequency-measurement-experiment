import { IpcRendererEvent, contextBridge, ipcRenderer } from "electron";

let workerPort: MessagePort;

ipcRenderer.on("port", (e: IpcRendererEvent) => {
  workerPort = e.ports[0];
  console.log("preload received port over ipcRenderer");
});

export type ContextBridgeApi = {
  // returns null if data acquisition was successfully started, 
  // a object holding metadata according to Client message in fs_data_logger protobuf description
  // if the data acquisition started successfully
  startDataAcquisition: () => Promise<any>;
  stopDataAcquisition: () => void;
  getDataSinceTimestamp: (
    tsFrom: number,
    subSamplingRatio: number
  ) => Promise<[number[], number[]]>;
  getDataInRange: (
    tsStart: number,
    tsStop: number,
    subSamplingRatio: number
  ) => Promise<[number[], number[]]>;
  getHost: () => Promise<string>;
};

const api: ContextBridgeApi = {
  startDataAcquisition: () => startDataAcquisition(),
  stopDataAcquisition: () => stopDataAcquisition(),
  getDataSinceTimestamp: (ts: number, subSamplingRatio: number) =>
    getDataSinceTimestamp(ts, subSamplingRatio),
  getDataInRange: (tsStart: number, tsStop: number, subSamplingRatio: number) =>
    getDataInRange(tsStart, tsStop, subSamplingRatio),
  getHost: () => getHost(),
};

// expose api to renderer.js
contextBridge.exposeInMainWorld("api", api);

const getDataSinceTimestamp = (tsFrom: number, subSamplingRatio: number) =>
  new Promise<[number[], number[]]>((res, rej) => {
    workerPort.onmessage = (e) => {
      res(e.data);
    };
    workerPort.postMessage({
      cmd: "getDataSinceTimestamp",
      tsFrom: tsFrom,
      subsampling: subSamplingRatio,
    });
  });

const getDataInRange = (
  tsStart: number,
  tsStop: number,
  subSamplingRatio: number
) =>
  new Promise<[number[], number[]]>((res, rej) => {
    workerPort.onmessage = (e) => {
      res(e.data);
    };
    workerPort.postMessage({
      cmd: "getDataInRange",
      tsStart: tsStart,
      tsStop: tsStop,
      subsampling: subSamplingRatio,
    });
  });

 const startDataAcquisition = () =>
    new Promise<any>((res, rej) => {
      workerPort.onmessage = (e) => {
        res(e.data);
      };
      workerPort.postMessage({
        cmd: "startDataAcquisition",
      });
    });

  const stopDataAcquisition = (): void => {
    workerPort.postMessage({
      cmd: "stopDataAcquisition",
    });
  }

  const getHost = () =>
  new Promise<any>((res, rej) => {
    workerPort.onmessage = (e) => {
      res(e.data);
    };
    workerPort.postMessage({
      cmd: "getHost",
    });
  });
  