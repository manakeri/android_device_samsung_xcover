#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *fp;
	char *buffer;
	char filename[256];
	size_t filesize;

	fp = fopen(filename , "rwb");
//	ftell(fp, filesize);

	fclose(fp);
	return 0;
}
