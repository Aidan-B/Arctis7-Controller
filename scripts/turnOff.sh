#!/bin/bash

# Check if we are currently going to headset output
test=$(pacmd list-sink-inputs | grep -e 'index: 0' -A 4 | tail -n 1)
if [[ "$test" == *"<alsa_output.usb-SteelSeries_SteelSeries_Arctis_7-00.stereo-game>"* ]]
then
	# Move to speakers
	pacmd move-sink-input 0 alsa_output.pci-0000_2d_00.4.analog-stereo
	pacmd move-sink-input 1 alsa_output.pci-0000_2d_00.4.analog-stereo
fi