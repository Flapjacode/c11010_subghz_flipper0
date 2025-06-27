# c11010_subghz_flipper0
# CC1101 SubGHz Scanner

A Flipper Zero application for scanning and analyzing SubGHz signals using the CC1101 transceiver module.

## Features

- **Static Frequency Scanning**: Monitor specific frequencies for SubGHz signals
- **Dynamic Frequency Hopping**: Automatically scan across multiple frequency bands
- **Signal Analysis**: Detect and analyze various SubGHz protocols
- **Saved Signals**: Store and replay commonly used signals
- **Real-time RSSI**: Monitor signal strength during scanning
- **Multiple Protocol Support**: Support for AM, FM, and various modulation schemes

## Installation

1. Copy the entire `cc1101_subghz` folder to your Flipper Zero's `apps_data` directory
2. Compile and install using the Flipper Zero build system:
   ```bash
   ./fbt fap_cc1101_subghz
   ```
3. The app will appear in the GPIO section of your Flipper Zero

## Hardware Requirements

- Flipper Zero device
- CC1101 transceiver module (if not using internal radio)
- Appropriate antenna for target frequency bands

## Usage

### Main Menu
- **Static Scan**: Scan a specific frequency
- **Hopping Scan**: Automatically scan multiple frequencies
- **Saved Signals**: View and transmit saved signals
- **Settings**: Configure scanning parameters

### Static Scanning
1. Select "Static Scan" from the main menu
2. Press OK to start scanning at the default frequency (433.92 MHz)
3. Detected signals will appear in the list with frequency, protocol, and RSSI
4. Use Up/Down arrows to navigate detected signals
5. Press Back to stop scanning

### Hopping Scanning
1. Select "Hopping Scan" from the main menu
2. Configure frequency ranges and hop intervals
3. Press OK to start the hopping scan
4. The scanner will automatically cycle through frequencies
5. Detected signals are logged with timestamps

### Saved Signals
1. Select "Saved Signals" from the main menu
2. Choose a signal from the predefined list
3. View signal details and transmission parameters
4. Press OK to transmit the selected signal

## Configuration

### Supported Frequencies
- 315 MHz (North America)
- 433.92 MHz (Europe/Asia)  
- 868 MHz (Europe)
- 915 MHz (North America)

### Modulation Types
- Amplitude Shift Keying (ASK)
- On-Off Keying (OOK)
- Frequency Shift Keying (FSK)
- Gaussian FSK (GFSK)

### Protocols Detected
- Fixed code remotes
- Rolling code systems
- Weather stations
- Tire pressure monitors
- Generic ASK/OOK signals

## File Structure

```
cc1101_subghz/
├── application.fam                    # App metadata
├── cc1101_subghz_app.c               # Main application
├── cc1101_subghz_app.h               # Public header
├── cc1101_subghz_app_i.h             # Internal structures
├── scenes/
│   ├── cc1101_subghz_scene.c         # Scene management
│   ├── cc1101_subghz_scene.h         # Scene definitions
│   ├── cc1101_subghz_scene_config.h  # Scene configuration
│   ├── cc1101_subghz_scene_start.c   # Main menu scene
│   ├── cc1101_subghz_scene_static_scan.c    # Static scanning
│   ├── cc1101_subghz_scene_hopping_scan.c   # Frequency hopping
│   ├── cc1101_subghz_scene_saved_signals.c  # Saved signals menu
│   ├── cc1101_subghz_scene_settings.c       # Settings menu
│   └── cc1101_subghz_scene_transmit.c       # Signal transmission
├── views/
│   ├── cc1101_scanning_view.h        # Scanning view header
│   ├── cc1101_scanning_view.c        # Scanning view implementation
│   ├── cc1101_hopping_view.h         # Hopping view header
│   └── cc1101_hopping_view.c         # Hopping view implementation
├── helpers/
│   ├── cc1101_subghz_custom_event.h  # Custom events
│   └── cc1101_subghz_types.h         # Type definitions
├── cc1101_icon.png                   # App icon (10x10 px)
└── README.md                         # This file
```

## Development

### Building
```bash
# From Flipper Zero firmware root
./fbt fap_cc1101_subghz

# Or build all FAPs
./fbt faps
```

### Adding New Protocols
1. Add protocol definition to `cc1101_subghz_types.h`
2. Implement decoder in the scanning views
3. Add protocol-specific settings if needed
4. Update the protocol detection logic

### Adding New Scenes
1. Add scene enum to `cc1101_subghz_scene.h`
2. Create scene implementation file in `scenes/`
3. Add scene entry to scene configuration
4. Update scene navigation logic

## Troubleshooting

### Common Issues
- **No signals detected**: Check antenna connection and frequency settings
- **App crashes**: Ensure proper memory management in custom views
- **Poor reception**: Adjust antenna length for target frequency
- **Build errors**: Verify Flipper Zero firmware version compatibility

### Debug Mode
Enable debug logging by setting `FURI_DEBUG` in the build configuration.

## Legal Notice

This application is for educational and authorized testing purposes only. Users are responsible for complying with local regulations regarding radio frequency usage and transmission. Do not use this tool to intercept or interfere with signals you are not authorized to access.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes following the Flipper Zero coding standards
4. Test thoroughly on actual hardware
5. Submit a pull request with detailed description

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Credits

- Flipper Zero team for the excellent development platform
- CC1101 datasheet and reference implementations
- SubGHz protocol research community

## Changelog

### v1.0.0
- Initial release
- Basic scanning functionality
- Static frequency monitoring
- Saved signals management
- Settings configuration

### v1.1.0 (Planned)
- Enhanced protocol detection
- Custom frequency ranges
- Signal recording and playback
- Improved user interface
