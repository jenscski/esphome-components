import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_PIN, CONF_ID
from .. import AW9523Component, aw9523_ns, CONF_AW9523

DEPENDENCIES = ["aw9523"]

AW9523FloatOutputChannel = aw9523_ns.class_("AW9523FloatOutputChannel", output.FloatOutput, cg.Component)

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
        cv.Required(CONF_ID): cv.declare_id(AW9523FloatOutputChannel),
        cv.Required(CONF_AW9523): cv.use_id(AW9523Component),
        cv.Required(CONF_PIN): cv.int_range(min=0, max=15),
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_AW9523])
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await output.register_output(var, config)
    cg.add(var.set_pin(config[CONF_PIN]))
    cg.add(var.set_parent(parent))
