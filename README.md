# ESPHome Components

## AW9523 GPIO Expander and LED Driver
Tested with Adafruit AW9523 GPIO Expander and LED Driver Breakout
https://www.adafruit.com/product/4886

```yaml
external_components:
  - source: github://jenscski/esphome-components@main
    components: [ aw9523 ]
```

## SH1107 OLED Driver
Tested with Adafruit FeatherWing OLED - 128x64 OLED
https://www.adafruit.com/product/4650

```yaml
external_components:
  - source: github://jenscski/esphome-components@main
    components: [ sh1107_base, sh1107_i2c ]
```
