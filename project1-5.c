#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void trim(char str[]){
    for(int i=0;i<strlen(str);i++){
        if(isspace(str[i])){
            for(int j=i;j<strlen(str);j++){
                str[j]=str[j+1];
            }
            i--;
        }
    }
}
void is_correct_tree(char str[]){
    int count=0;
    for(int i=0;i<strlen(str);i++){
        if(str[i]=='('){
            count++;
        }
        else if(str[i]==')'){
            count--;
        }
        else if(!isalnum(str[i]) && str[i]!='(' && str[i]!=')'){
            printf("ERROR");
            exit(1);
            return;
        }
    }
    if(count!=0){printf("ERROR");exit(1);}
}
void is_binary_tree(char str[]){
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
            printf("ERROR");
            return;
        }
        if(degree>2){
            printf("Output: False");
            return;
        }
    }
    printf("Output: True");
}
int main(){
    printf("Input: ");
    char tree[100];
    fgets(tree,100,stdin);

    trim(tree); //공백 제거
    is_correct_tree(tree); //올바른 트리인지 확인
    is_binary_tree(tree); //이진 트리인지 확인
    return 0;
}
