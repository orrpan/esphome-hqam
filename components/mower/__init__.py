import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_UPDATE_INTERVAL
from esphome.components import uart, sensor, text_sensor

mower_ns = cg.esphome_ns.namespace("mower")
Automower = mower_ns.class_("Automower", cg.Component)

CONF_BATTERY_LEVEL = "battery_level"
CONF_BATTERY_USED = "battery_used"
CONF_CHARGING_TIME = "charging_time"
CONF_BATTERY_VOLTAGE = "battery_voltage"
CONF_FIRMWARE_VERSION = "firmware_version"
CONF_MODE_TEXT = "mode"
CONF_STATUS_TEXT = "status"
CONF_LAST_CODE = "last_code"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(Automower),
        cv.Required("uart_id"): cv.use_id(uart.UARTComponent),
        cv.Required(CONF_UPDATE_INTERVAL): cv.positive_time_period_milliseconds,
        cv.Optional(CONF_BATTERY_LEVEL): sensor.sensor_schema(),
        cv.Optional(CONF_BATTERY_USED): sensor.sensor_schema(),
        cv.Optional(CONF_CHARGING_TIME): sensor.sensor_schema(),
        cv.Optional(CONF_BATTERY_VOLTAGE): sensor.sensor_schema(),
        cv.Optional(CONF_FIRMWARE_VERSION): sensor.sensor_schema(),
        cv.Optional(CONF_MODE_TEXT): text_sensor.text_sensor_schema(),
        cv.Optional(CONF_STATUS_TEXT): text_sensor.text_sensor_schema(),
        cv.Optional(CONF_LAST_CODE): text_sensor.text_sensor_schema(),
    }
).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    uart_var = yield cg.get_variable(config["uart_id"])
    var = cg.new_Pvariable(config[CONF_ID], uart_var, config[CONF_UPDATE_INTERVAL])
    cg.add(var.set_update_interval(config[CONF_UPDATE_INTERVAL]))
    cg.register_component(var, config)

    if CONF_BATTERY_LEVEL in config:
        sens = yield sensor.new_sensor(config[CONF_BATTERY_LEVEL])
        cg.add(var.set_battery_level_sensor(sens))

    if CONF_BATTERY_USED in config:
        sens = yield sensor.new_sensor(config[CONF_BATTERY_USED])
        cg.add(var.set_battery_used_sensor(sens))

    if CONF_CHARGING_TIME in config:
        sens = yield sensor.new_sensor(config[CONF_CHARGING_TIME])
        cg.add(var.set_charging_time_sensor(sens))

    if CONF_BATTERY_VOLTAGE in config:
        sens = yield sensor.new_sensor(config[CONF_BATTERY_VOLTAGE])
        cg.add(var.set_battery_voltage_sensor(sens))

    if CONF_FIRMWARE_VERSION in config:
        sens = yield sensor.new_sensor(config[CONF_FIRMWARE_VERSION])
        cg.add(var.set_firmware_version_sensor(sens))

    if CONF_MODE_TEXT in config:
        sens = yield text_sensor.new_text_sensor(config[CONF_MODE_TEXT])
        cg.add(var.set_mode_text_sensor(sens))

    if CONF_STATUS_TEXT in config:
        sens = yield text_sensor.new_text_sensor(config[CONF_STATUS_TEXT])
        cg.add(var.set_status_text_sensor(sens))

    if CONF_LAST_CODE in config:
        sens = yield text_sensor.new_text_sensor(config[CONF_LAST_CODE])
        cg.add(var.set_last_code_received_text_sensor(sens))
