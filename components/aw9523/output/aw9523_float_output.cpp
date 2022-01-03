#include "aw9523_float_output.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome
{
    namespace aw9523
    {

        static const char *const TAG = "aw9523_float_channel";

        void AW9523FloatOutputChannel::write_state(float state)
        {
            const float duty_rounded = roundf(state * 0xff);
            auto duty = static_cast<uint8_t>(duty_rounded);

            // ESP_LOGW(TAG, "State %.2f => %d", state, duty);
            this->parent_->set_pin_value(this->pin_, duty);
        }

        void AW9523FloatOutputChannel::setup()
        {
            ESP_LOGD(TAG, "setup pin %d", this->pin_);

            this->parent_->setup_led_driver(this->pin_);
            this->turn_off();
        }

        void AW9523FloatOutputChannel::dump_config()
        {
            ESP_LOGCONFIG(TAG, "AW9523 PWM:");
            ESP_LOGCONFIG(TAG, "  aw9523 pin: %d", this->pin_);
            LOG_FLOAT_OUTPUT(this);
        }

    } // namespace aw9523
} // namespace esphome
