#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "aw9523_gpio_pin.h"

namespace esphome
{
    namespace aw9523
    {

        static const char *const TAG = "aw9523_gpio_pin";

        void AW9523GPIOPin::setup() { pin_mode(flags_); }
        void AW9523GPIOPin::pin_mode(gpio::Flags flags) {
            this->flags_ = flags;
            this->parent_->pin_mode(this->pin_, flags);
        }
        gpio::Flags AW9523GPIOPin::get_flags() const { return this->flags_; }
        bool AW9523GPIOPin::digital_read() { return this->parent_->digital_read(this->pin_) != this->inverted_; }
        void AW9523GPIOPin::digital_write(bool value) { this->parent_->digital_write(this->pin_, value != this->inverted_); }
        size_t AW9523GPIOPin::dump_summary(char *buffer, size_t len) const
        {
            return snprintf(buffer, len, "%u via AW9523", pin_);
        }
    } // namespace aw9523
} // namespace esphome
