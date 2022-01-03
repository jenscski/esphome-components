#include "aw9523.h"
#include "esphome/core/log.h"

namespace esphome
{
    namespace aw9523
    {

        static const char *const TAG = "aw9523";

        void AW9523Component::setup()
        {
            ESP_LOGCONFIG(TAG, "Setting up AW9523...");

            if (this->reg(AW9523_REG_CHIPID).get() != 0x23)
            {
                this->mark_failed();
                return;
            }

            // reset
            const uint8_t data = 0x00;
            this->write_register(AW9523_REG_SOFTRESET, &data, 1);

            // all inputs
            this->reg(AW9523_REG_CONFIG0) = 0xff;
            this->reg(AW9523_REG_CONFIG1) = 0xff;

            // push pull
            this->reg(AW9523_REG_GCR) |= 0b00010000;

            // no interupt
            this->reg(AW9523_REG_INTENABLE0) = 0xff;
            this->reg(AW9523_REG_INTENABLE1) = 0xff;

            // setup input, output and led mode
            // this->write_byte_16(AW9523_REG_CONFIG0, this->config_);
            // this->write_byte_16(AW9523_REG_LEDMODE0, this->ledmode_);

            this->reg(AW9523_REG_CONFIG0) = (this->config_ & 0xff);
            this->reg(AW9523_REG_CONFIG1) = ((this->config_ >> 8) & 0xff);

            this->reg(AW9523_REG_LEDMODE0) = (this->ledmode_ & 0xff);
            this->reg(AW9523_REG_LEDMODE1) = ((this->ledmode_ >> 8) & 0xff);

            // for testing
            this->reg(0x24) = 0x1f;

            ESP_LOGCONFIG(TAG, "Config  reg %04x", this->config_);
            ESP_LOGCONFIG(TAG, "LedMode reg %04x", this->ledmode_);

            ESP_LOGCONFIG(TAG, "CID  %02x", (uint8_t)this->reg(AW9523_REG_CHIPID));
            ESP_LOGCONFIG(TAG, "GCR  %02x", (uint8_t)this->reg(AW9523_REG_GCR));

            ESP_LOGCONFIG(TAG, "CFG0 %02x", (uint8_t)this->reg(AW9523_REG_CONFIG0));
            ESP_LOGCONFIG(TAG, "CFG1 %02x", (uint8_t)this->reg(AW9523_REG_CONFIG1));

            ESP_LOGCONFIG(TAG, "LED0 %02x", (uint8_t)this->reg(AW9523_REG_LEDMODE0));
            ESP_LOGCONFIG(TAG, "LED1 %02x", (uint8_t)this->reg(AW9523_REG_LEDMODE1));

            ESP_LOGCONFIG(TAG, "INT0 %02x", (uint8_t)this->reg(AW9523_REG_INTENABLE0));
            ESP_LOGCONFIG(TAG, "INT1 %02x", (uint8_t)this->reg(AW9523_REG_INTENABLE1));
        }

        void AW9523Component::loop()
        {
            uint16_t value;
            this->read_byte_16(AW9523_REG_INPUT0, &value);

            // ESP_LOGD(TAG, "Loop, value %04x", value);
        }

        void AW9523Component::dump_config()
        {
            ESP_LOGCONFIG(TAG, "AW9523:");
            LOG_I2C_DEVICE(this)
        }

        void AW9523Component::set_pin_value(uint8_t pin, uint8_t val)
        {
            uint8_t reg{};

            // See Table 13. 256 step dimming control register
            if ((pin >= 0) && (pin <= 7))
            {
                reg = 0x24 + pin;
            }
            if ((pin >= 8) && (pin <= 11))
            {
                reg = 0x20 + pin - 8;
            }
            if ((pin >= 12) && (pin <= 15))
            {
                reg = 0x2C + pin - 12;
            }

            this->reg(reg) = val;
        }

        void AW9523Component::setup_led_driver(uint8_t pin)
        {
            if (this->is_failed())
                return;

            uint16_t value = (1 << pin);

            this->config_ &= ~value;  // clear bit
            this->ledmode_ &= ~value; // clear bit
        }

    } // namespace aw9523
} // namespace esphome
