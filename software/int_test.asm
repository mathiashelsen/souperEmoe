*=$c000
  ldy #$00
  ldx #$00
  lda #$00

.init_x:
  ldx #$00

.loop_x:
  cmp #$01
  beq .loop_y
  inx
  cpx #$09
  beq .init_x
  bne .loop_x

.init_y:
  ldy #$00

.loop_y:
  cmp #$00
  beq .loop_x
  iny
  cpy #$09
  beq .init_y
  bne .loop_y

!align $cf00,$cf00
.isr:
  eor #$01
  rti
