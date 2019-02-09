# include<stdio.h>
# include<string.h>
# include<stdlib.h>

int main(){

FILE *fp,*fi,*fo,*ff;
int start,counter,counter1,counter2,Pstart,Psize,size,temp,i,j,k,sp,l,m=0,n;
long int a,b;
char addr[10],addr1[10],ch[55],label[10],opt[10],opr[10],opt1[10],opvalue[10],refer[40][30],ch1,ch2;
fp=fopen("Inter.txt","r+");
fi=fopen("Symtab.txt","r+");
fo=fopen("Optab.txt","r+");
ff=fopen("Object.txt","w+");

if(!fp||!fi||!fo)
    printf("File Doesn't Exist!!!");

else{


//WRITING HEADER RECORD-----------------------------------------------------------------------------




    fscanf(fp,"%s%s%s%s",addr,label,opt,opr);
    counter = strtol(opr,NULL,16);
    Pstart = strtol(opr,NULL,16);
    a=ftell(fp);
    if(strcmp(opt,"START")==0)
    {
        fprintf(ff,"H^%s  ^00%0X",label,counter);

            while(!feof(fp)){

            sp=0;

            fscanf(fp,"%s",addr);
            b=ftell(fp);
            fgets(ch,30,fp);

            fseek(fp,b,SEEK_SET);

            for(i=0;ch[i]!='\0';i++)
              if(ch[i]==' '&&ch[i+1]!=' ')
                   sp++;

            if(sp==1)
               fscanf(fp,"%s",opt);

            if(sp==2)
               fscanf(fp,"%s%s",opt,opr);

            if(sp==3)
               fscanf(fp,"%s%s%s",label,opt,opr);

            }

            counter1 = strtol(addr,NULL,16);
            Psize = counter1-counter;
            fprintf(ff,"^00%0X",Psize);
            fseek(fp,a,SEEK_SET);

    }


//--------------------------------------------------------------------------------------------





//FOR TEXT RECORD-------------------------------------------------------------------------



//CALCULATING TEXT RECORD ROW SIZE------------------------------------------------------------


int m=0;

do{

a=ftell(fp);
size = 0;
fscanf(fp,"%s",addr);
fseek(fp,a,SEEK_SET);
start = strtol(addr,NULL,16);

do{

            sp=0;

            fscanf(fp,"%s",addr);
            counter = strtol(addr,NULL,16);
            size+=counter-start-size;

            b=ftell(fp);
            fgets(ch,30,fp);

            fseek(fp,b,SEEK_SET);

            for(i=0;ch[i]!='\0';i++)
              if(ch[i]==' '&&ch[i+1]!=' ')
                   sp++;

            if(sp==1)
               fscanf(fp,"%s",opt);

            if(sp==2)
               fscanf(fp,"%s%s",opt,opr);

            if(sp==3)
               fscanf(fp,"%s%s%s",label,opt,opr);


}while((size<=27)&&(strcmp(opt,"RESB")!=0)&&(strcmp(opt,"RESW")!=0)&&(strcmp(opt,"END")!=0));


//----------------------------------------------------------------------------------------------




//WRITING TEXT RECORD---------------------------------------------------------------------------



fseek(fp,a,SEEK_SET);

if(size<10)
fprintf(ff,"\nT^00%0X^0%0X",start,size);
else
fprintf(ff,"\nT^00%0X^%0X",start,size);

j=0;



while(!feof(fp)&&j<size){

                l=0;
                k=1;
                fseek(fo,0,SEEK_SET);
                fseek(fi,0,SEEK_SET);
                sp=0;
                fscanf(fp,"%s",addr);
                b=ftell(fp);
                fgets(ch,30,fp);

                if(ch[0]=='\n'){

                    fgets(ch,25,fp);
                    fgets(ch,25,fp);
                    continue;

                }

                fseek(fp,b,SEEK_SET);

                        for(i=0;ch[i]!='\0';i++)
                          if(ch[i]==' '&&ch[i+1]!=' ')
                               sp++;

                        if(sp==1)
                           fscanf(fp,"%s",opt);

                        if(sp==2)
                           fscanf(fp,"%s%s",opt,opr);

                        if(sp==3)
                           fscanf(fp,"%s%s%s",label,opt,opr);

                        a=ftell(fp);
                        fscanf(fp,"%s",addr);
                        temp = strtol(addr,NULL,16);
                        j+=temp-start-j;
                        fseek(fp,a,SEEK_SET);


                          if(sp==2){
                            for(i=0;opr[i]!='\0';i++)
                            if(opr[i]==','&&opr[i+1]=='X')
                            {
                                l=1;
                                opr[i]='\0';
                            }

                          }

                          if(strcmp(opt,"BYTE")==0){
                               if(opr[0]=='C'&&opr[1]=='\''){
                                 i=2;
                                 fprintf(ff,"^");
                                 while(opr[i]!='\'')
                                 fprintf(ff,"%0X",opr[i++]);
                               }
                               else if(opr[0]=='X'&&opr[1]=='\''){
                                 i=2;
                                 fprintf(ff,"^");
                                 while(opr[i]!='\'')
                                 fprintf(ff,"%c",opr[i++]);
                               }
                          }

                          else if(strcmp(opt,"WORD")==0){
                                 counter = strtol(opr,NULL,10);
                                 if(counter<16)
                                 fprintf(ff,"^00000%0X",counter);
                                 else if(counter<256)
                                 fprintf(ff,"^0000%0X",counter);
                                 else if(counter<4096)
                                 fprintf(ff,"^000%0X",counter);
                                 else
                                 fprintf(ff,"^00%0X",counter);
                          }

                          else
                          {
                            do{
                                fscanf(fo,"%s%s",opt1,opvalue);

                                 if(strcmp(opt,opt1)==0){
                                      k=0;
                                     break;
                                 }

                              }while(!feof(fo));

                            if(k==0){
                                fprintf(ff,"^%s",opvalue);
                            }
                                else{
                                    printf("Incorrect Mnemonic Operation %s Used, Object Code Can't Be Generated!!!!!\n",opt);
                                    continue;
                                }

                             if(sp==1)
                                fprintf(ff,"0000");

                             else{
                                do{
                                    fscanf(fi,"%s%s",addr1,opvalue);

                                     if(strcmp(opr,opvalue)==0){

                                         for(n=0;opr[n]!='\0';n++)
                                              refer[m][n] = opr[n];
                                              refer[m][n] = '\0';

                                           m++;
                                         break;
                                     }

                                  }while(!feof(fi));

                                counter = strtol(addr1,NULL,16);

                                if(l==0)
                                   fprintf(ff,"%0X",counter);

                                else if(l==1){
                                   counter=counter+0b1000000000000000;
                                   fprintf(ff,"%0X",counter);
                                }


                            }
                        }

}


//--------------------------------------------------------------------------------------------



//SCANNIG CONSECUTIVE RESB OR RESW DEFINING INSTRUCTIONS WHICH DON'T HAVE ANY OBJECT CODE-----


do{

            sp=0;

            a=ftell(fp);
            fscanf(fp,"%s",addr);

            b=ftell(fp);
            fgets(ch,30,fp);

            fseek(fp,b,SEEK_SET);

            for(i=0;ch[i]!='\0';i++)
              if(ch[i]==' '&&ch[i+1]!=' ')
                   sp++;

            if(sp==1)
               fscanf(fp,"%s",opt);

            if(sp==2)
               fscanf(fp,"%s%s",opt,opr);

            if(sp==3)
               fscanf(fp,"%s%s%s",label,opt,opr);


}while((strcmp(opt,"RESB")==0)||(strcmp(opt,"RESW")==0));

            fseek(fp,a,SEEK_SET);


//--------------------------------------------------------------------------------


}while(strcmp(opt,"END")!=0);



//--------------------------------------------------------------------------------



//WRITING END RECORD--------------------------------------------------------------


fprintf(ff,"\nE^00%0X",Pstart);


//--------------------------------------------------------------------------------




//CHECKING FOR LABELS THAT ARE NOT REFERENCED-------------------------------------


fseek(fp,0,SEEK_SET);

l=-1;
k=0;



do{

if(l>=1&&k==1)
    printf("The Labels That Are Not Referenced Are : ");

if(l>=1)
    printf(" %s",opvalue);

l=0;

fscanf(fi,"%s%s",addr1,opvalue);

if(strcmp(opvalue,"FIRST")==0)
    continue;

for(i=0;i<m;i++){
    if(strcmp(refer[i],opvalue)==0){
     l=0;
     break;
 }
l++;

// if(i==m-1)
  //  k++;
}

}while(!feof(fi));



//--------------------------------------------------------------------------------------------

}

//--------------------------------------------------------------------------------------------

fclose(fp);
fclose(fi);
fclose(fo);
fclose(ff);

}
