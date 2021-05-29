# Arctis 7 Headset Status

This project is an attempt to understand the comminication that occurs between the arctis 7 wireless headset and my computer. Armed with this knowledge I want to make the headset report back to the computer when it is on or off, and  automatically switch audio output to my speakers and back.

This should also be cross platform between windows and linux, as I have a dual boot machine and want it to be useful in both operating systems.

This application makes use of libusb-1.0 to handle USB communication.

## Building

The included makefile should  mean that after cloning this repo, building is as simple as running `make release`. use `make help` for additional assistance.

Note that this you will require the gcc/g++ compiler and [libusb](https://libusb.info/) installed on your system.

## ToDo:

Below is the list of features that I want to complete, and the issues that I should resolve as I go along

### Required

- [x] Poll for headset on/off status within headset class
- [ ] Switch audio output (linux) based on headset status
- [ ] compile on windows
- [ ] Switch audio output (windows) based on headset status

### Extending

- [ ] Add tray icon to report device status and exit program
- [ ] Improve error logging, ie: stack traces, logs, etc.
- [ ] Understand what all the contoll transfers the steelseries engine software send and their replies mean
- [ ] Figure out how to read the headset battery, and report in tray icon menu (maybe even a 25% battery warning, if that's possible)

