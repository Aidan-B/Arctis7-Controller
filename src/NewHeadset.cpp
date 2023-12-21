#include "NewHeadset.h"

Headset::Headset(libusb_device_handle* handle) : handle(handle) {}

void Headset::read(Packet* request, Packet* response, int timeout) {

    int ret;
    int acutual_length;

    ret = libusb_control_transfer(
        handle, 
        (LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE),
        LIBUSB_REQUEST_SET_CONFIGURATION,
        0x0206, // Not sure what this means
        0x0005, // Not sure what this means
        reinterpret_cast<uint8_t*>(request),
        31,
        // sizeof(*request),
        timeout
    );
    if (ret < LIBUSB_SUCCESS) {
        throw libusb_error(ret);
    }

    ret = libusb_interrupt_transfer(
        handle,
        endpoint,
        reinterpret_cast<uint8_t*>(response),
        sizeof(*response),
        &acutual_length,
        timeout
    );
    if (ret != LIBUSB_SUCCESS) {
        throw libusb_error(ret);
    }
}

void Headset::write(Packet* request, int timeout) {
    int ret;
    
    ret = libusb_control_transfer(
        handle, 
        (LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE),
        LIBUSB_REQUEST_SET_CONFIGURATION,
        0x0206, // Not sure what this means
        0x0005, // Not sure what this means
        reinterpret_cast<uint8_t*>(request),
        31,
        // sizeof(*request),
        timeout
    );
    if (ret < LIBUSB_SUCCESS) {
        throw libusb_error(ret);
    }
}

void Headset::blink_transmitter_led(bool enable) {
    BlinkTransmitterLED request(enable);
    write(&request);
}

void Headset::inactivity_shutoff(uint8_t minutes) {
    InactivityShutoff request(minutes);
    write(&request);
}

void Headset::mic_sidetone(bool enabled, MicSidetone::IntensityValues intensity) {
    if (!enabled) {
        intensity = MicSidetone::disabled;
    }
    MicSidetone request(enabled, intensity);
    write(&request);
}

void Headset::mic_volume(uint8_t volume) {
    if (volume > 100) {
        volume = 100;
    }
    MicVolume request(volume);
    write(&request);
}

bool Headset::connection() {
    Connection request;
    Connection response;
    read(&request, &response);
    
    return response.is_connected();
}

uint8_t Headset::battery() {
    Battery request;
    Battery response;
    read(&request, &response);

    return response.get_charge();
}
