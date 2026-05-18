/**
* @author Nia Jacobs, 2002559351, Assignment 1
* @brief This Program consists of (1) The Assembler and (2) The Interpretor. The Assembler uses 2 passes ((1)Cleaning up code, I use Intermediate.txt for outputting this clean code (2) turning this cleaned code
into machine code through it's opcode and label location). The first pass will get rid of comments, white space, and store labels. The second pass will read intermediate.txt, find the
values of the col 2 and col3(if there is one), and calculate it's memory location. Finally the interpretation, uses the memory location to execute the code through if else statements based on
the opcode chart provided to us and the symbol table(for jumping back to where the symbol was found in col 1 and storing values).
* @note I pledge my word of honor that I have complied with UNLV's
* Academic Integrity Policy while completing this assignment.
* @note Expected input includes 6 test files that represent assembly code (test1.txt.test2.txt.....test6.txt)
* @note After being passed through an assembler, the interpretor should execute each line of machine code. Output is based on what is executed such as calculating a sum, area, sum of squares, seqeuence, or loop.
This is then output to a terminal; however, during the assembler, I use "intermediate.txt" as both a output and input file.
*/

// Libraries
#include <iostream>
#include <iomanip> 
#include <fstream>
#include <string>

using namespace std;
// Global constant variables
const int LABEL_COUNT = 100; // Default number of labels
const int OPCODE_COUNT = 18; // Default number of opcodes
const int MAX_CHARS = 5; // Maximum number of characters
const int STACK_SIZE = 1000; // Maximum capacity of array
// list of opcodes needed for interpretation
const string OPCODE_LIST = "const get put ld st add sub mul div cmp jpos jz j jl jle jg jge halt";

// <--
// Structs=====================================================================================================
struct Symbol
{
    string name;
    int mem;
};

struct Opcode
{
    string name;
    int code;
};

//Functions====================================================================================================

/**
* @brief Splits Opcode List into the different names and stores them in an array.
* @param intruc the line of string taken in. In this case the intructions of this language
* @param arr the array where the instruction names are stored
* @return is void, but updates the Opcode array for storing given table
*/
void splitOpcodes(string instruc, Opcode arr[])
{
    
    string word = " "; 
    int space = 0;
    string line = instruc;
    //cout << line << endl;

    for (int i = 0; i < 17; i++) // i could replace 17 with line.length() - 1 for more inclusive code
    {
        space = line.find(' ');
        //cout << space << endl;
        word = line.substr(0, space);
        arr[i].name = word;
        //cout << arr[i].name << endl;
        line = line.substr(space + 1, line.length());
    }
    
    word = line.substr(0, line.length());
    arr[17].name = word;
    //cout << arr[17].name << endl;

}

/**
* @brief returns bool of input matching a opcode name
* @param input input string, supposed to be an instruction
* @param arr the array where the instruction names are stored
* @return boolean, opcode or not
*/
bool isOpcode(string input, Opcode arr[])
{
    bool valid = false;

    for (int i = 0; i < 18; i++)
    {
    if (input == arr[i].name)
    {
        valid = true;
    }
    }

    return valid;
}

/**
* @brief returns bool of input matching a symbol name
* @param input input string, supposed to be a label
* @param arr the array where the symbols are stored
* @param int updates where the symbol is found
* @return if it's a symbol or not 
*/
bool isSymbol(string input, Symbol arr[], int &the_one)
{
    bool valid = false;

    for (int i = 0; i < 100; i++)
    {
    if (input == arr[i].name)
    {
        valid = true;
        the_one = i;
        break;
    }
    }

    return valid;
}

/**
* @brief returns int assigned to opcode name
* @param  opname string of supposed opcode name
* @param arr the array where the instruction names are stored
* @return the opcode number stored with the name of the opcode
*/
int getOpcode (string opname, Opcode arr[])
{
    int code = -1;
    if (isOpcode(opname, arr) == true)
    {
    for (int i = 0; i < 18; i++)
    {
        if (opname == arr[i].name)
        {
            code = arr[i].code;

        }
    }
    }

    return code;
}




/**
* @brief sees the memory location based on if there is a label or not
* @param  opname string of opcode name
* @param arr the array where the instruction names are stored
* @param table the array where symbol's are stored
* @return the memory location of the label address
*/
int getLocation (int address, int line_number, string sym, Symbol s_table[])
{
    int memory_location = 0;

    if (isSymbol(sym, s_table, address) == true && sym != "")
    {
        memory_location = address;
    }
    else
    {
        memory_location = line_number; //instead of -1 I set it to line_number, to go to next avaible memory
    }
    //questions for TA: what does position of nect available memory location mean.. what happens when

    return memory_location;

}

/**
* @brief returns if the label is a number
* @param  label The label name to be judged
* @return if it's a number or not 
*/
bool isNumber (string label) 
{
    bool number = false;

    if (label == "one" || label == "zero" || label == "ten" )
    {
        number = true;
    }

    return number;
}


/**
* @brief Made of (1) Assembler: Take input "assembly" code turns it into machine code within two passes
and (2)Interpretor: Will execute code by reading the machine code
* @param argc The number of command-line arguments.
* @param argv A vector of command-line arguments.
* @return an int for a successful run or not
*/
int main(int argc, char *argv[]) { // ENTERING MAIN POINT -------------------------------------------------------------------------------------------------------------

Opcode table[18];
Symbol s_table[LABEL_COUNT] = { };
for (int i = 0; i < 18; i++)
{
    table[i].code = i;
}
// parse the list of opcodes
splitOpcodes(OPCODE_LIST, table);
// check command-line arguments
string open = "";
string arg;
arg = argv[1];

if (argv[1][4] == '1') // checking the number after test in the cmmd arg.
{
    open = "test1.txt";
}
else if (argv[1][4] == '2')
{
    open = "test2.txt";
}
else if (argv[1][4] == '3')
{
    open = "test3.txt";
}
else if (argv[1][4] == '4')
{
    open = "test4.txt";
}
else if (argv[1][4] == '5')
{
    open = "test5.txt";
}
else if (argv[1][4] == '6')
{
    open = "test6.txt";
}
else 
{
    cout << "ERROR" << endl;
    return 0;
}

//cout << open << endl;

//
//
// ASSEMBLER PASS #1 oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
//variables
string line;
ifstream file;
ofstream inter_code;
string pass1_opcode[100] = {}, pass1_operand[100] = {}; 
file.open(open); //testing files
inter_code.open("intermediate.txt");

// (1) Removing comments ==========================================================================================================================================================
int line_number = 0;
while (getline(file, line))
{

   bool col3 = false; 

    if(line.find(';') < 100 )
    {
      if (line.find(';') != 0) // if the comment happens after valid info
      {
      line = line.substr(0,line.find(';'));  
      }
      else // if the comment is the whole line
      {
      continue; 
      }
    }

    if (line.find(' ') > 1000 && line.find('\t') > 1000) // skips past empty line
    {
        continue;
    }
   
    

// (2) Remember labels ========================================================================================================================================================
    int first = 0, last = 0; //token from first char to last char
    bool label = false;
    bool number = false;

    //cout << line_number << " : " <<  (line[0] != ' ') << endl;

    if (line[0] != ' ' && line[0] != '\t')
    {
        label = true;
        for (int j = 0; j < line.length(); j++)
        {
            if(line[j] == ' ' || line[j] == '\t') //if a space or tab: used as a seperator
            {
                        number = false;
                        last = j;
                        s_table[line_number].name = line.substr(first,last);
                        number = isNumber(s_table[line_number].name);
                        if (number == true)
                        {
                            if(s_table[line_number].name == "one")
                            {
                                s_table[line_number].mem = 1;
                            }
                            if(s_table[line_number].name == "zero")
                            {
                                s_table[line_number].mem = 0;
                            }
                            if(s_table[line_number].name == "ten")
                            {
                                s_table[line_number].mem = 10;
                            }
                        }
                        
                        line = line.substr(j, line.length()); //sets up for col 2 and a possible col3

                        //cout << "line:" << line_number << endl;
                        //cout << "label:" << s_table[line_number].name << endl;
                        
                        break;
            }
                    
         }
    }

    // (3) Save instruction/opcode and address ===========================================================================================================================
    
    first = 0, last = 0;
    bool opcode = false;
    bool code_got = false;
    bool column3 = false;
    
    if (line[0] == ' ' || line[0] == '\t') //runs if no col 1 or uses string from label extractor (starts with a space or tab)
    {
        
        for (int i = 0; i < line.length(); i++)
        {
           
            if (line[i] != ' ' && line[i] != '\t') //------------------------------
            {
               
               line = line.substr(i, line.length());
        
                if (opcode == false) //retrieving col 2
                {
                    for (int j = 0; j < line.length() + 1; j++)
                    {
                        if(opcode == true) 
                        {
                            break;
                        }
                    
                        if(line[j] == ' ' || line[j] == '\0' ||  line[j] == '\n' ||  line[j] == '\t') //getting opcode
                        {
                            first = 0;
                            //cout << line_number << ": " << line.find('\t') << endl;
                            if (line.find('\t') < 100)
                            {
                                last = line.find('\t');
                            }
                            else if (line.find(' ') < 100)
                            {
                                last = line.find(' ');
                            }
                            else
                            {
                                last = line.find(' ');
                            }

                            pass1_opcode[line_number] = line.substr(first, last);

                            if  (line[j] == '\0' || ((line.find(' ') > 1000) && (line.find('\t') > 1000)))
                            {
                                col3 = false;
                                opcode = true;
                                line = line.substr(j, line.length());
                            
                                break;
                            }
                            else 
                            {
                                col3 = true;
                            }
                       
                            line = line.substr(j, line.length());
                            //cout <<  line << endl;
                            //cout << line.length() << endl;
                            opcode = true;
                            break;
                        }
                    
                    }

                    if (opcode == true)
                    {
                        code_got = opcode;
                        column3 = col3;
                //cout << "opcode: " << opcode << endl;
                //cout << "col 3: " << col3 << endl;
                
                    break;
                    }
                }

            }  //////////////////////////////end of col 2
        
                
        }
    }//===========================================================================================================
        
       
        
    if ((code_got == true && column3 == true) == 1) // retrieving col 3, if needed +++++++++++++++++++++++++++++++++++++++++++++++++++++++
    {
                  
        first = 0, last = 0;
        bool done = false;
    
       
            for (int i = 0; i < line.length(); i++)
            {
                if (done == true)
                {
                    break;
                }

                if (line[i] != ' ' && line[i] != '\t') //------------------------------
                {
                               
                    line = line.substr(i, line.length());
                               
                                
                    for (int j = 0; j < line.length() + 1; j++)
                    {
                    
                        if(line[j] == ' ' || line[j] == '\0' ||  line[j] == '\n' ||  line[j] == '\t') //getting opcode
                            {
                                first = 0;
                                            
                                if (line.find('\t') < 100)
                                {
                                    last = line.find('\t');
                                }
                                else if (line.find(' ') < 100)
                                {
                                    last = line.find(' ');
                                }
                                else
                                {
                                    last = line.length();
                                }

                                pass1_operand[line_number] = line.substr(first, last);
                                          
                                done = true;
                                break;
                            }
                    }
                                
                }
            }
                    
                    
    } //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    // (4) Write generated code to an output file
    if (col3 == true)
    {
        for (size_t i = 0; i < pass1_operand[line_number].length(); i++)
        {
            if (pass1_operand[line_number][i] == ' ' || pass1_operand[line_number][i] == '\t')
            {
                pass1_operand[line_number] = pass1_operand[line_number].substr(0, i);
                break;
            }
        }
        
        for (size_t i = 0; i < pass1_opcode[line_number].length(); i++)
        {
            if (pass1_opcode[line_number][i] == ' ' || pass1_opcode[line_number][i] == '\t')
            {
                pass1_opcode[line_number] = pass1_opcode[line_number].substr(0, i);
                break;
            }
        }
        
        inter_code << pass1_opcode[line_number] << " " << pass1_operand[line_number] << endl;
        line_number++;
        
        //cout <<  pass1_operand[line_number] << endl;
        continue;
    }
    else
    {
        for (size_t i = 0; i < pass1_opcode[line_number].length(); i++)
        {
            if (pass1_opcode[line_number][i] == ' ' || pass1_opcode[line_number][i] == '\t')
            {
                pass1_opcode[line_number] = pass1_opcode[line_number].substr(0, i);
                break;
            }
        }
    inter_code << pass1_opcode[line_number] << endl;
    line_number++;
    }

    
} //end of while 



//
//
// --------------------------------------------
//
// PASS #2 ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
//
// (1) Read output file

ifstream machine_c;
string p2_line; //pass2 line
string pass2_opcode, pass2_operand;
machine_c.open("intermediate.txt");
int opcodee = 0, labell = 0, mem = 0; // doubling end letters to represent second pass var
int MemoryStack[STACK_SIZE] = {};
int counter = 0;

while (getline(machine_c, p2_line)) 
{

        int space = 0;
        space = p2_line.find(' ');
        pass2_opcode = p2_line.substr(0,space);
        //cout << pass2_opcode;

        p2_line = p2_line.substr(space + 1, line.length()-1);
        pass2_operand = p2_line.substr(0,line.length()-1);
        //cout << "|" << pass2_operand << "<3" << p2_line.find('\t') << endl;

        if(p2_line.find(' ') < 1000)
        {
            pass2_operand = p2_line.substr(0,p2_line.find(' '));
            
        }
 
        //cout << pass2_operand << endl;
// (2) Replace numeric values and pack them into
        opcodee = getOpcode(pass2_opcode, table);
        //cout << getOpcode(pass2_opcode, table) << endl;
        int the_one = 0; // the one is where symbol.name is, it's updated during getLocation's isSymbol()
        labell = getLocation(the_one,line_number, pass2_operand,s_table);
        mem = STACK_SIZE * opcodee + labell;
        MemoryStack[counter] = mem;
        counter++;
        //cout << mem << endl;

}
//cout << line_number << endl;

//  
       
//
// --------------------------------------------
//
// INTEPRETER
//
// (1) Execute instructions in sequential order

cout << "Running program..." << endl;
int r = 0, ip = 0; // register variable
int op_value = 0, addy = 0;


/* ignore for loops ++++ meant for testing
for (int i = 0; i < line_number; i++)
{
    cout << i << " : " << s_table[i].name << endl;
    if (s_table[i].name == " ")
    {
        cout << " uh oh" << endl;
    }
    
}
*/
/*
for (int i = 0; i < line_number - 1; i++)
{
   
    //cout << i << " ::: " "opcode: " <<  MemoryStack[ip] / 1000 << " , ";
    //cout << "addy: " <<  MemoryStack[ip] % 1000 << endl;
    ip++;
}
*/ 


do
{
    
    //cout << ip << endl;
    op_value = MemoryStack[ip] / 1000;
    addy = MemoryStack[ip] % 1000;

    if (table[op_value].name == "get")
    {
        cin >> r;

        if (cin.fail())
        {
           cout << "ERROR" << endl;
           return 0;

        }

        cout << "read: " << r << endl;
        ip++;
        
    }
    else if (table[op_value].name == "put")
    {
        cout << "result: " << r << endl;
        ip++;
    }
    else if (table[op_value].name == "ld")
    {
        r = s_table[addy].mem;
        ip++;
    }
    else if (table[op_value].name == "st")
    {
        
        s_table[addy].mem = r;
        ip++;
    }
    else if (table[op_value].name == "add")// ( 5 line) _+_+_+_+_+_+_+_+_+_+_+_
    {
        
        r += s_table[addy].mem;
        ip++;
    }
    else if (table[op_value].name == "sub")
    {
        r -= s_table[addy].mem;
        ip++;
    }
    else if (table[op_value].name == "mul")
    {
        r *= s_table[addy].mem;
        ip++;
    }
    else if (table[op_value].name == "div")
    {
        r /= s_table[addy].mem;
        ip++;
    }
    else if (table[op_value].name == "cmp")
    {
        r -= s_table[addy].mem;
        ip++;
    }
    else if (table[op_value].name == "jpos") // (10 line) _+_+_++_+_+_+_+_+_+_+
    {
       if (r>0)
       {
            ip = addy;
       }
       else
       {
            ip++;
       }
    }
    else if (table[op_value].name == "jz") 
    {
        if (r==0)
        {
            ip = addy;
        }
        else
        {
            ip++;
        }
    }
    else if (table[op_value].name == "j") 
    {  
        ip = addy;  
    }
    else if (table[op_value].name == "jl") 
    {
       if (r<0)
       {
            ip = addy;
       }
       else
       {
            ip++;
       }
    }
    else if (table[op_value].name == "jle") 
    {
       if (r<=0)
       {
            ip = addy;
       }
       else
       {
            ip++;
       }
    }
    else if (table[op_value].name == "jg") 
    {
       if (r>0)
       {
            ip = addy;
       }
       else
       {
            ip++;
       }
    }
    else if (table[op_value].name == "jge") 
    {
       if (r>=0)
       {
            ip = addy;
       }
       else
       {
            ip++;
       }
    }
    else if (table[op_value].name == "halt") 
    {
       ip = -1;
       //cout << "halted" << endl;
    }
    else
    {
        ip = -1;
        //cout << "nothing" << endl;
    }


    

} while (ip >= 0);

cout << "** Program terminated **" << endl;

// -->
return EXIT_SUCCESS;
}

