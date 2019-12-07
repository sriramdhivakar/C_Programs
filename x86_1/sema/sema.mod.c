#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
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
	{ 0xeb9dcc92, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x5c2c45e9, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x5f122761, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x4db54248, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x237d03ad, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x45d942a0, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0xc70469d7, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0xd981ac86, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x29c28ad8, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x9d1bde47, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x49b450a1, __VMLINUX_SYMBOL_STR(cdev_alloc) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x156a8a59, __VMLINUX_SYMBOL_STR(down_trylock) },
	{ 0x78e739aa, __VMLINUX_SYMBOL_STR(up) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x319dcd28, __VMLINUX_SYMBOL_STR(module_refcount) },
	{ 0x2ef0e183, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "A9FC88F3534584BEC0EEF2C");
