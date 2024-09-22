## TouchpadVolumeControl

This project adds the ability to control the volume of a laptop via its touchpad.
This project does not disregard the already existing shortcuts for the touchpad. Use it at your own discretion.

Tested OS: Fedora 38

# Requirements
  - ALSA
    ```bash
    sudo dnf install alsa-lib-devel
    ```
  - libevdev
    ```bash
    sudo dnf install libevdev-devel
    sudo ln -s /usr/include/libevdev-1.0/libevdev /usr/include/libevdev
    ```

