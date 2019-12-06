#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
    .name = KBUILD_MODNAME,
    .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
    .exit = cleanup_module,
#endif
    .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
    __used
__attribute__((section("__versions"))) = {
    { 0x3b69997a, "module_layout" },
    { 0xa60f9d25, "remove_proc_entry" },
    { 0x27e1a049, "printk" },
    { 0xb4390f9a, "mcount" },
    { 0x6b2dc060, "dump_stack" },
    { 0x3021c756, "create_proc_entry" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
    "depends=";


MODULE_INFO(srcversion, "406DAC495A7B01F2D55D24A");
