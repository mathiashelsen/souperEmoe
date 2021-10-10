*=$c000

pos_x = $50   ;; [0..39]
dir_x = $51   ;; 0 = left, 1 = right 

pos_y = $52   ;; [0..25]
dir_y = $53   ;; 0 = down, 1 = up

screen_lb = $54
screen_hb = $55

delay_val = $56

SCNKEY    = $ff9f

.start:
  lda #$03
  sta pos_x
  lda #$05
  sta pos_y
  lda #01
  sta dir_x
  sta dir_y
  lda #$00
  sta screen_lb
  lda #$04
  sta screen_hb
  lda #$0f
  sta delay_val
  jmp main

.fullBox:
  !byte $51

main:
  jsr .update_position
  jsr .clear_screen
  jsr .draw_screen
  jsr .delay
  jsr SCNKEY
  jsr .keyboard_func
  jmp main
  
.update_position:
  lda pos_x
  ldx dir_x
  cpx #$0           ;; Check if the X -direction is to the left
  beq +
  jsr .move_x_right ;; If not, move to the right
  ldx #$00          ;; Branch to move_y
  cpx #$00
  beq .move_y
+ jsr .move_x_left  ;; If it is, move to the left
.move_y:
  lda pos_y
  ldx dir_y
  cpx #$0           ;; Check if the direction is to the left
  beq +
  jsr .move_y_right
  rts
+ jsr .move_y_left
  rts
                    ;; Else we move x to the right
.move_x_right:
  cmp #$27          ;; Check if we dont fall of the screen
  bne +             ;; If we do, dont move but flip
  jsr .flip_x
  jsr .move_x_left
  rts
+ clc
  adc #$01          ;; Else increase the position
  sta pos_x
  rts               ;; Return to main

.flip_x:
  txa               ;; Move direction from X->Acc
  eor #$01          ;; Flip the LSB and store
  sta dir_x
  lda pos_x
  rts
  
.move_x_left:
  cmp #$00          ;; Check if we dont fall of the screen
  bne +             ;; If we do, dont move but flip
  jsr .flip_x
  jsr .move_x_right
  rts
+ sec
  sbc #$01          ;; Else decrease the position
  sta pos_x
  rts

.move_y_right:
  cmp #$19          ;; Check if we dont fall of the screen
  bne +             ;; If we do, dont move but flip
  jsr .flip_y
  jsr .move_y_left
  rts
+ clc
  adc #$01          ;; Else increase the position
  sta pos_y
  rts               ;; Return to main

.flip_y:
  txa               ;; Move direction from X->Acc
  eor #$01          ;; Flip the LSB and store
  sta dir_y
  lda pos_y
  rts
  
.move_y_left:
  cmp #$00          ;; Check if we dont fall of the screen
  bne +             ;; If we do, dont move but flip
  jsr .flip_y
  jsr .move_y_right
  rts
+ sec
  sbc #$01          ;; Else decrease the position
  sta pos_y
  rts

.clear_screen:
  lda #$00
  sta screen_lb
  lda #$04
  sta screen_hb

  ldx #$00
.clear_screen_loop:
  jsr .clear_row_start
  clc
  lda screen_lb
  adc #$28
  sta screen_lb
  lda screen_hb
  adc #$00
  sta screen_hb
  inx
  cpx #$19
  bne .clear_screen_loop
  rts

.clear_row_start:
  ldy #$00
  lda #$20
.clear_row:
  sta (screen_lb),y
  iny
  cpy #$29
  bne .clear_row
  rts

.draw_screen:
  lda #$00          ;; Initialize the base address of the screen
  sta screen_lb     ;; to 0x0400
  lda #$04
  sta screen_hb

  lda #$00          ;; Loop over all lines
  cmp pos_y
  beq .draw_screen_x ;; If this is the line we need to draw...
.increase_screen_y:
  clc
  adc #$01          ;; If not, goto the next line 
  tax
  clc
  lda screen_lb     ;; Increase the screen address by 40
  adc #$28
  sta screen_lb
  lda screen_hb
  adc #$00
  sta screen_hb
  txa               ;; Retrieve the index
  cmp pos_y
  bne .increase_screen_y

.draw_screen_x
  ldy #$00
  cpy pos_x
  beq .fill_cursor
.increase_col:
  iny
  cpy pos_x
  bne .increase_col
.fill_cursor:
  lda .fullBox
  sta (screen_lb),y ;; Will write to address ({screen_hb, screen_lb} + y) 
  lda #$20
.final_screen_x:
  rts


.delay:
  ldy #$00
start_x_loop:
  ldx #$00
delay_x_loop:
  inx 
  cpx #$ff
  bne delay_x_loop
delay_y_loop:
  iny
  cpy delay_val
  bne start_x_loop
  rts

.keyboard_func:
  ldx $c6
  cpx #$00
  beq +
  ldy $0277
  cpy #$41
  bne +
  lda dir_x
  eor #$01
  sta dir_x
  lda dir_y
  eor #$01
  sta dir_y
+ rts
