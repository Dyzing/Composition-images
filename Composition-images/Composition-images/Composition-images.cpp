// Composition-images.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <jpeglib.h>
#include <stdio.h>
#include <corona.h>

int  c;
FILE* stream, * stream2;
int main(int argc, char* argv[])
{
    errno_t err;
    err = fopen_s(&stream, "../Photos/part1.jpg", "r");
    if (err == 0)
    {
        printf("The file 'crt_fopen_s.c' was opened\n");
    }
    if (stream)
    {
        while ((c = getc(stream)) != EOF)
        {
            putchar(c);
        }
        fclose(stream);
    }
	//jpeg_stdio_dest(&cinfo, outfile);
    return 0;

}
