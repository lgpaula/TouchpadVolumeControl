# Touchpad Volume Control

This project adds the ability to control the volume of a laptop via its touchpad.

## Key Features
  * Triple finger up and down gestures for volume control.
  * Triple tap to mute/unmute

## Prerequisites
Make sure these libraries are installed in your computer.
  * ALSA
```bash
sudo dnf install alsa-lib-devel
```
  * libevdev
```bash
sudo dnf install libevdev-devel
```
Create a symbolic link for libevdev
```bash
sudo ln -s /usr/include/libevdev-1.0/libevdev /usr/include/libevdev
```

## Installation
  1. Download the latest release.
  2. Extract the files.
  3. Go into the extracted folder and run the commads:
  ```bash
  mkdir build && cd build && cmake ..
  ```
  4. Run the executable file to launch the application. Ensure the software is running with administrator privileges to enable it to control the volume.

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