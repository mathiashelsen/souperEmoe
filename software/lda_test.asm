*= $0000
lda #$ef
lda #$0f
lda #$00
lda #$01
sta $2f
lda #$ff
sta $11ff
lda #$00
lda $11ff
ldx #$01
lda #$02
sta $44,x
lda #$00
lda $44,x

lda #$ee      ; This is a test for addressing a la (ZP,X)
sta $1110     ;Write value 0xEE to memory address 0x1100
lda #$10
sta $22
lda #$11
sta $23
ldx #$2
lda ($20,x)   ;Read from [(0x20 + x + 1), (0x20 + x)] = [0x23, 0x22] = 0x1110

lda #$bb
sta $403a
lda #$81
sta $3104
ldy #$e9
lda ($a4),y
