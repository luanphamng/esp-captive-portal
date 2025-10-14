# ESP32 Captive Portal

A customizable captive portal implementation for ESP32 using AsyncWebServer and LittleFS. When devices connect to the ESP32's WiFi access point, they are automatically presented with a captive portal interface.

## Features

- ✨ Automatic captive portal detection on client devices
- 🌐 Custom landing page (`captive.html`)
- 📱 Responsive web interface
- 💾 File system based content using LittleFS
- ⚡ Async web server for better performance

## Prerequisites

- [PlatformIO](https://platformio.org/install) installed
- ESP32 development board
- USB cable
- [Visual Studio Code](https://code.visualstudio.com/) (recommended)

## Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/luanphamng/esp-captive-portal.git
   cd esp-captive-portal
   ```

2. Open the project in PlatformIO:
   - Open VS Code
   - Install PlatformIO extension if you haven't
   - Click "Open Project" and select the esp-captive-portal folder

## Building and Uploading

1. Build the project:
   ```bash
   pio run
   ```

2. Upload the filesystem:
   ```bash
   pio run -t uploadfs
   ```

3. Upload the firmware:
   ```bash
   pio run -t upload
   ```

## Project Structure

```
esp-captive-portal/
├── src/                 # Source files
│   └── main.cpp        # Main application code
├── data/               # Web content files
│   ├── captive.html    # Initial captive portal page
│   ├── index.html      # Main application page
│   ├── notifications.css
│   ├── notifications.js
│   └── assets/         # Static assets
│       ├── css/
│       └── js/
└── platformio.ini      # PlatformIO configuration
```

## Configuration

The project uses `platformio.ini` for build configuration:
- LittleFS for file system
- ESP32 board settings
- Required libraries
- Debug settings

## Troubleshooting

### Permission Issues on Linux/Ubuntu
If you can't flash on Ubuntu/Linux, set the correct permissions:
```bash
sudo chmod 777 /dev/ttyUSB0  # Replace ttyUSB0 with your port
```

### Build Issues
If you encounter "build_command.json" errors:
1. Delete the `output/build_command.json` file
2. Rebuild the project

### Common Issues
- If the portal doesn't appear, try clearing your device's network cache
- Make sure all files are properly uploaded to LittleFS
- Check serial monitor for debugging information

## Contributing

1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a new Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details

## Acknowledgments

- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [AsyncTCP](https://github.com/me-no-dev/AsyncTCP)
- ESP32 Community

