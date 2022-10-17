#include <errno.h>
#include "err_codes.h"

/******************************************************
 *** EXPOSED METHODS                                ***
 ******************************************************/

const char* err_codes_get_msg(const ErrorCode err_code) {
	switch (err_code) {
		case ERR_NO_ROM_PATH:
			return "No ROM path specified! Usage: yobemag [-l <0..4>] <ROM>";
		case ERR_TOO_MANY_ARGS:
			return "You provided too many arguments! Usage: yobemag [-l <0..4>] <ROM>";
		case ERR_WRONG_USAGE:
			return "Usage: yobemag [-l <0..4>] <ROM>";
		case ERR_FILE_IO:
			return strerror(errno);
		case ERR_MMAP_FAILED:
			return "mmap failed!";
		case ERR_MUNMAP_FAILED:
			return strerror(errno);
		case ERR_STRTOL_FAILED:
			return "strtol failed!";
		default:
			return "Unknown error code!";
	}
}
