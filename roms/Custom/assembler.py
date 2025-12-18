import sys

def main():
    if(len(sys.argv) < 2):
        print("Usage: python assembler.py <rom.asm>")
        exit(1)
        
    ip_file = sys.argv[1]   
    op_file = ip_file.split(".")[0] + ".ch8"
    
    print(f"Assembling {ip_file}")
    program_data = assemble(ip_file)
    
    try:
        with open(op_file, 'wb') as file:
            file.write(program_data)
            print(f"Machine Code Saved to {file}")
            print(f"Size: {len(program_data)} Bytes")
            
    except IOError:
        print("Error Writing to file")
    
def assemble(ip_file):
    binary_opcode_data = bytearray()
    try:    
        with open(ip_file, 'r') as file:
            for line_num, line in enumerate(file, 1):
                content = line.strip() #remove empty lines
                if not content or content.startswith(";"): continue
                opcode = parse(content)
                if opcode is not None:
                    binary_opcode_data.extend(opcode.to_bytes(2, 'big')) #big Endian
                else:
                    print(f"Error at line: {line_num} Couldn't Parse {content}")
                
    except FileNotFoundError:
        print(f"Error: The file {ip_file} was not found.")
        exit(1)
        
    return binary_opcode_data
        
def parse(content):
        code = content.split(';')[0] #remove anything written after ;
        code = code.replace(',',' ')
        code = code.strip() #remove comment lines entirely
        
        tokens = code.split()
        if not tokens: return
        
        name = tokens[0].upper() #the instruction (JP/CLS)
        args = [arg.upper() for arg in tokens[1:]] #the args (V0/200)
        
        return lookup(name, args)
       
def lookup(name, args):
    match(name):
        case "CLS" : return 0x00E0
        case "RET" : return 0x00EE
        case "JP" : 
            if(args[0] == "V0"):
                addr = int(args[1], 16) #16 = Hexadecimal Base
                return 0xB000 | addr
            else:
                addr = int(args[0], 16)
                return 0x1000 | addr
        case "CALL" :
            addr = int(args[0], 16)
            return 0x2000 | addr
        case "SE" : 
            if(args[1].startswith("V")):
                x_val = int(args[0][1:], 16) #slice the first letter (V)
                y_val = int(args[1][1:], 16) #and convert rest to int (hexadecimal)
                return 0x5000 | (x_val << 8) | (y_val << 4)
            else:
                x_val = int(args[0][1:], 16)
                kk = int(args[1], 16)
                return 0x3000 | (x_val << 8) | kk
        case "SNE" : 
            if(args[1].startswith("V")):
                x_val = int(args[0][1:], 16)
                y_val = int(args[1][1:], 16)
                return 0x9000 | (x_val << 8) | (y_val << 4)
            else:
                x_val = int(args[0][1:], 16)
                kk = int(args[1], 16)
                return 0x4000 | (x_val << 8) | kk
        case "LD" : 
            return handle_ld(args)
        case "ADD" : 
            if(args[0].startswith("V")):
                x_val = int(args[0][1:], 16)
                if(args[1].startswith("V")):
                    y_val = int(args[1][1:], 16)
                    return 0x8004 | (x_val << 8) | (y_val << 4)
                else:
                    kk = int(args[1], 16)
                    return 0x7000 | (x_val << 8) | kk
            elif(args[0] == 'I'):
                x_val = int(args[1][1:], 16)
                return 0xF01E | (x_val << 8)
        case "OR" : 
            x_val = int(args[0][1:], 16)
            y_val = int(args[1][1:], 16)
            return 0x8001 | (x_val << 8) | (y_val << 4)
        case "AND" : 
            x_val = int(args[0][1:], 16)
            y_val = int(args[1][1:], 16)
            return 0x8002 | (x_val << 8) | (y_val << 4)
        case "XOR" : 
            x_val = int(args[0][1:], 16)
            y_val = int(args[1][1:], 16)
            return 0x8003 | (x_val << 8) | (y_val << 4)
        case "SUB" : 
            x_val = int(args[0][1:], 16)
            y_val = int(args[1][1:], 16)
            return 0x8005 | (x_val << 8) | (y_val << 4)
        case "SHR" : 
            x_val = int(args[0][1:], 16)
            y_val = int(args[1][1:], 16)
            return 0x8006 | (x_val << 8) | (y_val << 4)
        case "SUBN" : 
            x_val = int(args[0][1:], 16)
            y_val = int(args[1][1:], 16)
            return 0x8007 | (x_val << 8) | (y_val << 4)
        case "SHL" : 
            x_val = int(args[0][1:], 16)
            y_val = int(args[1][1:], 16)
            return 0x800E | (x_val << 8) | (y_val << 4)
        case "RND" : 
            x_val = int(args[0][1:], 16)
            kk = int(args[1], 16)
            return 0xC000 | (x_val << 8) | kk
        case "DRW" : 
            x_val = int(args[0][1:], 16)
            y_val = int(args[1][1:], 16)
            n = int(args[2], 16)
            return 0xD000 | (x_val << 8) | (y_val << 4) | n
        case "SKP" : 
            x_val = int(args[0][1:], 16)
            return 0xE09E | (x_val << 8)
        case "SKNP" : 
            x_val = int(args[0][1:], 16)
            return 0xE0A1 | (x_val << 8)

def handle_ld(args):
    
    if(args[0].startswith("V")):
        x_val = int(args[0][1:], 16)
        if(args[1].startswith("V")):
            y_val = int(args[1][1:], 16)
            return 0x8000 | (x_val << 8) | (y_val << 4)
            
        elif(args[1] == "DT"): return 0xF007 | (x_val << 8)
        elif(args[1] == "K"): return 0xF00A | (x_val << 8)
        elif(args[1] == '[I]'): return 0xF065 | (x_val << 8)
            
        else:
            kk = int(args[1], 16)
            return 0x6000 | (x_val << 8) | kk
            
    elif(args[0] == 'I'):
        addr = int(args[1], 16)
        return 0xA000 | addr
        
    elif(args[0] == '[I]'):
        x_val = int(args[1][1:], 16)
        return 0xF055 | (x_val << 8)
        
    elif(args[0] == 'F'):
        x_val = int(args[1][1:], 16)
        return 0xF029 | (x_val << 8)
        
    elif(args[0] == 'B'):
        x_val = int(args[1][1:], 16)
        return 0xF033 | (x_val << 8)
        
    elif(args[0] == 'DT'):
        x_val = int(args[1][1:], 16)
        return 0xF015 | (x_val << 8)
        
    elif(args[0] == 'ST'):
        x_val = int(args[1][1:], 16)
        return 0xF018 | (x_val << 8)


main()