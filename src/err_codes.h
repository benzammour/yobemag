#ifndef YOBEMAG_ERR_CODES_H
#define YOBEMAG_ERR_CODES_H

#include <stdint.h>
#include <SDL2/SDL.h>
#include "cli.h"
#include "logging.h"
#include "rom.h"

#define YOBEMAG_EXIT(err_code)                                                                                          \
do {                                                                                            \
    log_exit_msg(__FILE__, __LINE__, err_codes_get_msg(err_code)); \
    fflush(stdout);                                                                                   \
    fflush(stderr);                                                                                                               \
    SDL_Quit();                                                                                                            \
    rom_destroy();                                                                                    \
    exit(EXIT_FAILURE);                                                                                         \
} while (0)

typedef enum ErrorCode {
    /**
     * @brief Signal failure due to FS errors. Errors are retrieved with `strerr(errno)`.
     */
    ERR_FILE_IO = -1,
	/**
	 *
	 */
	ERR_NO_ROM_PATH,
	/**
	 *
	 */
	ERR_TOO_MANY_ARGS,
	/**
	 *
	 */
	ERR_WRONG_USAGE,
	/**
	 *
	 */
	ERR_MMAP_FAILED,
	/**
	 *
	 */
	ERR_MUNMAP_FAILED,
	/**
	 *
	 */
	ERR_STRTOL_FAILED,
} ErrorCode;

const char* err_codes_get_msg(ErrorCode err_code);

#endif //YOBEMAG_ERR_CODES_H
