deps_config := \
	/home/ryan/esp/esp-idf/components/app_trace/Kconfig \
	/home/ryan/esp/esp-idf/components/aws_iot/Kconfig \
	/home/ryan/esp/esp-idf/components/bt/Kconfig \
	/home/ryan/esp/esp-idf/components/esp32/Kconfig \
	/home/ryan/esp/esp-idf/components/ethernet/Kconfig \
	/home/ryan/esp/esp-idf/components/fatfs/Kconfig \
	/home/ryan/esp/esp-idf/components/freertos/Kconfig \
	/home/ryan/esp/esp-idf/components/heap/Kconfig \
	/home/ryan/esp/esp-idf/components/log/Kconfig \
	/home/ryan/esp/esp-idf/components/lwip/Kconfig \
	/home/ryan/esp/esp-idf/components/mbedtls/Kconfig \
	/home/ryan/esp/esp-idf/components/openssl/Kconfig \
	/home/ryan/esp/esp-idf/components/pthread/Kconfig \
	/home/ryan/esp/esp-idf/components/spi_flash/Kconfig \
	/home/ryan/esp/esp-idf/components/spiffs/Kconfig \
	/home/ryan/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/ryan/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/ryan/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/ryan/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/ryan/projects/BibClock/firmware/main/Kconfig.projbuild \
	/home/ryan/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/ryan/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
