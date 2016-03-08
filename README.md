# Parallel-Sparse-Multiplication
Parallel Sparse Multiplication Using C++

Problem: Multiply Two Sparse Matrices using Parallel Threads in C++ and print the time taken for the operation. 
		Eg; 		
			Matrix A of dimension N x M
			Matrix B of dimension M x P

			the cordinates and weights of the Non Zero positions are given as input.
			K1 denotes the number of non zero coordinates in Matrix A
			K2 denotes the number of non zero coordinates in Matrix B

			x11 : X coordinate of non Zero position in Matrix A ( at (x = 1) )
			y11 : Y coordinate of Non zero position in Matrix B ( at (y = 1) )
			w11 : Weight at corresponding posiiton of Matrix A				 

Input Format:

	Input is given as a file "Input.in" which have the following:
		N   M   P
		K1  K2
		x11	  y11 	w11
		x12	  y12	  w12
		..	  ..  	..
		..	  ..  	..
		x1k1	y1k1	w1k1
		x21	  y21 	w21
		x22	  y22 	w22
		..	  ..  	..
		..	  ..  	..
		x2k2	y2k2	w2k2

Output Format:

	Output is generated as a file "Output.out" and have the following convention:
		x1	y1	w1
		x2	y2	w2
		..	..	..
		..	..	..
		xk	yk	wk
		
		Time Taken : xxx.xxxx

Note: 

	1. Change the MAX_THREADS accordingly and observe the change


