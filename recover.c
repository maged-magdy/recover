#include<stdlib.h>
#include<stdio.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        // if the user enters no or more files name
        // we print the correct usage and close the program
        printf("Usage: . / recover image\n");
        return 1;
    }
    else if (argc == 2)
    {
        // open memory card file with read attribute
        FILE* memory_card = fopen(argv[1], "r");

        if (memory_card == NULL)
        {
            // check if opening the file failed
            // if failed we notify the user then exits
            printf("image cannot be opened for reading\n");
            return 1;
        }
        else
        {
            // define a buffer dynamically allocated on the heap
            BYTE* buffer = malloc(512);
            char* filename = malloc(3);
            int counter = 0;
            FILE* Output_file = NULL;
            // reads a 512 Byte from the memory card
            while (fread(buffer, 512, 1, memory_card) == 1)
            {
                if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) && ((buffer[3] & 0xf0) == 0xe0))
                {
                    // we found the header of a JPG
                    sprintf(filename, "%03i.jpg", counter);
                    Output_file = fopen(filename, "w");
                    if (counter == 0)
                    {
                        fwrite(buffer, 512, 1, Output_file);
                        counter++;
                    }
                    else if (counter > 0)
                    {
                        fclose(Output_file);
                        sprintf(filename, "%03i.jpg", counter);
                        Output_file = fopen(filename, "w");
                        fwrite(buffer, 512, 1, Output_file);
                        counter++;
                    }
                }
                else if(Output_file != NULL)
                {
                    fwrite(buffer, 512, 1, Output_file);
                }
            }
            free(buffer);
            free(filename);
            if (Output_file != NULL)
            {
                fclose(Output_file);
            }
        }

    }
    return 0;
}