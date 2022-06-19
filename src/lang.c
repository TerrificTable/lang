#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>






int getLength(char *inp) {
    int i = 0;
    while (inp[i] != '\0') {
        i++;
    }
    return i+1;
}

int getIndex(char *inp, char find) {
    for (int i=0; i < getLength(inp); i++) {
        if (inp[i] == find) return i;
    }
    return -1;
}

void resetBuf(char buf[]) {
    for (int i=0; i < getLength(buf); i++) {
        buf[i] = '\0';
    }
}

void replaceChar(char buf[], char find, char replace) {
    for (int i=0; i < getLength(buf); i++) {
        if (buf[i] == find) buf[i] = replace;
    }
}

void remove_repeated(char *s, char *another) {
    for (int i=0; i < getLength(s); i++) {
        if (s[i] == another[i]) 
            s[i] = '\0';
    }
}




void lang(char *command_ptr, char *input) {
    bool comment = false;
    bool args = 0;
    char *command_pointer = command_ptr;
    char command;
    char buffer[1024];

    while (command = *command_pointer++)
        switch (command) {
        case '#':
            comment = true;
            break;
        case '\n':
            comment = false;
            break;

        case 'p':
            if (comment) break;
            while (command_ptr[0] != command) *command_ptr++;
            
            if (command_pointer[getIndex(command_ptr, command)] == 'u' && command_ptr[getIndex(command_ptr, command) + 2] == 't') {        
                for (int i=0; i < getLength(command_pointer); i++) {
                    if (command_pointer[i-1] == ' ' && !args) args = 1;
                    else if (command_pointer[i] == ';') {args = 0; break; }
                    if (args) { printf("%c", command_pointer[i]); *command_ptr++; }
                }
                printf("\n");
            } else { continue; }
            break;

        case 's':
            if (comment) break;
            while (command_ptr[0] != command) *command_ptr++;

            if (command_pointer[getIndex(command_ptr, command)] == 'y' && command_ptr[getIndex(command_ptr, command) + 2] == 's') {
                int j = 0;
                for (int i=0; i < getLength(command_pointer); i++) {
                    if (command_pointer[i-1] == ' ' && !args) args = 1;
                    else if (command_pointer[i] == ';') {args = 0; break; }
                    if (args) {
                        buffer[j] = command_pointer[i];
                        j++;
                    }
                }
                system(buffer);
                resetBuf(buffer);
                
            } else { continue; }
            break;    

        case 'f':
            if (comment) break;
            while (command_ptr[0] != command) *command_ptr++;

            if (command_pointer[getIndex(command_ptr, command)] == 'w') {
                int j = 0;
                for (int i=0; i < getLength(command_pointer); i++) {
                    if (command_pointer[i-1] == ' ' && !args) args = 1;
                    else if (command_pointer[i] == ';') {args = 0; break; }
                    if (args) {
                        buffer[j] = command_pointer[i];
                        j++;
                    }
                }
                char filename[1024];
                char content[1024];
                
                sscanf(buffer, "%s %255[^\r\n]", &filename, &content);

                FILE *fp = fopen(filename, "w");
                replaceChar(content, '\\n', '\n');
                fprintf(fp, "%s", content);
                fclose(fp);

                resetBuf(buffer);
            } else if (command_pointer[getIndex(command_ptr, command)] == 'r') {        
                while (command_ptr[0] != command) *command_ptr++;

                int j = 0;
                for (int i=0; i < getLength(command_pointer); i++) {
                    if (command_pointer[i-1] == ' ' && !args) args = 1;
                    else if (command_pointer[i] == ';') {args = 0; break; }
                    if (args) {
                        buffer[j] = command_pointer[i];
                        j++;
                    }
                }
                char filename[1024];
                char *buf = 0;
                long length;

                sscanf(buffer, "%s", &filename);


                FILE *fp = fopen(filename, "r");
                if (fp) {
                    fseek(fp, 0, SEEK_END);
                    length = ftell(fp);
                    fseek(fp, 0, SEEK_SET);
                    buf = (char*) malloc((length + 1) * sizeof(char));
                    
                    if (buf) {
                        fread(buf, sizeof(char), length, fp);
                    }
                    fclose(fp);

                    buf[length - 1] = '\0';
                    printf("%s", buf);
                } else {
                    printf("File '%s' not found!\n", filename);
                }


                fclose(fp);
                resetBuf(buffer);
            } else { continue; }
            break;

        case ' ':
            break;
        }
        
    printf("\n");
}


int main(int argc, char *argv[]) {
    char *buffer = 0;
    long length;
    FILE *fp;
    char *ch;

    fp = fopen (argv[1], "r");

    if (fp) {
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        buffer = (char*) malloc((length + 1) * sizeof(char));
        
        if (buffer) {
            fread(buffer, sizeof(char), length, fp);
        }
        fclose(fp);
    }
    buffer[length] = '\0';

    lang(buffer, "");
    return 0;
}
