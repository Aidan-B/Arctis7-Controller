# Power On

This capture is just turning on the headset, then turning it off.

At the start of the capture, the headset is off.
At around the 5s mark, the headset is turned on.
At around the 20s mark, the headset is turned off again.

### Notes:

The device is polled while off with reads:
```
0x06 0x18 (?? Battery status ??)
0x06 0x14 (?? Connection status ??)
```

The majority of the transfer appears to be `0x06 0x28` (audio eq stuff), 
appears to be all reads?

The remainder are either:
- reads:     
    ```
    0x06 0x10 (???)
    ```

- writes:
    ```
    0x06 0x55 ... (Blink  transmitter LED)
    0x06 0x45 ... (???)
    0x06 0x33 ... (Mic Volume)
    0x06 0x35 ... (Mic Sidetone)
    0x06 0x51 ... (Inactivity Shutoff)
    0x06 0x09 ... (???)
    ```