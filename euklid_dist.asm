SECTION .data
	; no data

SECTION .text
	global euklid_dist
; C-Funktion: void euklid_dist(int size, double* sum, double* X, double* X_old)
;
; Parameter auf der Stack:
;	X_old 	<< ebp + 20 << Adresse von X_old
;	X 	<< ebp + 16 << Adresse von X
;	sum	<< ebp + 12 << Adresse von sum
;	size	<< ebp + 8  << Matrixgroesse

euklid_dist:
	push ebp	; neuer Stackframe
	mov ebp, esp
	
	push eax
	push ecx
	push edx

	mov ecx, 0		; Zaehler i = 0
	xorpd xmm2, xmm2	; sum = 0
	mov eax, [ebp+20]	; Adresse von X_old
	mov edx, [ebp+16]	; Adresse von X

L1:
	cmp ecx, [ebp+8]	; i mit Matrixgroesse vergleichen
	jge L2

	; sum += (X_old[i] - X[i])*(X_old[i] - X[i]);
	movupd xmm0, [eax + 8*ecx]	; X_old[i] laden
	movupd xmm1, [edx + 8*ecx]	; X[i] laden
	subpd xmm0, xmm1		; X_old[i] - X[i]
	mulpd xmm0, xmm0		; (X_old[i] - X[i])²
	addpd xmm2, xmm0		; sum += (X_old[í] - X[i])²
	add ecx, 2			;fuer 2 double gleichzeitig berechnet
	jmp L1

L2:
	; sum = xmm2[0] + xmm2[1]
	xorpd xmm3, xmm3	; xmm3 zu Null setzen
	addsd xmm3, xmm2	; nun xmm3[0] = xmm2[0]
	shufpd xmm2, xmm2, 0x1	; xmm2[0] und xmm2[1] tauschen
	addsd xmm3, xmm2	; xmm3[0] += (altes) xmm2[1], jetzt xmm2[0]

	; Ergebnis sum sichern
	mov edx, [ebp+12]	; Adresse von sum
	movsd [edx], xmm3	; xmm3[0] speichern

	pop edx
	pop ecx
	pop eax

	mov esp, ebp
	pop ebp
	ret
