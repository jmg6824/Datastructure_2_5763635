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
void counting_degree(char str[]){
    int count=0;
    for(int i=0;i<strlen(str);i++){
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
        if(i==0 && degree>=2){
            printf("False");
            return;
        }
        if(degree>2){
            printf("False");
            return;
        }
    }
    printf("True");
}
void check_binary_tree(char str[]){
    if(str[0]!='('){
        printf("ERROR\n");
        return;
    }
    counting_degree(str);
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
    check_binary_tree(str);
}
int main(){
    char tree[100];
    printf("트리를 입력하시오: ");
    fgets(tree, sizeof(tree), stdin);

    erase_space_in_string(tree);
    remove_empty(tree);
    confirm_correct_input(tree);
    return 0;
}