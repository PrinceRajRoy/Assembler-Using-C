# include<stdio.h>
# include<string.h>
# include<stdlib.h>

int main(){

FILE *fp,*fo,*fi;
int i,j=0,sp,LOCCTR,counter,cl,ERROR_FLAG=0,addrs,value;
long int a,b;
char ch[20],label[10],opt[10],opr[10],label1[10],ch1,ch2;
fp=fopen("Program.txt","r+");
fo=fopen("Symtab.txt","a+");
fi=fopen("Inter.txt","w+");

if(!fp)
    printf("File Doesn't Exist!!!");

else{


    a=ftell(fp);
    fscanf(fp,"%s %s",label,opt);                                        //Read First Line
    if(strcmp(opt,"START")==0){                                          //Checking If Opcode Is START
           fscanf(fp,"%s",opr);                                          //Initializing LOCCTR to starting address

    LOCCTR = strtol(opr,NULL, 16);

    fseek(fp,0,SEEK_SET);
    fgets(ch,25,fp);

    fprintf(fi,"%0X      %s",LOCCTR,ch);
    fseek(fi,-1,SEEK_CUR);
    }
    else
        {
        LOCCTR=0;                                          //else Initializing LOCCTR to 0
        fseek(fp,0,SEEK_SET);
        }

    while(!feof(fp)){

    fseek(fo,0,SEEK_SET);
    fgets(ch,25,fp);

    if(ch[0]=='.'){
    fprintf(fi,"\n          %s",ch);
    fgets(ch,25,fp);
    fprintf(fi,"          %s",ch);
    fgets(ch,25,fp);
    ch[strlen(ch)-1]='\0';
    fprintf(fi,"          %s",ch);
    continue;
    }

    sp=0;

    b=strlen(ch)+1;

       fseek(fp,-b,SEEK_CUR);

    for(i=0;ch[i]!='\0';i++)
          if(ch[i]==' '&&ch[i+1]!=' ')
            sp++;

    fprintf(fi,"\n%0X      %s",LOCCTR,ch);
    fseek(fi,-1,SEEK_CUR);


    if(sp==1){

            fscanf(fp,"%s",opt);
            ch1=fgetc(fp);

            if(j>0)
            LOCCTR+=3;

            }


    else if(sp==2){

    ch1=fgetc(fp);

    fseek(fp,-1,SEEK_CUR);

          if(ch1==' '){                                               //Only Opcode And Operand Present

            fgets(ch,25,fp);

            if(j>0)
               LOCCTR+=3;

          }

          else{                                                      //All of them (Label,Opcode And Operand) Are Present

              fscanf(fp,"%s %s %s",label,opt,opr);

              if(strcmp(label,"END")==0)
                break;

              ch1=fgetc(fp);

                cl=0;

                          do{
                            fscanf(fo,"%s %s",addrs,label1);

                                  if(strcmp(label,label1)==0)
                                     {
                                         cl=1;
                                         break;
                                     }

                            }while((ch2=fgetc(fo))!=EOF);

                                 if(cl>0)
                                      ERROR_FLAG=1;

                                  else
                                      fprintf(fo,"\n%0X   %s",LOCCTR,label);


                                  if(strcmp(opt,"WORD")==0)
                                         LOCCTR+=3;

                                  else if(strcmp(opt,"RESW")==0){

                                        counter=atoi(opr);
                                        LOCCTR+=3*counter;

                                       }

                                  else if(strcmp(opt,"RESB")==0){

                                        counter=atoi(opr);
                                        LOCCTR+=counter;

                                        }

                                  else if(strcmp(opt,"BYTE")==0){
                                       if(opr[0]=='C')
                                         LOCCTR+=strlen(opr)-3;
                                       else if(opr[0]=='X')
                                         LOCCTR+=(strlen(opr)-3)/2;
                                  }

                                  else
                                         LOCCTR+=3;

                }
            }
      j++;
    }
}

fclose(fp);
fclose(fo);
fclose(fi);

}

