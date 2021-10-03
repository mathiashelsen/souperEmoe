*=$c000

start:
  jsr draw_text

msg:
  !scr "   hello world"

draw_text:
  ldx #$00

draw_loop:
  lda msg,x
  sta $05e0,x
  inx
  cpx #$0e
  bne draw_loop
  jmp draw_text
