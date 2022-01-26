#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/i2c/i2c.h"

#define AW9523_REG_CHIPID 0x10
#define AW9523_REG_SOFTRESET 0x7F
#define AW9523_REG_INPUT0 0x00
#define AW9523_REG_INPUT1 0x01
#define AW9523_REG_OUTPUT0 0x02
#define AW9523_REG_OUTPUT1 0x03
#define AW9523_REG_CONFIG0 0x04
#define AW9523_REG_CONFIG1 0x05
#define AW9523_REG_INTENABLE0 0x06
#define AW9523_REG_INTENABLE1 0x07
#define AW9523_REG_GCR 0x11
#define AW9523_REG_LEDMODE0 0x12
#define AW9523_REG_LEDMODE1 0x13

namespace esphome
{
    namespace aw9523
    {

        class AW9523Component : public Component, public i2c::I2CDevice
        {
        public:
            AW9523Component() = default;

            float get_setup_priority() const override { return setup_priority::HARDWARE; }

            void setup() override;
            void dump_config() override;

            void loop() override;

            void set_divider(uint8_t divider);
            uint8_t get_divider();

            float get_max_current();

            void led_driver(uint8_t pin);
            void set_pin_value(uint8_t pin, uint8_t val);

            void pin_mode(uint8_t pin, gpio::Flags flags);
            void digital_write(uint8_t pin, bool bit_value);
            bool digital_read(uint8_t pin);

        private:
            uint16_t value_{};
            uint8_t divider_{};
        };

    } // namespace aw9523
} // namespace esphome
