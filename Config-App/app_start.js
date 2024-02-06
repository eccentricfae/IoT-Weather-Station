const { app, BrowserWindow } = require('electron/main')

const createWindow = () => {
    const win = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            nodeIntegration: true,
            contextIsolation: false,
            nodeIntegrationInWorker: true
        }
    })

    win.loadFile('main.html')
}

app.whenReady().then(() => {
    createWindow()

    const { ipcMain, dialog } = require('electron');

    try {
        ipcMain.handle("save_dialog", (e, data) => {
            const jetpack = require("fs-jetpack");
            var path = dialog.showOpenDialogSync({
                buttonLabel: "Save here",
                properties: ["openDirectory"],
            });
            //  TODO Change name! .txt or .json?
            var full_path = path + "/settings.txt";
            jetpack.write(full_path, data);
        })
    }
    catch (error) {
        alert(error);
    }

    app.on('activate', () => {
        if (BrowserWindow.getAllWindows().length === 0) {
            createWindow()
        }
    })
})

app.on('window-all-closed', () => {
    app.quit()
})