#ifndef YOBEMAG_ERRCODES_H
#define YOBEMAG_ERRCODES_H

typedef enum ErrorCode {
    /**
     * @brief Signal successful completion
     */
    ERR_SUCCESS,
    /**
     * @brief Signal generic failure to complete
     */
    ERR_FAILURE,
    /**
     * @brief Signal failure due to a timeout
     */
    ERR_TIMEOUT,
    /**
     * @brief Signal failure due to memory errors
     */
    ERR_MEMORY,
    /**
     * @brief Signal failure due to FS errors
     */
    ERR_FILE_IO,
} ErrorCode;

#endif /* YOBEMAG_ERRCODES_H */
