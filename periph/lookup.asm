;; Boilerplate code for lookup procedure

        .global  lookup

        .thumb                          ; generate Thumb-2 code
        .text                           ; switch to code (ROM) section

;; Place declarations and references for variables here
        .align  4                       ; force word alignment
		;; row -> R0
		;; col -> R1
		;; size -> R2
		;; array -> R3
;; Place code for lookup here
        .align  4                       ; force word alignment
lookup: .asmfunc
        push    {LR}                    ; save return address
		MUL R1, R1, R2 ;; R0 = row * size
		ADD R0, R0, R1 ;; R0 = row * size + col
		;;MOV R1, #100 ;; R1 = 100
		;;MUL R1, R1, R1 ;; R1 = 100 * 100
size:	;;CMP R0, R1
		;;BLT return ;; if row * size + col < 100*100 go to return
		;;MOV R0, #65535 ;; else return 0xffff
		;;B done
return:	LDRB R0, [R0, R3] ;; return value of array[row*size+col]

done:   pop     {PC}                    ; return to caller

        .endasmfunc
        .end

