*=$c000

lda #$50
adc #$50 ; Value 0xA0 in the accumulator, V flag set
adc #$60 ; Value 0x00 in the accumulator, Z/C flag set
