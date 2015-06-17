SECTIONS {
  .arduino_ctors : {
    . = ALIGN(2);
    PROVIDE(__arduino_ctors_start = .);
    SORT(.ctors.*)
    (.ctors)
    PROVIDE(__arduino_ctors_end = .);
  } > ROM
}
