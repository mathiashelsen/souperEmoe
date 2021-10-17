*     = $C000

jmp main

msg:
  !scr "hello world"

main:
  ldx #$00
- lda msg,x
  sta $4400,x
  inx
  cpx #$11
  bne -

set_background_colors:

  lda #$04
  sta $d021
  lda #$05
  sta $d022
  lda #$06
  sta $d023
  lda #$07
  sta $d024

set_multicolor:
  lda #$10
  sta $d016

set_vic_bank:
  lda #$02
  sta $dd00

  jsr copy_charset

  jmp main

!zone copy_charset

dest_ptr  = $10
src_ptr   = $12
line_ctr  = $14

copy_charset:
  lda #<charset
  sta src_ptr
  lda #>charset
  sta src_ptr+1
  lda #$00
  sta dest_ptr
  lda #$48
  sta dest_ptr+1
  ldx #$00
  ldy #$00
  lda #$00
  sta line_ctr

.copy_line:
  lda (src_ptr) , y
  sta (dest_ptr), y
  inx
  iny
  cpx #$10          ;; Copy 16 entries
  bne .copy_line

  cpy #$00          ;; Check for overflow on y
  bne .next_page
  inc src_ptr +1    ;; On overflow, increase the pointer MSB
  inc dest_ptr+1
  

.next_page:
  inc line_ctr
  lda line_ctr
  cmp 128
  beq .copy_done
  ldx #$00
  jmp .copy_line

.copy_done:
  rts

!source "charset.asm"
