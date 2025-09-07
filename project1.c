#include <stdio.h>
#include <string.h>
#include <ctype.h>
void erase_space_in_string(char str[]){
    for(int i=0;i<strlen(str);i++){
        if(str[i]==' '){
            for(int j=i;j<strlen(str);j++){
                str[j]=str[j+1];
            }
            i--;
        }
    }
}
void confirm_correct_input(char str[]){
    int left_par=0, right_par=0;
    for(int i=0;i<strlen(str);i++){
        if(str[i]=='(') left_par++;
        else if(str[i]==')') right_par++;
    }
    if(left_par!=right_par){
        printf("ERROR\n");
        return;
    }
    if(str[0]!=('(')){
        printf("ERROR\n");
        return;
    }
    for(int i=1;i<strlen(str)-1;i++){
        if(isalpha(str[i])||str[i]=='('||str[i]==')'){
            continue;
        }
        else{
            printf("ERROR\n");
            return;
        }
    }
}
void remove_empty(char str[]){
    for(int i=0;i<strlen(str);i++){
        if(str[i]=='(' && str[i+1]==')'){
            for(int j=i;j<strlen(str);j++){
                str[j]=str[j+2];
            }
            i--;
        }
    }
}
int counting_degree(char str[]){
    for(int i=0;i<strlen(str)-1;i++){
        int degree=0;
        if(str[i]=='('){
            int par_count=1;
            int j=i+1;
            while(par_count!=0){
                if(str[j]=='(') par_count++;
                else if(str[j]==')') par_count--;
                else if(isalpha(str[j]) && par_count==1) degree++;
                j++;
            }
        }
        if(i==0 && degree==2){printf("False"); return 0;}
        else if(i!=0 && degree==0){printf("False"); return 0;}
        else if(degree>2){printf("False"); return 0;}
    }
    printf("True");
    return 0;
}
void check_binary_tree(char str[]){
    if(str[0]!='('){
        printf("ERROR\n");
        return;
    }
    counting_degree(str);
}
int main(){
    char tree[100];
    printf("트리를 입력하시오: ");
    fgets(tree, sizeof(tree), stdin);

    erase_space_in_string(tree);
    remove_empty(tree);
    confirm_correct_input(tree);
    check_binary_tree(tree);
    return 0;
}