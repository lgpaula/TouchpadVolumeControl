# Touchpad Volume Control

This project adds the ability to control the volume of a laptop via its touchpad.

## Key Features
  * Triple finger up and down gestures for volume control.
  * Triple tap to mute/unmute

## Prerequisites
Make sure these libraries are installed in your computer.
```bash
sudo dnf update
sudo dnf install gcc-c++
```
- Cmake
```bash
sudo dnf install cmake
```
- ALSA
```bash
sudo dnf install alsa-lib-devel
```
- libevdev
```bash
sudo dnf install libevdev-devel
```

## Installation
  ### Build method
   1. Download the latest release.
   2. Extract the files.
   3. Go into the extracted folder and run the commands:
   ```bash
   mkdir build && cd build && cmake ..
   ```
   4. Run the executable file with the following command to ensure the software runs with administrator privileges and on a clean environment:
  ```bash
  sudo -E ./TouchpadVolumeControl 
  ```
  ### AppImage method
   1. Download the AppImage file from the latest release.
   2. Enable execution permission in the program's properties.
   3. Run the executable file with the following command to ensure the software runs with administrator privileges and on a clean environment:
  ```bash
  sudo -E ./TouchpadVolumeControl-x86_64.AppImage 
  ```
## Disclaimers
  1. This project does not disregard the already existing shortcuts for the touchpad. Use it at your own discretion.
  2. Tested OS: Fedora 40

## Acknowledgments
This software uses the following third-party libraries:

- **alsa-lib**
    - Licensed under the GNU Lesser General Public License v2.1 (LGPL-2.1).
    - [License Text](licenses/LICENSE.LGPL-2.1)

- **libevdev**
    - Licensed under the MIT License.
    - [License Text](licenses/LICENSE.libevdev)
