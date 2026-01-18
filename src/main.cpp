#include "NewHeadset.h"
#include "TimevalDurationCast.h"

#include <libusb-1.0/libusb.h>
// #include <pulse/something.h>

#include <poll.h>
#include <stdio.h>
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


	// TODO: This error handling just exits when anything bad happens.
	//		 It should be a little more robust. (Get rid of try-throw-catch)
	try {
		// Initialize the library
		err = libusb_init(&ctx);
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


		const libusb_pollfd** pollfds = libusb_get_pollfds(ctx);
		timeval tv;
		timeval zero_tv{.tv_sec=0, .tv_usec=0};
		std::vector<pollfd> fds;
		for (const libusb_pollfd** it = pollfds; *it != nullptr; it++) {
			fds.emplace_back(pollfd{(*it)->fd, (*it)->events, 0});
		}
		libusb_free_pollfds(pollfds);

		// listen for stdin too!
		fds.emplace_back(pollfd{fileno(stdin), POLLIN, 0});

		{
			bool connected = false;
			int charge = 0;
			bool connected_updated = false;
			bool charge_updated = false;

			Headset headset(handle);
			headset.start_interrupt_listener();
			headset.set_battery_callback(
				[&charge, &charge_updated](int soc) {
					charge = soc;
					charge_updated = true;
				});
			headset.set_connection_callback(
				[&connected, &connected_updated](bool con) {
					connected = con;
					connected_updated = true;
				});

			std::chrono::time_point start = std::chrono::steady_clock::now();
			std::chrono::duration poll_period = std::chrono::seconds(5);
			std::chrono::time_point poll_timeout = start + poll_period;
			long int timeout_ms = 0;

			bool should_exit = false;
			while (!should_exit) {

				// Calculate the next timeout (from libusb or our periodic polling)
				libusb_get_next_timeout(ctx, &tv);
				std::chrono::time_point now = std::chrono::steady_clock::now();
				std::chrono::milliseconds libusb_period = std::chrono::duration_cast<std::chrono::milliseconds>(tv);
				std::chrono::time_point libusb_timeout = libusb_period + now;
				if (libusb_timeout < poll_timeout) {
					timeout_ms = std::chrono::duration_cast<std::chrono::milliseconds>(tv).count();
				} else {
					timeout_ms = std::chrono::duration_cast<std::chrono::milliseconds>(poll_timeout - now).count();
				}
				if (timeout_ms < 0) { timeout_ms = 0; }


				int ready_fds = poll(fds.data(), fds.size(), timeout_ms);

				if (ready_fds > 0) {
					for (auto fd : fds) {
						if (fd.fd == fileno(stdin) && fd.revents == POLLIN) {
							std::string input;
							getline(std::cin, input);

							if (input == "exit") {
								should_exit = true;
							} else if (input == "c") {
								headset.get_connection();
							} else if (input == "b") {
								headset.get_battery();
							}

						} else if (fd.revents != 0) {
							libusb_handle_events_timeout(ctx, &zero_tv);
						}
					}
				} else if (ready_fds == 0) { // timeout
					if (now > poll_timeout) {
						poll_timeout += poll_period;
						headset.get_battery();
						headset.get_connection();
					}
					if (now > libusb_timeout) {
						libusb_handle_events_timeout(ctx, &zero_tv);
					}
				}

				// Make use of any new data we may have received.
				if (charge_updated) {
					printf("Charge: %d\n", charge);
					charge_updated = false;
				}
				if (connected_updated) {
					printf("Connected %s\n", connected ? "true" : "false");
					connected_updated = false;
				}
			}
		}

		// Clean up any transfers that have been cancelled when Headset was destroyed
		while(poll(fds.data(),fds.size(), 100)) {
			libusb_handle_events_timeout(ctx, &zero_tv);
		}

		libusb_release_interface(handle, Headset::interface);
		if (detachedKernelDriver) {
			libusb_attach_kernel_driver(handle, Headset::interface);
		}
		libusb_close(handle);
		libusb_exit(ctx);

	}
	catch(const libusb_error& e) {
		std::cerr << "Error: " << libusb_strerror(e) << std::endl;
	}
	catch(const libusb_transfer_status& e) {
		std::cerr << "Error: " << libusb_error_name(e) << std::endl;
	}

	return 0;
}
