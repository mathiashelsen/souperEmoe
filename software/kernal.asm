*=$e000
  NOP

!align $ff9f, $ff9f
SCNKEY:
  NOP
  NOP
  RTS

!align $fffa, $fffa
NMI_VECTOR:
  !word $c000
RST_VECTOR:
  !word $c000
IRQ_VECTOR:
  !word $c000
