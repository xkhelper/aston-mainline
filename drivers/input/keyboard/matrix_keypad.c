// SPDX-License-Identifier: GPL-2.0-only
/*
 *  GPIO driven matrix keyboard driver
 *
 *  Copyright (c) 2008 Marek Vasut <marek.vasut@gmail.com>
 *
 *  Based on corgikbd.c
 */

#include <linux/types.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/gpio.h>
<<<<<<< HEAD
#include <linux/input/matrix_keypad.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/of_platform.h>

struct matrix_keypad {
	const struct matrix_keypad_platform_data *pdata;
	struct input_dev *input_dev;
	unsigned int row_shift;

	unsigned int row_irqs[MATRIX_MAX_ROWS];
	unsigned int num_row_irqs;
=======
#include <linux/gpio/consumer.h>
#include <linux/input/matrix_keypad.h>
#include <linux/slab.h>
#include <linux/of.h>

struct matrix_keypad {
	struct input_dev *input_dev;
	unsigned int row_shift;

	unsigned int col_scan_delay_us;
	/* key debounce interval in milli-second */
	unsigned int debounce_ms;
	bool drive_inactive_cols;

	struct gpio_desc *row_gpios[MATRIX_MAX_ROWS];
	unsigned int num_row_gpios;

	struct gpio_desc *col_gpios[MATRIX_MAX_ROWS];
	unsigned int num_col_gpios;

	unsigned int row_irqs[MATRIX_MAX_ROWS];
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	DECLARE_BITMAP(wakeup_enabled_irqs, MATRIX_MAX_ROWS);

	uint32_t last_key_state[MATRIX_MAX_COLS];
	struct delayed_work work;
	spinlock_t lock;
	bool scan_pending;
	bool stopped;
};

/*
 * NOTE: If drive_inactive_cols is false, then the GPIO has to be put into
 * HiZ when de-activated to cause minmal side effect when scanning other
 * columns. In that case it is configured here to be input, otherwise it is
 * driven with the inactive value.
 */
<<<<<<< HEAD
static void __activate_col(const struct matrix_keypad_platform_data *pdata,
			   int col, bool on)
{
	bool level_on = !pdata->active_low;

	if (on) {
		gpio_direction_output(pdata->col_gpios[col], level_on);
	} else {
		gpio_set_value_cansleep(pdata->col_gpios[col], !level_on);
		if (!pdata->drive_inactive_cols)
			gpio_direction_input(pdata->col_gpios[col]);
	}
}

static void activate_col(const struct matrix_keypad_platform_data *pdata,
			 int col, bool on)
{
	__activate_col(pdata, col, on);

	if (on && pdata->col_scan_delay_us)
		udelay(pdata->col_scan_delay_us);
}

static void activate_all_cols(const struct matrix_keypad_platform_data *pdata,
			      bool on)
{
	int col;

	for (col = 0; col < pdata->num_col_gpios; col++)
		__activate_col(pdata, col, on);
}

static bool row_asserted(const struct matrix_keypad_platform_data *pdata,
			 int row)
{
	return gpio_get_value_cansleep(pdata->row_gpios[row]) ?
			!pdata->active_low : pdata->active_low;
=======
static void __activate_col(struct matrix_keypad *keypad, int col, bool on)
{
	if (on) {
		gpiod_direction_output(keypad->col_gpios[col], 1);
	} else {
		gpiod_set_value_cansleep(keypad->col_gpios[col], 0);
		if (!keypad->drive_inactive_cols)
			gpiod_direction_input(keypad->col_gpios[col]);
	}
}

static void activate_col(struct matrix_keypad *keypad, int col, bool on)
{
	__activate_col(keypad, col, on);

	if (on && keypad->col_scan_delay_us)
		udelay(keypad->col_scan_delay_us);
}

static void activate_all_cols(struct matrix_keypad *keypad, bool on)
{
	int col;

	for (col = 0; col < keypad->num_col_gpios; col++)
		__activate_col(keypad, col, on);
}

static bool row_asserted(struct matrix_keypad *keypad, int row)
{
	return gpiod_get_value_cansleep(keypad->row_gpios[row]);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
}

static void enable_row_irqs(struct matrix_keypad *keypad)
{
	int i;

<<<<<<< HEAD
	for (i = 0; i < keypad->num_row_irqs; i++)
=======
	for (i = 0; i < keypad->num_row_gpios; i++)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		enable_irq(keypad->row_irqs[i]);
}

static void disable_row_irqs(struct matrix_keypad *keypad)
{
	int i;

<<<<<<< HEAD
	for (i = 0; i < keypad->num_row_irqs; i++)
=======
	for (i = 0; i < keypad->num_row_gpios; i++)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		disable_irq_nosync(keypad->row_irqs[i]);
}

/*
 * This gets the keys from keyboard and reports it to input subsystem
 */
static void matrix_keypad_scan(struct work_struct *work)
{
	struct matrix_keypad *keypad =
		container_of(work, struct matrix_keypad, work.work);
	struct input_dev *input_dev = keypad->input_dev;
	const unsigned short *keycodes = input_dev->keycode;
<<<<<<< HEAD
	const struct matrix_keypad_platform_data *pdata = keypad->pdata;
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	uint32_t new_state[MATRIX_MAX_COLS];
	int row, col, code;

	/* de-activate all columns for scanning */
<<<<<<< HEAD
	activate_all_cols(pdata, false);

	memset(new_state, 0, sizeof(new_state));

	for (row = 0; row < pdata->num_row_gpios; row++)
		gpio_direction_input(pdata->row_gpios[row]);

	/* assert each column and read the row status out */
	for (col = 0; col < pdata->num_col_gpios; col++) {

		activate_col(pdata, col, true);

		for (row = 0; row < pdata->num_row_gpios; row++)
			new_state[col] |=
				row_asserted(pdata, row) ? (1 << row) : 0;

		activate_col(pdata, col, false);
	}

	for (col = 0; col < pdata->num_col_gpios; col++) {
=======
	activate_all_cols(keypad, false);

	memset(new_state, 0, sizeof(new_state));

	for (row = 0; row < keypad->num_row_gpios; row++)
		gpiod_direction_input(keypad->row_gpios[row]);

	/* assert each column and read the row status out */
	for (col = 0; col < keypad->num_col_gpios; col++) {

		activate_col(keypad, col, true);

		for (row = 0; row < keypad->num_row_gpios; row++)
			new_state[col] |=
				row_asserted(keypad, row) ? BIT(row) : 0;

		activate_col(keypad, col, false);
	}

	for (col = 0; col < keypad->num_col_gpios; col++) {
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		uint32_t bits_changed;

		bits_changed = keypad->last_key_state[col] ^ new_state[col];
		if (bits_changed == 0)
			continue;

<<<<<<< HEAD
		for (row = 0; row < pdata->num_row_gpios; row++) {
			if ((bits_changed & (1 << row)) == 0)
=======
		for (row = 0; row < keypad->num_row_gpios; row++) {
			if (!(bits_changed & BIT(row)))
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
				continue;

			code = MATRIX_SCAN_CODE(row, col, keypad->row_shift);
			input_event(input_dev, EV_MSC, MSC_SCAN, code);
			input_report_key(input_dev,
					 keycodes[code],
					 new_state[col] & (1 << row));
		}
	}
	input_sync(input_dev);

	memcpy(keypad->last_key_state, new_state, sizeof(new_state));

<<<<<<< HEAD
	activate_all_cols(pdata, true);
=======
	activate_all_cols(keypad, true);
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

	/* Enable IRQs again */
	spin_lock_irq(&keypad->lock);
	keypad->scan_pending = false;
	enable_row_irqs(keypad);
	spin_unlock_irq(&keypad->lock);
}

static irqreturn_t matrix_keypad_interrupt(int irq, void *id)
{
	struct matrix_keypad *keypad = id;
	unsigned long flags;

	spin_lock_irqsave(&keypad->lock, flags);

	/*
	 * See if another IRQ beaten us to it and scheduled the
	 * scan already. In that case we should not try to
	 * disable IRQs again.
	 */
	if (unlikely(keypad->scan_pending || keypad->stopped))
		goto out;

	disable_row_irqs(keypad);
	keypad->scan_pending = true;
	schedule_delayed_work(&keypad->work,
<<<<<<< HEAD
		msecs_to_jiffies(keypad->pdata->debounce_ms));
=======
			      msecs_to_jiffies(keypad->debounce_ms));
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)

out:
	spin_unlock_irqrestore(&keypad->lock, flags);
	return IRQ_HANDLED;
}

static int matrix_keypad_start(struct input_dev *dev)
{
	struct matrix_keypad *keypad = input_get_drvdata(dev);

	keypad->stopped = false;
	mb();

	/*
	 * Schedule an immediate key scan to capture current key state;
	 * columns will be activated and IRQs be enabled after the scan.
	 */
	schedule_delayed_work(&keypad->work, 0);

	return 0;
}

static void matrix_keypad_stop(struct input_dev *dev)
{
	struct matrix_keypad *keypad = input_get_drvdata(dev);

	spin_lock_irq(&keypad->lock);
	keypad->stopped = true;
	spin_unlock_irq(&keypad->lock);

	flush_delayed_work(&keypad->work);
	/*
	 * matrix_keypad_scan() will leave IRQs enabled;
	 * we should disable them now.
	 */
	disable_row_irqs(keypad);
}

static void matrix_keypad_enable_wakeup(struct matrix_keypad *keypad)
{
	int i;

<<<<<<< HEAD
	for_each_clear_bit(i, keypad->wakeup_enabled_irqs, keypad->num_row_irqs)
=======
	for_each_clear_bit(i, keypad->wakeup_enabled_irqs,
			   keypad->num_row_gpios)
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		if (enable_irq_wake(keypad->row_irqs[i]) == 0)
			__set_bit(i, keypad->wakeup_enabled_irqs);
}

static void matrix_keypad_disable_wakeup(struct matrix_keypad *keypad)
{
	int i;

<<<<<<< HEAD
	for_each_set_bit(i, keypad->wakeup_enabled_irqs, keypad->num_row_irqs) {
=======
	for_each_set_bit(i, keypad->wakeup_enabled_irqs,
			 keypad->num_row_gpios) {
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
		disable_irq_wake(keypad->row_irqs[i]);
		__clear_bit(i, keypad->wakeup_enabled_irqs);
	}
}

static int matrix_keypad_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct matrix_keypad *keypad = platform_get_drvdata(pdev);

	matrix_keypad_stop(keypad->input_dev);

	if (device_may_wakeup(&pdev->dev))
		matrix_keypad_enable_wakeup(keypad);

	return 0;
}

static int matrix_keypad_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct matrix_keypad *keypad = platform_get_drvdata(pdev);

	if (device_may_wakeup(&pdev->dev))
		matrix_keypad_disable_wakeup(keypad);

	matrix_keypad_start(keypad->input_dev);

	return 0;
}

static DEFINE_SIMPLE_DEV_PM_OPS(matrix_keypad_pm_ops,
				matrix_keypad_suspend, matrix_keypad_resume);

static int matrix_keypad_init_gpio(struct platform_device *pdev,
				   struct matrix_keypad *keypad)
{
<<<<<<< HEAD
	const struct matrix_keypad_platform_data *pdata = keypad->pdata;
	int i, irq, err;

	/* initialized strobe lines as outputs, activated */
	for (i = 0; i < pdata->num_col_gpios; i++) {
		err = devm_gpio_request(&pdev->dev,
					pdata->col_gpios[i], "matrix_kbd_col");
		if (err) {
			dev_err(&pdev->dev,
				"failed to request GPIO%d for COL%d\n",
				pdata->col_gpios[i], i);
			return err;
		}

		gpio_direction_output(pdata->col_gpios[i], !pdata->active_low);
	}

	for (i = 0; i < pdata->num_row_gpios; i++) {
		err = devm_gpio_request(&pdev->dev,
					pdata->row_gpios[i], "matrix_kbd_row");
		if (err) {
			dev_err(&pdev->dev,
				"failed to request GPIO%d for ROW%d\n",
				pdata->row_gpios[i], i);
			return err;
		}

		gpio_direction_input(pdata->row_gpios[i]);
	}

	if (pdata->clustered_irq > 0) {
		err = devm_request_any_context_irq(&pdev->dev,
				pdata->clustered_irq,
				matrix_keypad_interrupt,
				pdata->clustered_irq_flags,
				"matrix-keypad", keypad);
		if (err < 0) {
			dev_err(&pdev->dev,
				"Unable to acquire clustered interrupt\n");
			return err;
		}

		keypad->row_irqs[0] = pdata->clustered_irq;
		keypad->num_row_irqs = 1;
	} else {
		for (i = 0; i < pdata->num_row_gpios; i++) {
			irq = gpio_to_irq(pdata->row_gpios[i]);
			if (irq < 0) {
				err = irq;
				dev_err(&pdev->dev,
					"Unable to convert GPIO line %i to irq: %d\n",
					pdata->row_gpios[i], err);
				return err;
			}

			err = devm_request_any_context_irq(&pdev->dev,
					irq,
					matrix_keypad_interrupt,
					IRQF_TRIGGER_RISING |
						IRQF_TRIGGER_FALLING,
					"matrix-keypad", keypad);
			if (err < 0) {
				dev_err(&pdev->dev,
					"Unable to acquire interrupt for GPIO line %i\n",
					pdata->row_gpios[i]);
				return err;
			}

			keypad->row_irqs[i] = irq;
		}

		keypad->num_row_irqs = pdata->num_row_gpios;
=======
	bool active_low;
	int nrow, ncol;
	int err;
	int i;

	nrow = gpiod_count(&pdev->dev, "row");
	ncol = gpiod_count(&pdev->dev, "col");
	if (nrow < 0 || ncol < 0) {
		dev_err(&pdev->dev, "missing row or column GPIOs\n");
		return -EINVAL;
	}

	keypad->num_row_gpios = nrow;
	keypad->num_col_gpios = ncol;

	active_low = device_property_read_bool(&pdev->dev, "gpio-activelow");

	/* initialize strobe lines as outputs, activated */
	for (i = 0; i < keypad->num_col_gpios; i++) {
		keypad->col_gpios[i] = devm_gpiod_get_index(&pdev->dev, "col",
							    i, GPIOD_ASIS);
		err = PTR_ERR_OR_ZERO(keypad->col_gpios[i]);
		if (err) {
			dev_err(&pdev->dev,
				"failed to request GPIO for COL%d: %d\n",
				i, err);
			return err;
		}

		gpiod_set_consumer_name(keypad->col_gpios[i], "matrix_kbd_col");

		if (active_low ^ gpiod_is_active_low(keypad->col_gpios[i]))
			gpiod_toggle_active_low(keypad->col_gpios[i]);

		gpiod_direction_output(keypad->col_gpios[i], 1);
	}

	for (i = 0; i < keypad->num_row_gpios; i++) {
		keypad->row_gpios[i] = devm_gpiod_get_index(&pdev->dev, "row",
							    i, GPIOD_IN);
		err = PTR_ERR_OR_ZERO(keypad->row_gpios[i]);
		if (err) {
			dev_err(&pdev->dev,
				"failed to request GPIO for ROW%d: %d\n",
				i, err);
			return err;
		}

		gpiod_set_consumer_name(keypad->row_gpios[i], "matrix_kbd_row");

		if (active_low ^ gpiod_is_active_low(keypad->row_gpios[i]))
			gpiod_toggle_active_low(keypad->row_gpios[i]);
	}

	return 0;
}

static int matrix_keypad_setup_interrupts(struct platform_device *pdev,
					  struct matrix_keypad *keypad)
{
	int err;
	int irq;
	int i;

	for (i = 0; i < keypad->num_row_gpios; i++) {
		irq = gpiod_to_irq(keypad->row_gpios[i]);
		if (irq < 0) {
			err = irq;
			dev_err(&pdev->dev,
				"Unable to convert GPIO line %i to irq: %d\n",
				i, err);
			return err;
		}

		err = devm_request_any_context_irq(&pdev->dev, irq,
						   matrix_keypad_interrupt,
						   IRQF_TRIGGER_RISING |
							IRQF_TRIGGER_FALLING,
						   "matrix-keypad", keypad);
		if (err < 0) {
			dev_err(&pdev->dev,
				"Unable to acquire interrupt for row %i: %d\n",
				i, err);
			return err;
		}

		keypad->row_irqs[i] = irq;
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	}

	/* initialized as disabled - enabled by input->open */
	disable_row_irqs(keypad);

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_OF
static struct matrix_keypad_platform_data *
matrix_keypad_parse_dt(struct device *dev)
{
	struct matrix_keypad_platform_data *pdata;
	struct device_node *np = dev->of_node;
	unsigned int *gpios;
	int ret, i, nrow, ncol;

	if (!np) {
		dev_err(dev, "device lacks DT data\n");
		return ERR_PTR(-ENODEV);
	}

	pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata) {
		dev_err(dev, "could not allocate memory for platform data\n");
		return ERR_PTR(-ENOMEM);
	}

	pdata->num_row_gpios = nrow = gpiod_count(dev, "row");
	pdata->num_col_gpios = ncol = gpiod_count(dev, "col");
	if (nrow < 0 || ncol < 0) {
		dev_err(dev, "number of keypad rows/columns not specified\n");
		return ERR_PTR(-EINVAL);
	}

	pdata->no_autorepeat = of_property_read_bool(np, "linux,no-autorepeat");

	pdata->wakeup = of_property_read_bool(np, "wakeup-source") ||
			of_property_read_bool(np, "linux,wakeup"); /* legacy */

	pdata->active_low = of_property_read_bool(np, "gpio-activelow");

	pdata->drive_inactive_cols =
		of_property_read_bool(np, "drive-inactive-cols");

	of_property_read_u32(np, "debounce-delay-ms", &pdata->debounce_ms);
	of_property_read_u32(np, "col-scan-delay-us",
						&pdata->col_scan_delay_us);

	gpios = devm_kcalloc(dev,
			     pdata->num_row_gpios + pdata->num_col_gpios,
			     sizeof(unsigned int),
			     GFP_KERNEL);
	if (!gpios) {
		dev_err(dev, "could not allocate memory for gpios\n");
		return ERR_PTR(-ENOMEM);
	}

	for (i = 0; i < nrow; i++) {
		ret = of_get_named_gpio(np, "row-gpios", i);
		if (ret < 0)
			return ERR_PTR(ret);
		gpios[i] = ret;
	}

	for (i = 0; i < ncol; i++) {
		ret = of_get_named_gpio(np, "col-gpios", i);
		if (ret < 0)
			return ERR_PTR(ret);
		gpios[nrow + i] = ret;
	}

	pdata->row_gpios = gpios;
	pdata->col_gpios = &gpios[pdata->num_row_gpios];

	return pdata;
}
#else
static inline struct matrix_keypad_platform_data *
matrix_keypad_parse_dt(struct device *dev)
{
	dev_err(dev, "no platform data defined\n");

	return ERR_PTR(-EINVAL);
}
#endif

static int matrix_keypad_probe(struct platform_device *pdev)
{
	const struct matrix_keypad_platform_data *pdata;
	struct matrix_keypad *keypad;
	struct input_dev *input_dev;
	int err;

	pdata = dev_get_platdata(&pdev->dev);
	if (!pdata) {
		pdata = matrix_keypad_parse_dt(&pdev->dev);
		if (IS_ERR(pdata))
			return PTR_ERR(pdata);
	} else if (!pdata->keymap_data) {
		dev_err(&pdev->dev, "no keymap data defined\n");
		return -EINVAL;
	}

=======
static int matrix_keypad_probe(struct platform_device *pdev)
{
	struct matrix_keypad *keypad;
	struct input_dev *input_dev;
	bool wakeup;
	int err;

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	keypad = devm_kzalloc(&pdev->dev, sizeof(*keypad), GFP_KERNEL);
	if (!keypad)
		return -ENOMEM;

	input_dev = devm_input_allocate_device(&pdev->dev);
	if (!input_dev)
		return -ENOMEM;

	keypad->input_dev = input_dev;
<<<<<<< HEAD
	keypad->pdata = pdata;
	keypad->row_shift = get_count_order(pdata->num_col_gpios);
=======
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	keypad->stopped = true;
	INIT_DELAYED_WORK(&keypad->work, matrix_keypad_scan);
	spin_lock_init(&keypad->lock);

<<<<<<< HEAD
=======
	keypad->drive_inactive_cols =
		device_property_read_bool(&pdev->dev, "drive-inactive-cols");
	device_property_read_u32(&pdev->dev, "debounce-delay-ms",
				 &keypad->debounce_ms);
	device_property_read_u32(&pdev->dev, "col-scan-delay-us",
				 &keypad->col_scan_delay_us);

	err = matrix_keypad_init_gpio(pdev, keypad);
	if (err)
		return err;

	keypad->row_shift = get_count_order(keypad->num_col_gpios);

	err = matrix_keypad_setup_interrupts(pdev, keypad);
	if (err)
		return err;

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	input_dev->name		= pdev->name;
	input_dev->id.bustype	= BUS_HOST;
	input_dev->open		= matrix_keypad_start;
	input_dev->close	= matrix_keypad_stop;

<<<<<<< HEAD
	err = matrix_keypad_build_keymap(pdata->keymap_data, NULL,
					 pdata->num_row_gpios,
					 pdata->num_col_gpios,
=======
	err = matrix_keypad_build_keymap(NULL, NULL,
					 keypad->num_row_gpios,
					 keypad->num_col_gpios,
>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
					 NULL, input_dev);
	if (err) {
		dev_err(&pdev->dev, "failed to build keymap\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
	if (!pdata->no_autorepeat)
		__set_bit(EV_REP, input_dev->evbit);
	input_set_capability(input_dev, EV_MSC, MSC_SCAN);
	input_set_drvdata(input_dev, keypad);

	err = matrix_keypad_init_gpio(pdev, keypad);
	if (err)
		return err;

=======
	if (!device_property_read_bool(&pdev->dev, "linux,no-autorepeat"))
		__set_bit(EV_REP, input_dev->evbit);

	input_set_capability(input_dev, EV_MSC, MSC_SCAN);
	input_set_drvdata(input_dev, keypad);

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	err = input_register_device(keypad->input_dev);
	if (err)
		return err;

<<<<<<< HEAD
	device_init_wakeup(&pdev->dev, pdata->wakeup);
=======
	wakeup = device_property_read_bool(&pdev->dev, "wakeup-source") ||
		 /* legacy */
		 device_property_read_bool(&pdev->dev, "linux,wakeup");
	device_init_wakeup(&pdev->dev, wakeup);

>>>>>>> 2d5404caa8 (Linux 6.12-rc7)
	platform_set_drvdata(pdev, keypad);

	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id matrix_keypad_dt_match[] = {
	{ .compatible = "gpio-matrix-keypad" },
	{ }
};
MODULE_DEVICE_TABLE(of, matrix_keypad_dt_match);
#endif

static struct platform_driver matrix_keypad_driver = {
	.probe		= matrix_keypad_probe,
	.driver		= {
		.name	= "matrix-keypad",
		.pm	= pm_sleep_ptr(&matrix_keypad_pm_ops),
		.of_match_table = of_match_ptr(matrix_keypad_dt_match),
	},
};
module_platform_driver(matrix_keypad_driver);

MODULE_AUTHOR("Marek Vasut <marek.vasut@gmail.com>");
MODULE_DESCRIPTION("GPIO Driven Matrix Keypad Driver");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:matrix-keypad");
