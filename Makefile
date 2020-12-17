all: db upload

upload:
	pio run -e esp32dev -t upload

serial:
	pio run -e serial -t upload

db:
	pio run -t compiledb
