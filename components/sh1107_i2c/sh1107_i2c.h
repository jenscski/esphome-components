#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sh1107_base/sh1107_base.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome
{
    namespace sh1107_i2c
    {

        class I2CSH1107 : public sh1107_base::SH1107, public i2c::I2CDevice
        {
        public:
            void setup() override;
            void dump_config() override;

        protected:
            void command(uint8_t value) override;
            void write_display_data() override;

            enum ErrorCode
            {
                NONE = 0,
                COMMUNICATION_FAILED
            } error_code_{NONE};
        };

    } // namespace sh1107_i2c
} // namespace esphome