*=$c000

lda #$50
adc #$50 ; Value 0xA0 in the accumulator, V flag set
adc #$60 ; Value 0x00 in the accumulator, Z/C flag set
adc #$f6   ; Add -10 + 1 (C) = -9 into te ACC
adc #$14  ; Add 20 = 21/0xB

clc
sed
lda #$41
adc #$52  ; Results should be 0x93
adc #$07  ; Result should be 0

clc
lda #$26
adc #$24 ; Results should 0x50 and not 0x4A
