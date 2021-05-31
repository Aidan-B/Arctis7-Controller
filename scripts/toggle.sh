#!/bin/bash
#set -x

# need to inlclude:
# load-module module-remap-sink sink_name=Stereo-Out sink_properties="device.description='Stereo output'" master=alsa_output.usb-SteelSeries_SteelSeries_Arctis_7-00.stereo-game
# load-module module-remap-sink sink_name=Mono-Out sink_properties="device.description='Mono output'" master=alsa_output.usb-SteelSeries_SteelSeries_Arctis_7-00.mono-chat
# at the end of /ect/pulse/default.pa
# the order matters, the first entry will have index 0, and the second will have index 1

# Check if we are currently going to speaker output
test=$(pacmd list-sink-inputs | grep -e 'index: 0' -A 4 | tail -n 1)
if [[ "$test" == *"<alsa_output.pci-0000_2d_00.4.analog-stereo>"* ]]
then
	# Move to headset
	pacmd move-sink-input 0 alsa_output.usb-SteelSeries_SteelSeries_Arctis_7-00.stereo-game
	pacmd move-sink-input 1 alsa_output.usb-SteelSeries_SteelSeries_Arctis_7-00.mono-chat
else
	# Move to speakers
	pacmd move-sink-input 0 alsa_output.pci-0000_2d_00.4.analog-stereo
	pacmd move-sink-input 1 alsa_output.pci-0000_2d_00.4.analog-stereo
fi