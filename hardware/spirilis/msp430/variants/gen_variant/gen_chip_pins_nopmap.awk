BEGIN {
	pinnum = 1
	printf "#ifdef ARDUINO_MAIN\n"
	printf "/* GPIO pin definitions - generated from awk script\n"
	printf " *\n"
	printf " * Fields: <P1/P2/etc>, <Port Pin#>, <ADC chan#>, <NOT_A_TIMER | TxNy enum>\n"
	printf " */\n"
	printf "const gpio_pin_definition_t chip_pins[] = {\n"
	printf "\t{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // filler for index #0\n"
}

/^ *#/ {next}

/^0/ {
	printf "\t{NOT_A_PORT, 0, -1, NOT_A_TIMER},  // %d - %s\n", pinnum, $7
	pinnum++
	next
}

$3 != "-1" {
	adc[$3] = pinnum
}

$5 != "-1" {
	printf "\t{P%s, BIT%d, %d, T%d%c%d},  // %d - %s\n", $1, $2, $3, substr($4,2,1), substr($4,1,1), $5, pinnum, $7
	gpio["P" $1 "_" $2] = pinnum
	pinnum++
	next
}

{
	printf "\t{P%s, BIT%d, %d, NOT_A_TIMER},  // %d - %s\n", $1, $2, $3, pinnum, $7
	gpio["P" $1 "_" $2] = pinnum
	pinnum++
}

END {
	printf "};\n"
	printf "#else\n"
	printf "extern const gpio_pin_definition_t chip_pins[];\n"
	printf "#endif /* ARDUINO_MAIN */\n"
	printf "\n"
	printf "#define CHIP_PIN_COUNT %d\n", pinnum - 1
	printf "\n"
	printf "// ADC pins\n"
	for (chan in adc) {
		printf "static const uint16_t A%d = %d << 8;\n", chan, adc[chan]
	}

	printf "\n"

	printf "// Px_y definitions\n"
	for (pxy in gpio) {
		printf "static const uint16_t %s = %d << 8;\n", pxy, gpio[pxy]
	}
}
