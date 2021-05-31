# Arctis 7 Headset Status

This project is an attempt to understand the comminication that occurs between the arctis 7 wireless headset and my computer. Armed with this knowledge I want to make the headset report back to the computer when it is on or off, and  automatically switch audio output to my speakers and back.

This should also be cross platform between windows and linux, as I have a dual boot machine and want it to be useful in both operating systems.

This application makes use of libusb-1.0 to handle USB communication.

## Building

The included makefile (designed for use on linux) should mean that after cloning this repo, building is as simple as running `make release`. use `make help` for additional assistance.

Note that this you will require the gcc/g++ compiler and [libusb](https://libusb.info/) installed on your system.

### Building for Windows

If you wish to build the windows executable you will need mingw, and to specify TARGET=WINDOWS in the makefile. You can probably convert the makefile and `build/build.mk` for use on windows with some tweaks, or use wsl, but this has not been tested.

The appropriate dlls will be required from [libusb](https://libusb.info/) downloads. Place the `include/libusb-1.0` folder in `target/windows/include/` and copy `MinGW64/dll/libusb-1.0.dll` and `MinGW64/dll/libusb-1.0.dll.a` into `target/windows/lib`.

Then, in order to run the application, make sure that you have `libusb-1.0.dll`, `libgcc_s_seh-1.dll`, `libstdc++-6.dll`, and `libwinpthread-1.dll` (included with mingw) in the same directory as the executable or on your path (or you could statically compile the files, which would admittedly be easier than this).

In addition, use of libusb on windows requires the WinUSB driver to be loaded for the device being targeted. You can read more from the [libusb github](https://github.com/libusb/libusb/wiki/Windows#How_to_use_libusb_on_Windows). Following the recommended steps means using [Zadig](https://zadig.akeo.ie/) and installing the WinUSB driver for the `Steelseris Arctis 7 (Interface 5)`. It should be painless, but it does mean that your device will **no longer be compatible with the steelseries engine**

## ToDo:

Below is the list of features that I want to complete, and the issues that I should resolve as I go along

### Required

- [x] Poll for headset on/off status within headset class
- [x] (HACK) Switch audio output (linux) based on headset status
- [ ] Switch audio output (linux) based on headset status
- [x] Compile for windows
- [ ] Switch audio output (windows) based on headset status

### Extending

- [ ] Static compilation option with makefile
- [ ] Add tray icon to report device status and exit program
- [ ] Improve error logging, ie: stack traces, logs, etc.
- [ ] Understand what all the contoll transfers the steelseries engine software send and their replies mean
- [ ] Figure out how to read the headset battery, and report in tray icon menu (maybe even a 25% battery warning, if that's possible)

