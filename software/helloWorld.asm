*=$0000

start:
  jsr draw_text

msg:
  !scr "   Hello world"

draw_text:
  ldx #$00

draw_loop:
  lda msg,x
  sta $05e0,x
  inx
  cpx #$0e
  bne draw_loop
  jsr draw_text
