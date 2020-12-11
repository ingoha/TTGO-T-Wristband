all: db
	pio run -t upload

db:
	pio run -t compiledb
