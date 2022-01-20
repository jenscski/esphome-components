#include "sh1107_base.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome
{
    namespace sh1107_base
    {

        static const char *const TAG = "sh1107";

#define SH1107_MAX_CONTRAST 255

#define SH1107_COMMAND_DISPLAY_OFF 0xAE
#define SH1107_COMMAND_DISPLAY_ON 0xAF
#define SH1107_COMMAND_SET_DISPLAY_CLOCK_DIV 0xD5
#define SH1107_COMMAND_SET_MULTIPLEX 0xA8
#define SH1107_COMMAND_SET_DISPLAY_OFFSET_Y 0xD3
#define SH1107_COMMAND_SET_START_LINE 0x40
#define SH1107_COMMAND_CHARGE_PUMP 0x8D
#define SH1107_COMMAND_MEMORY_MODE 0x20
#define SH1107_COMMAND_SEGRE_MAP 0xA0
#define SH1107_COMMAND_COM_SCAN_INC 0xC0
#define SH1107_COMMAND_COM_SCAN_DEC 0xC8
#define SH1107_COMMAND_SET_COM_PINS 0xDA
#define SH1107_COMMAND_SET_CONTRAST 0x81
#define SH1107_COMMAND_SET_PRE_CHARGE 0xD9
#define SH1107_COMMAND_SET_VCOM_DETECT 0xDB
#define SH1107_COMMAND_DISPLAY_ALL_ON_RESUME 0xA4
#define SH1107_COMMAND_DEACTIVATE_SCROLL 0x2E
#define SH1107_COMMAND_COLUMN_ADDRESS 0x21
#define SH1107_COMMAND_PAGE_ADDRESS 0x22
#define SH1107_COMMAND_NORMAL_DISPLAY 0xA6
#define SH1107_COMMAND_INVERSE_DISPLAY 0xA7

        void SH1107::setup()
        {
            this->init_internal_(this->get_buffer_length_());

            // Turn off display during initialization (0xAE)
            this->command(SH1107_COMMAND_DISPLAY_OFF);

            this->command(0x00);
            this->command(0x10);
            this->command(0xb0);

            this->command(0xDC);
            this->command(0x00);

            // this->command(SH1107_COMMAND_SET_CONTRAST);
            // this->command(0x3F);

            this->command(SH1107_COMMAND_MEMORY_MODE);
            this->command(SH1107_COMMAND_SEGRE_MAP);

            this->command(SH1107_COMMAND_COM_SCAN_INC);

            this->command(SH1107_COMMAND_DISPLAY_ALL_ON_RESUME);

            this->command(SH1107_COMMAND_NORMAL_DISPLAY);

            this->command(SH1107_COMMAND_SET_MULTIPLEX);
            this->command(0x3f);

            switch (this->model_)
            {
            case SH1107Model::SH1107_MODEL_64_128:
                this->set_rotation(display::DISPLAY_ROTATION_90_DEGREES);

                this->command(SH1107_COMMAND_SET_DISPLAY_OFFSET_Y);
                this->command(0x60);
                break;

            case SH1107Model::SH1107_MODEL_128_128:
                this->set_rotation(display::DISPLAY_ROTATION_0_DEGREES);

                this->command(SH1107_COMMAND_SET_DISPLAY_OFFSET_Y);
                this->command(0x00);
                break;

            default:
                break;
            }

            // Set oscillator frequency to 4'b1000 with no clock division (0xD5)
            this->command(SH1107_COMMAND_SET_DISPLAY_CLOCK_DIV);
            this->command(0x41);

            this->command(SH1107_COMMAND_SET_PRE_CHARGE);
            this->command(0x22);

            this->command(SH1107_COMMAND_SET_VCOM_DETECT);
            this->command(0x35);

            // dcdc
            this->command(0xAd);
            this->command(0x8a);

            set_contrast(this->contrast_);
            set_invert(this->invert_);

            this->fill(Color::BLACK); // clear display - ensures we do not see garbage at power-on
            this->display();          // ...write buffer, which actually clears the display's memory

            delay(100);

            this->turn_on();
        }
        void SH1107::display()
        {
            this->write_display_data();
        }
        void SH1107::update()
        {
            this->do_update_();
            this->display();
        }
        void SH1107::set_contrast(float contrast)
        {
            ESP_LOGD(TAG, "Constrast %f", contrast);

            // validation
            this->contrast_ = clamp(contrast, 0.0F, 1.0F);
            // now write the new contrast level to the display (0x81)
            this->command(SH1107_COMMAND_SET_CONTRAST);
            this->command(int(SH1107_MAX_CONTRAST * (this->contrast_)));
        }

        void SH1107::set_invert(bool invert)
        {
            this->invert_ = invert;

            if (this->invert_)
                this->command(SH1107_COMMAND_INVERSE_DISPLAY);
            else
                this->command(SH1107_COMMAND_NORMAL_DISPLAY);
        }

        bool SH1107::is_on() { return this->is_on_; }
        void SH1107::turn_on()
        {
            this->command(SH1107_COMMAND_DISPLAY_ON);
            this->is_on_ = true;
        }
        void SH1107::turn_off()
        {
            this->command(SH1107_COMMAND_DISPLAY_OFF);
            this->is_on_ = false;
        }
        int SH1107::get_height_internal()
        {
            switch (this->model_)
            {
            case SH1107_MODEL_64_128:
            case SH1107_MODEL_128_128:
                return 128;
            default:
                return 0;
            }
        }
        int SH1107::get_width_internal()
        {
            switch (this->model_)
            {
            case SH1107_MODEL_64_128:
                return 64;
            case SH1107_MODEL_128_128:
                return 128;
            default:
                return 0;
            }
        }
        size_t SH1107::get_buffer_length_()
        {
            return size_t(this->get_width_internal()) * size_t(this->get_height_internal()) / 8u;
        }
        void HOT SH1107::draw_absolute_pixel_internal(int x, int y, Color color)
        {
            if (x >= this->get_width_internal() || x < 0 || y >= this->get_height_internal() || y < 0)
                return;

            uint16_t pos = x + (y / 8) * this->get_width_internal();
            uint8_t subpos = y & 0x07;
            if (color.is_on())
            {
                this->buffer_[pos] |= (1 << subpos);
            }
            else
            {
                this->buffer_[pos] &= ~(1 << subpos);
            }
        }
        void SH1107::fill(Color color)
        {
            uint8_t fill = color.is_on() ? 0xFF : 0x00;
            for (uint32_t i = 0; i < this->get_buffer_length_(); i++)
                this->buffer_[i] = fill;
        }
        const char *SH1107::model_str_()
        {
            switch (this->model_)
            {
            case SH1107_MODEL_64_128:
                return "SH1107 128x64";
            case SH1107_MODEL_128_128:
                return "SH1107 128x128";
            default:
                return "Unknown";
            }
        }

    } // namespace sh1107_base
} // namespace esphome