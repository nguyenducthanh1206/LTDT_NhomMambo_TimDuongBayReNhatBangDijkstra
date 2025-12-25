const { contextBridge, ipcRenderer } = require("electron")

contextBridge.exposeInMainWorld("api", {
  send: (msg) => ipcRenderer.send("send-to-cpp", msg),
  onData: (callback) =>
    ipcRenderer.on("cpp-output", (event, data) => callback(data))
})
