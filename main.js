console.log("your code is running")
const { app, BrowserWindow, ipcMain } = require("electron")
const { spawn } = require("child_process")
const path = require("path")

let win
let cpp

function createWindow() {
  win = new BrowserWindow({
    width: 900,
    height: 500,
    webPreferences: {
      nodeIntegration: true,
      contextIsolation: false
    }
  })

  win.webContents.openDevTools()
  win.loadFile("index.html")

  const exe = path.join(__dirname, "dijkstra.exe") 

  cpp = spawn(exe)

  cpp.stdout.on("data", (data) => {
    win.webContents.send("cpp-output", data.toString())
  })
}

ipcMain.on("send-to-cpp", (_, msg) => {
  cpp.stdin.write(msg)
})

app.whenReady().then(createWindow)
