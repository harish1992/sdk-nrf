
include(${ZEPHYR_NRF_MODULE_DIR}/sysbuild/extensions.cmake)

add_overlay_dts(uicr ${APP_DIR}/app.overlay)
add_overlay_config(uicr ${APP_DIR}/sysbuild/uicr.conf)
