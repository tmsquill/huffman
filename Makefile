all: huffencode huffdecode

clean:
	rm huffencode huffdecode

huffencode: debug.h ftable.h ftable.c htree.h htree.c pquene.h pquene.c huffman.h huffman.c huffencode.c
	gcc -o huffencode ftable.c htree.c pquene.c huffman.c huffencode.c

huffdecode: debug.h ftable.h ftable.c htree.h htree.c pquene.h pquene.c huffman.h huffman.c huffdecode.c
	gcc -o huffdecode ftable.c htree.c pquene.c huffman.c huffdecode.c
