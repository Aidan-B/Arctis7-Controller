# Arctis 7 Headset Status

This project is an attempt to understand the comminication that occurs between the arctis 7 wireless headset and my computer. Armed with this knowledge I want to make the headset report back to the computer when it is on or off, and  automatically switch audio output to my speakers and back.

~~This should also be cross platform between windows and linux, as I have a dual boot machine and want it to be useful in both operating systems.~~
Cross platform is no longer a priority and development effort is focused on Linux. this will allow for the elimination of busy-waiting threads to handle libusb communication, and Windows users can continue to use the steelseries software provided to them.

This application makes use of libusb-1.0 to handle USB communication.

## Building

The included makefile (designed for use on linux) should mean that after cloning this repo, building is as simple as running `make release`. Use `make help` for additional assistance.

Note that this you will require the gcc/g++ compiler and [libusb](https://libusb.info/) installed on your system.

## ToDo:

Below is the list of features that I want to complete, and the issues that I should resolve as I go along

### Required

- [x] Poll for headset on/off status within headset class
- [x] (HACK) Switch audio output (linux) based on headset status
- [ ] Switch audio output (linux) based on headset status
- [ ] Report battery life
- [x] ~~Compile for windows~~

### Extending

- [ ] Understand all steelseries engine software transfers mean
- [ ] CLI for configuration settings to mimic steelseries engine
- [ ] Add tray icon to report device status, change settings, exit program
- [ ] Static compilation option with makefile
- [ ] Improve error logging, ie: stack traces, logs, etc.

