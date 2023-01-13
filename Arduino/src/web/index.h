#include "Arduino.h"

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>LED Hue Controller</title>
    <style>
        /* CSS RESET */
        *, *::before, *::after {box-sizing: border-box; margin: 0; line-height: calc(1em + 0.5rem);
            overflow-wrap: break-word;
        }
        html, body {font-family: Tahoma ,serif; cursor: default; height: 100%}
        input, button, textarea, select { font: inherit; font-size: 1rem;}
        /* --------- */

        .main-conatiner {
            margin: auto;
            max-width: 600px;
            display: flex;
            align-items: center;
            flex-direction: column;
        }
        .main-conatiner__title {
            color: #8F95D3;
            font-size: 3rem;
            margin: 0.7rem 0;
        }

        .modes-container {
        }

        .modes-container__title {
            text-align: center;
            color: #555358;
            font-size: 2rem;
        }
        .modes-container__mode-list {
            padding-top: 1rem;
            border-top: 0.1rem solid #555358;
        }
        .modes-container__list-item {
            color: #555358;
            font-size: 2rem;
            text-decoration: none;
            margin: 0 1rem;
            padding: 0 0.6rem;
        }
        .modes-container__list-item--active {
            color: #59CD90;
        }
        .effect-container {
            width: 100%;
            padding: 1rem;
            margin-top: 1rem;
            border-radius: 1rem;
            box-shadow: 0 2rem 2rem -3.8rem inset;
            display: grid;
            overflow: hidden;
        }

        .static-effect-container {
            transition: all 0.3s ease-in;
            grid-column: 1;
            grid-row: 1;
        }

        .fade-effect-container {
            transition: all 0.2s ease-in;
            grid-column: 1;
            grid-row: 1;
            transform: translateX(120%);
        }


        .static-effect-container:not(.effect-container--active) {
            transform: translateX(-120%);
        }
        .fade-effect-container.effect-container--active {
            transform: translateY(0);
        }

        #demobox {
            background-color: #cfc ;
            width: 1rem;
            height: 1rem;
            padding: 10px;
        }
    </style>
    <script>
        const activeContainer = 'effect-container--active';
        let activeMode = 0;
        function onColorChange(input) {
            console.log(input.value);
        }

        function onModePress(id) {
            updateModeRequest(id);
        }

        function setActiveModeContainerClass(id) {
            if (id === activeMode)
                return;
            activeMode = id;
            document.querySelector('.' + activeContainer).classList.remove(activeContainer);
            document.getElementById('effect-container-' + id).classList.add(activeContainer);
        }

        //Get and update mode
        function getModeRequest() {
            const xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function() {
                if (this.readyState === 4 && this.status === 200) {
                    const mode = this.responseText;
                    if (!isNumeric(mode))
                        return;
                    console.log(mode);
                    setActiveModeContainerClass(parseInt(mode));
                }
            };
            xhr.open("GET", "/mode", true);
            xhr.send();
        }
        getModeRequest();
        setInterval(getModeRequest, 1000);

        //Send an update to mode
        function updateModeRequest(id) {
            let xhr = new XMLHttpRequest();
            xhr.open("GET", "/update?mode=" + id, true);
            xhr.send();
        }

        function isNumeric(value) {
            return /^\d+$/.test(value);
        }
    </script>
</head>
<body>
    <div class="main-conatiner">
        <p class="main-conatiner__title">LED Hue Controller</p>
        <div class="modes-container">
            <p class="modes-container__title">Mode</p>
            <div class="modes-container__mode-list">
                <a class="modes-container__list-item modes-container__list-item--active"  onclick="onModePress(0)">STATIC</a>
                <a class="modes-container__list-item" onclick="onModePress(1)">FADE</a>
            </div>
        </div>
        <div id="effect-container" class="effect-container">
            <div id="effect-container-0" class="static-effect-container effect-container--active">
                <div id="demobox"></div>
                <div>
                    <input type="color" id="head" name="head" oninput="onColorChange(this)" value="#e66465">
                    <label for="head">Custom</label>
                </div>
            </div>
            <div id="effect-container-1" class="fade-effect-container">
                <p>asdasd</p>
            </div>
        </div>
    </div>
</body>
</html>
)rawliteral";