const consoleStatusTextEl = document.getElementById("console-status-text");
const consoleStatusIconEl = document.getElementById("console-status-icon");
const consoleEl = document.getElementById("console");
const commandStatusTextEl = document.getElementById("command-status-text");
const lockBottomCheckboxEl = document.getElementById("checkbox-lock-bottom");
const pauseCheckboxEl = document.getElementById("checkbox-pause");

const consoleUtils = {
    lastUpdateId: -1,
    connectionRetries: 0,
    isNewUpdate(logUpdateId) {
        if (logUpdateId === this.lastUpdateId)
            return false;
        this.lastUpdateId = logUpdateId;
        return true;
    },
    setConnectedIcon(connected) {
        consoleStatusIconEl.textContent = connected ? "🟢" : "🔴";
    },
    setStatusText(text) {
        consoleStatusTextEl.textContent = text;
    },
    setConsoleContent(text) {
        consoleEl.value = text;
        if (lockBottomCheckboxEl.checked)
            consoleEl.scrollTop = consoleEl.scrollHeight;
    },
    setConnected() {
        this.setConnectedIcon(true);
        this.setStatusText("Connected!")
    },
    setError(optionalText = null) {
        this.setConnectedIcon(false);
        this.setStatusText((optionalText ? optionalText + " | " : "") + "Connecting" + ".".repeat(1 + (this.connectionRetries++ % 3)))
    }
}

const commandUtils = {
    setCommandStatusText(text) {
        commandStatusTextEl.textContent = text;
        setTimeout(() => {
            commandStatusTextEl.textContent = "";
        }, 2000);
    },
    sendCommand(command) {
        requestFunctions
            .sendCommand(command)
            .then(response => {
                if (response.success) {
                    this.setCommandStatusText("Command received.");
                } else {
                    this.setCommandStatusText(`Failed to receive command, response status code: ${response.status}`);
                }
            });
    },
    onCommandInputKeyUp(event) {
        if (event.key !== 'Enter')
            return;
        this.sendCommand(event.target.value)
        event.target.value = "";
    },
}

const requestFunctions = {
    getConsoleUpdateId(timeout = 5000) {
        return new Promise((resolve, reject) => {
            const abortController = new AbortController();
            const signal = abortController.signal;

            const connectionTimeoutTimer = setTimeout(() => abortController.abort(), timeout)

            fetch("/logger?update-id", {signal})
                .then(response => {
                    clearTimeout(connectionTimeoutTimer);
                    if (response.ok)
                        return response.text();
                    throw new Error(`Failed to connect, response status code: ${response.status}`)
                })
                .then(updateIdString => {
                    resolve(parseInt(updateIdString));
                })
                .catch(error => {
                    if (error.name === "AbortError")
                        reject("Connection timed out...")
                    else
                        reject(error.message);
                })

        })
    },
    getConsoleFeed: () => {
        return fetch("/logger")
            .then(response => {
                if (response.ok)
                    return response.text();
                throw new Error(`Failed to fetch console feed, status code: ${response.status}`)
            });
    },
    sendCommand(commandString) {
        return fetch(`/debug-functions?cmd=${commandString}`)
            .then(response => {
                return {success: response.ok, status: response.status};
            })
    }
}

async function listenForConsoleUpdates() {
    while (true) {
        if (pauseCheckboxEl.checked) {
            consoleUtils.setStatusText("Paused");
            await new Promise(r => setTimeout(r, 1000));
            continue;
        }

        try {
            const updateId = await requestFunctions.getConsoleUpdateId();
            consoleUtils.setConnected();
            if (consoleUtils.isNewUpdate(updateId)) {
                handleConsoleUpdate();
            }
        } catch (errorMessage) {
            consoleUtils.setError(errorMessage);
        }
        await new Promise(r => setTimeout(r, 1000))
    }
}

function handleConsoleUpdate() {
    requestFunctions
        .getConsoleFeed()
        .then(responseText => consoleUtils.setConsoleContent(responseText))
}

function start() {
    void listenForConsoleUpdates();
}

start();
