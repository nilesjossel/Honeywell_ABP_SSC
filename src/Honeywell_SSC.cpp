#include "Honeywell_SSC.h"
#include <stdio.h>
#include <string.h>

Honeywell_SSC::Honeywell_SSC(i2c_inst_t* i2c, uint8_t address, float p_min, float p_max, const char* unit_string)
    : _i2c(i2c), _address(address), _p_min(p_min), _p_max(p_max) {
    set_unit(unit_string);
}

void Honeywell_SSC::set_unit(const char* unit_string) {
    if (strcmp(unit_string, "psi") == 0) {
        _unit = UNIT_PSI;
    } else if (strcmp(unit_string, "pa") == 0) {
        _unit = UNIT_PA;
    } else if (strcmp(unit_string, "kpa") == 0) {
        _unit = UNIT_KPA;
    } else if (strcmp(unit_string, "mpa") == 0) {
        _unit = UNIT_MPA;
    } else if (strcmp(unit_string, "mbar") == 0) {
        _unit = UNIT_MBAR;
    } else if (strcmp(unit_string, "bar") == 0) {
        _unit = UNIT_BAR;
    } else {
        _unit = UNIT_UNKNOWN;
    }
}

float Honeywell_SSC::raw_to_pressure(uint16_t output) {
    return ((float)(output - _output_min) * (_p_max - _p_min) / (_output_max - _output_min)) + _p_min;
}

float Honeywell_SSC::raw_to_temperature(uint16_t output) {
    return ((float)output * 200.0 / 2047.0) - 50.0;
}

void Honeywell_SSC::update() {
    uint8_t buffer[4];
    int result = i2c_read_blocking(_i2c, _address, buffer, 4, false);
    if (result != 4) {
        _status = STATUS_DIAGNOTIC;
        return;
    }

    _status = (Status)((buffer[0] >> 6) & 0x03);
    _bridge_data = ((buffer[0] & 0x3F) << 8) | buffer[1];
    _temperature_data = (buffer[2] << 3) | (buffer[3] >> 5);

    _pressure = raw_to_pressure(_bridge_data);
    _temperature = raw_to_temperature(_temperature_data);
}

const char* Honeywell_SSC::unit() const {
    switch (_unit) {
        case UNIT_PSI: return "psi";
        case UNIT_PA: return "Pa";
        case UNIT_KPA: return "kPa";
        case UNIT_MPA: return "MPa";
        case UNIT_MBAR: return "mbar";
        case UNIT_BAR: return "bar";
        default: return "unknown";
    }
}

const char* Honeywell_SSC::error_msg() const {
    switch (_status) {
        case STATUS_NOERROR: return "No error";
        case STATUS_COMMANDMODE: return "Command mode";
        case STATUS_STALEDATA: return "Stale data";
        case STATUS_DIAGNOTIC: return "Diagnostic fault";
        default: return "Unknown status";
    }
}