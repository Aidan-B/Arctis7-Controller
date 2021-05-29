#include <iostream>
#include <thread>
#include <string.h>
#include <libusb-1.0/libusb.h>

#include "ArctisHeadset.h"

void InterruptListenerThread(libusb_device *device, libusb_device_handle* handle, unsigned char endpoint, const bool &listen) {


	const int reply_len = libusb_get_max_packet_size(device, endpoint);
	const unsigned char poweroff[5] = { 0x01, 0x00, 0x00, 0x00, 0x00 };
	unsigned char reply[reply_len];
	int actual_len;

	int err;

	while (listen) {
		//listen for interrupt transfer on the HID in endpoint of the headphones
		err = libusb_interrupt_transfer(handle, endpoint, reply, reply_len, &actual_len, 0);
		if (err != LIBUSB_SUCCESS) { //error occured
			std::cout << "Interupt Listener: " <<libusb_strerror(err) << std::endl;

		} else if (actual_len) {
			//We got an interrupt for the headphone poweroff signal
			if (memcmp(reply, poweroff, actual_len) == 0) {
				std::cout << "Headphones powered off";
			
			} else {
				for (int i = 0; i < actual_len; i++) //just print the raw hex data
					printf("%02x ", reply[i]);
			}
			std::cout << std::endl;	
		} else {
			std::cout << "NULL" << std::endl; //We somehow got a zero length buffer?
		}
	}
	return;
}

int main() {

	libusb_context *ctx = NULL; //a libusb session
	int ret; //for return values
	
	//setup
	ret = libusb_init(&ctx); //initialize the library for the session we just declared
	if(ret != LIBUSB_SUCCESS) {
		std::cout << libusb_strerror(ret) << std::endl;
		return 1;
	}
	libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_INFO);
	
	try
	{	
		Arctis7Headset headset(ctx);
		
		unsigned char req_one[31] = {0x06, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		unsigned char req_two[31] = {0x06, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		
		std::string input;
		do {
			headset.FeatureRequest(req_one, 31);
			headset.FeatureRequest(req_two, 31);
		} while (getline( std::cin, input ) && input.empty());
		
	}
	catch(const std::exception& e) {
		std::cerr << "Exception occured: " << e.what() << "\n";
	} catch(const libusb_error& e) {
		std::cerr << "Exception occured: " << libusb_strerror(e) << std::endl;
	} catch(const libusb_transfer_status& e) {
		std::cerr << "Exception occured: " << libusb_error_name(e) << std::endl;
	} catch (...) {
		std::cerr <<  "An unknown exception has occurred" << std::endl;
	}

	libusb_exit(ctx); //needs to be called to end the

	return 0;
}
