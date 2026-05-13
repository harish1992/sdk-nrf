# Copyright (c) 2026 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)

target_sources(app PRIVATE 
                src/main.c
                ${ZEPHYR_HAL_NORDIC_MODULE_DIR}/ironside/se/src/uicr_deploy.c)
