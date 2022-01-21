import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sh1107_base, i2c
from esphome.const import CONF_ID, CONF_LAMBDA, CONF_PAGES

AUTO_LOAD = ["sh1107_base"]
DEPENDENCIES = ["i2c"]

sh1107_i2c = cg.esphome_ns.namespace("sh1107_i2c")
I2CSH1107 = sh1107_i2c.class_("I2CSH1107", sh1107_base.SH1107, i2c.I2CDevice)

CONFIG_SCHEMA = cv.All(
    sh1107_base.SH1107_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(I2CSH1107),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x3C)),
    cv.has_at_most_one_key(CONF_PAGES, CONF_LAMBDA),
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await sh1107_base.setup_sh1107(var, config)
    await i2c.register_i2c_device(var, config)