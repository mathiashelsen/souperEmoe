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
