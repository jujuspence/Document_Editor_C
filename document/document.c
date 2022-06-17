#include "document.h"
#include <string.h>
#include <stdio.h>

/*Julian Spencer, 115970458, jspenc02*/

int replace_text_aux(char *line_location,const char *target,const char *replace);
int load_document_aux(Document *doc, int new_p);

int init_document(Document *doc, const char *name){

        if(doc == NULL || name == NULL){
                return FAILURE;
        }else if(1 > strlen(name) || strlen(name) > MAX_STR_SIZE){
                return FAILURE;
        }
        strcpy(doc->name, name);
        doc->number_of_paragraphs = 0;

        return SUCCESS;
}


int reset_document(Document *doc){

        if(doc == NULL){
        return FAILURE;
        }
        doc -> number_of_paragraphs = 0;
        return SUCCESS;
}

int print_document(Document *doc){
      
        int i,j,idx = 0,p_count = doc->number_of_paragraphs;
        Paragraph focus_p; 
    
        if(doc == NULL){
                return FAILURE;
        }
    
        printf("Document name: \"%s\"\n",doc->name);
        printf("Number of Paragraphs: %d\n",doc->number_of_paragraphs);

        while( p_count != 0){
                focus_p = doc->paragraphs[idx++];
                for(i = 0; i < focus_p.number_of_lines; i++){
                /*iterates through the lines of paragraph*/
                        j=0;
                        while(focus_p.lines[i][j] != '\0'){
                                printf("%c",focus_p.lines[i][j]);
                                j++;
                        }
                printf("\n");
                }
                p_count--;
                if(p_count != 0){
                printf("\n");
                }
        }


return SUCCESS;
}

int add_paragraph_after(Document *doc, int paragraph_number){

        int i;
        Paragraph added;

        if(doc == NULL || doc->number_of_paragraphs == MAX_PARAGRAPHS){
                return FAILURE;
        }
        if(paragraph_number < 0 || paragraph_number > doc->number_of_paragraphs){
                return FAILURE;
        }

        added.number_of_lines = 0;

        if(paragraph_number == doc->number_of_paragraphs){
                doc->paragraphs[paragraph_number] = added;
        }else if(paragraph_number == 0){
                for(i = doc->number_of_paragraphs; i > 0; i--){
                        doc->paragraphs[i] = doc->paragraphs[i - 1];
                }
                doc->paragraphs[0] = added;
        }else{
                for(i = doc->number_of_paragraphs;i > paragraph_number + 1; i--){
                        doc->paragraphs[i] = doc->paragraphs[i-1];
                }
                doc->paragraphs[paragraph_number] = added;
        }


        doc->number_of_paragraphs++;
        return SUCCESS;
}

int add_line_after(Document *doc, int paragraph_number, int line_number, const char *new_line){

        Paragraph *target_p = NULL;
        int i, total_lines;
        if(new_line == NULL || doc == NULL){
                return FAILURE;
        }else if(doc->number_of_paragraphs < paragraph_number){
                return FAILURE;
        }

        /*was dereferencing a NULL pointer*/
        target_p = &doc->paragraphs[paragraph_number - 1];
        total_lines = target_p->number_of_lines;

        if(total_lines < line_number || line_number < 0){
                return FAILURE;
        }

        if(line_number == total_lines){
                strcpy(target_p->lines[line_number],new_line);
        }else if(line_number == 0){
                for(i = total_lines; i > 0; i--){
                        strcpy(target_p->lines[i], target_p->lines[i-1]);
                }
                strcpy(target_p->lines[0],new_line);
        }else if(line_number > 0 && line_number < total_lines){

                for(i = total_lines; i >= line_number; i--){
                        strcpy(target_p->lines[i], target_p->lines[i-1]);
                }

                strcpy(target_p->lines[line_number],new_line);
        }

(target_p->number_of_lines)++;
return SUCCESS;
}

int get_number_lines_paragraph(Document *doc, int paragraph_number, int *number_of_lines){

        if(doc == NULL || number_of_lines == NULL){
                return FAILURE;
        }
        if(paragraph_number < 0 || paragraph_number > doc->number_of_paragraphs){
                return FAILURE;
        }

        *number_of_lines = doc->paragraphs[paragraph_number-1].number_of_lines;
                return SUCCESS;
}

int append_line(Document *doc, int paragraph_number, const char *new_line){

        Paragraph *target_p = NULL;
        int  total_lines;
        if(doc == NULL || new_line == NULL){
                return FAILURE;
        }
        if(paragraph_number <= 0 || paragraph_number > doc->number_of_paragraphs){
                return FAILURE;
        }

        target_p = &doc->paragraphs[paragraph_number-1];
        total_lines = target_p->number_of_lines;

        if(total_lines == MAX_PARAGRAPHS){
                return FAILURE;
        }


        strcpy(target_p->lines[total_lines],new_line);
        target_p->number_of_lines++;


return SUCCESS;
}

int remove_line(Document *doc, int paragraph_number, int line_number){

        Paragraph *target_p = &doc->paragraphs[paragraph_number-1];

        int i, total_lines = target_p->number_of_lines;

        if(doc == NULL || paragraph_number <= 0){
                return FAILURE;
        }
        if(paragraph_number > doc->number_of_paragraphs){
                return FAILURE;
        }

        if(line_number == 0 || target_p->number_of_lines < line_number){
                return FAILURE;
        }

        for(i = line_number-1; i < total_lines-1; i++){
                strcpy(target_p->lines[i], target_p->lines[i+1]);
        }

target_p->number_of_lines--;
return SUCCESS;
}

int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines){

        int i,count = 0,new_p = 1, p_count = doc->number_of_paragraphs,idx = 0;
        Paragraph *target_p = &doc->paragraphs[count];

        if(doc == NULL || data == NULL || data_lines == 0){
                return FAILURE;
        }
        if(p_count == MAX_PARAGRAPHS){
                return FAILURE;
        }

        for(i = 0; i < data_lines; i++){
                if(strcmp(data[i],"") == 0){
                        new_p++;
                }
        }


        if((new_p + p_count) > MAX_PARAGRAPHS){
                new_p = MAX_PARAGRAPHS - p_count;
        }

        if(p_count != 0){
                load_document_aux(doc,new_p);
        }

        for(i = 0; i < data_lines;i++){

                if(strcmp(data[i], "") == 0){
                        target_p = &doc->paragraphs[++count];
                        idx=0;
                        doc->number_of_paragraphs++;
                }else{
                        strcpy(target_p->lines[idx++],data[i]);
                        target_p->number_of_lines++;
                }
        }

        doc->number_of_paragraphs++;
        return SUCCESS;
}

int load_document_aux(Document *doc, int new_p){
/* shifts the document paragraphs to make room for new ones*/

        int i, p_count = doc->number_of_paragraphs,total_p;
        Paragraph blank_p;
        blank_p.number_of_lines = 0;
        total_p = new_p + p_count;

        for(i = total_p -1; i > new_p - 1; i--){
                doc->paragraphs[i] = doc->paragraphs[i - new_p];
        }

        for(i =0; i < new_p; i++){
                doc->paragraphs[i] = blank_p;
        }
return SUCCESS;
}

int replace_text(Document *doc, const char *target, const char *replacement){


        Paragraph *pgraphs;
        char *found;
        int i, p_count = 0;
        if(target == NULL || replacement == NULL || doc == NULL){
                return FAILURE;
        }

        while(p_count < doc->number_of_paragraphs){

                pgraphs = &doc->paragraphs[p_count++];

                for(i = 0; i < pgraphs->number_of_lines; i++){

                        if(strstr(pgraphs->lines[i], target) != NULL){
                                found = strstr(pgraphs->lines[i],target);
                                replace_text_aux(found, target,replacement);           
                        }
                }
        }

        return SUCCESS;
}

int replace_text_aux(char *line_location,const char *target,const char *replace){
        int target_len = (int)strlen(target);
        char *rest_of_line, *final = NULL;

        rest_of_line = &line_location[0] + target_len;

        strcpy(final,replace);
        strcat(final, rest_of_line);
        line_location = final;


return SUCCESS;
}

int highlight_text(Document *doc, const char *target){
        int end_target,t_size = (int)strlen(target);
        char replace[MAX_STR_SIZE];

        if(doc == NULL || target == NULL){
        return FAILURE;
        }
        end_target = t_size + 2;

        strcpy(replace,HIGHLIGHT_START_STR);
        strcat(replace,target);
        strcat(replace, HIGHLIGHT_END_STR);
        replace[end_target] = '\0';

        replace_text(doc, target, replace);


return SUCCESS;
}

int remove_text(Document *doc, const char *target){

        const char *blank = "";

        if(doc == NULL || target == NULL){
                return FAILURE;
        }

        replace_text(doc, target, blank);

return SUCCESS;
}
