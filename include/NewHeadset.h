#ifndef __NEW_HEADSET_H__
#define __NEW_HEADSET_H__

#include "Packets.h"

#include <libusb-1.0/libusb.h>


class Headset {
private:
    libusb_device_handle *handle;

//TODO: Make read/write private once you have a good idea what all the protocols do
public:
    /**
     * @brief Perform a read operation on the headset.
     * @param request   The read to perform
     * @param response  The read data is returned in this struct
     * @param timeout   The timeout for each individual usb operation
    */
    void read(Packet* request, Packet* response, int timeout = 5000);

    /**
     * @brief Perform a write operation on the headset.
     * @param request   The data to be written
     * @param timeout   The timeout for the operation
    */
    void write(Packet* request, int timeout = 5000);

public:
    static const unsigned char endpoint = 3 | LIBUSB_ENDPOINT_IN; // Arctis 7 headset HID endpoint
    static const int interface = 5; //USB interface used by Arctis 7

    Headset(libusb_device_handle *handle);

    /**
     * @brief Configures whether or not the transmitter LED should blink when
     *        the headset is off
     * @param enable   Blink when true, off when false
    */
    void blink_transmitter_led(bool enable);

    /**
     * @brief Configures how quickly the headset should turn off after a period
     *        of inactivity
     * @param minutes   The number of minutes before the headset turns off
    */
    void inactivity_shutoff(uint8_t minutes);

    /**
     * @brief Configures the sidetone of the headset
     * @param enabled   If sidetone is enabled
     * @param intensity The intensity of the sidetone feedback
    */
    void mic_sidetone(bool enabled, MicSidetone::IntensityValues intensity);
    
    /**
     * @brief Configures the volume of the microphone
     * @param volume    The percentage of the max microphone volume
    */
    void mic_volume(uint8_t volume);
    
    // TODO: Not yet supported
    void sound_setting() {};
    
    /**
     * @brief Is the headset connected to the receiver
     * @returns true if connected, false otherwise
    */
    bool connection();

    /**
     * @brief Gets the status of the battery
     * @returns The battery state of charge as a percentage
    */
    uint8_t battery();
};

#endif // __NEW_HEADSET_H__
