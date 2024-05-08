# TouchpadVolumeControl

# (sudo dnf install libevdev-devel)
# (sudo ln -s /usr/include/libevdev-1.0/libevdev /usr/include/libevdev)

to run via terminal
# (g++ main.cpp -o touchpad pkg-config --cflags --libs libevdev)

to get devices
# cat /proc/bus/input/devices

for audio
# sudo dnf install alsa-lib-devel

gotta run with sudo -E to get the permissions to change the volume
# sudo -E ./project_name