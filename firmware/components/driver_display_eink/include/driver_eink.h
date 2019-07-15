#ifndef DRIVER_EINK_H
#define DRIVER_EINK_H

#include <stdint.h>
#include <esp_err.h>

#include "driver_eink_types.h"

/** the width of the eink display */
#define DRIVER_EINK_WIDTH  296

/** the height of the eink display */
#define DRIVER_EINK_HEIGHT 128

/** the max number of layers used for greyscale */
#define DRIVER_EINK_MAX_LAYERS 16

#define EINK_BUFFER_SIZE DRIVER_EINK_WIDTH*DRIVER_EINK_HEIGHT

__BEGIN_DECLS

/** Initialize the eink display
 * @return ESP_OK on success; any other value indicates an error
 */
extern esp_err_t driver_eink_init();

/** driver_eink_update 'lut' settings */
enum driver_eink_lut
{
	DRIVER_EINK_LUT_CUSTOM  = -1,
	DRIVER_EINK_LUT_FULL    =  0,
	DRIVER_EINK_LUT_NORMAL  =  1,
	DRIVER_EINK_LUT_FASTER  =  2,
	DRIVER_EINK_LUT_FASTEST =  3,
	DRIVER_EINK_LUT_DEFAULT = DRIVER_EINK_LUT_FULL,
	DRIVER_EINK_LUT_MAX     = DRIVER_EINK_LUT_FASTEST,
};

/** config-settings structure */
struct driver_eink_update {
	/** lut index */
	int lut;
	/** optional lut flags */
	int lut_flags;
	/** the raw lut data if DRIVER_EINK_LUT_CUSTOM is selected */
	const struct driver_eink_lut_entry *lut_custom;
	/** raw setting for the number of dummy lines */
	int reg_0x3a;
	/** raw setting for the time per line */
	int reg_0x3b;
	/** the start column for partial-screen-updates */
	int y_start;
	/** the end column for partial-screen-updates */
	int y_end;
};

/** refresh the eink display with given config-settings
 * @param buf the raw buffer to write to the screen
 * @param upd_conf the config-settings to use
 */
extern void driver_eink_update(const uint32_t *buf, const struct driver_eink_update *upd_conf);

/** driver_eink_display flags settings */
typedef int driver_eink_flags_t;

// bitmapped flags:
/** the input buffer is 8 bits per pixel instead of 1 bit per pixel */
#define DISPLAY_FLAG_8BITPIXEL    1
/** rotate the output 180 degrees */
#define DISPLAY_FLAG_ROTATE_180   2
/** update internal buffer; use driver_eink_update() to push update to screen */
#define DISPLAY_FLAG_NO_UPDATE    4
/** ensure the screen gets a full update */
#define DISPLAY_FLAG_FULL_UPDATE  8

// fields and sizes:
/** the lut is stored in bits 8-11 */
#define DISPLAY_FLAG_LUT_BIT      8
/** the lut is stored in bits 8-11 */
#define DISPLAY_FLAG_LUT_SIZE     4

/** macro for specifying driver_eink_flags_t lut type */
#define DISPLAY_FLAG_LUT(x) ((1+(x)) << DISPLAY_FLAG_LUT_BIT)

/**
 * display image
 *
 * @param img contains the image in 1 bit per pixel or 8 bits per pixel
 * @param flags see `driver_eink_flags_t`
 */
extern void driver_eink_display(const uint8_t *img, driver_eink_flags_t flags);

/**
 * display greyscale image (hack)
 *
 * @param img contains the image in 1 bit per pixel or 8 bits per pixel
 * @param flags see `driver_eink_flags_t`
 * @param layers the number of layers used. (the max number of layers depends on te
 *    display type)
 */
extern void driver_eink_display_greyscale(const uint8_t *img, driver_eink_flags_t flags, int layers);

/**
 * go in deep sleep mode. this disables the ram in the eink chip. a wake-up is needed
 * to continue using the eink display
 */
extern void driver_eink_deep_sleep(void);

/**
 * wake-up from deep sleep mode.
 */
extern void driver_eink_wakeup(void);

__END_DECLS

#endif // DRIVER_EINK_H
