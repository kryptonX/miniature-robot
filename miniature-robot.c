/*
Copyright (C) 2012 Edward Chernysh

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// This is a decremented version of Brainfuck. A "loopless" version of Brainfuck

// Relies on:
/*
          - kcsc.exe
          - adlock.exe
		  - bfxt.exe
*/

/** 
    BF.net is a compiler for BrainfuckXT (http://esolangs.org/wiki/BrainfuckXT) targeting:
          - the .NET framework
          - Microsoft Windows
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>

#define MAXSIZE 30000
#define STRING_MAXSIZE 10000

#ifndef FILE_OPERATORS
#define FILE_END (0)
#define EXISTS (1)
#endif

const char *csharp_begin = "using System;\nusing System.Collections.Generic;\n\nclass Program\n{\n\tstatic void Main(string[] args)\n\t{\n\t\tList<Int32> var = new List<int>();\n\t\tList<string> str = new List<string>();\n\t\tint index = 0, strdex = 0, indexer = 0;\n\t\tfor (int i = 0; i <= 30000; i++)\n\t\t\tvar.Add(0);\n\t\tfor (int i = 0; i < 10000; i++)\n\t\t\tstr.Add(\"\");\n";
const char *csharp_end = "\n\t}\n};";
int file_exists(char*);
void die(const char*, ...);
int fprintln(char*, const char*);    // \n included

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
       printf("error: no input file\n");          
    }
    else if (argc == 2)           // single file
    {
       FILE *f = fopen(argv[1], "r");
       if (f == 0)
          die("error: could not open file.");
       else 
       {
            char *fline, *newf = strcat(argv[1], ".cs");
            unsigned int i;
            short q = 0, ex = 0;
            if (file_exists(newf) == EXISTS)
               remove(newf);
            FILE *cs = fopen(newf, "w");            
            // <usage>
            unsigned int index = 0, strdex = 0, indexer = 0;
            // </usage>
            fprintf(cs, csharp_begin);
            while (fgets(fline, sizeof(fline), f) != FILE_END)
            {
                  for (i = 0; i < strlen(fline); i++)
                  {
                      switch(fline[i])
                      {
                            case '>':    // increment pointer
                                 if (index < MAXSIZE) 
                                 {                             
                                      fprintf(cs, "\t\tindex++;\n");
                                      index++;
                                 }
                            break;
                            case '<':   // decrement pointer
                                 if (index > 0)
                                 {
                                     fprintf(cs, "\t\tindex--;\n");
                                     index--;           
                                 }
                            break;
                            case ')':  // increment string pointer
                                 if (strdex < STRING_MAXSIZE)
                                 {
                                     fprintf(cs, "\t\tstrdex++;\n");
                                     strdex++;
                                 }
                            break;
                            case '(': // decrement string pointer
                                 if (strdex > 0)
                                 {
                                     fprintf(cs, "\t\tstrdex--;\n");
                                     strdex--;           
                                 }
                            break;
                            case '+': // increment current slot
                                 fprintf(cs, "\t\tvar[index]++;\n");   
                            break;      
                            case '-': // decrement current slot
                                 fprintf(cs, "\t\tvar[index]--;\n");
                            break;                                  
                            case '.':
                                 fprintf(cs, "\t\tSystem.Console.Write((char)var[index]);\n");
                            break;                      
                            case ',':
                                 fprintf(cs, "\t\tvar[index] = System.Console.Read();\n");
                            break;
                            case '[':
                                 fprintf(cs, "\t\twhile (var[index] != 0)\n\t\t{\n");
                            break;
                            case ']':
                                 fprintf(cs, "\t\t}\n");
                            break;
                            case '&':
                                 fprintf(cs, "\t\tstr[strdex] = System.Console.ReadLine();\n");
                            break;
                            case '$':
                                 fprintf(cs, "\t\tSystem.Console.Write(str[strdex].ToString());\n");
                            break;                            
                            case '?':
                                 if (q == 0)
                                 {
                                       fprintf(cs, "\t\tif (var[index] == 0)\n\t\t{\n");
                                       q = 1;
                                 }
                                 else if (q == 1)
                                 {
                                       fprintf(cs, "\t\t}\n");
                                       q = 0;
                                 }
                            break;
                            case '!':
                                 if (q == 0)
                                 {
                                      fprintf(cs, "\t\tif (var[index] != 0)\n\t\t{\n");
                                      q = 1;
                                 }
                                 else if (q == 1)
                                 {
                                      fprintf(cs, "\t\t}\n");
                                      q = 0;
                                 }
                            break;
                            case ':':
                                 fprintf(cs, "\t\tstr[strdex] = ((char)(var[index])).ToString();\n");
                            break;
                            case ';':
                                 fprintf(cs, "\t\tstr[strdex] += ((char)(var[index]));\n");
                            break;
                            case '{':
                                 // truncated                                    
                            break;
                            case '}':
                                 // truncated                                     
                            break;
                            case '*':    // increment indexing integer
                                 fprintf(cs, "\t\tindexer++;\n");
                                 indexer++;
                            break;
                            case '~':
                                 if (indexer > 0)
                                    fprintf(cs, "\t\tindexer--;\n");
                            break; 
                            case '|':   // print character from string
                                    fprintf(cs, "\t\tSystem.Console.Write((str[strdex])[indexer]);\n");
                            break;
                            case '%':           
                                    fprintf(cs, "\t\tusing (System.IO.StreamWriter wObj = new System.IO.StreamWriter(str[strdex]))\n\t\t{\n\t\t\twObj.WriteLine(str[strdex + 1]);\n\t\t\twObj.Dispose();\n\t\t}\n");
                            break;
                            case '#':
                            break;
                      };
                  }                                                                              
            }            
            fprintf(cs, csharp_end);
            fclose(cs);
            fclose(f);
            if (file_exists("a.exe") == 1)
               remove("a.exe");
            char *location = "bin\\kcsc.exe -c a.cs";            
            if (file_exists("a.cs") == 1)
               remove("a.cs");
            rename(newf, "a.cs");
            strcat(newf, ".exe");
            system(location);
            remove("a.cs");
            if (file_exists(newf) == 1)
               remove(newf);             
            rename("a.exe", newf);                   
       }
    }
    return EXIT_SUCCESS;
}

int file_exists(char *file_name)
{
    FILE *f = fopen(file_name, "r");
    if (f == 0)
    {        
       fclose(f); 
       return 0; 
    }
    else {
       fclose(f); 
       return 1;
    }
}

void die(const char *msg, ...)
{
     printf("%s\n", msg);
     exit(EOF);     
}

int fprintln(char *target, const char *line)
{
    FILE *f = fopen(target, "a");
    if (f == 0)
       return 0;
    else
    {
        fprintf(f, "%s\n", line);
        fclose(f);
        return 1;    
    }
}
