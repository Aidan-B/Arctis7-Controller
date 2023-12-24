#include "NewHeadset.h"

#include <stdio.h>

Headset::Headset(libusb_device_handle* handle) : 
    device(libusb_get_device(handle)),
    handle(handle) {}

Headset::~Headset() {
    for(auto i : active_transfers) {
        libusb_cancel_transfer(i);
    }
}

void Headset::start_interrupt_listener() {
    
    int ret;
    const int buffer_size = libusb_get_max_packet_size(device, endpoint) * 2;
    uint8_t* interrupt_buffer = new uint8_t[buffer_size];

    libusb_transfer* transfer = libusb_alloc_transfer(0);
    if (transfer == nullptr) {
        throw libusb_error(LIBUSB_ERROR_OTHER);
    }
    active_transfers.push_back(transfer);

    libusb_fill_interrupt_transfer(
        transfer, 
        handle,
        endpoint,
        interrupt_buffer,
        buffer_size,
        Headset::handle_interrupt,
        this,
        0 //unlimited
    );

    ret = libusb_submit_transfer(transfer);
    if (ret != LIBUSB_SUCCESS) {
        throw libusb_error(ret);
    }
}

void Headset::handle_interrupt(libusb_transfer* transfer) {

    if (transfer->status == LIBUSB_TRANSFER_COMPLETED && transfer->actual_length > 0) {
        // In the event of transfer cancellation, the headset object could have
        // been cleaned up, so don't try to access its memory
        
        // TODO: Send the data to a callback so the user can make use of it
        Headset *headset = static_cast<Headset*>(transfer->user_data);
        (void) headset;

        if (transfer->actual_length == sizeof(Packet)) {
            Packet* packet = reinterpret_cast<Packet*>(transfer->buffer);

            if (packet->command == Packet::battery) {
                if (headset->battery_callback != nullptr) {
                    headset->battery_callback(reinterpret_cast<Battery*>(packet)->get_charge());
                }
            } else if (packet->command == Packet::connection) {
                if (headset->connected_callback != nullptr) {
                    headset->connected_callback(reinterpret_cast<Connection*>(packet)->is_connected());
                }
            }
        }

        // for (int i = 0; i < transfer->actual_length; i++) {
        //     printf("%02x ", transfer->buffer[i]);
        // }
        // printf("\n");

        // Resumbit transfer to continue listening for interrupts
        int err = libusb_submit_transfer(transfer);
        if (err < LIBUSB_SUCCESS) {
            throw libusb_error(err);
        }

    } else if (transfer->status == LIBUSB_TRANSFER_CANCELLED) {
        delete transfer->buffer;
        libusb_free_transfer(transfer);

    } else {
        throw libusb_transfer_status(transfer->status);
    }
}

void Headset::handle_control(libusb_transfer* transfer) {
    
    if (transfer->status == LIBUSB_TRANSFER_COMPLETED) {
        // In the event of transfer cancellation, the headset object could have
        // been cleaned up, so don't try to access its memory
        Headset *headset = static_cast<Headset*>(transfer->user_data);
        for (std::vector<libusb_transfer*>::iterator iter = headset->active_transfers.begin(); 
            iter != headset->active_transfers.end(); ++iter)
        {
            if( *iter == transfer )
            {
                headset->active_transfers.erase( iter );
                break;
            }
        }

    } else if (transfer->status != LIBUSB_TRANSFER_CANCELLED) {
        throw libusb_transfer_status(transfer->status);
    }
    
    delete transfer->buffer;
    libusb_free_transfer(transfer);

}

void Headset::send_control_transfer(Packet* request, int timeout) {

    libusb_transfer* transfer = libusb_alloc_transfer(0);
    if (transfer == nullptr) {
        throw libusb_error(LIBUSB_ERROR_OTHER);
    }
    active_transfers.push_back(transfer); 
    
    uint8_t* buffer = new uint8_t[sizeof(*request)+8]();
    libusb_fill_control_setup(
        buffer,
        (LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE),
        LIBUSB_REQUEST_SET_CONFIGURATION,
        0x0206, // Not sure what this means
        0x0005, // Not sure what this means
        31
    );
    libusb_fill_control_transfer(
        transfer,
        handle,
        buffer,
        Headset::handle_control,
        this,
        timeout
    );
    std::copy(
        reinterpret_cast<uint8_t*>(request),
        reinterpret_cast<uint8_t*>(request)+sizeof(*request),
        libusb_control_transfer_get_data(transfer)
    );

    int ret = libusb_submit_transfer(transfer);
    if (ret != LIBUSB_SUCCESS) {
        throw libusb_error(ret);
    }
}

void Headset::set_blink_transmitter_led(bool enable) {
    BlinkTransmitterLED request(enable);
    send_control_transfer(&request);
}

void Headset::set_inactivity_shutoff(uint8_t minutes) {
    InactivityShutoff request(minutes);
    send_control_transfer(&request);
}

void Headset::set_mic_sidetone(bool enabled, MicSidetone::IntensityValues intensity) {
    if (!enabled) {
        intensity = MicSidetone::disabled;
    }
    MicSidetone request(enabled, intensity);
    send_control_transfer(&request);
}

void Headset::set_mic_volume(uint8_t volume) {
    if (volume > 100) {
        volume = 100;
    }
    MicVolume request(volume);
    send_control_transfer(&request);
}

void Headset::set_connection_callback(std::function<void(bool)> callback) {
    connected_callback = callback;
}

void Headset::set_battery_callback(std::function<void(int)> callback) {
    battery_callback = callback;
}

bool Headset::get_connection() {
    Connection request;

    // TODO: setup a callback function to place the data somewhere

    send_control_transfer(&request);
    
    return false;
}

uint8_t Headset::get_battery() {
    Battery request;

    // TODO: setup a callback function to place the data somewhere

    send_control_transfer(&request);

    return 0;
}
