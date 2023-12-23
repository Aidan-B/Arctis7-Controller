#ifndef __NEW_HEADSET_H__
#define __NEW_HEADSET_H__

#include "Packets.h"

#include <libusb-1.0/libusb.h>
#include <vector>


class Headset {
private:
    libusb_device* device;
    libusb_device_handle *handle;
    std::vector<libusb_transfer*> active_transfers;

    void (*connected_callback)(void*, bool) = nullptr;
    void* connected_input = nullptr;
    void (*battery_callback)(void*, int) = nullptr;
    void* state_of_charge_input = nullptr;

    /**
     * @brief Callback function for usb interrupts
    */
    static void handle_interrupt(libusb_transfer* transfer);
    
    /**
     * @brief Callback function for usb control transfers
    */
    static void handle_control(libusb_transfer* transfer);

    /**
     * @brief Send a control transfer to the headset.
     * @param request   The packet to send
     * @param timeout   The timeout for the usb transaction in ms
    */
    void send_control_transfer(Packet* request, int timeout = 1000);

public:
    static const unsigned char endpoint = 3 | LIBUSB_ENDPOINT_IN; // Arctis 7 headset HID endpoint
    static const int interface = 5; //USB interface used by Arctis 7

    Headset(libusb_device_handle *handle);
    ~Headset();

    /**
     * @brief This function sets up the interrupt input transfers required to 
    *         read data from the device. As such, it must be called before data
    *         is read to/from the device
    */
    void start_interrupt_listener();

    /**
     * @brief Configures whether or not the transmitter LED should blink when
     *        the headset is off
     * @param enable   Blink when true, off when false
    */
    void set_blink_transmitter_led(bool enable);

    /**
     * @brief Configures how quickly the headset should turn off after a period
     *        of inactivity
     * @param minutes   The number of minutes before the headset turns off
    */
    void set_inactivity_shutoff(uint8_t minutes);

    /**
     * @brief Configures the sidetone of the headset
     * @param enabled   If sidetone is enabled
     * @param intensity The intensity of the sidetone feedback
    */
    void set_mic_sidetone(bool enabled, MicSidetone::IntensityValues intensity);
    
    /**
     * @brief Configures the volume of the microphone
     * @param volume    The percentage of the max microphone volume
    */
    void set_mic_volume(uint8_t volume);
    
    // TODO: Not yet supported
    void sound_setting() {};
    

    void set_connection_callback(void (*callback)(void*, bool), void* data);
    void set_battery_callback(void (*callback)(void*, int), void* data);

    /**
     * @brief Is the headset connected to the receiver
     * @returns true if connected, false otherwise
    */
    bool get_connection();

    /**
     * @brief Gets the status of the battery
     * @returns The battery state of charge as a percentage
    */
    uint8_t get_battery();
};

#endif // __NEW_HEADSET_H__
