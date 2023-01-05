
write_line = ""
line_width = 0
# Load and read the file
with open("keylist.txt") as file:
    for line in file:
        clean_line = line[0 : len(line)-1]
        if len(clean_line):
            key_string = ""
            for c in clean_line:
                if( c == ' ' or c == '('):
                    break
                key_string += c
            write_line += key_string
            write_line += ', '
            line_width += 1
            if( line_width > 16):
                line_width = 0
                print(write_line)
                write_line = ""