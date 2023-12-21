#include "NewHeadset.h"

#include <libusb-1.0/libusb.h>
#include <iostream>

void print_packet(const Packet *packet) {
    const uint8_t* buf = reinterpret_cast<const uint8_t*>(packet);
    for (long unsigned int i = 0; i < sizeof(*packet); i++) {
        printf("%02x ", buf[i]);
    }
    printf("\n");
}

int main() {

	libusb_context* ctx = nullptr; //a libusb session
    libusb_device** device_list = nullptr;
    libusb_device* device = nullptr;
	libusb_device_handle* handle = nullptr;
	bool detachedKernelDriver = false;

    int err = 0;
	

	// Initialize the library
	err = libusb_init(&ctx);
	
	// TODO: This error handling just exits when anything bad happens. 
	//		 It should be a little more robust. (Get rid of try-throw-catch)
	try {
		if (err != LIBUSB_SUCCESS) {
			throw libusb_error(err);
		}
		
		// libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_DEBUG);
		
		// Get the list of devices, then select the correct one from the list
		ssize_t size = libusb_get_device_list(ctx, &device_list);
		if (size < 0) {
			throw libusb_error(size);
		}
		for (ssize_t i = 0; i < size; i++) {
			libusb_device_descriptor desc;
			err = libusb_get_device_descriptor(device_list[i], &desc);
			if (err != LIBUSB_SUCCESS) {
				throw libusb_error(err);
			}
			if (desc.idVendor == 0x1038 &&      // SteelSeries ApS
				desc.idProduct == 0x12ad &&
				desc.iManufacturer == 4 &&      // SteelSeries
				desc.iProduct == 5)             // SteelSeries Arctis 7
			{
				device = device_list[i];
				break;
			}
		}
		if (device == nullptr) {
			throw libusb_error(LIBUSB_ERROR_NOT_FOUND);
		}

		// Open the headset with libusb and claim the interface
		err = libusb_open(device, &handle);
		if (err != LIBUSB_SUCCESS) {
			throw libusb_error(err);
		}

		libusb_free_device_list(device_list, true);
		device = nullptr;

		if (libusb_has_capability(LIBUSB_CAP_SUPPORTS_DETACH_KERNEL_DRIVER) && 
			libusb_kernel_driver_active(handle, Headset::interface))
		{    
			detachedKernelDriver = true;
			err = libusb_detach_kernel_driver(handle, Headset::interface);
			if (err != LIBUSB_SUCCESS) {
				libusb_close(handle);
				
				throw libusb_error(err);
			}
		}

		err = libusb_claim_interface(handle, Headset::interface);
		if (err != LIBUSB_SUCCESS) {
			if (detachedKernelDriver) {
				libusb_attach_kernel_driver(handle, Headset::interface);
			}
			libusb_close(handle);
			throw libusb_error(err);
		}


		Headset headset(handle);

		Battery request;
		Packet response{.command=0};
		
		headset.read(&request, &response);
		print_packet(&response);


		if (detachedKernelDriver) {
			libusb_attach_kernel_driver(handle, Headset::interface);
		}
		libusb_close(handle);

	}
	catch(const libusb_error& e) {
		std::cerr << "Error: " << libusb_strerror(e) << std::endl;
	}

	libusb_exit(ctx);

	return 0;
}
