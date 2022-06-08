#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/display/display_buffer.h"

namespace esphome
{
    namespace sh1107_base
    {

        enum SH1107Model
        {
            SH1107_MODEL_64_128 = 0,
            SH1107_MODEL_128_128,
        };

        class SH1107 : public PollingComponent, public display::DisplayBuffer
        {
        public:
            void setup() override;

            void display();

            void update() override;

            void set_model(SH1107Model model) { this->model_ = model; }
            void init_contrast(float contrast) { this->contrast_ = contrast; }
            void set_contrast(float contrast);
            void init_invert(bool invert) { this->invert_ = invert; }
            void set_invert(bool invert);
            bool is_on();
            void turn_on();
            void turn_off();
            float get_setup_priority() const override { return setup_priority::PROCESSOR; }
            void fill(Color color) override;
            
            display::DisplayType get_display_type() override { return display::DisplayType::DISPLAY_TYPE_GRAYSCALE; }

        protected:
            virtual void command(uint8_t value) = 0;
            virtual void write_display_data() = 0;

            void draw_absolute_pixel_internal(int x, int y, Color color) override;

            int get_height_internal() override;
            int get_width_internal() override;
            size_t get_buffer_length_();
            const char *model_str_();

            SH1107Model model_{SH1107_MODEL_64_128};
            bool is_on_{false};
            float contrast_{0.25};
            bool invert_{false};
        };

    } // namespace sh1107_base
} // namespace esphome
