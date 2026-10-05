// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2020-2021, The Linux Foundation. All rights reserved.
 */

#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/pinctrl/pinctrl.h>

#include "pinctrl-msm.h"
#include "pinctrl-yupik.h"

static const struct msm_pinctrl_soc_data yupik_pinctrl = {
.pins = yupik_pins,
.npins = ARRAY_SIZE(yupik_pins),
.functions = yupik_functions,
.nfunctions = ARRAY_SIZE(yupik_functions),
.groups = yupik_groups,
.ngroups = ARRAY_SIZE(yupik_groups),
.reserved_gpios = yupik_reserved_gpios,
.ngpios = 176,
.qup_regs = yupik_qup_regs,
.nqup_regs = ARRAY_SIZE(yupik_qup_regs),
.wakeirq_map = yupik_pdc_map,
.nwakeirq_map = ARRAY_SIZE(yupik_pdc_map),
};

/* By default, all the gpios that are mpm wake capable are enabled.
 * The following list disables the gpios explicitly
 */
static const unsigned int config_mpm_wake_disable_gpios[] = { 127 };

static void yupik_pinctrl_config_mpm_wake_disable_gpios(void)
{
unsigned int i;
unsigned int n_gpios = ARRAY_SIZE(config_mpm_wake_disable_gpios);

for (i = 0; i < n_gpios; i++)
msm_gpio_mpm_wake_set(config_mpm_wake_disable_gpios[i], false);
}

static int yupik_pinctrl_probe(struct platform_device *pdev)
{
const struct msm_pinctrl_soc_data *pinctrl_data;
int ret;

pinctrl_data = of_device_get_match_data(&pdev->dev);
if (!pinctrl_data)
return -EINVAL;

ret = msm_pinctrl_probe(pdev, pinctrl_data);
if (ret)
return ret;

yupik_pinctrl_config_mpm_wake_disable_gpios();

return 0;
}

static const struct of_device_id yupik_pinctrl_of_match[] = {
{ .compatible = "qcom,yupik-pinctrl", .data = &yupik_pinctrl },
{ },
};

static struct platform_driver yupik_pinctrl_driver = {
.driver = {
.name = "yupik-pinctrl",
.of_match_table = yupik_pinctrl_of_match,
},
.probe = yupik_pinctrl_probe,
.remove = msm_pinctrl_remove,
};

static int __init yupik_pinctrl_init(void)
{
return platform_driver_register(&yupik_pinctrl_driver);
}
arch_initcall(yupik_pinctrl_init);

static void __exit yupik_pinctrl_exit(void)
{
platform_driver_unregister(&yupik_pinctrl_driver);
}
module_exit(yupik_pinctrl_exit);

MODULE_DESCRIPTION("QTI yupik pinctrl driver");
MODULE_LICENSE("GPL v2");
MODULE_DEVICE_TABLE(of, yupik_pinctrl_of_match);
