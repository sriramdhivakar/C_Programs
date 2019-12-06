#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xba997dd0, "module_layout" },
	{ 0xfe990052, "gpio_free" },
	{ 0xbd416da1, "gpiod_unexport" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x48dea93, "gpiod_set_raw_value" },
	{ 0x7cfad8cd, "gpiod_direction_output_raw" },
	{ 0xaf62c0cc, "gpiod_export" },
	{ 0xc88bd805, "gpio_to_desc" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

