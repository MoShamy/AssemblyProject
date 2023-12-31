#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <functional>
#include <cstdint>

using functs = std::map<std::string, std::function<void()>>;

int programCounter = 0;
int registerFile[32] = {0};
std::map<int, int> dataMemory;
functs functMap;

struct instruction
{
    std::string opcode;
    int rs;
    int rt;
    int rd;
    int immediate;
};

std::vector<instruction> instructions;

void parseRInstruction(instruction& temp, std::string line){
    temp.rd = stoi(line.substr(0, line.find(',')));
    line.erase(0, line.find(',') + 3);
    temp.rs = stoi(line.substr(0, line.find(',')));
    line.erase(0, line.find(',') + 3);
    temp.rt = stoi(line.substr(0, line.find(',')));
}


void parseIInstruction(instruction& temp, std::string line){

    int rt = stoi(line.substr(0, line.find(',')));
    temp.rt = rt;
    line.erase(0, line.find(',') + 3);
    int rs = stoi(line.substr(0, line.find(',')));
    temp.rs = rs;
    line.erase(0, line.find(',') + 2);

    // Extract the immediate value
    int immediate = stoi(line);
    temp.immediate = immediate;

}

//LUI AUIPC JAL
void parseUInstruction(instruction& temp, std::string line){
    temp.rd = stoi(line.substr(0, line.find(',')));
    line.erase(0, line.find(',') + 2);

    // Extract the immediate value
    int immediate = stoi(line);
    temp.immediate = immediate;
}

//BEQ ,BNE, BLT ,BGE, BLTU ,BGEU
void parseBInstruction(instruction& temp, std::string line){
    temp.rd = stoi(line.substr(0, line.find(',')));
    line.erase(0, line.find(',') + 3);
    temp.rs = stoi(line.substr(0, line.find(',')));
    line.erase(0, line.find(',') + 2);
    temp.rt = stoi(line.substr(0, line.find(',')));

    // Extract the immediate value
    int immediate = stoi(line);
    temp.immediate = immediate;

}

void parseJALRInstruction(instruction& temp, std::string line){
    temp.rd = stoi(line.substr(0, line.find(',')));
    line.erase(0, line.find(',') + 3);
    temp.rs = stoi(line.substr(0, line.find(',')));
    line.erase(0, line.find(',') + 2);

    // Extract the immediate value
    int immediate = stoi(line);
    temp.immediate = immediate;

}

//LB LW LBU LHU
void parseLoadInstruction(instruction& temp, std::string line){
    temp.rd = stoi(line.substr(0, line.find(',')));
    line.erase(0, line.find(',') + 1);
    temp.immediate = stoi(line.substr(0, line.find('(')));
    line.erase(0, line.find('(') + 2);
    temp.rs = stoi(line.substr(0, line.find(')')));
}


//SB SH SW
void parseStoreInstruction(instruction& temp, std::string line){
    temp.rt = stoi(line.substr(0, line.find(',')));
    line.erase(0, line.find(',') + 1);
    temp.immediate = stoi(line.substr(0, line.find('(')));
    line.erase(0, line.find('(') + 2);
    temp.rs = stoi(line.substr(0, line.find(')')));
}



void readInstructions(std::string filename)
{
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cout << "File not opened";
        exit(1);
    }

    while (getline(file, line)) {
        instruction temp;
        temp.opcode = line.substr(0, line.find(' '));
        line.erase(0, line.find(' ') + 2);

        try {
            //////////////////////////////
            if (temp.opcode =="FENCE"|| temp.opcode =="ECALL"|| temp.opcode =="EBREAK"){
                break;
            }
            else if (temp.opcode == "ADD" || temp.opcode == "SUB" || temp.opcode =="SLL"|| temp.opcode =="SLT"|| temp.opcode =="SLTU"|| temp.opcode =="XOR   "|| temp.opcode =="SRL"|| temp.opcode =="SRA"|| temp.opcode =="OR"|| temp.opcode =="AND") {
                parseRInstruction(temp, line);
            } else if (temp.opcode == "ADDI"|| temp.opcode == "SLTI" || temp.opcode =="SLTIU"|| temp.opcode =="XORI"|| temp.opcode =="ORI"|| temp.opcode =="ANDI"|| temp.opcode =="SLLI"|| temp.opcode =="SRLI"|| temp.opcode =="SRAI") {
                parseIInstruction(temp, line);
            }
            else if (temp.opcode =="BEQ"|| temp.opcode == "BNE"|| temp.opcode =="BLT"|| temp.opcode =="BGE"|| temp.opcode ==" BLTU" || temp.opcode =="BGEU"){
                parseBInstruction(temp, line);
            }
            else if (temp.opcode =="LUI"|| temp.opcode == "AUIPC"|| temp.opcode == "JAL" ){
                parseUInstruction(temp, line);
            }
            else if(temp.opcode =="JALR"){
                parseJALRInstruction(temp, line);
            }
            else if (temp.opcode =="LB"|| temp.opcode =="LW"|| temp.opcode =="LBU"|| temp.opcode =="LHU"){
                parseLoadInstruction(temp, line);
            }
            else if (temp.opcode =="SW"|| temp.opcode =="SB"|| temp.opcode =="SH"){
                parseStoreInstruction(temp, line);
            }


            else {
                std::cout << "Unknown instruction: " << temp.opcode << std::endl;
                exit(1); // Exit the program with an error status code.
            }

            instructions.push_back(temp);
        } catch (const std::exception& e) {
            std::cerr << "An exception occurred: " << e.what() << std::endl;
            exit(1);
        }
    }
}

// void printInstructions()
// {
//     for (int i = 0; i < instructions.size(); i++)
//     {
//         std::cout << instructions[i].opcode << " " << instructions[i].rs << " " << instructions[i].rt << " " << instructions[i].rd << " " << instructions[i].immediate << std::endl;
//     }
// }

std::string decToBinary(int n) 
{ 
    std::vector<int> binaryNum(32); 
    std::vector<int> arr(32);
    std::string output = "";
    // counter for binary array 
    int i = 0; 
    while (i < 32) { 
        // storing remainder in binary array and the rest with 0
        if ( n <= 0) {
            binaryNum[i] = 0;
            i++;
            continue;
        }
        else {
            binaryNum[i] = n % 2; 
            n = n / 2; 
            i++;
        }
    } 
    for (int j = i - 1; j >= 0; j--) 
        arr[32-1-j] = binaryNum[j]; 
    for (int i = 0; i < 32; i++)
        output += std::to_string(arr[i]);
    return output; 
} 
std::string decToHex(int n) 
{ 
    // ans string to store hexadecimal number 
    std::string ans = ""; 
    
    while (n != 0) { 
        int rem = 0; 
        char ch; 
        rem = n % 16; 
        if (rem < 10) { 
            ch = rem + 48; 
        } 
        else { 
            ch = rem + 55; 
        } 
        ans += ch; 
        n = n / 16; 
    } 
    int i = 0, j = ans.size() - 1; 
    while(i <= j) 
    { 
      std::swap(ans[i], ans[j]); 
      i++; 
      j--; 
    } 
    if (ans.empty()) {
        ans = "0";
    }
    return ans; 
}


void printRegisterFile()
{
    std::cout << "Register File:" << std::endl;
    for (int i = 0; i < 32; i++)
    {
        std::cout << "x" << i << ": " << registerFile[i] << "  | Binary : " << decToBinary(registerFile[i]) << "  | Hex: " << decToHex(registerFile[i]) << std::endl;
    }
}


void printDataMemory()
{
    std::cout << "Data Memory:" << std::endl;

    // if dqtq ,e,ory is e,pty print dqtq ,e,ory is e,pty else do the for loop

    for (auto it = dataMemory.begin(); it != dataMemory.end(); it++)
    {
        std::cout << it->first << ": " << it->second << "  ";
        std::cout << "| Bin:  " << decToBinary(it->first) << ": " << decToBinary(it->second) << "  ";
        std::cout << "| Hex:  " << decToHex(it->first) << ": " << decToHex(it->second) << std::endl;
    }
}

void printPC()
{
    std::cout << "PC: " << programCounter*4<< "  ";
    std::cout << "| Bin: " << decToBinary(programCounter*4) << "  ";
    std::cout << "| Hex: " << decToHex(programCounter*4) << std::endl;
}

void printAll()
{
    printRegisterFile();
    printDataMemory();
    printPC();
    std::cout<<"===============================================\n";
}

void addFunction()
{
    registerFile[instructions[programCounter].rd] = registerFile[instructions[programCounter].rs] + registerFile[instructions[programCounter].rt];
}

void subFunction()
{
    registerFile[instructions[programCounter].rd] = registerFile[instructions[programCounter].rs] - registerFile[instructions[programCounter].rt];
}

void andFunction()
{
    registerFile[instructions[programCounter].rd] = registerFile[instructions[programCounter].rs] & registerFile[instructions[programCounter].rt];
}

void orFunction()
{
    registerFile[instructions[programCounter].rd] = registerFile[instructions[programCounter].rs] | registerFile[instructions[programCounter].rt];
}

void luiFunction()
{
    registerFile[instructions[programCounter].rd] = instructions[programCounter].immediate << 12;
}

void auipcFunction()
{
    registerFile[instructions[programCounter].rd] = (instructions[programCounter].immediate << 12) + programCounter;
}

void jalFunction()
{
    // Save the return address in register rd
    registerFile[instructions[programCounter].rd] = programCounter + 1;

    // Jump to the target address
    programCounter += instructions[programCounter].immediate;
}

void jalrFunction()
{
    programCounter = registerFile[instructions[programCounter].rs] + (instructions[programCounter].immediate << 1);
    registerFile[instructions[programCounter].rd] = programCounter + 1;
}

void beqFunction()
{
    if (registerFile[instructions[programCounter].rs] == registerFile[instructions[programCounter].rd])
    {
        programCounter = programCounter + (instructions[programCounter].immediate << 1);
    }
}

void bneFunction()
{
    if (registerFile[instructions[programCounter].rs] != registerFile[instructions[programCounter].rt])
    {
        programCounter = programCounter + (instructions[programCounter].immediate << 1);
    }
}

void bltFunction()
{
    if (registerFile[instructions[programCounter].rs] < registerFile[instructions[programCounter].rd])
    {
        programCounter = programCounter + (instructions[programCounter].immediate << 1);
    }
}

void bgeFunction()
{
    if (registerFile[instructions[programCounter].rs] >= registerFile[instructions[programCounter].rt])
    {
        programCounter = programCounter + (instructions[programCounter].immediate << 1);
    }
}

void bltuFunction()
{
    uint32_t rs = registerFile[instructions[programCounter].rs];
    uint32_t rt = registerFile[instructions[programCounter].rt];

    if (rs < rt)
    {
        programCounter = programCounter + (instructions[programCounter].immediate << 1);
    }
}

void bgeuFunction()
{
    uint32_t rs = registerFile[instructions[programCounter].rs];
    uint32_t rt = registerFile[instructions[programCounter].rt];

    if (rs >= rt)
    {
        programCounter = programCounter + (instructions[programCounter].immediate << 1);
    }
}

void lbFunction()
{
    if (dataMemory.find(registerFile[instructions[programCounter].rs]) == dataMemory.end())
    {
        dataMemory[registerFile[instructions[programCounter].rs]] = 0;
        registerFile[instructions[programCounter].rd] = 0;
        return;
    }

    registerFile[instructions[programCounter].rd] = dataMemory[registerFile[instructions[programCounter].rs]] << 24 >> 24;
}

void lhFunction()
{
    if (dataMemory.find(registerFile[instructions[programCounter].rs]) == dataMemory.end())
    {
        dataMemory[registerFile[instructions[programCounter].rs]] = 0;
        registerFile[instructions[programCounter].rd] = 0;
        return;
    }

    registerFile[instructions[programCounter].rd] = dataMemory[registerFile[instructions[programCounter].rs]] << 16 >> 16;
}

void lwFunction()
{
    if (dataMemory.find(registerFile[instructions[programCounter].rs]) == dataMemory.end())
    {
        dataMemory[registerFile[instructions[programCounter].rs]] = 0;
        registerFile[instructions[programCounter].rd] = 0;
        return;
    }

    registerFile[instructions[programCounter].rd] = dataMemory[registerFile[instructions[programCounter].rs]];
}

void lbuFunction()
{
    if (dataMemory.find(registerFile[instructions[programCounter].rs]) == dataMemory.end())
    {
        dataMemory[registerFile[instructions[programCounter].rs]] = 0;
        registerFile[instructions[programCounter].rd] = 0;
        return;
    }

    uint32_t temp = dataMemory[registerFile[instructions[programCounter].rs]];
    registerFile[instructions[programCounter].rd] = temp << 24 >> 24;
}

void lhuFunction()
{
    if (dataMemory.find(registerFile[instructions[programCounter].rs]) == dataMemory.end())
    {
        dataMemory[registerFile[instructions[programCounter].rs]] = 0;
        registerFile[instructions[programCounter].rd] = 0;
        return;
    }

    uint32_t temp = dataMemory[registerFile[instructions[programCounter].rs]];
    registerFile[instructions[programCounter].rd] = temp << 16 >> 16;
}

void sbFunction()
{
    dataMemory[registerFile[instructions[programCounter].rs] + instructions[programCounter].immediate] = registerFile[instructions[programCounter].rt] << 24 >> 24;
}

void shFunction()
{
    dataMemory[registerFile[instructions[programCounter].rs] + instructions[programCounter].immediate] = registerFile[instructions[programCounter].rt] << 16 >> 16;
}

void swFunction()
{
    dataMemory[registerFile[instructions[programCounter].rs] + instructions[programCounter].immediate] = registerFile[instructions[programCounter].rt];
}

void addiFunction()
{
    registerFile[instructions[programCounter].rt] = registerFile[instructions[programCounter].rs] + instructions[programCounter].immediate;
}

void sltiFunction()
{
    if (registerFile[instructions[programCounter].rs] < instructions[programCounter].immediate)
    {
        registerFile[instructions[programCounter].rt] = 1;
    }
    else
    {
        registerFile[instructions[programCounter].rt] = 0;
    }
}


///////////////////////////////////////////////////////////////////////////////////////
void sltiuFunction()
{
    uint32_t rs = registerFile[instructions[programCounter].rs];
    uint32_t immediate = instructions[programCounter].immediate;

    if (rs < immediate)
    {
        registerFile[instructions[programCounter].rt] = 1;
    }
    else
    {
        registerFile[instructions[programCounter].rt] = 0;
    }
}

void xoriFunction()
{
    registerFile[instructions[programCounter].rt] = registerFile[instructions[programCounter].rs] ^ instructions[programCounter].immediate;
}

void oriFunction()
{
    registerFile[instructions[programCounter].rt] = registerFile[instructions[programCounter].rs] | instructions[programCounter].immediate;
}

void andiFunction()
{
    registerFile[instructions[programCounter].rt] = registerFile[instructions[programCounter].rs] & instructions[programCounter].immediate;
}

void slliFunction()
{
    registerFile[instructions[programCounter].rt] = registerFile[instructions[programCounter].rs] << instructions[programCounter].immediate;
}

void srliFunction()
{
    registerFile[instructions[programCounter].rt] = uint32_t(registerFile[instructions[programCounter].rs]) >> instructions[programCounter].immediate;
}

void sraiFunction()
{
    registerFile[instructions[programCounter].rt] = registerFile[instructions[programCounter].rs] >> instructions[programCounter].immediate;
}

void sllFunction()
{
    registerFile[instructions[programCounter].rd] = registerFile[instructions[programCounter].rs] << registerFile[instructions[programCounter].rt];
}

void sltFunction()
{
    if (registerFile[instructions[programCounter].rs] < registerFile[instructions[programCounter].rt])
    {
        registerFile[instructions[programCounter].rd] = 1;
    }
    else
    {
        registerFile[instructions[programCounter].rd] = 0;
    }
}

void sltuFunction()
{
    uint32_t rs = registerFile[instructions[programCounter].rs];
    uint32_t rt = registerFile[instructions[programCounter].rt];

    if (rs < rt)
    {
        registerFile[instructions[programCounter].rd] = 1;
    }
    else
    {
        registerFile[instructions[programCounter].rd] = 0;
    }
}

void xorFunction()
{
    registerFile[instructions[programCounter].rd] = registerFile[instructions[programCounter].rs] ^ registerFile[instructions[programCounter].rt];
}

void srlFunction()
{
    registerFile[instructions[programCounter].rd] = uint32_t(registerFile[instructions[programCounter].rs]) >> registerFile[instructions[programCounter].rt];
}

void sraFunction()
{
    registerFile[instructions[programCounter].rd] = registerFile[instructions[programCounter].rs] >> registerFile[instructions[programCounter].rt];
}

void haltFunctions()
{
    printAll();
    exit(0);
}



void initFunctMap()
{
    functMap["ADD"] = addFunction;
    functMap["SUB"] = subFunction;
    functMap["AND"] = andFunction;
    functMap["OR"] = orFunction;
    functMap["XOR"] = xorFunction;
    functMap["NOR"] = xorFunction;
    functMap["SLT"] = sltFunction;
    functMap["ADDI"] = addiFunction;
    functMap["ANDI"] = andiFunction;
    functMap["ORI"] = oriFunction;
    functMap["XORI"] = xoriFunction;
    functMap["SLTI"] = sltiFunction;
    functMap["SLTIU"] = sltiuFunction;
    functMap["SLLI"] = slliFunction;
    functMap["SRLI"] = srliFunction;
    functMap["SRAI"] = sraiFunction;
    functMap["SLL"] = sllFunction;
    functMap["SRL"] = srlFunction;
    functMap["SRA"] = sraFunction;
    functMap["LUI"] = luiFunction;
    functMap["AUIPC"] = auipcFunction;
    functMap["JAL"] = jalFunction;
    functMap["JALR"] = jalrFunction;
    functMap["BEQ"] = beqFunction;
    functMap["BNE"] = bneFunction;
    functMap["BLT"] = bltFunction;
    functMap["BGE"] = bgeFunction;
    functMap["BLTU"] = bltuFunction;
    functMap["BGEU"] = bgeuFunction;
    functMap["LB"] = lbFunction;
    functMap["LH"] = lhFunction;
    functMap["LW"] = lwFunction;
    functMap["LBU"] = lbuFunction;
    functMap["LHU"] = lhuFunction;
    functMap["SB"] = sbFunction;
    functMap["SH"] = shFunction;
    functMap["SW"] = swFunction;
    functMap["ECALL"] = haltFunctions;
    functMap["FENCE"] = haltFunctions;
    functMap["EBREAK"] = haltFunctions;
}

void execute()
{
    while (programCounter < instructions.size())
    {
        functMap[instructions[programCounter].opcode]();
        programCounter++;
        printAll();
    }
}

int main()
{

    int x; //instruction file number
    std:: cout<<"Which instruction file would you like to use?\nEnter a number from 1 to 6: \n";
    std::cin>>x;
    readInstructions("instructions" + std::to_string(x) + ".txt");
    std::cout<<instructions.size()<<std::endl;
    initFunctMap();
    execute();
    printAll();
    return 0;
}
