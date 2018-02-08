/**
 * Based on JulianEberius/k811fn/master/win/k811fn/k811fnCLI/k811fnCLI.cpp from
 * https://github.com/JulianEberius/k811fn
 */

#include "stdafx.h"

static const int LOGITECH = 0x046d;
static const int K810 = 0xb319;
static const int TARGET_USAGE = 1;
static const int TARGET_USAGE_PAGE = 65280;

static const unsigned char setFnLockMsg[] = { 0x10, 0xFF, 0x06, 0x15, 0x00, 0x00, 0x00 };
static const unsigned char setFnUnlockMsg[] = { 0x10, 0xFF, 0x06, 0x15, 0x01, 0x00, 0x00 };
static const int msgLength = 7;

bool sendMessage(const unsigned char msg[]) {
	int res;
	hid_device *handle;
	struct hid_device_info *devs, *cur_dev;
	bool result = false;
	res = hid_init();
	devs = hid_enumerate(LOGITECH, K810);
	cur_dev = devs;
	while (cur_dev) {
		if (cur_dev->usage == TARGET_USAGE &&
			cur_dev->usage_page == TARGET_USAGE_PAGE) {
			handle = hid_open_path(cur_dev->path);

			res = hid_write(handle, msg, msgLength);
			if (res != msgLength) {
				printf("error: %ls\n", hid_error(handle));
			}

			hid_close(handle);
			if (res < 0) {
				result = false;
				break;
			}
			else {
				result = true;
				break;
			}
		}
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);
	hid_exit();
	return result;
}

bool setFnLock(void) {
	return sendMessage(setFnLockMsg);
}

bool setFnUnlock(void) {
	return sendMessage(setFnUnlockMsg);
}

void printHelp(void) {
	printf("Usage: K810FnLock <-on|-off>\n");
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printHelp();
		return 1;
	}

	if (strcmp(argv[1], "-on") == 0) {
		return setFnLock();
	}
	else if (strcmp(argv[1], "-off") == 0) {
		return setFnUnlock();
	}
	else {
		printHelp();
		return 1;
	}

	return 0;
}
