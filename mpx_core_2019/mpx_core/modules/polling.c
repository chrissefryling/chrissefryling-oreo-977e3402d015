#include "polling.h"

char serial_input(char * buffer, int * count) 
{
	//int size = 0;
 	char character;
	int position = 0;
    int end_position = 0;
	char current[30];//buffer array
	while(1){
		if(*count == position) // checks if at end of buffer, if so, returns
		{ 
			return 0;
		}
		if(inb(0x3f8+5)&1){ //checks if character has been input into COM1
			character = inb(0x3f8); //COM1
			current[0] = character; //single number
			current[1] = '\0'; //end
			serial_print(current); //print number typed

			if(character == '\n' || character == '\r'){  //if enter is pressed or new line appears
				serial_print("\n"); //print new line
				return current[0]; //returns letter
			} 
			else if(character == 127 || character == '\b'){ // checks if backspace
				position--; // shifts position to left
                end_position--;
				buffer[position] = buffer[position+1]; 
				serial_print("\b \b");
			}

            
            else if (character == '[')    // checks is arrows is pressed arrows are actually '\033[A' or B or C or D
            {
                char arrowType = ' ';
                if(inb(0x3f8+5)&1)
                {
                    arrowType = inb(0x3f8); //get the A B C D to identify the arrow type
                } 

                if(arrowType == 'A')
                {
                }
                else if(arrowType == 'B')
                {
                }
                else if(arrowType == 'C')
                {
                    if(position ==end_position )
                    {
                        buffer[end_position] = ' ';
                        end_position++;
                        position++;
                    }
                    else
                    {
                        char current_a[2] = {"0"};
			            current_a[0] = buffer[position+1];
                        current_a[1] = '\0';
                        serial_print(current_a);
                        position++;
                    }
                }
                else if(arrowType == 'D')
                {
                
                        serial_print("\b");
                    if(position == end_position)
                    {
                        end_position--;
                                            }
                    else
                    {
                         serial_print("\b");
                        if(position!=0)
                        {
                            position--;
                        }

                    }

                }
                else
                {
                    serial_print("arrow is invialed");
                }


            }
			else{
                    if(position == end_position)
                    {	
                        buffer[end_position] = character;
				        end_position++;
                        position++; //increases position in buffer as letter is typed

                    }
                    else
                    {
                        buffer[position] = character;
                        position++;
                    }
						
                }
		}
	}
	return current[0]; //returns typed char
}

