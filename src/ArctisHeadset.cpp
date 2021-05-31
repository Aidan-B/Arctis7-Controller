#ifdef DEBUG
    #define DEBUG_PRINT(x) do { std::cerr << x << std::endl; } while (0)
#else
    #define DEBUG_PRINT(x) do { } while (0)
#endif

#include "ArctisHeadset.h"

#include <iostream>
#include <string.h>

Arctis7Headset::Arctis7Headset(libusb_context* ctx)
{
    DEBUG_PRINT("Constructing headset object");
    m_ctx = ctx;
    m_device = nullptr;

    int err = 0;
    libusb_device **list;

    ssize_t size = libusb_get_device_list(ctx, &list);
    if (size < 0)
        throw libusb_error((int)size);
    
    for (ssize_t i = 0; i < size; i++) {
        libusb_device_descriptor desc;
        err = libusb_get_device_descriptor(list[i], &desc);
        if (err != LIBUSB_SUCCESS)
            throw libusb_error(err);
    
        if (desc.idVendor == 0x1038 &&      // SteelSeries ApS
            desc.idProduct == 0x12ad &&
            desc.iManufacturer == 4 &&      // SteelSeries
            desc.iProduct == 5)             // SteelSeries Arctis 7
        {
            m_descriptor = desc;
            m_device = list[i];
            break;
        }
    }
    if (m_device == nullptr) {
        throw libusb_error(LIBUSB_ERROR_NOT_FOUND);
    }
    DEBUG_PRINT("Found device, claiming");
    err = libusb_open(m_device, &m_handle);
    if (err != LIBUSB_SUCCESS)
        throw libusb_error(err);

    libusb_free_device_list(list, true);

    if (libusb_has_capability(LIBUSB_CAP_SUPPORTS_DETACH_KERNEL_DRIVER) && 
        libusb_kernel_driver_active(m_handle, m_interface))
    {    
        m_detachedDriver = true;
        err = libusb_detach_kernel_driver(m_handle, m_interface);
        if (err != LIBUSB_SUCCESS) {
            libusb_close(m_handle);
            throw libusb_error(err);
        }
    }
    else {
        m_detachedDriver = false;
    }

    err = libusb_claim_interface(m_handle, m_interface);
	if (err != LIBUSB_SUCCESS) {
        if (m_detachedDriver)
            libusb_attach_kernel_driver(m_handle, m_interface);
        libusb_close(m_handle);
    	throw libusb_error(err);
    }

    DEBUG_PRINT("Starting PollPowerStatusThread");
	m_listenForInterrupts = true;
    m_interruptThread = std::thread(&Arctis7Headset::InterruptListenerThread, this);
    m_powerStatusThread = std::thread(&Arctis7Headset::PollPowerStatusThread, this);
}

Arctis7Headset::~Arctis7Headset()
{   
    libusb_cancel_transfer(m_interruptTransfer);
    m_listenForInterrupts = false;
    m_powerStatusThread.join();
    m_interruptThread.join();
    libusb_free_transfer(m_interruptTransfer);

    libusb_release_interface(m_handle, m_interface);
	if (libusb_has_capability(LIBUSB_CAP_SUPPORTS_DETACH_KERNEL_DRIVER))
        libusb_attach_kernel_driver(m_handle, m_interface);
	libusb_close(m_handle);
}

void Arctis7Headset::FeatureRequest(unsigned char *requestBuff, const int requestLen) {
    int err = 0;
    DEBUG_PRINT("Checking status:");
    
    CheckInterruptThreadException();
    err = libusb_control_transfer(m_handle, (LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE),  LIBUSB_REQUEST_SET_CONFIGURATION, 0x0206, 0x0005, requestBuff, requestLen, 5000);
    if (err < LIBUSB_SUCCESS){
        DEBUG_PRINT("ERROR: control " << libusb_strerror(err));
        throw libusb_error(err);
    }    
}

void Arctis7Headset::CheckInterruptThreadException() {
    //Need to detect if the thread is bad, then report that.
    if (m_interruptThreadException) {
        try{
            std::rethrow_exception(m_interruptThreadException);
        }
        catch(const std::exception& e) {
            std::cerr << "Thread exited with exception: " << e.what() << "\n";
            __throw_exception_again;
        } catch(const libusb_error& e) {
            std::cerr << "Thread exited with exception: " << libusb_strerror(e) << "\n";
            __throw_exception_again;
        } catch(const libusb_transfer_status& e) {
            std::cerr << "Thread exited with exception: " << libusb_error_name(e) << "\n";
            __throw_exception_again;
        } catch(...) {
            __throw_exception_again;
        }
    }
}

void Arctis7Headset::updatePowerStatus(bool status) {
    if (status != m_poweredOn) 
        if (m_printIncomingMessages >= change) {
            if (status) {
                std::cout << "Powered on" << std::endl;
                // system("./scripts/turnOn.sh");
            } else {
                std::cout << "Powered off" << std::endl;
                // system("./scripts/turnOff.sh");
            }
        }

    if (m_printIncomingMessages >= info)
        std::cout << (status ? "Headphones are connected" : "Headphones are not connected")<< std::endl;

    m_poweredOn = status;
}

void Arctis7Headset::PollPowerStatusThread() {
    
    unsigned char req[31] = { 0 };
    req[0] = 0x06;
    req[1] = 0x14;
    
    auto next = std::chrono::steady_clock::now() + m_pollingInterval;
    
    try {
        while (m_listenForInterrupts)
        {
            FeatureRequest(req, 31);
            
            next += m_pollingInterval;
            std::this_thread::sleep_until(next); 
        }
    }
    catch (...) {
        m_listenForInterrupts = false;
        m_interruptThreadException = std::current_exception();
    }
    
}

void Arctis7Headset::InterruptListenerThread() {

	int err;
    const int reply_len = libusb_get_max_packet_size(m_device, m_endpoint);
	unsigned char reply[reply_len];
    
    DEBUG_PRINT("alloc_transfer");
    m_interruptTransfer = libusb_alloc_transfer(1);
    try {
        DEBUG_PRINT("fill_interrupt_transfer");
        libusb_fill_interrupt_transfer(m_interruptTransfer, m_handle, m_endpoint, reply, reply_len, handleInterrupt, this, 0);
        DEBUG_PRINT("submit_transfer");
        err = libusb_submit_transfer(m_interruptTransfer);
        if (err < LIBUSB_SUCCESS)
            throw libusb_error(err);

        while (m_listenForInterrupts) {
            err = libusb_handle_events(m_ctx);
            if (err != LIBUSB_SUCCESS){
                throw libusb_error(err);
            }
        }
    }
    catch (...) {
        m_listenForInterrupts = false;
        m_interruptThreadException = std::current_exception();
    }

	return;
}

void LIBUSB_CALL Arctis7Headset::handleInterrupt(libusb_transfer *transfer) {
    DEBUG_PRINT("handleInterrupt");
    //TODO: I need to figure out what all of these different numbers actually mean...
    // void ArctisHeadset::handleInterrupt(const unsigned char *buffer, const int &len) {
    static const unsigned char poweroff[5] = { 0x01, 0x00, 0x00, 0x00, 0x00 };
    static const unsigned char isDisconnected[31] = { 0x06, 0x14, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    static const unsigned char isConnected[31] = { 0x06, 0x14, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    Arctis7Headset *headset = static_cast<Arctis7Headset*>(transfer->user_data);

    try {
        if (transfer->status != LIBUSB_TRANSFER_COMPLETED) { //error occured
            throw libusb_transfer_status(transfer->status);
        } else if (transfer->actual_length > 0) {
            
            if (memcmp(transfer->buffer, poweroff, transfer->actual_length) == 0 ||
                memcmp(transfer->buffer, isDisconnected, transfer->actual_length) == 0)
            {
                headset->updatePowerStatus(false);
            }
            else if (memcmp(transfer->buffer, isConnected, transfer->actual_length) == 0)
            {
                headset->updatePowerStatus(true);
            }
            
            if (headset->m_printIncomingMessages >= all)
            {
                for (int i = 0; i < transfer->actual_length; i++) {//just print the raw hex data
                    printf("%02x ", transfer->buffer[i]);
                }
                std::cout << std::endl;	
            }

        } else {
            std::cout << "NULL" << std::endl; //We somehow got a zero length buffer?
        }

        // Resumbit transfer
        int err = libusb_submit_transfer(headset->m_interruptTransfer);
        if (err < LIBUSB_SUCCESS)
            throw libusb_error(err);
    }
    catch (...) {
        headset->m_listenForInterrupts = false;
        headset->m_interruptThreadException = std::current_exception();
    }
}