/*
 * THIS FILE IS AUTOMATICALLY GENERATED
 *
 * Generator:     sensirion-driver-generator 0.37.0
 * Product:       sfm_sf06
 * Model-Version: 1.5.0
 */
/*
 * Copyright (c) 2024, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "sfm_sf06_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c.h"
#include "sensirion_i2c_hal.h"

#define sensirion_hal_sleep_us sensirion_i2c_hal_sleep_usec

static uint8_t communication_buffer[18] = {0};

static uint8_t _i2c_address;
static int16_t _flow_offset = 0.0;
static int16_t _flow_scale_factor = 1.0;
static uint16_t _flow_unit = 0;

void sfm_sf06_init(uint8_t i2c_address) {
    _i2c_address = i2c_address;
}

float sfm_sf06_signal_temperature(int16_t temperature_raw) {
    float temperature = 0.0;
    temperature = temperature_raw / 200.0;
    return temperature;
}

float sfm_sf06_signal_flow(int16_t flow_raw, int16_t flow_scale_factor,
                           int16_t flow_offset) {
    float flow = 0.0;
    float scale_factor = (float)(flow_scale_factor);
    flow = (float)(flow_raw - (int32_t)(flow_offset)) / scale_factor;
    return flow;
}

int16_t sfm_sf06_start_o2_continuous_measurement() {
    int16_t local_error = 0;
    local_error = sfm_sf06_read_scale_offset_unit(
        SFM_SF06_START_O2_CONTINUOUS_MEASUREMENT_CMD_ID, &_flow_scale_factor,
        &_flow_offset, &_flow_unit);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    local_error = ll_sfm_sf06_start_o2_continuous_measurement();
    return local_error;
}

int16_t sfm_sf06_start_air_continuous_measurement() {
    int16_t local_error = 0;
    local_error = sfm_sf06_read_scale_offset_unit(
        SFM_SF06_START_AIR_CONTINUOUS_MEASUREMENT_CMD_ID, &_flow_scale_factor,
        &_flow_offset, &_flow_unit);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    local_error = ll_sfm_sf06_start_air_continuous_measurement();
    return local_error;
}

int16_t sfm_sf06_start_n2o_continuous_measurement() {
    int16_t local_error = 0;
    local_error = sfm_sf06_read_scale_offset_unit(
        SFM_SF06_START_N2O_CONTINUOUS_MEASUREMENT_CMD_ID, &_flow_scale_factor,
        &_flow_offset, &_flow_unit);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    local_error = ll_sfm_sf06_start_n2o_continuous_measurement();
    return local_error;
}

int16_t sfm_sf06_start_co2_continuous_measurement() {
    int16_t local_error = 0;
    local_error = sfm_sf06_read_scale_offset_unit(
        SFM_SF06_START_CO2_CONTINUOUS_MEASUREMENT_CMD_ID, &_flow_scale_factor,
        &_flow_offset, &_flow_unit);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    local_error = ll_sfm_sf06_start_co2_continuous_measurement();
    return local_error;
}

int16_t sfm_sf06_start_n2o_o2_continuous_measurement(uint16_t volume_fraction) {
    int16_t local_error = 0;
    local_error = sfm_sf06_read_scale_offset_unit(
        SFM_SF06_START_N2O_O2_CONTINUOUS_MEASUREMENT_CMD_ID,
        &_flow_scale_factor, &_flow_offset, &_flow_unit);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    local_error =
        ll_sfm_sf06_start_n2o_o2_continuous_measurement(volume_fraction);
    return local_error;
}

int16_t sfm_sf06_start_co2_o2_continuous_measurement(uint16_t volume_fraction) {
    int16_t local_error = 0;
    local_error = sfm_sf06_read_scale_offset_unit(
        SFM_SF06_START_CO2_O2_CONTINUOUS_MEASUREMENT_CMD_ID,
        &_flow_scale_factor, &_flow_offset, &_flow_unit);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    local_error =
        ll_sfm_sf06_start_co2_o2_continuous_measurement(volume_fraction);
    return local_error;
}

int16_t sfm_sf06_start_air_o2_continuous_measurement(uint16_t volume_fraction) {
    int16_t local_error = 0;
    local_error = sfm_sf06_read_scale_offset_unit(
        SFM_SF06_START_AIR_O2_CONTINUOUS_MEASUREMENT_CMD_ID,
        &_flow_scale_factor, &_flow_offset, &_flow_unit);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    local_error =
        ll_sfm_sf06_start_air_o2_continuous_measurement(volume_fraction);
    return local_error;
}

int16_t sfm_sf06_read_measurement_data_raw(int16_t* flow, int16_t* temperature,
                                           uint16_t* status_word) {
    int16_t local_error = 0;
    local_error =
        ll_sfm_sf06_read_measurement_data(flow, temperature, status_word);
    return local_error;
}

int16_t sfm_sf06_read_measurement_data(float* a_flow, float* a_temperature,
                                       uint16_t* a_status_word) {
    int16_t raw_flow = 0;
    int16_t raw_temp = 0;
    uint16_t status = 0u;
    int16_t local_error = 0;
    local_error =
        sfm_sf06_read_measurement_data_raw(&raw_flow, &raw_temp, &status);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    *a_flow = sfm_sf06_signal_flow(raw_flow, _flow_scale_factor, _flow_offset);
    *a_temperature = sfm_sf06_signal_temperature(raw_temp);
    *a_status_word = (uint16_t)(status);
    return local_error;
}

int16_t sfm_sf06_update_concentration(uint16_t volume_fraction) {
    int16_t local_error = 0;
    local_error = sfm_sf06_update_concentration_set(volume_fraction);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    local_error = sfm_sf06_update_concentration_activate();
    return local_error;
}

int16_t ll_sfm_sf06_start_o2_continuous_measurement() {
    int16_t local_error = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    uint16_t local_offset = 0;
    local_offset =
        sensirion_i2c_add_command16_to_buffer(buffer_ptr, local_offset, 0x3603);
    local_error =
        sensirion_i2c_write_data(_i2c_address, buffer_ptr, local_offset);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    sensirion_i2c_hal_sleep_usec(12 * 1000);
    return local_error;
}

int16_t ll_sfm_sf06_start_air_continuous_measurement() {
    int16_t local_error = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    uint16_t local_offset = 0;
    local_offset =
        sensirion_i2c_add_command16_to_buffer(buffer_ptr, local_offset, 0x3608);
    local_error =
        sensirion_i2c_write_data(_i2c_address, buffer_ptr, local_offset);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    return local_error;
}

int16_t ll_sfm_sf06_start_n2o_continuous_measurement() {
    int16_t local_error = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    uint16_t local_offset = 0;
    local_offset =
        sensirion_i2c_add_command16_to_buffer(buffer_ptr, local_offset, 0x3615);
    local_error =
        sensirion_i2c_write_data(_i2c_address, buffer_ptr, local_offset);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    sensirion_i2c_hal_sleep_usec(12 * 1000);
    return local_error;
}

int16_t ll_sfm_sf06_start_co2_continuous_measurement() {
    int16_t local_error = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    uint16_t local_offset = 0;
    local_offset =
        sensirion_i2c_add_command16_to_buffer(buffer_ptr, local_offset, 0x361e);
    local_error =
        sensirion_i2c_write_data(_i2c_address, buffer_ptr, local_offset);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    sensirion_i2c_hal_sleep_usec(12 * 1000);
    return local_error;
}

int16_t
ll_sfm_sf06_start_air_o2_continuous_measurement(uint16_t volume_fraction) {
    int16_t local_error = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    uint16_t local_offset = 0;
    local_offset =
        sensirion_i2c_add_command16_to_buffer(buffer_ptr, local_offset, 0x3632);
    local_offset = sensirion_i2c_add_uint16_t_to_buffer(
        buffer_ptr, local_offset, volume_fraction);
    local_error =
        sensirion_i2c_write_data(_i2c_address, buffer_ptr, local_offset);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    sensirion_i2c_hal_sleep_usec(12 * 1000);
    return local_error;
}

int16_t
ll_sfm_sf06_start_n2o_o2_continuous_measurement(uint16_t volume_fraction) {
    int16_t local_error = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    uint16_t local_offset = 0;
    local_offset =
        sensirion_i2c_add_command16_to_buffer(buffer_ptr, local_offset, 0x3639);
    local_offset = sensirion_i2c_add_uint16_t_to_buffer(
        buffer_ptr, local_offset, volume_fraction);
    local_error =
        sensirion_i2c_write_data(_i2c_address, buffer_ptr, local_offset);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    sensirion_i2c_hal_sleep_usec(12 * 1000);
    return local_error;
}

int16_t
ll_sfm_sf06_start_co2_o2_continuous_measurement(uint16_t volume_fraction) {
    int16_t local_error = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    uint16_t local_offset = 0;
    local_offset =
        sensirion_i2c_add_command16_to_buffer(buffer_ptr, local_offset, 0x3646);
    local_offset = sensirion_i2c_add_uint16_t_to_buffer(
        buffer_ptr, local_offset, volume_fraction);
    local_error =
        sensirion_i2c_write_data(_i2c_address, buffer_ptr, local_offset);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    sensirion_i2c_hal_sleep_usec(12 * 1000);
    return local_error;
}

int16_t ll_sfm_sf06_read_measurement_data(int16_t* flow, int16_t* temperature,
                                          uint16_t* status_word) {
    int16_t local_error = NO_ERROR;
    uint8_t buffer_ptr[9] = {0};
    local_error = sensirion_i2c_read_data_inplace(_i2c_address, buffer_ptr, 6);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    *flow = sensirion_common_bytes_to_int16_t(&buffer_ptr[0]);
    *temperature = sensirion_common_bytes_to_int16_t(&buffer_ptr[2]);
    *status_word = sensirion_common_bytes_to_uint16_t(&buffer_ptr[4]);
    return local_error;
}

int16_t sfm_sf06_update_concentration_set(uint16_t volume_fraction) {
    int16_t local_error = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    uint16_t local_offset = 0;
    local_offset =
        sensirion_i2c_add_command16_to_buffer(buffer_ptr, local_offset, 0xe17d);
    local_offset = sensirion_i2c_add_uint16_t_to_buffer(
        buffer_ptr, local_offset, volume_fraction);
    local_error =
        sensirion_i2c_write_data(_i2c_address, buffer_ptr, local_offset);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    return local_error;
}

int16_t sfm_sf06_update_concentration_activate() {
    int16_t local_error = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    uint16_t local_offset = 0;
    local_offset =
        sensirion_i2c_add_command16_to_buffer(buffer_ptr, local_offset, 0xe000);
    local_error =
        sensirion_i2c_write_data(_i2c_address, buffer_ptr, local_offset);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    return local_error;
}

int16_t sfm_sf06_stop_continuous_measurement() {
    int16_t local_error = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    uint16_t local_offset = 0;
    local_offset =
        sensirion_i2c_add_command16_to_buffer(buffer_ptr, local_offset, 0x3ff9);
    local_error =
        sensirion_i2c_write_data(_i2c_address, buffer_ptr, local_offset);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    return local_error;
}

int16_t sfm_sf06_configure_averaging(uint16_t average_window) {
    int16_t local_error = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    uint16_t local_offset = 0;
    local_offset =
        sensirion_i2c_add_command16_to_buffer(buffer_ptr, local_offset, 0x366a);
    local_offset = sensirion_i2c_add_uint16_t_to_buffer(
        buffer_ptr, local_offset, average_window);
    local_error =
        sensirion_i2c_write_data(_i2c_address, buffer_ptr, local_offset);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    return local_error;
}

int16_t sfm_sf06_read_scale_offset_unit(uint16_t command_code,
                                        int16_t* flow_scale_factor,
                                        int16_t* flow_offset,
                                        uint16_t* flow_unit) {
    int16_t local_error = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    uint16_t local_offset = 0;
    local_offset =
        sensirion_i2c_add_command16_to_buffer(buffer_ptr, local_offset, 0x3661);
    local_offset = sensirion_i2c_add_uint16_t_to_buffer(
        buffer_ptr, local_offset, command_code);
    local_error =
        sensirion_i2c_write_data(_i2c_address, buffer_ptr, local_offset);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    local_error = sensirion_i2c_read_data_inplace(_i2c_address, buffer_ptr, 6);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    *flow_scale_factor = sensirion_common_bytes_to_int16_t(&buffer_ptr[0]);
    *flow_offset = sensirion_common_bytes_to_int16_t(&buffer_ptr[2]);
    *flow_unit = sensirion_common_bytes_to_uint16_t(&buffer_ptr[4]);
    return local_error;
}

int16_t sfm_sf06_enter_sleep() {
    int16_t local_error = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    uint16_t local_offset = 0;
    local_offset =
        sensirion_i2c_add_command16_to_buffer(buffer_ptr, local_offset, 0x3677);
    local_error =
        sensirion_i2c_write_data(_i2c_address, buffer_ptr, local_offset);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    return local_error;
}

int16_t sfm_sf06_exit_sleep() {
    int16_t local_error = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    uint16_t local_offset = 0;
    local_offset =
        sensirion_i2c_add_command8_to_buffer(buffer_ptr, local_offset, 0x0);
    sensirion_i2c_write_data(_i2c_address, buffer_ptr, local_offset);
    return local_error;
}

int16_t sfm_sf06_read_product_identifier(uint32_t* product_identifier,
                                         uint8_t* serial_number,
                                         uint16_t serial_number_size) {
    int16_t local_error = NO_ERROR;
    uint8_t* buffer_ptr = communication_buffer;
    uint16_t local_offset = 0;
    local_offset =
        sensirion_i2c_add_command16_to_buffer(buffer_ptr, local_offset, 0xe102);
    local_error =
        sensirion_i2c_write_data(_i2c_address, buffer_ptr, local_offset);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    local_error = sensirion_i2c_read_data_inplace(_i2c_address, buffer_ptr, 12);
    if (local_error != NO_ERROR) {
        return local_error;
    }
    *product_identifier = sensirion_common_bytes_to_uint32_t(&buffer_ptr[0]);
    sensirion_common_copy_bytes(&buffer_ptr[4], (uint8_t*)serial_number,
                                serial_number_size);
    return local_error;
}