#include "Sdfat2.h"
#include <stdint.h>
#include "variables.hpp"
#include "preprocess.hpp"
#include "debug.hpp"

#define LINELENGTH 80
#define INIVALUESIZE 20
#define MAX_NUM_VARIABLES 200

//global variable storage
stored_var VARIABLES[MAX_NUM_VARIABLES];
int NUM_VARIABLES = 0;

bool LoadVariablesFromSDCard(void)
{
    /*Here is our internal variables that need to always be here*/

    VARIABLES[NUM_VARIABLES].type = TYPE_F32;
    VARIABLES[NUM_VARIABLES].data.f32 = 0;
    strncpy(VARIABLES[NUM_VARIABLES].name, "COM_LOOP/DS", VAR_MAX_NAME);
    NUM_VARIABLES++;

    /*Here are the ones loaded from SD card*/

    SdFatSdioEX sd_card;
    if (!sd_card.begin()) return false;
    sd_card.chvol();
    File variable_file;
    if(!variable_file.open("VARLIST.INI", O_READ)) return false;

    char input_line[LINELENGTH + 1]; //idk if you need to add 1 or not

    while(1)
    {
        if(!variable_file.available()) break;
        int readlength = variable_file.readBytesUntil('\n', input_line, LINELENGTH);
        char variable_name[VAR_MAX_NAME + 1];
        char variable_value[INIVALUESIZE + 1];

        char ini_format_string[] = "%" xstr(VAR_MAX_NAME) "[^= ] =%" xstr(INIVALUESIZE) "s";

        if(readlength == 0 || input_line[0] == '\r' || input_line[0] == ';')
        {
            //will skip: empty lines, lines with only CRLF, and lines starting with a comment (semicolon)
            DebugMsg("Var load: line skipped\n");
            continue;
        }

        if(2 == sscanf(input_line, ini_format_string, variable_name, variable_value))
        {
            DebugMsg("Var load: "); DebugMsg(variable_name); DebugMsg(" = "); DebugMsg(variable_value); DebugLn();

			//Capitalize everything
			for(unsigned int i = 0; i < strlen(variable_value); i++)
            {
                if(variable_value[i] >= 'a' && variable_value[i] <= 'z')
                {
                    variable_value[i] &= 0xDF;
                }
            }
			
            uint8_t vartype = 0;
            if(strcmp(variable_value, "F32") == 0)
                vartype = TYPE_F32;
            else if (strcmp(variable_value, "I32") == 0)
                vartype = TYPE_I32;
            else if (strcmp(variable_value, "U32") == 0)
                vartype = TYPE_U32;
            else
            {
                continue; //unrecognized variable type
            }
            
            for(unsigned int i = 0; i < strlen(variable_name); i++)
            {
                if(variable_name[i] >= 'a' && variable_name[i] <= 'z')
                {
                    variable_name[i] &= 0xDF;
                }
            }

            strncpy(VARIABLES[NUM_VARIABLES].name, variable_name, VAR_MAX_NAME);
            VARIABLES[NUM_VARIABLES].type = vartype;
            VARIABLES[NUM_VARIABLES].data.u32 = 0;

            NUM_VARIABLES++;
        }

    }

    variable_file.close();

    return true;
}