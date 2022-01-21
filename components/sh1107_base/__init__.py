import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display
from esphome.const import (
    CONF_LAMBDA,
    CONF_MODEL,
    CONF_CONTRAST,
    CONF_INVERT,
)

sh1107_base_ns = cg.esphome_ns.namespace("sh1107_base")
SH1107 = sh1107_base_ns.class_("SH1107", cg.PollingComponent, display.DisplayBuffer)
SH1107Model = sh1107_base_ns.enum("SH1107Model")

MODELS = {
    "SH1107_64X128": SH1107Model.SH1107_MODEL_64_128,
    "SH1107_128X128": SH1107Model.SH1107_MODEL_128_128,
}

SH1107_MODEL = cv.enum(MODELS, upper=True, space="_")


SH1107_SCHEMA = display.FULL_DISPLAY_SCHEMA.extend(
    {
        cv.Required(CONF_MODEL): SH1107_MODEL,
        cv.Optional(CONF_CONTRAST, default=0.2): cv.percentage,
        cv.Optional(CONF_INVERT, default=False): cv.boolean,
    }
).extend(cv.polling_component_schema("1s"))


async def setup_sh1107(var, config):
    await cg.register_component(var, config)
    await display.register_display(var, config)

    cg.add(var.set_model(config[CONF_MODEL]))
    if CONF_CONTRAST in config:
        cg.add(var.init_contrast(config[CONF_CONTRAST]))
    if CONF_INVERT in config:
        cg.add(var.init_invert(config[CONF_INVERT]))
    if CONF_LAMBDA in config:
        lambda_ = await cg.process_lambda(
            config[CONF_LAMBDA], [(display.DisplayBufferRef, "it")], return_type=cg.void
        )
        cg.add(var.set_writer(lambda_))