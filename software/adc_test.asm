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


  
  ldx #$0
counter_loop:
  inx
  cpx #$0a
  bne counter_loop
  ldx #$ef

  sec
  lda #$40
  rol
  rol
  ror
  ror

  jmp final_run
inc_by_3:
  inx
  inx
  inx
  rts

final_run:
  ldx #$0
  jsr inc_by_3
  txa

