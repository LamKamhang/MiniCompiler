int func(int a,int b){
    char var[128]="A";
    a=0x4455;
    b=0x6677;
    return a+b;
}
int main(){
    func(0x8899,0x1100);
    return 0;
}