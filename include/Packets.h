#ifndef __PACKETS_H__
#define __PACKETS_H__

#include <cstdint>
#include <cassert>

struct Packet {

    enum : uint8_t {
        blink_led = 0x55,
        inactive_shutoff = 0x51,
        mic_sidetone = 0x35,
        mic_volume = 0x33,
        sound_settings = 0x28,
        connection = 0x14,
        battery = 0x18,
        unknown1 = 0x10,
        unknown2 = 0x09,
        unknown3 = 0x45
    };

    const uint8_t header = 0x06;
    const uint8_t command = 0;
    uint8_t data[29] = {0};
};

struct BlinkTransmitterLED : public Packet {

    uint8_t& m_enabled = data[1];

    BlinkTransmitterLED(const bool enabled) :
        Packet{
            .command=blink_led,
            .data = {0x01}
        }
    {
        if (enabled) {
            m_enabled = 0x02;
        }
    };
};

struct InactivityShutoff : public Packet {

    uint8_t& m_minutes = data[0];

    InactivityShutoff(const uint8_t minutes) :
        Packet{
            .command=inactive_shutoff,
        }
    {
        m_minutes = minutes;
    };
};

struct MicSidetone : public Packet {

    enum IntensityValues : uint16_t {
        disabled = 0x00,
        low = 0x04,
        medium = 0x0a,
        high =  0x12
    };

    int8_t& m_enabled = reinterpret_cast<int8_t&>(data[0]);
    uint16_t& m_intensity = reinterpret_cast<uint16_t&>(data[2]);

    MicSidetone(const bool enabled, const IntensityValues intensity) :
        Packet{
            .command=mic_sidetone,
        }
    {
        if (!enabled) { assert(intensity == disabled); }

        m_enabled = enabled;
        m_intensity = intensity;
    };
};

struct MicVolume : public Packet {
    MicVolume(const uint8_t volume) :
        Packet{
            .command=mic_volume,
            .data = {0, volume}
        }
    {
        assert(volume <= 100);
    };
};

struct SoundSetting : public Packet {
    SoundSetting() :
        Packet{
            .command=sound_settings,
        }
    {
        // Unsupported for now
        assert(false);
    };
};

struct Connection : public Packet {
    Connection() :
        Packet{
            .command=connection,
        }
    {};

    bool is_connected() {
        return data[0] == 0x03;
    }
};

struct Battery : public Packet {
    Battery() :
        Packet{
            .command=battery,
        }
    {};

    int get_charge() {
        //TODO: I think this might be wrong, and there is other data that is
        //      sometimes in the returned packet. I need to figure out what it
        //      means.
        return data[0];
    }
};

#endif //__PACKETS_H__
