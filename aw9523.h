#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/i2c/i2c.h"

#define AW9523_LED_MODE 0x3 ///< Special pinMode() macro for constant current

#define AW9523_REG_CHIPID 0x10     ///< Register for hardcode chip ID
#define AW9523_REG_SOFTRESET 0x7F  ///< Register for soft resetting
#define AW9523_REG_INPUT0 0x00     ///< Register for reading input values
#define AW9523_REG_OUTPUT0 0x02    ///< Register for writing output values
#define AW9523_REG_CONFIG0 0x04    ///< Register for configuring direction
#define AW9523_REG_CONFIG1 0x05    ///< Register for configuring direction
#define AW9523_REG_INTENABLE0 0x06 ///< Register for enabling interrupt
#define AW9523_REG_INTENABLE1 0x07 ///< Register for enabling interrupt
#define AW9523_REG_GCR 0x11        ///< Register for general configuration
#define AW9523_REG_LEDMODE0 0x12   ///< Register for configuring const current
#define AW9523_REG_LEDMODE1 0x13   ///< Register for configuring const current

namespace esphome
{
    namespace aw9523
    {

        class AW9523Component : public Component, public i2c::I2CDevice
        {
        public:
            AW9523Component() = default;

            void setup() override;
            void dump_config() override;

            void loop() override;

            // void pin_mode(uint8_t pin, gpio::Flags flags);
            void setup_led_driver(uint8_t pin);

            void set_pin_value(uint8_t pin, uint8_t val);

        private:
            uint16_t config_{0xffff};
            uint16_t ledmode_{0xffff};
        };

    } // namespace aw9523
} // namespace esphome
