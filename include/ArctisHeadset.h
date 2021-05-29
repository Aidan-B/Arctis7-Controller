#include <exception>
#include <thread>
#include <libusb-1.0/libusb.h>

#ifndef __ARCTIS_HEADSET_H__
#define __ARCTIS_HEADSET_H__

typedef enum {
    silent, //Dont print headset status messages
    change, // Only  print a message if the status changes
    info,   // Print basic information messages
    debug,  // Print all messages that may be helpful for debugging
    all     // Print everything (this includes hex dumps)
} outputVerbosity;

class Arctis7Headset
{
private:
    // Status
    bool m_poweredOn = false; // TODO: Implement this, we want to know the headset state as soon as it turns on
    outputVerbosity m_printIncomingMessages = change;
    bool m_detachedDriver;
    std::chrono::milliseconds m_pollingInterval = std::chrono::milliseconds(1000);

    // USB stuff
    libusb_context *m_ctx;
    libusb_device *m_device;
    libusb_device_handle *m_handle;
    libusb_device_descriptor m_descriptor;

    // Headset HID info
    static const unsigned char m_endpoint = 3 | LIBUSB_ENDPOINT_IN; // Arctis 7 headset HID endpoint
    static const int m_interface = 5; //USB interface used by Arctis 7

    // Thread
    std::thread m_interruptThread;
    std::thread m_powerStatusThread;
    std::exception_ptr m_interruptThreadException = nullptr;
    libusb_transfer *m_interrupt_transfer;
    // Rethrows m_interruptThreadException if thread dies.
    // This method should be called before attempting to use an async libusb resource
    void CheckInterruptThreadException();
    // When false, stops the interrupt thread
    bool m_listenForInterrupts;
    void InterruptListenerThread();
    void PollPowerStatusThread();

    // void handleInterrupt(const unsigned char *buffer, const int& len);
    static void handleInterrupt(libusb_transfer *transfer);

    Arctis7Headset();
    void updatePowerStatus(bool status);

public:
    Arctis7Headset(libusb_context* ctx);
    ~Arctis7Headset();

    void FeatureRequest(unsigned char *requestBuff, const int requestLen);
    void SetOutputVerbosity(outputVerbosity level) { m_printIncomingMessages = level; }
    bool isOn() { return m_poweredOn; }
};

#endif