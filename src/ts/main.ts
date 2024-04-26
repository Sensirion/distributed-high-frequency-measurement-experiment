import * as path from 'path'

import { app, utilityProcess, MessageChannelMain, BrowserWindow } from "electron";

const createWindow = async () => {
  const win = new BrowserWindow({
    width: 800,
    height: 600,
    autoHideMenuBar: true,
    webPreferences: {
      preload: path.join(__dirname, "preload.js"),
    },
  });

  
  var child = utilityProcess.fork(path.join(__dirname, 'data_worker.js'));
  const { port1, port2 } = new MessageChannelMain();
  child.postMessage(null, [port1]);

  win.once('ready-to-show', () => {
    win.webContents.postMessage('port', null, [port2])
  });
  
  win.loadFile("build/html/index.html");
};

app.whenReady().then(async () => {
  await createWindow();

  app.on("activate", () => {
    if (BrowserWindow.getAllWindows().length === 0) createWindow();
  });
});

app.on("window-all-closed", () => {
  if (process.platform !== "darwin") app.quit();
});

