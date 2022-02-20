SKETCH_NAME=DramTester

PORT=/dev/cu.usbserial-0001
FBQN=arduino:avr:nano

all:
	@echo "Usage:"
	@echo "  make <build>       - build sketch for $(FBQN)"
	@echo "  make <upload>      - upload sketch to $(PORT)"
	@echo "  make <terminal>    - connect to $(PORT)"
	@echo "  make <run>"

build:
	arduino-cli compile --fqbn $(FBQN) $(SKETCH_NAME)

upload:
	arduino-cli upload -p $(PORT) --fqbn $(FBQN) $(SKETCH_NAME)

terminal:
	stty -f $(PORT) raw 9600
	cat $(PORT)

run: build upload terminal
