var accordion_containers = document.getElementsByClassName("accordion_container");
for (var i = 0; i < accordion_containers.length; i++) {
    accordion_containers[i].style.display = "none";
}

var aux_num = 0;
var j = 0;
const color_list = ["#dcbaf5", "white", "#F4BB44", "#99EDC3"];
setInterval(
    () => {
        var body = document.body;
        body.style.backgroundImage = "linear-gradient(to right, " + color_list[j % color_list.length] + ", " + aux_num + "%, " + color_list[(j+1) % color_list.length] + ")";
        aux_num++;
        if (aux_num == 99) {
            aux_num = 0;
            j--;
            if (j < 0) j = color_list.length - 1;
        }
    },
    100
);

function accordion_toggle(id) {
    var button = document.getElementById(id + "_button");
    var div = document.getElementById(id);
    if (!div) {
        console.log("Failed to toggle the accordion / div of id: " + id);
        return;
    }

    if (div.style.display === "none") {
        div.style.display = "block";
        button.innerHTML = button.innerHTML.slice(0, -1) + "▲" /*↑*/;
    } else {
        div.style.display = "none";
        button.innerHTML = button.innerHTML.slice(0, -1) + "▼" /*↓*/;
    }
}

function create_settings_file() {
    if (document.getElementById("wifi_name").value.toString().trim().length        == 0
        || document.getElementById("wifi_password").value.toString().trim().length == 0) {
        alert("Wifi's Name or Password is empty!");
        return;
    }

    var setting_elements = document.getElementsByClassName("setting_element");
    
    var settings_json = {};

    for (var i = 0; i < setting_elements.length; i++) {
        var name = setting_elements[i].id;
        var value = setting_elements[i].value;

        if (isNaN(value)) {
            settings_json[name] = value;
        } else {
            settings_json[name] = Number(value);
        }
    }

    try {
        const { ipcRenderer } = require("electron");
        ipcRenderer.invoke("save_dialog", JSON.stringify(settings_json));
    }
    catch (error) {
        alert(error);
    }
}
