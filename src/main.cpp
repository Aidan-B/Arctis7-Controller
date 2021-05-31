#include "ArctisHeadset.h"

#include <libusb-1.0/libusb.h>
#include <iostream>

int main() {

	libusb_context *ctx = NULL; //a libusb session
	int ret; //for return values
	
	//setup
	ret = libusb_init(&ctx); //initialize the library for the session we just declared
	if(ret != LIBUSB_SUCCESS) {
		std::cout << libusb_strerror(ret) << std::endl;
		return 1;
	}
	// libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_DEBUG);
	libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_INFO);
	try
	{	
		Arctis7Headset headset(ctx);
		
		// unsigned char req_one[31] = {0x06, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		// unsigned char req_two[31] = {0x06, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		
		std::string input;
		do {
			// headset.FeatureRequest(req_one, 31);
			// headset.FeatureRequest(req_two, 31);
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
