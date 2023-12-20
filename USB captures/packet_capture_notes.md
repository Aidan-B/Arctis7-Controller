# Packet Capture Notes

All packets seem to start with `0x06`, A fixed header perhaps?

The next byte seems to indicate the command that is being sent (e.g. Connection status). Subsequent bytes appear to be command specific:
e.g.
```
command = {0x06, 0xXX, 0xXX, ..., 0x00}
            ^     ^     ^
            |     |     |
        header?   |     |
               Command  |
                    Command specific
```

## Commands

### Blink transmitter LED - `0x55`
byte 2 is `0x01`

byte 3 is the status, enabled = `0x02`, disabled = `0x00`

i.e.:
```
enabled = {0x06, 0x55, 0x01, 0x02},
disabled = {0x06, 0x55, 0x01, 0x00}
```

### Inactivity Shutoff - `0x51`
    
byte 2 is the number of minutes until inactivity Shutoff.
To never shutoff, `0x00` is used.

e.g.:
```
5 min = {0x06, 0x51, 0x05}
10 min = {0x06, 0x51, 0x0a} (0x01 == 10)
30 min = {0x06, 0x51, 0x1e} (0x1e == 30)
Never = {0x06, 0x51, 0x00}
```

### Mic Sidetone - `0x35`
Byte 2 appears to enable/disable sidetone with `0x01` or `0x00`

Byte 3 is always zero (maybe a 16 bit integer was used?)

Byte 4 appears to be the intensity, ranging from `0x04`,` 0x0a`, `0x12`
e.g.:
```
off = {0x06, 0x35, 0x00, 0x00, 0x00}
medium = {0x06, 0x35, 0x01, 0x00, 0x0a}
```
### Mic Volume - `0x33`
Byte 2 appears to always be `0x00` again (16 bit int?)

Byte 3 appears to be the volume as a percentage `0x00` to `0x64` (0 to 100)
i.e.:
```
0% = {0x06, 0x33, 0x00, 0x00}
50% = {0x06, 0x33, 0x00, 0x32}
100% = {0x06, 0x33, 0x00, 0x64}
```

### Various Sound Settings (EQ?) - `0x28`
These seem to be bundled into their own thing

#### Sub-commands:
- **Dynamic Range Compression** - `0x07`
- **Equalizer** - `0x03` or `0x04` (and maybe something others, I haven't tested this much)


### Unknown Commands

- `0x10`
- `0x14` - polled every 5s. Appears to be connected status
- `0x18` - polled every 5s. I suspect this is the battery status, the indicator light was yellow at 49%, and it goes up as the headset is charging and powered on.
- `0x09`
- `0x45`


## Misc.

The filter I've been using to get the data sent/received in wireshark:

```
(usb.device_address == 3) && ((_ws.col.protocol == "USBHID") && (usb.src == "host") || (_ws.col.protocol == "USB" &&  (usb.src == "1.3.3") ) )
```