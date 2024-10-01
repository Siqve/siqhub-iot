#### Run & Upload (Ask)

``$env = Read-Host 'Enter environment' ; if ($env) {pio run -t upload -e $env --upload-port com3}``


#### Run & Upload (WiFi) (Ask)

``$env = Read-Host 'Enter environment' ; if ($env) {pio run -t upload -e $env}; ``

#### Run & Upload (WiFi) (Desk)

``pio run -t upload -e desk``


#### Run, Upload & Monitor2
``pio run -t upload --upload-port com3 ; if ($?) {pio device monitor -p com3 -b 115200}``