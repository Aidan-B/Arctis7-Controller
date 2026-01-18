#include "NewHeadset.h"
#include "TimevalDurationCast.h"

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <libusb-1.0/libusb.h>
// #include <pulse/something.h>

#include <poll.h>
#include <stdio.h>
#include <getopt.h>
#include <iostream>

static const int default_timeout_ms = 1000;

int get_char_index(const char* str, const char search) {
    const char* c = str;
    while (*c != '\0') {
        if (*c == search) {
            return c - str;
        }
        c++;
    }
    return -1;
}

enum class HeadsetOps {
    connected,
    battery,
    blink,
    inactivity,
    sidetone,
    volume,
};

void print_help(const char* name) {
    printf(
        "Communicate with a connected Steelseries Arctis 7 wireless headset\n"
        "\n"
        "Usage: %s [options]\n"
        "\n"
        "Options:\n"
        "  -h, --help            \tDisplay this help message\n"
        "  -a, --all             \tQuery all device information\n"
        "  -q, --query <arg>     \tQuery for argument\n"
        "  -s, --set <arg=value> \tSet device argument to value\n"
        "\n"
        "Query Arguments:\n"
        "  connected  \tQuery connected status\n"
        "  battery    \tQuery current battery status\n"
        "\n"
        "Set Arguments:\n"
        "  blink      \tBlink Transmitter LED: true or false\n"
        "  inactivity \tInactivity shutoff time, in minutes. 0 for never\n"
        "  sidetone   \tSet microphone sidetone intensity: disabled, low, medium, high\n"
        "  volume     \tSet microphone volume: percentage, 0-100\n"
        "\n"
        "Example:\n"
        "  %s -q connected -s blink=true\n"
        , name, name
    );
}

std::pair<std::vector<std::pair<HeadsetOps, std::string>>, int> parse_args(int argc, char* argv[]) {
    std::pair<std::vector<std::pair<HeadsetOps, std::string>>, int> args{};
    args.second = default_timeout_ms;
    int opt;
    struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"all", no_argument, NULL, 'a'},
        {"timeout", required_argument, NULL, 't'},
        {"query", required_argument, NULL, 'q'},
        {"set", required_argument, NULL, 's'},
        {NULL, 0, NULL, 0}};

    while((opt = getopt_long(argc, argv, "haq:s:", long_options, NULL)) != -1) {
        char key[256] = {0};
        char value[256] = {0};
        switch(opt) {
            case 'h': {
                print_help(argv[0]);
                exit(0);
            }
            case 'a': {
                printf("Querying all device information\n");
                args.first.emplace_back(HeadsetOps::connected, "");
                args.first.emplace_back(HeadsetOps::battery, "");
                break;
            }
            case 't': {
                try {
                    args.second = std::stoi(optarg);
                } catch (const std::invalid_argument& _) {
                    printf("Invalid timeout %s\n", optarg);
                    exit(0);
                }
                break;
            }
            case 'q': {
                sscanf(optarg, "%255s", key);
                if (strncmp("connected", key, strlen(key)) == 0) {
                    args.first.emplace_back(HeadsetOps::connected, "");
                } else if (strncmp("battery", key, strlen(key)) == 0) {
                    args.first.emplace_back(HeadsetOps::battery, "");
                } else {
                    printf("Unknown query argument \"%s\"\n", key);
                }
                break;
            }
            case 's': {
                sscanf(optarg, "%255[^=]=%255[^=]", key, value);
                if (strncmp("blink", key, strlen(key)) == 0) {
                    args.first.emplace_back(HeadsetOps::blink, value);
                } else if (strncmp("inactivity", key, strlen(key)) == 0) {
                    args.first.emplace_back(HeadsetOps::inactivity, value);
                } else if (strncmp("sidetone", key, strlen(key)) == 0) {
                    args.first.emplace_back(HeadsetOps::sidetone, value);
                } else if (strncmp("volume", key, strlen(key)) == 0) {
                    args.first.emplace_back(HeadsetOps::volume, value);
                } else {
                    printf("Unknown set argument \"%s\"\n", key);
                }
                break;
            }
        }
    }
    return args;
}

libusb_device_handle* get_headset_handle(libusb_context* ctx, bool& detachedKernelDriver) {

    int err = 0;
    libusb_device** device_list = nullptr;
    libusb_device* device = nullptr;
	libusb_device_handle* handle = nullptr;

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
	return handle;
}

int main(int argc, char* argv[]) {

    auto[operations, timeout] = parse_args(argc, argv);
    if (operations.size() == 0)  {
        print_help(argv[0]);
        return 1;
    }

	libusb_context* ctx = nullptr;
	bool detachedKernelDriver = false;
    int err = 0;

	try {
		// Initialize the library
		err = libusb_init_context(&ctx, NULL, 0);
		if (err != LIBUSB_SUCCESS) {
			throw libusb_error(err);
		}
		libusb_device_handle* handle = get_headset_handle(ctx, detachedKernelDriver);

		const libusb_pollfd** pollfds = libusb_get_pollfds(ctx);
		timeval zero_tv{.tv_sec=0, .tv_usec=0};
		std::vector<pollfd> fds;
		for (const libusb_pollfd** it = pollfds; *it != nullptr; it++) {
			fds.emplace_back(pollfd{(*it)->fd, (*it)->events, 0});
		}
		libusb_free_pollfds(pollfds);

        {
            int remaining_ops = operations.size();
    		Headset headset(handle);
    		headset.start_interrupt_listener();

            for (auto& arg : operations) {
                switch (arg.first) {
                    case HeadsetOps::connected: {
                  		headset.set_connection_callback(
                  			[&remaining_ops](bool con) {
                  			    printf("Connected: %s\n", con ? "true" : "false");
                                  remaining_ops--;
                  			});
                        headset.get_connection();
                        break;
                    }
                    case HeadsetOps::battery: {
                  		headset.set_battery_callback(
                  			[&remaining_ops](int soc) {
                  			    printf("Charge: %d%%\n", soc);
                                  remaining_ops--;
                  			});
                        headset.get_battery();
                        break;
                    }
                    case HeadsetOps::blink: {
                        bool enable = false;
                        if ("true" == arg.second ||
                            "TRUE" == arg.second ||
                            "True" == arg.second ||
                            "1" == arg.second)
                        {
                            enable = true;
                        }
                        remaining_ops--;

                        printf("Set blink transmitter LED: %s\n", enable ? "true" : "false");
                        headset.set_blink_transmitter_led(enable);
                        libusb_handle_events_timeout_completed(ctx, &zero_tv, NULL);
                        break;
                    }
                    case HeadsetOps::inactivity: {
                        uint8_t minutes = 0;
                        try {
                            minutes = std::stoi(arg.second.c_str());
                        } catch (const std::invalid_argument& _) {}
                        remaining_ops--;

                        printf("Set inactivity shuttoff: %dmin\n", minutes);
                        headset.set_inactivity_shutoff(minutes);
                        libusb_handle_events_timeout_completed(ctx, &zero_tv, NULL);
                        break;
                    }
                    case HeadsetOps::sidetone: {
                        bool enable = false;
                        MicSidetone::IntensityValues intensity = MicSidetone::IntensityValues::disabled;

                        if ("low" == arg.second) {
                            enable = true;
                            intensity = MicSidetone::IntensityValues::low;
                            printf("Set microphone sidetone inensity: low\n");
                        } else if ("medium" == arg.second) {
                            enable = true;
                            intensity = MicSidetone::IntensityValues::medium;
                            printf("Set microphone sidetone inensity: medium\n");
                        } else if ("high" == arg.second) {
                            enable = true;
                            intensity = MicSidetone::IntensityValues::high;
                            printf("Set microphone sidetone inensity: high\n");
                        } else {
                            printf("Set microphone sidetone disabled\n");
                        }
                        remaining_ops--;

                        headset.set_mic_sidetone(enable, intensity);
                        libusb_handle_events_timeout_completed(ctx, &zero_tv, NULL);
                        break;
                    }
                    case HeadsetOps::volume: {
                        uint8_t volume = 100;
                        try {
                            volume = std::stoi(arg.second.c_str());
                        } catch (const std::invalid_argument& _) {}
                        remaining_ops--;

                        printf("Set microphone volume: %d%%\n", volume);
                        headset.set_mic_volume(volume);
  						libusb_handle_events_timeout_completed(ctx, &zero_tv, NULL);
                        break;
                    }
                }
            }

            while (poll(fds.data(), fds.size(), timeout) != 0)  {
				for (auto fd : fds) {
					if (fd.revents != 0) {
						libusb_handle_events_timeout_completed(ctx, &zero_tv, NULL);
					}
				}
				if (remaining_ops == 0) {
					break;
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
		std::cerr << "Error: (" << e << ") " << libusb_strerror(e) << std::endl;
		return 1;
	}
	catch(const libusb_transfer_status& e) {
		std::cerr << "Error: (" << e << ") " << libusb_error_name(e) << std::endl;
		return 1;
	}

	return 0;
}
