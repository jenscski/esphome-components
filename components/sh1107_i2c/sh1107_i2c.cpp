#include "sh1107_i2c.h"
#include "esphome/core/log.h"

namespace esphome
{
    namespace sh1107_i2c
    {

        static const char *const TAG = "sh1107_i2c";

        void I2CSH1107::setup()
        {
            ESP_LOGCONFIG(TAG, "Setting up I2C SH1107...");

            auto err = this->write(nullptr, 0);
            if (err != i2c::ERROR_OK)
            {
                this->error_code_ = COMMUNICATION_FAILED;
                this->mark_failed();
                return;
            }

            SH1107::setup();
        }

        void I2CSH1107::dump_config()
        {
            LOG_DISPLAY("", "I2C SH1107", this);
            LOG_I2C_DEVICE(this);
            ESP_LOGCONFIG(TAG, "  Model: %s", this->model_str_());
            ESP_LOGCONFIG(TAG, "  Inverted Color: %s", YESNO(this->invert_));
            ESP_LOGCONFIG(TAG, "  Contrast: %.2f", this->contrast_);
            LOG_UPDATE_INTERVAL(this);

            if (this->error_code_ == COMMUNICATION_FAILED)
            {
                ESP_LOGE(TAG, "Communication with SH1107 failed!");
            }
        }

        void I2CSH1107::command(uint8_t value)
        {
            this->write_byte(0x00, value);
        }

        void HOT I2CSH1107::write_display_data()
        {
            uint32_t i = 0;
            for (uint8_t page = 0; page < (uint8_t)this->get_height_internal() / 8; page++)
            {
                this->command(0xB0 + page); // row
                this->command(0x00);        // lower column
                this->command(0x10);        // higher column

                for (uint8_t x = 0; x < (uint8_t)this->get_width_internal() / 16; x++)
                {
                    uint8_t data[16];
                    for (uint8_t &j : data)
                        j = this->buffer_[i++];
                    this->write_bytes(0x40, data, sizeof(data));
                }
            }
        }

    } // namespace sh1107_i2c
} // namespace esphome