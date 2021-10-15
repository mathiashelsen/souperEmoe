*=$c000

pos_x                 = $50   ;; [0..39]
dir_x                 = $51   ;; 0 = left, 1 = right 

pos_y                 = $52   ;; [0..25]
dir_y                 = $53   ;; 0 = down, 1 = up

paddle_left           = $80
paddle_right          = $81
screen_lb             = $54   ;; Both are used in the (ZP),y addressing mode
screen_hb             = $55   ;; to compose the screen RAM address
screen_base           = $60   ;; We alternate between 0x400 and 0x800
delay_val             = $56
move_ball_delay       = $57
move_ball_delay_init  = $58
lives                 = $59
start_squares         = $10   ;; Each byte holds 8 boxes
squares_ctr_x         = $0a   ;; How many bytes on a row have been drawn [0..3]
squares_ctr_y         = $0b   ;; How many rows have been drawn [0..5]
square_byte           = $0c   ;; The current byte being decoded

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
  sta screen_base
  lda #$0f
  sta delay_val
  lda #$0f
  sta paddle_left
  lda #$18
  sta paddle_right
  lda #$01
  sta move_ball_delay_init
  sta move_ball_delay
  lda #$03
  sta lives
  lda #$ff
  ldy #$00
- sta start_squares,y
  iny
  cpy #$18
  bne -
  jmp main

.fullBox:
  !byte $51

.game_over_msg:
  !scr "game over"

.lives_msg:
  !scr "lives:"

main:
  jsr clear_screen
  jsr draw_screen
  jsr draw_paddle
  jsr draw_lives
  jsr draw_boxes
  jsr flip_draw_buffer
  jsr delay
  jsr SCNKEY
  jsr keyboard_func
  jsr update_ball
  jmp main

update_ball:
  ldx move_ball_delay
  dex
  beq +
  stx move_ball_delay
  rts
+ lda move_ball_delay_init
  sta move_ball_delay
  jsr detect_collisions
  jsr update_position
  rts
  
update_position:
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
  cmp #$18          ;; Check if we dont fall of the screen
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

clear_screen:
  lda #$00
  sta screen_lb
  lda screen_base
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

draw_screen:
  lda #$00          ;; Initialize the base address of the screen
  sta screen_lb     ;; to 0x0400
  lda screen_base
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


delay:
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

keyboard_func:
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
+ cpy #$71
  bne +
  jsr move_paddle_left
+ cpy #$72
  bne +
  jsr move_paddle_right
+ rts

move_paddle_left:
  lda paddle_left
  cmp #$00
  beq +
  sec
  sbc #$01
  sta paddle_left
  lda paddle_right
  sec
  sbc #$01
  sta paddle_right
+ rts

move_paddle_right:
  lda paddle_right
  cmp #$28
  beq +
  clc
  adc #$01
  sta paddle_right
  lda paddle_left
  clc
  adc #$01
  sta paddle_left
+ rts

draw_paddle:
  lda #$98
  sta screen_lb
  lda screen_base ;;#$07
  clc
  adc #$03
  sta screen_hb
  ldy #$00
  lda #$45
  cpy paddle_left
  beq .draw_paddle_active
- iny
  cpy paddle_left
  bne -
.draw_paddle_active:
  sta (screen_lb),y
  iny
  cpy paddle_right
  bne .draw_paddle_active
  rts

;; start_squares
;; squares_ctr_x
;; squares_ctr_y
draw_boxes:
  lda #$00
  sta squares_ctr_x
  sta squares_ctr_y
  lda #$00
  clc
  adc #$2a
  sta screen_lb
  lda screen_base
  adc #$00 
  sta screen_hb
  lda start_squares
  sta square_byte
--  lda #$00
  sta squares_ctr_x
- jsr .draw_single_byte
  ldx squares_ctr_x
  inx
  stx squares_ctr_x
  lda screen_lb
  clc
  adc #$08
  sta screen_lb
  lda start_squares,x
  sta square_byte
  cpx #$04
  bne -
  clc
  lda screen_lb
  adc #$08
  sta screen_lb
  lda screen_hb
  adc #$00
  sta screen_hb
  ldx squares_ctr_y
  inx
  stx squares_ctr_y
  cpx #$02
  bne --
  rts

;; Acc = byte to be drawn
.draw_single_byte:
  lda square_byte
  ldy #$0
- and #$01
  beq +
  lda #$5a
  sta (screen_lb),y
+ iny
  lda square_byte
  lsr
  sta square_byte
  cpy #$08
  bne -
  rts

detect_collisions:
  lda pos_y
  cmp #$16
  bne +
  lda pos_x
  cmp paddle_left
  bmi +
  cmp paddle_right
  bpl +
  lda dir_y
  eor #$01
  sta dir_y
+ lda pos_y
  cmp #$17
  bne +
  jsr initialize_ball
  lda lives
  sec
  sbc #$01
  beq .goto_final
  sta lives
  rts
.goto_final:
  jsr final
+ rts

initialize_ball:
  lda #$14
  sta pos_x
  lda #$15
  sta pos_y
  lda #$0
  sta dir_y
  rts

draw_lives:
  lda #$00
  sta screen_lb
  lda screen_base
  sta screen_hb
  ldy #$00
- lda .lives_msg,y
  sta (screen_lb), y 
  cpy #$6
  beq +
  iny
  bne -
+ lda lives
  clc
  adc #$30
  sta (screen_lb),y
  rts

final:
  lda #$04
  sta screen_base
  lda #$10
  sta $d018
  lda #$ef
  sta screen_lb
  lda screen_base
  clc
  adc #$01
  sta screen_hb
  ldy #$00
- lda .game_over_msg,y
  sta (screen_lb), y 
  cpy #$8
  beq +
  iny
  bne -
+ jmp final

flip_draw_buffer:
  lda screen_base
  cmp #$04
  bne +
  lda #$08
  sta screen_base
  lda #$10
  sta $d018
  rts
+ lda #$04
  sta screen_base
  lda #$20
  sta $d018
  rts
