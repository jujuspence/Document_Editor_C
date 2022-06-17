#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sysexits.h>
#include <ctype.h>
#include "document.h"

/*Julian Spencer, 115970458, jspenc02*/

static int valid_command(Document *doc, const char *line);

static int valid_command(Document *doc, const char *line){
/*validates a command, returns 0 for invalid command*/
        int i = 0,count = 0,l_num = 0,p_num = -1,failed = 0,args = 0, invalid = 0;
        char arg1[MAX_STR_SIZE + 1],command[20], **end_str = NULL,arg2[MAX_STR_SIZE+1],arg3[MAX_STR_SIZE+1],str[MAX_STR_SIZE+1], *data = NULL;
        const char *str1 = NULL, *str2 = NULL;

        args = sscanf(line," %s %s %s %s ",command,arg1,arg2,arg3);
        if(command[0] == '#'){
                return 0;
        }else if(args >= 2){
                str1 = &arg1[0];
                str2 = &arg2[0];
                if(isdigit(str1[0]) != 0){
                        p_num = (int)strtol(str1,end_str,10);
                }
        }
        if(strcmp(command,"exit") == 0 || strcmp(command,"quit") == 0){
                if(args == 1){
                        exit(EX_USAGE);
                }else{
                        return 0;
                }
  }else if(strcmp(command,"add_paragraph_after") == 0){

                if(args == 2){
                        if(p_num == -1){
                                return 0;
                        }else{
                                failed = add_paragraph_after(doc,p_num);
                                if(failed != SUCCESS){
                                        printf("add_paragraph_after failed\n");
                                }
                        }
                        return 1;
                }else{
                        return 0;
                }
        }else if(strcmp(command,"add_line_after") == 0){
                if(args == 4){
                        if(arg3[0] != '*' || p_num == -1){
                                return 0;
                        }
                        if(2 >= (int)strlen(str2)){
                                if(!isdigit(arg2[0])){
                                        return 0;
                                }
                                if((int)strlen(str2) == 2){
                                        if(!isdigit(arg2[1])){
                        /*cond. should catch strings and negatives*/
                                                return 0;
                                        }
                                }
                        }else{
                                return 0;
                        }
                        l_num = (int)strtol(str2,end_str,10);
                        strcpy(str, strchr(line, '*'));
                        str2 = str + 1;
                        failed = add_line_after(doc,p_num,l_num,str2);
                        if(failed != SUCCESS){
                                printf("add_line_after failed\n");
                        }
                }else{
                        return 0;
                }
                return 1;
  }else if(strcmp(command,"print_document") == 0){
                if(args == 1){
                        print_document(doc);
                }else{
                        return 0;
                }
                return 1;
        }else if(strcmp(command,"append_line") == 0){
                if(args >= 3){
                        if(p_num == -1){
                                return 0;
                        }else if(arg2[0] != '*'){
                                return 0;
                        }
                        strcpy(str, strchr(line, '*'));
                        str2 = str + 1;
                        failed = append_line(doc,p_num,str2);
                        if(failed != SUCCESS){
                                printf("append_line failed\n");
                        }

                }else{
                        return 0;
                }
                return 1;
        }else if(strcmp(command,"remove_line") == 0){
                if(args == 3 && p_num != -1){
                        while(i < (int)strlen(str2)){
                                if(!isdigit(str2[i++])){
                                        return 0;
                                }
                        }
                        l_num = (int)strtol(str2,end_str,10);
                        failed = remove_line(doc,p_num,l_num);
                        if(failed != SUCCESS){
                                printf("remove_line failed\n");
                        }
                }else{
                        return 0;
                }
                return 1;
 }else if(strcmp(command,"load_file") == 0){
                if(args == 2){
                        failed = load_file(doc,str1);
                        if(failed != 0){
                                printf("load_file failed\n");
                        }
                }else{
                        return 0;
                }
                return 1;
        }else if(strcmp(command,"replace_text") == 0){
                if(args >= 3){
                /*create while loop that captures string in ""*/
                        data = (char*)&line[12];
                        while(data[i] != '\0' && data[i] != '\n'){
                                /*str[i] = data[0];*/
                                if(data[i] == '"'){
                                        invalid++;
                                }else if(invalid == 1){
                                        arg1[count++] =(char)str[i];
                                }else if(invalid == 2){
                                        arg1[count] = '\0';
                                        count = 0;
                                }else if(invalid == 3){
                                        arg2[count++] = (char)str[i];
                                }else if(invalid == 4){
                                        arg2[count] = '\0';
                                        break;
                                }
                                i++;
                        }
                        if(invalid != 4){
                                return 0;
                        }
                        str1 = arg1;
                        str2 = arg2;
                        failed = replace_text(doc,str1,str2);
                        if(failed != SUCCESS){
                                printf("replace_text failed\n");
                        }
                }else{
                        return 0;
                }
                return 1;
 }else if(strcmp(command,"highlight_text") == 0){
                if(args >= 2){
                        data = (char*)&line[14];
                        while(str[i] != '\0' && str[i] != '\n'){
                                if(data[i] == '"'){
                                        invalid++;
                                }else if(invalid == 1){
                                        arg1[count++] = (char)str[i];
                                }else if(invalid == 2){
                                        break;
                                }
                                i++;
                        }
                        if(invalid != 2){
                                return 0;
                        }
                        str1 = (char*)&arg1;
                        highlight_text(doc,str);
                }else{
                        return 0;
                }
                return 1;
        }else if(strcmp(command,"remove_text") == 0){
                if(args >= 2){
                        data = (char*)&line[11];
                        while(str[i] != '\0' && str[i] != '\n'){
                                if(data[i] == '"'){
                                        invalid++;
                                }else if(invalid == 1){
                                        arg1[count++] = (char)str[i];
                                }else if(invalid == 2){
                                        break;
                                }
                                i++;
                        }
                        if(invalid != 2){
                                return 0;
                        }
                        str1 = (char*)&arg1;
                        remove_text(doc,str1);
                }else{
                        return 0;
                }
 }else if(strcmp(command,"save_document") == 0){
                if(args == 2){
                        failed = save_document(doc,str);
                        if(failed != SUCCESS){
                                printf("save_document failed\n ");
                        }
                }else{
                        return 0;
                }
                return 1;
        }
        return 0;
}

int main(int arg_num, char *argv[]){
        FILE *input = NULL;
        Document doc; 
        int empty,invalid = 0;
        char first,line[MAX_STR_SIZE + 1];
        const char *line_arg = NULL;    

        init_document(&doc,"main_document");

        if(arg_num == 2){
                if(fopen(argv[1],"r")){
                        input = fopen(argv[1], "r");
                }else{
                        fprintf(stderr, "%s cannot be opened.", argv[1]);
                }
        }else if(arg_num == 1){
                input = stdin;
        }else{
                perror("Usage: user_interface\n");
                perror("Usage: user_interface <filename>\n");
                exit(EX_USAGE);
        }

        if(input == stdin){
        printf("> ");
        }
        fgets(line,MAX_STR_SIZE + 1, input);
        line_arg = &line[0];
        while(line_arg != NULL && strcmp(line,"quit\n") && strcmp(line,"exit\n") ){
                line[strcspn(line,"\n")] = 0;
                empty = sscanf(line_arg," %c",&first);
                if(empty != EOF  && first != '#'){
                        invalid = valid_command(&doc, line_arg);
                        if(invalid == 0){
                                printf("Invalid Command\n");
                        }
                }
                if(input == stdin){
                printf("> ");
                }
                line_arg = fgets(line,MAX_STR_SIZE + 1, input);
        }
fclose(input);
return SUCCESS;
}
