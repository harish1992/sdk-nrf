/*
 * Copyright (c) 2026 Nordic Semiconductor ASA.
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-4-Clause
 */

#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <stdlib.h>
#include "ironside/se/api.h"
#include <ironside/se/uicr.h>
#include <ironside/se/uicr_deploy.h>
#include <zephyr/devicetree.h>


#define SOME_ADDRESS 0x00020000
#define SOME_ADDRESS_C 0x0FFF800C

static int cmd_hello(const struct shell *sh, size_t argc, char **argv)
{
	shell_print(sh, "Hello World!");
	return 0;
}

static int cmd_uicr_lock(const struct shell *sh, size_t argc, char **argv)
{
	struct UICR *const uicr = (struct UICR *)DT_REG_ADDR(DT_NODELABEL(uicr));

	if (uicr->ERASEPROTECT == UICR_ERASEPROTECT_PALL_PROTECTED) {
		shell_print(sh, "ERASEPROTECT enabled; aborting sample to avoid bricking the device.\n");
		return 0;
	}

	int err = uicr_deploy_lock_contents();
	shell_print(sh, "Error: 0x%x", err);
}
static int cmd_sleep(const struct shell *sh, size_t argc, char **argv)
{
	int ms = atoi(argv[1]);

	shell_print(sh, "sleeping %d ms...", ms);
	k_msleep(ms);
	shell_print(sh, "awake");
	return 0;
}

static int cmd_reboot(const struct shell *sh, size_t argc, char **argv)
{
	shell_print(sh, "rebooting...");
	k_msleep(100);
	NVIC_SystemReset();
	return 0;
}

static int cmd_snapshot(const struct shell *sh, size_t argc, char **argv)
{
	shell_print(sh, "snapshot start");
	ironside_se_snapshot_capture(IRONSIDE_SE_SNAPSHOT_CAPTURE_INCREMENT_COUNTER);
	return 0;
}

static int cmd_recover(const struct shell *sh, size_t argc, char **argv)
{
	ironside_se_snapshot_recovery();
	return 0;
}

static int cmd_bfault(const struct shell *sh, size_t argc, char **argv)
{
	uint32_t *addr =  (uint32_t*)SOME_ADDRESS;
	int val = *addr;
	*addr = 0xDEEDDEAD;
    shell_print(sh, "%d", val);
    
    addr =  (uint32_t*)SOME_ADDRESS_C;
	val = *addr;
	*addr = 0xDEEDDEAD;
    shell_print(sh, "%d", val);

	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_test, SHELL_CMD(hello, NULL, "Print hello", cmd_hello),
			       SHELL_CMD_ARG(sleep, NULL, "Sleep <ms>", cmd_sleep, 2, 0),
			       SHELL_CMD(reboot, NULL, "Reboot device", cmd_reboot),
			       SHELL_CMD(capture, NULL, "Snapshot capture", cmd_snapshot),
			       SHELL_CMD(busfault, NULL, "busfault", cmd_bfault),
			       SHELL_CMD(recover, NULL, "recover", cmd_recover),
			       SHELL_CMD(lock_uicr, NULL, "uicr lock", cmd_uicr_lock),
			       SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(test, &sub_test, "Test commands", NULL);

int main(void)
{
	printk("\nUART shell ready. Type 'test help' for commands.\n");
	return 0;
}
