#ifndef YOBEMAG_ERRCODES_H
#define YOBEMAG_ERRCODES_H

#include <stdint.h>

typedef enum ErrorCode {
    /**
     * @brief Signal successful completion
     */
    ERR_SUCCESS = 0,
    /**
     * @brief Signal generic failure to complete
     */
    ERR_FAILURE = -1,
    /**
     * @brief Signal failure due to a timeout
     */
    ERR_TIMEOUT = -2,
    /**
     * @brief Signal failure due to memory errors
     */
    ERR_MEMORY = -3,
    /**
     * @brief Signal failure due to FS errors
     */
    ERR_FILE_IO = -4,
} ErrorCode;

#endif /* YOBEMAG_ERRCODES_H */
