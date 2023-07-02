# Introduction
This is reference to create a esp captive portal
Anytime the esp32 connected to wifi AP, a portal appeared
After click on the link on the captive.html, user will be redirected to index.html

# Libraries

## You need to install these libraries to able to compile this project

+ ESPAsyncWebServer (Download zip file from https://github.com/me-no-dev/ESPAsyncWebServer, then add as Zip library on Arduino IDE)
+ AsyncTCP (Via Arduino IDE's Libraries Manager)


# Issue

1. Can not flash on Ubuntu: Please give it permission by execute this command:
```C++
    $ sudo chmod 777 /dev/tty***
```
    where *** is the id of your ESP32 connection

2. Can not find build_command.json
    Remove output/build_command.json then rebuild

