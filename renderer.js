const { ipcRenderer } = require('electron');

// 1. Initial City Data
let cities = ["Ha Noi", "Ho Chi Minh", "Bangkok", "Tokyo", "Singapore", "Kuala Lumpur", "Sydney", "New York"];

// 2. UI Refresh Logic
function setupDropdowns() {
    const ids = ["add_from", "add_to", "comp_from", "comp_to"];
    ids.forEach(id => {
        const select = document.getElementById(id);
        if (!select) return; // Guard against missing elements
        select.innerHTML = ""; 
        cities.forEach((name, index) => {
            let opt = document.createElement("option");
            opt.value = index;
            opt.innerHTML = name;
            select.appendChild(opt);
        });
    });
}

// 3. System Logging Helper (Replaces Alert to prevent freezing)
function logToSystem(message, isError = false) {
    const output = document.getElementById("output");
    const prefix = isError ? "[ERROR] " : "[SYSTEM] ";
    output.value += `${prefix}${message}\n`;
    output.scrollTop = output.scrollHeight;
}

// 4. Add Flight Logic (Fixed to ensure inputs stay enabled)
function addFlight() {
    const pBox = document.getElementById("add_price");
    const lBox = document.getElementById("add_luggage");
    
    // Explicitly re-enable in case of previous errors
    pBox.disabled = false;
    lBox.disabled = false;

    const a = document.getElementById("add_from").value;
    const b = document.getElementById("add_to").value;
    const p = parseFloat(pBox.value);
    const l = parseFloat(lBox.value);

    // Hard check for 0 or negative values
    if (isNaN(p) || p <= 0 || isNaN(l) || l <= 0) {
        logToSystem("Flight price and luggage must be greater than 0!", true);
        pBox.classList.add("input-error");
        lBox.classList.add("input-error");
        return;
    }

    if (a === b) {
        logToSystem("Please select two different cities.", true);
        return;
    }
    
    pBox.classList.remove("input-error");
    lBox.classList.remove("input-error");

    ipcRenderer.send("send-to-cpp", `4\n${a}\n${b}\n${p}\n${l}\n`);
    
    // Success message logic will come back from C++ to the 'cpp-output' listener
    pBox.value = "";
    lBox.value = "";
}

// 5. Compute Cost Logic
function computeCost() {
    const s = document.getElementById("comp_from").value;
    const t = document.getElementById("comp_to").value;
    const ticketClass = document.getElementById("ticketClass").value;
    const kgInput = document.getElementById("comp_kg");
    const kg = parseFloat(kgInput.value);
    if (s === t) {
        logToSystem("Start and destination cities must be different.", true);
        return;
    }
    if (isNaN(kg) || kg <= 0) {
        logToSystem("Luggage weight must be greater than 0 kg to compute cost.", true);
        kgInput.classList.add("input-error");
        kgInput.focus();
        return;
    }
    kgInput.classList.remove("input-error");
    ipcRenderer.send("send-to-cpp", `5\n${s}\n${t}\n${ticketClass}\n${kg}\n`);
}

// 6. Manage Cities
function addCity() {
    const input = document.getElementById("new_city_name");
    const cityName = input.value.trim();
    if (!cityName) {
        logToSystem("Please enter a city name.", true);
        return;
    }

    ipcRenderer.send("send-to-cpp", `2\n${cityName}\n`);
    
    cities.push(cityName);
    setupDropdowns();
    input.value = "";
    logToSystem(`City '${cityName}' added to the list.`);
}

function showCityList() {
    const list = cities.map((city, index) => `${index}: ${city}`).join(" | ");
    logToSystem(`Current Cities: ${list}`);
}

// 7. IPC Listener
ipcRenderer.on("cpp-output", (event, data) => {
    const output = document.getElementById("output");
    output.value += data;
    output.scrollTop = output.scrollHeight;
});

// Initialize
setupDropdowns();