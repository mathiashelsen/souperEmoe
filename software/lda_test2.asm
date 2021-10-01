*=$0000

lda #$51
sta $a4
lda #$3f
sta $a5
lda #$bb
sta $403a
lda #$81
sta $3104
ldy #$e9
lda ($a4),y
