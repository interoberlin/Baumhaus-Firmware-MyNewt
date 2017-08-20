
AR_FOLDER = ../../bin/targets/floorsensor/app
APACHE_MYNEWT_CORE = ../../repos/apache-mynewt-core

CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld

ARS += ../../bin/targets/floorsensor/app/boot/bootutil/boot_bootutil.a
ARS += ../../bin/targets/floorsensor/app/boot/split/boot_split.a
ARS += ../../bin/targets/floorsensor/app/crypto/mbedtls/crypto_mbedtls.a
ARS += ../../bin/targets/floorsensor/app/encoding/base64/encoding_base64.a
ARS += ../../bin/targets/floorsensor/app/encoding/cborattr/encoding_cborattr.a
ARS += ../../bin/targets/floorsensor/app/encoding/tinycbor/encoding_tinycbor.a
#ARS += ../../bin/targets/floorsensor/app/hw/bsp/nrf51dk-16kbram-openocd/hw_bsp_nrf51dk-16kbram-openocd.a
ARS += ../../bin/targets/floorsensor/app/hw/cmsis-core/hw_cmsis-core.a
ARS += ../../bin/targets/floorsensor/app/hw/drivers/nimble/nrf51/hw_drivers_nimble_nrf51.a
ARS += ../../bin/targets/floorsensor/app/hw/drivers/uart/uart_hal/hw_drivers_uart_uart_hal.a
ARS += ../../bin/targets/floorsensor/app/hw/hal/hw_hal.a
ARS += ../../bin/targets/floorsensor/app/hw/mcu/nordic/nrf51xxx/hw_mcu_nordic_nrf51xxx.a
ARS += ../../bin/targets/floorsensor/app/kernel/os/kernel_os.a
#ARS += ../../bin/targets/floorsensor/app/libc/baselibc/libc_baselibc.a
ARS += ../../bin/targets/floorsensor/app/mgmt/imgmgr/mgmt_imgmgr.a
ARS += ../../bin/targets/floorsensor/app/mgmt/mgmt/mgmt_mgmt.a
ARS += ../../bin/targets/floorsensor/app/mgmt/newtmgr/mgmt_newtmgr.a
ARS += ../../bin/targets/floorsensor/app/mgmt/newtmgr/nmgr_os/mgmt_newtmgr_nmgr_os.a
ARS += ../../bin/targets/floorsensor/app/mgmt/newtmgr/transport/ble/mgmt_newtmgr_transport_ble.a
ARS += ../../bin/targets/floorsensor/app/net/nimble/controller/net_nimble_controller.a
ARS += ../../bin/targets/floorsensor/app/net/nimble/host/net_nimble_host.a
ARS += ../../bin/targets/floorsensor/app/net/nimble/host/services/ans/net_nimble_host_services_ans.a
ARS += ../../bin/targets/floorsensor/app/net/nimble/host/services/gap/net_nimble_host_services_gap.a
ARS += ../../bin/targets/floorsensor/app/net/nimble/host/services/gatt/net_nimble_host_services_gatt.a
ARS += ../../bin/targets/floorsensor/app/net/nimble/host/store/ram/net_nimble_host_store_ram.a
ARS += ../../bin/targets/floorsensor/app/net/nimble/net_nimble.a
ARS += ../../bin/targets/floorsensor/app/net/nimble/transport/ram/net_nimble_transport_ram.a
ARS += ../../bin/targets/floorsensor/app/sys/config/sys_config.a
ARS += ../../bin/targets/floorsensor/app/sys/flash_map/sys_flash_map.a
ARS += ../../bin/targets/floorsensor/app/sys/id/sys_id.a
ARS += ../../bin/targets/floorsensor/app/sys/mfg/sys_mfg.a
ARS += ../../bin/targets/floorsensor/app/sys/reboot/sys_reboot.a
ARS += ../../bin/targets/floorsensor/app/sys/stats/full/sys_stats_full.a
ARS += ../../bin/targets/floorsensor/app/sys/sysinit/sys_sysinit.a
ARS += ../../bin/targets/floorsensor/app/time/datetime/time_datetime.a
ARS += ../../bin/targets/floorsensor/app/util/mem/util_mem.a

LDFLAGS += -lgcc -nostartfiles -specs=nosys.specs -static
LDFLAGS += -T nrf51xxaa-image1.ld
LDFLAGS += -T $(APACHE_MYNEWT_CORE)/hw/mcu/nordic/nrf51xxx/nrf51.ld

all: mynewt.elf

%.elf: $(ARS)
	$(CC) $(LDFLAGS) -Wl,--whole-archive $^ -o $@
