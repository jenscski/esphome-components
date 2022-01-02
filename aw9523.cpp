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
      auto gcrreg = this->reg(AW9523_REG_GCR);
      gcrreg |= 0b00010000;

      // no interupt
      this->reg(AW9523_REG_INTENABLE0) = 0xff;
      this->reg(AW9523_REG_INTENABLE1) = 0xff;

      // ESP_LOGCONFIG(TAG, "CID  %02x", this->reg(AW9523_REG_CHIPID).get());
      // ESP_LOGCONFIG(TAG, "GCR  %02x", this->reg(AW9523_REG_GCR).get());
      // ESP_LOGCONFIG(TAG, "CFG0 %02x", this->reg(AW9523_REG_CONFIG0).get());
      // ESP_LOGCONFIG(TAG, "CFG1 %02x", this->reg(AW9523_REG_CONFIG1).get());
      // ESP_LOGCONFIG(TAG, "INT0 %02x", this->reg(AW9523_REG_INTENABLE0).get());
      // ESP_LOGCONFIG(TAG, "INT1 %02x", this->reg(AW9523_REG_INTENABLE1).get());
    }

    void AW9523Component::dump_config()
    {
      ESP_LOGCONFIG(TAG, "AW9523:");
      LOG_I2C_DEVICE(this)
    }

  } // namespace aw9523
} // namespace esphome
