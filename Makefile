all:cl ulf ulm

bu:
	pio run

ul:
	pio run -t upload

ulf:
	pio run -t uploadfs

ulm:
	pio run -t upload
	pio device monitor

m:
	pio device monitor

cl:
	pio run -t clean
	pio run -t erase