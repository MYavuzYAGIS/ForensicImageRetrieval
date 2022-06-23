#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>

// is use malloc, no memory leak should happen!

int main(int argc, char *argv[])
{
 
 if(argc !=2){
     fprintf(stderr,"Program takes only one parameter which is the forensic image.\n");
     return EXIT_FAILURE;
 }

FILE *forensic= fopen(argv[1], "r");
if(forensic==NULL){
    fprintf(stderr,"File %s could not be opened\n",argv[1]);
    return EXIT_FAILURE;
}

// lets define some generic values to use iteratively

int blocksize = 512;
FILE *image=NULL;  // this is for the new file.
int counter = 0; // number of founded items
char name[8]; // 000.jpg + \0
unsigned char* buff;
buff= malloc(blocksize); // This is the 512 byte buff are to read and write the chunks.

while(fread(buff, 512, 1 ,forensic)){     
    // forensici oku, 512 btylar seklinde, ve buffa yaz.
    // check the new jpeg file!
    if(buff[0]==0xff && buff[1]==0xd8 && buff[2]==0xff && (buff[3] & 0xf0)== 0xe0){
        // if magic headers are in place!
        // but what if a jpeg file is already open? we do not want to concetanete so we need to close if first!
        if(counter>0){
            fclose(image);
        }

        sprintf(name, "%03d.jpg", counter);  // name will be contigous,
        printf("name for the image %s is being created automatically\n",name);
        image = fopen(name,"w");  // open the image with the name in the write mode
        printf("opening image %s\n",name);
        printf("writing info into the image %s\n",name);
        printf("image %s is created succesfully\n",name);

        if(image== NULL){
            fclose(forensic);
            free(buff);
            fprintf(stderr,"JPEG file %s could not be created!",name);
            return EXIT_FAILURE;
        }

        counter++;   // so we have a counter incrementation, so that when new header is found, it will be zeroed again per line 42

    }

    if(counter>0){
        
        fwrite(buff, 512,1,image);
    }
}

fclose(image);
fclose(forensic);
free(buff);
return EXIT_SUCCESS;

}