# Summary:

Arctis 7 Chat -> Endpoint 1 IN
Arctis 7 Chat -> Endpoint 6 OUT
Arctis 7 Game -> Endpoint 2 OUT

Arctis 7 HID  -> Endpoint 3 IN
  on device shutoff endpoint 3 sends
  `URB_INTERRUPT in: 00000001 00000000 00000000 00000000 00000000`

Every 5 seconds arctis software polls endpoint 3 for some information

Something like this:

**3.2.3 -> Host**
```
  HID Data: 06180000000000000000000000000000000000000000000000000000000000
0110 0001 1000 0000000000000000000000000000000000000000000000000000000000000 
  USB URB
    [Source: 3.2.3]
    [Destination: host]
    URB id: 0xffff8966b1446540
    URB type: URB_COMPLETE ('C')
    URB transfer type: URB_INTERRUPT (0x01)
    Endpoint: 0x83, Direction: IN
        1... .... = Direction: IN (1)
        .... 0011 = Endpoint number: 3
    Device: 2
    URB bus id: 3
    Device setup request: not relevant ('-')
    Data: present (0)
    URB sec: 1620952724
    URB usec: 512102
    URB status: Success (0)
    URB length [bytes]: 31
    Data length [bytes]: 31
    [bInterfaceClass: HID (0x03)]
    Unused Setup Header
    Interval: 1
    Start frame: 0
    Copy of Transfer Flags: 0x00000200, Dir IN
        .... .... .... .... .... .... .... ...0 = Short not OK: False
        .... .... .... .... .... .... .... ..0. = ISO ASAP: False
        .... .... .... .... .... .... .... .0.. = No transfer DMA map: False
        .... .... .... .... .... .... ..0. .... = No FSBR: False
        .... .... .... .... .... .... .0.. .... = Zero Packet: False
        .... .... .... .... .... .... 0... .... = No Interrupt: False
        .... .... .... .... .... ...0 .... .... = Free Buffer: False
        .... .... .... .... .... ..1. .... .... = Dir IN: True
        .... .... .... ...0 .... .... .... .... = DMA Map Single: False
        .... .... .... ..0. .... .... .... .... = DMA Map Page: False
        .... .... .... .0.. .... .... .... .... = DMA Map SG: False
        .... .... .... 0... .... .... .... .... = Map Local: False
        .... .... ...0 .... .... .... .... .... = Setup Map Single: False
        .... .... ..0. .... .... .... .... .... = Setup Map Local: False
        .... .... .0.. .... .... .... .... .... = DMA S-G Combined: False
        .... .... 0... .... .... .... .... .... = Aligned Temp Buffer: False
    Number of ISO descriptors: 0
```
**Host -> 2.3.2**
```
  No HID Data

  USB URB
    [Source: host]
    [Destination: 3.2.3]
    URB id: 0xffff8966b1446540
    URB type: URB_SUBMIT ('S')
    URB transfer type: URB_INTERRUPT (0x01)
    Endpoint: 0x83, Direction: IN
        1... .... = Direction: IN (1)
        .... 0011 = Endpoint number: 3
    Device: 2
    URB bus id: 3
    Device setup request: not relevant ('-')
    Data: not present ('<')
    URB sec: 1620952724
    URB usec: 512149
    URB status: Operation now in progress (-EINPROGRESS) (-115)
    URB length [bytes]: 64
    Data length [bytes]: 0
    [Response in: 18]
    [bInterfaceClass: HID (0x03)]
    Unused Setup Header
    Interval: 1
    Start frame: 0
    Copy of Transfer Flags: 0x00000200, Dir IN
        .... .... .... .... .... .... .... ...0 = Short not OK: False
        .... .... .... .... .... .... .... ..0. = ISO ASAP: False
        .... .... .... .... .... .... .... .0.. = No transfer DMA map: False
        .... .... .... .... .... .... ..0. .... = No FSBR: False
        .... .... .... .... .... .... .0.. .... = Zero Packet: False
        .... .... .... .... .... .... 0... .... = No Interrupt: False
        .... .... .... .... .... ...0 .... .... = Free Buffer: False
        .... .... .... .... .... ..1. .... .... = Dir IN: True
        .... .... .... ...0 .... .... .... .... = DMA Map Single: False
        .... .... .... ..0. .... .... .... .... = DMA Map Page: False
        .... .... .... .0.. .... .... .... .... = DMA Map SG: False
        .... .... .... 0... .... .... .... .... = Map Local: False
        .... .... ...0 .... .... .... .... .... = Setup Map Single: False
        .... .... ..0. .... .... .... .... .... = Setup Map Local: False
        .... .... .0.. .... .... .... .... .... = DMA S-G Combined: False
        .... .... 0... .... .... .... .... .... = Aligned Temp Buffer: False
    Number of ISO descriptors: 0
```
**2.3.2 -> Host**
```
  HID Data: 06140100000000000000000000000000000000000000000000000000000000
  USB URB
    [Source: 3.2.3]
    [Destination: host]
    URB id: 0xffff8966b1446540
    URB type: URB_COMPLETE ('C')
    URB transfer type: URB_INTERRUPT (0x01)
    Endpoint: 0x83, Direction: IN
        1... .... = Direction: IN (1)
        .... 0011 = Endpoint number: 3
    Device: 2
    URB bus id: 3
    Device setup request: not relevant ('-')
    Data: present (0)
    URB sec: 1620952724
    URB usec: 527101
    URB status: Success (0)
    URB length [bytes]: 31
    Data length [bytes]: 31
    [Request in: 16]
    [Time from request: 0.014952000 seconds]
    [bInterfaceClass: HID (0x03)]
    Unused Setup Header
    Interval: 1
    Start frame: 0
    Copy of Transfer Flags: 0x00000200, Dir IN
        .... .... .... .... .... .... .... ...0 = Short not OK: False
        .... .... .... .... .... .... .... ..0. = ISO ASAP: False
        .... .... .... .... .... .... .... .0.. = No transfer DMA map: False
        .... .... .... .... .... .... ..0. .... = No FSBR: False
        .... .... .... .... .... .... .0.. .... = Zero Packet: False
        .... .... .... .... .... .... 0... .... = No Interrupt: False
        .... .... .... .... .... ...0 .... .... = Free Buffer: False
        .... .... .... .... .... ..1. .... .... = Dir IN: True
        .... .... .... ...0 .... .... .... .... = DMA Map Single: False
        .... .... .... ..0. .... .... .... .... = DMA Map Page: False
        .... .... .... .0.. .... .... .... .... = DMA Map SG: False
        .... .... .... 0... .... .... .... .... = Map Local: False
        .... .... ...0 .... .... .... .... .... = Setup Map Single: False
        .... .... ..0. .... .... .... .... .... = Setup Map Local: False
        .... .... .0.. .... .... .... .... .... = DMA S-G Combined: False
        .... .... 0... .... .... .... .... .... = Aligned Temp Buffer: False
    Number of ISO descriptors: 0
```
**Host -> 2.3.2**
```
  USB URB
    [Source: host]
    [Destination: 3.2.3]
    URB id: 0xffff89668b620480
    URB type: URB_SUBMIT ('S')
    URB transfer type: URB_INTERRUPT (0x01)
    Endpoint: 0x83, Direction: IN
        1... .... = Direction: IN (1)
        .... 0011 = Endpoint number: 3
    Device: 2
    URB bus id: 3
    Device setup request: not relevant ('-')
    Data: not present ('<')
    URB sec: 1620952724
    URB usec: 527735
    URB status: Operation now in progress (-EINPROGRESS) (-115)
    URB length [bytes]: 64
    Data length [bytes]: 0
    [Response in: 22]
    [bInterfaceClass: HID (0x03)]
    Unused Setup Header
    Interval: 1
    Start frame: 0
    Copy of Transfer Flags: 0x00000200, Dir IN
        .... .... .... .... .... .... .... ...0 = Short not OK: False
        .... .... .... .... .... .... .... ..0. = ISO ASAP: False
        .... .... .... .... .... .... .... .0.. = No transfer DMA map: False
        .... .... .... .... .... .... ..0. .... = No FSBR: False
        .... .... .... .... .... .... .0.. .... = Zero Packet: False
        .... .... .... .... .... .... 0... .... = No Interrupt: False
        .... .... .... .... .... ...0 .... .... = Free Buffer: False
        .... .... .... .... .... ..1. .... .... = Dir IN: True
        .... .... .... ...0 .... .... .... .... = DMA Map Single: False
        .... .... .... ..0. .... .... .... .... = DMA Map Page: False
        .... .... .... .0.. .... .... .... .... = DMA Map SG: False
        .... .... .... 0... .... .... .... .... = Map Local: False
        .... .... ...0 .... .... .... .... .... = Setup Map Single: False
        .... .... ..0. .... .... .... .... .... = Setup Map Local: False
        .... .... .0.. .... .... .... .... .... = DMA S-G Combined: False
        .... .... 0... .... .... .... .... .... = Aligned Temp Buffer: False
    Number of ISO descriptors: 0
```
Then we wait 5 seconds and it repeats

But when we are connected we have:
**3.2.3 -> Host** 
```
  HID Data: 06184701000000000000000000000000000000000000000000000000000000
```
**Host -> 2.3.2**  
**2.3.2 -> Host**
```
  HID Data: 06140300000000000000000000000000000000000000000000000000000000

  HID Data: 06281800000000000000000000000000000000000000000000000000000000
  HID Data: 06280500200000008d06ff0000ff00c0ff0000000000000000000000000000
  HID Data: 06280600200000008e00000000000000000000000000000000000000000000
  HID Data: 0628036834dcac395500000000000000000000000000000000000000000000
  HID Data: 0628036934dc2c3a5600000000000000000000000000000000000000000000
  HID Data: 0628036a34dcac395700000000000000000000000000000000000000000000
  HID Data: 0628036b0000803f5800000000000000000000000000000000000000000000
  HID Data: 0628036ca257f93f5900000000000000000000000000000000000000000000
  HID Data: 0628036db40573bf5a8d06ff0000ff00c0ff00000000000000000000000000
  HID Data: 06280360010000005b8e000000000000000000000000000000000000000000
  HID Data: 0628046834dcac397155000000000000000000000000000000000000000000
  HID Data: 0628046934dc2c3a7256000000000000000000000000000000000000000000
  HID Data: 0628046a34dcac397357000000000000000000000000000000000000000000
  HID Data: 0628046b0000803f7458000000000000000000000000000000000000000000
  HID Data: 0628046ca257f93f7559000000000000000000000000000000000000000000
  HID Data: 0628046db40573bf765a8d06ff0000ff00c0ff000000000000000000000000
  HID Data: 0628046001000000775b8e0000000000000000000000000000000000000000
  HID Data: 0628037834dcac395c71550000000000000000000000000000000000000000
  HID Data: 0628037934dc2c3a5d72560000000000000000000000000000000000000000
  HID Data: 0628037a34dcac395e73570000000000000000000000000000000000000000
  HID Data: 0628037b0000803f5f74580000000000000000000000000000000000000000
  HID Data: 0628037ca257f93f6075590000000000000000000000000000000000000000
  HID Data: 0628037db40573bf61765a8d06ff0000ff00c0000000000000000000000000
  HID Data: 062803700100000062775b8e00000000000000000000000000000000000000
  HID Data: 0628047834dcac39785c715500000000000000000000000000000000000000
  HID Data: 0628047934dc2c3a795d725600000000000000000000000000000000000000
  HID Data: 0628047a34dcac397a5e735700000000000000000000000000000000000000
  HID Data: 0628047b0000803f7b5f745800000000000000000000000000000000000000
  HID Data: 0628047ca257f93f7c60755900000000000000000000000000000000000000
  HID Data: 0628047db40573bf7d61765a8d06ff0000ff00000000000000000000000000
  HID Data: 06280470010000007e62775b8e000000000000000000000000000000000000
  HID Data: 062803883e6d793f63785c7155000000000000000000000000000000000000
  HID Data: 062803893e6df9bf64795d7256000000000000000000000000000000000000
  HID Data: 0628038a3e6d793f657a5e7357000000000000000000000000000000000000
  HID Data: 0628038b0000803f667b5f7458000000000000000000000000000000000000
  HID Data: 0628038ca257f93f677c607559000000000000000000000000000000000000
  HID Data: 0628038db40573bf687d61765a8d06ff0000ff000000000000000000000000
  HID Data: 0628038001000000697e62775b8e0000000000000000000000000000000000
  HID Data: 062804883e6d793f7f63785c71550000000000000000000000000000000000
  HID Data: 062804893e6df9bf8064795d72560000000000000000000000000000000000
  HID Data: 0628048a3e6d793f81657a5e73570000000000000000000000000000000000
  HID Data: 0628048b0000803f82667b5f74580000000000000000000000000000000000
  HID Data: 0628048ca257f93f83677c6075590000000000000000000000000000000000
  HID Data: 0628048db40573bf84687d61765a8d06ff0000000000000000000000000000
  HID Data: 062804800100000085697e62775b8e00000000000000000000000000000000
  HID Data: 062803983e6d793f6a7f63785c715500000000000000000000000000000000
  HID Data: 062803993e6df9bf6b8064795d725600000000000000000000000000000000
  HID Data: 0628039a3e6d793f6c81657a5e735700000000000000000000000000000000
  HID Data: 0628039b0000803f6d82667b5f745800000000000000000000000000000000
  HID Data: 0628039ca257f93f6e83677c60755900000000000000000000000000000000
  HID Data: 0628039db40573bf6f84687d61765a8d06ff00000000000000000000000000
  HID Data: 06280390010000007085697e62775b8e000000000000000000000000000000
  HID Data: 062804983e6d793f866a7f63785c7155000000000000000000000000000000
  HID Data: 062804993e6df9bf876b8064795d7256000000000000000000000000000000
  HID Data: 0628049a3e6d793f886c81657a5e7357000000000000000000000000000000
  HID Data: 0628049b0000803f896d82667b5f7458000000000000000000000000000000
  HID Data: 0628049ca257f93f8a6e83677c607559000000000000000000000000000000
  HID Data: 0628049db40573bf8b6f84687d61765a8d06ff000000000000000000000000
  HID Data: 06280490010000008c7085697e62775b8e0000000000000000000000000000
  HID Data: 062807106700000091866a7f63785c71550000000000000000000000000000
  HID Data: 06280720f600000092876b8064795d72560000000000000000000000000000
  HID Data: 062807020000000093886c81657a5e73570000000000000000000000000000
  HID Data: 062807116700000096896d82667b5f74580000000000000000000000000000
...and more...
  HID Data: 06101301130100004607000000000000000000000000000000000000000000
  HID Data: 06101301130100001531900000000000000000000000000000000000000000
  HID Data: 06101301130100003f4d2a91866a7f63785c71000000000000000000000000
  HID Data: 06184701000000000000000000000000000000000000000000000000000000
  HID Data: 06140300000000000000000000000000000000000000000000000000000000
```
When it's back to just polling, it repeats this:
```
  HID Data: 06184703000000000000000000000000000000000000000000000000000000
  HID Data: 06140300000000000000000000000000000000000000000000000000000000
```

And when it is turned off, we have:
```
  HID Data: 0100000000
```
followed by repeating:
```
  HID Data: 06180000000000000000000000000000000000000000000000000000000000
  HID Data: 06140100000000000000000000000000000000000000000000000000000000
```


From this we can see that we always start with `0110`followed by `0001` when doing normal polling.
- `1000 0000 0000` for first reply
- `0100 0000 0001` for second reply

followed by `0010` when performing a bootup connection


### Device Info

```
Number of possible configurations: 1
Device Class: 0
VendorID: 4152  ProductID: 4781

Interfaces: 6
Number of alternate settings: 1
        Interface Number: 0
        Number of endpoints: 0
Number of alternate settings: 2
        Interface Number: 1
        Number of endpoints: 0
        Interface Number: 1
        Number of endpoints: 1
        Descriptor Type: 5
        EP Address: 129
Number of alternate settings: 2
        Interface Number: 2
        Number of endpoints: 0
        Interface Number: 2
        Number of endpoints: 1
        Descriptor Type: 5
        EP Address: 6
Number of alternate settings: 1
        Interface Number: 3
        Number of endpoints: 0
Number of alternate settings: 3
        Interface Number: 4
        Number of endpoints: 0
        Interface Number: 4
        Number of endpoints: 1
        Descriptor Type: 5
        EP Address: 2
        Interface Number: 4
        Number of endpoints: 1
        Descriptor Type: 5
        EP Address: 2
Number of alternate settings: 1
        Interface Number: 5
        Number of endpoints: 1
        Descriptor Type: 5
        EP Address: 131
```

```
$ lsusb -d  103:12ad -vv

Bus 003 Device 002: ID 1038:12ad SteelSeries ApS SteelSeries Arctis 7
Device Descriptor:
  bLength                18
  bDescriptorType         1
  bcdUSB               1.10
  bDeviceClass            0 
  bDeviceSubClass         0 
  bDeviceProtocol         0 
  bMaxPacketSize0        64
  idVendor           0x1038 SteelSeries ApS
  idProduct          0x12ad 
  bcdDevice            1.19
  iManufacturer           4 SteelSeries 
  iProduct                5 SteelSeries Arctis 7
  iSerial                 0 
  bNumConfigurations      1
  Configuration Descriptor:
    bLength                 9
    bDescriptorType         2
    wTotalLength       0x01bd
    bNumInterfaces          6
    bConfigurationValue     1
    iConfiguration          0 
    bmAttributes         0x80
      (Bus Powered)
    MaxPower              100mA
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        0
      bAlternateSetting       0
      bNumEndpoints           0
      bInterfaceClass         1 Audio
      bInterfaceSubClass      1 Control Device
      bInterfaceProtocol      0 
      iInterface              1 Arctis 7 Chat
      AudioControl Interface Descriptor:
        bLength                10
        bDescriptorType        36
        bDescriptorSubtype      1 (HEADER)
        bcdADC               1.00
        wTotalLength       0x0046
        bInCollection           2
        baInterfaceNr(0)        1
        baInterfaceNr(1)        2
      AudioControl Interface Descriptor:
        bLength                12
        bDescriptorType        36
        bDescriptorSubtype      2 (INPUT_TERMINAL)
        bTerminalID             6
        wTerminalType      0x0402 Headset
        bAssocTerminal         80
        bNrChannels             1
        wChannelConfig     0x0000
        iChannelNames           0 
        iTerminal               0 
      AudioControl Interface Descriptor:
        bLength                 9
        bDescriptorType        36
        bDescriptorSubtype      6 (FEATURE_UNIT)
        bUnitID                 3
        bSourceID               6
        bControlSize            1
        bmaControls(0)       0x03
          Mute Control
          Volume Control
        bmaControls(1)       0x00
        iFeature                0 
      AudioControl Interface Descriptor:
        bLength                 9
        bDescriptorType        36
        bDescriptorSubtype      3 (OUTPUT_TERMINAL)
        bTerminalID             7
        wTerminalType      0x0101 USB Streaming
        bAssocTerminal          0
        bSourceID               3
        iTerminal               0 
      AudioControl Interface Descriptor:
        bLength                12
        bDescriptorType        36
        bDescriptorSubtype      2 (INPUT_TERMINAL)
        bTerminalID            96
        wTerminalType      0x0101 USB Streaming
        bAssocTerminal          0
        bNrChannels             1
        wChannelConfig     0x0000
        iChannelNames           0 
        iTerminal               0 
      AudioControl Interface Descriptor:
        bLength                 9
        bDescriptorType        36
        bDescriptorSubtype      6 (FEATURE_UNIT)
        bUnitID                 4
        bSourceID              96
        bControlSize            1
        bmaControls(0)       0x03
          Mute Control
          Volume Control
        bmaControls(1)       0x00
        iFeature                0 
      AudioControl Interface Descriptor:
        bLength                 9
        bDescriptorType        36
        bDescriptorSubtype      3 (OUTPUT_TERMINAL)
        bTerminalID            80
        wTerminalType      0x0402 Headset
        bAssocTerminal          6
        bSourceID               4
        iTerminal               0 
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        1
      bAlternateSetting       0
      bNumEndpoints           0
      bInterfaceClass         1 Audio
      bInterfaceSubClass      2 Streaming
      bInterfaceProtocol      0 
      iInterface              0 
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        1
      bAlternateSetting       1
      bNumEndpoints           1
      bInterfaceClass         1 Audio
      bInterfaceSubClass      2 Streaming
      bInterfaceProtocol      0 
      iInterface              0 
      AudioStreaming Interface Descriptor:
        bLength                 7
        bDescriptorType        36
        bDescriptorSubtype      1 (AS_GENERAL)
        bTerminalLink           7
        bDelay                  0 frames
        wFormatTag         0x0001 PCM
      AudioStreaming Interface Descriptor:
        bLength                32
        bDescriptorType        36
        bDescriptorSubtype      2 (FORMAT_TYPE)
        bFormatType             1 (FORMAT_TYPE_I)
        bNrChannels             1
        bSubframeSize           2
        bBitResolution         16
        bSamFreqType            8 Discrete
        tSamFreq[ 0]         8000
        tSamFreq[ 1]        11025
        tSamFreq[ 2]        16000
        tSamFreq[ 3]        22050
        tSamFreq[ 4]        24000
        tSamFreq[ 5]        32000
        tSamFreq[ 6]        44100
        tSamFreq[ 7]        48000
      Endpoint Descriptor:
        bLength                 9
        bDescriptorType         5
        bEndpointAddress     0x81  EP 1 IN
        bmAttributes           13
          Transfer Type            Isochronous
          Synch Type               Synchronous
          Usage Type               Data
        wMaxPacketSize     0x0090  1x 144 bytes
        bInterval               1
        bRefresh                0
        bSynchAddress           0
        AudioStreaming Endpoint Descriptor:
          bLength                 7
          bDescriptorType        37
          bDescriptorSubtype      1 (EP_GENERAL)
          bmAttributes         0x01
            Sampling Frequency
          bLockDelayUnits         0 Undefined
          wLockDelay         0x0000
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        2
      bAlternateSetting       0
      bNumEndpoints           0
      bInterfaceClass         1 Audio
      bInterfaceSubClass      2 Streaming
      bInterfaceProtocol      0 
      iInterface              0 
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        2
      bAlternateSetting       1
      bNumEndpoints           1
      bInterfaceClass         1 Audio
      bInterfaceSubClass      2 Streaming
      bInterfaceProtocol      0 
      iInterface              0 
      AudioStreaming Interface Descriptor:
        bLength                 7
        bDescriptorType        36
        bDescriptorSubtype      1 (AS_GENERAL)
        bTerminalLink          96
        bDelay                  0 frames
        wFormatTag         0x0001 PCM
      AudioStreaming Interface Descriptor:
        bLength                32
        bDescriptorType        36
        bDescriptorSubtype      2 (FORMAT_TYPE)
        bFormatType             1 (FORMAT_TYPE_I)
        bNrChannels             1
        bSubframeSize           2
        bBitResolution         16
        bSamFreqType            8 Discrete
        tSamFreq[ 0]         8000
        tSamFreq[ 1]        11025
        tSamFreq[ 2]        16000
        tSamFreq[ 3]        22050
        tSamFreq[ 4]        24000
        tSamFreq[ 5]        32000
        tSamFreq[ 6]        44100
        tSamFreq[ 7]        48000
      Endpoint Descriptor:
        bLength                 9
        bDescriptorType         5
        bEndpointAddress     0x06  EP 6 OUT
        bmAttributes           13
          Transfer Type            Isochronous
          Synch Type               Synchronous
          Usage Type               Data
        wMaxPacketSize     0x0090  1x 144 bytes
        bInterval               1
        bRefresh                0
        bSynchAddress           0
        AudioStreaming Endpoint Descriptor:
          bLength                 7
          bDescriptorType        37
          bDescriptorSubtype      1 (EP_GENERAL)
          bmAttributes         0x01
            Sampling Frequency
          bLockDelayUnits         0 Undefined
          wLockDelay         0x0000
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        3
      bAlternateSetting       0
      bNumEndpoints           0
      bInterfaceClass         1 Audio
      bInterfaceSubClass      1 Control Device
      bInterfaceProtocol      0 
      iInterface              2 Arctis 7 Game
      AudioControl Interface Descriptor:
        bLength                 9
        bDescriptorType        36
        bDescriptorSubtype      1 (HEADER)
        bcdADC               1.00
        wTotalLength       0x0028
        bInCollection           1
        baInterfaceNr(0)        4
      AudioControl Interface Descriptor:
        bLength                12
        bDescriptorType        36
        bDescriptorSubtype      2 (INPUT_TERMINAL)
        bTerminalID             1
        wTerminalType      0x0101 USB Streaming
        bAssocTerminal          0
        bNrChannels             2
        wChannelConfig     0x0003
          Left Front (L)
          Right Front (R)
        iChannelNames           0 
        iTerminal               0 
      AudioControl Interface Descriptor:
        bLength                10
        bDescriptorType        36
        bDescriptorSubtype      6 (FEATURE_UNIT)
        bUnitID                 2
        bSourceID               1
        bControlSize            1
        bmaControls(0)       0x03
          Mute Control
          Volume Control
        bmaControls(1)       0x00
        bmaControls(2)       0x00
        iFeature                0 
      AudioControl Interface Descriptor:
        bLength                 9
        bDescriptorType        36
        bDescriptorSubtype      3 (OUTPUT_TERMINAL)
        bTerminalID             5
        wTerminalType      0x0302 Headphones
        bAssocTerminal          0
        bSourceID               2
        iTerminal               0 
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        4
      bAlternateSetting       0
      bNumEndpoints           0
      bInterfaceClass         1 Audio
      bInterfaceSubClass      2 Streaming
      bInterfaceProtocol      0 
      iInterface              0 
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        4
      bAlternateSetting       1
      bNumEndpoints           1
      bInterfaceClass         1 Audio
      bInterfaceSubClass      2 Streaming
      bInterfaceProtocol      0 
      iInterface              0 
      AudioStreaming Interface Descriptor:
        bLength                 7
        bDescriptorType        36
        bDescriptorSubtype      1 (AS_GENERAL)
        bTerminalLink           1
        bDelay                  0 frames
        wFormatTag         0x0001 PCM
      AudioStreaming Interface Descriptor:
        bLength                32
        bDescriptorType        36
        bDescriptorSubtype      2 (FORMAT_TYPE)
        bFormatType             1 (FORMAT_TYPE_I)
        bNrChannels             1
        bSubframeSize           2
        bBitResolution         16
        bSamFreqType            8 Discrete
        tSamFreq[ 0]         8000
        tSamFreq[ 1]        11025
        tSamFreq[ 2]        16000
        tSamFreq[ 3]        22050
        tSamFreq[ 4]        24000
        tSamFreq[ 5]        32000
        tSamFreq[ 6]        44100
        tSamFreq[ 7]        48000
      Endpoint Descriptor:
        bLength                 9
        bDescriptorType         5
        bEndpointAddress     0x02  EP 2 OUT
        bmAttributes           13
          Transfer Type            Isochronous
          Synch Type               Synchronous
          Usage Type               Data
        wMaxPacketSize     0x0090  1x 144 bytes
        bInterval               1
        bRefresh                0
        bSynchAddress           0
        AudioStreaming Endpoint Descriptor:
          bLength                 7
          bDescriptorType        37
          bDescriptorSubtype      1 (EP_GENERAL)
          bmAttributes         0x01
            Sampling Frequency
          bLockDelayUnits         0 Undefined
          wLockDelay         0x0000
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        4
      bAlternateSetting       2
      bNumEndpoints           1
      bInterfaceClass         1 Audio
      bInterfaceSubClass      2 Streaming
      bInterfaceProtocol      0 
      iInterface              0 
      AudioStreaming Interface Descriptor:
        bLength                 7
        bDescriptorType        36
        bDescriptorSubtype      1 (AS_GENERAL)
        bTerminalLink           1
        bDelay                  0 frames
        wFormatTag         0x0001 PCM
      AudioStreaming Interface Descriptor:
        bLength                32
        bDescriptorType        36
        bDescriptorSubtype      2 (FORMAT_TYPE)
        bFormatType             1 (FORMAT_TYPE_I)
        bNrChannels             2
        bSubframeSize           2
        bBitResolution         16
        bSamFreqType            8 Discrete
        tSamFreq[ 0]         8000
        tSamFreq[ 1]        11025
        tSamFreq[ 2]        16000
        tSamFreq[ 3]        22050
        tSamFreq[ 4]        24000
        tSamFreq[ 5]        32000
        tSamFreq[ 6]        44100
        tSamFreq[ 7]        48000
      Endpoint Descriptor:
        bLength                 9
        bDescriptorType         5
        bEndpointAddress     0x02  EP 2 OUT
        bmAttributes           13
          Transfer Type            Isochronous
          Synch Type               Synchronous
          Usage Type               Data
        wMaxPacketSize     0x0120  1x 288 bytes
        bInterval               1
        bRefresh                0
        bSynchAddress           0
        AudioStreaming Endpoint Descriptor:
          bLength                 7
          bDescriptorType        37
          bDescriptorSubtype      1 (EP_GENERAL)
          bmAttributes         0x01
            Sampling Frequency
          bLockDelayUnits         0 Undefined
          wLockDelay         0x0000
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        5
      bAlternateSetting       0
      bNumEndpoints           1
      bInterfaceClass         3 Human Interface Device
      bInterfaceSubClass      0 
      bInterfaceProtocol      0 
      iInterface              3 SteelSeries Arctis 7
        HID Device Descriptor:
          bLength                 9
          bDescriptorType        33
          bcdHID               1.11
          bCountryCode            0 Not supported
          bNumDescriptors         1
          bDescriptorType        34 Report
          wDescriptorLength     398
         Report Descriptors: 
           ** UNAVAILABLE **
      Endpoint Descriptor:
        bLength                 7
        bDescriptorType         5
        bEndpointAddress     0x83  EP 3 IN
        bmAttributes            3
          Transfer Type            Interrupt
          Synch Type               None
          Usage Type               Data
        wMaxPacketSize     0x0020  1x 32 bytes
        bInterval               1
Device Status:     0x0000
  (Bus Powered)
```