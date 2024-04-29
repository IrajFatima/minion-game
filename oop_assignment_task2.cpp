#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <string>
#include <iomanip>
#include "utility.h"     //set_cursor_at(x,y)  //read_fron(x,y)  //SetColor(text color,back color)
#include <chrono>
#include <cstdlib>
#include <random>  
using namespace std;
random_device rd;
mt19937 gen(rd());//for random numbers

class Minion{
    private:
        int speed;
        bool life;
        int power;
        int color;
        int x;
        int y;
    public:
        Minion(){
            uniform_int_distribution<> dis(0, 19);      //generating random numbers for x and y coordinates
            uniform_int_distribution<> dis_power(1, 5); //generating random numbvers for power
            uniform_int_distribution<> dis_color(1, 15);//generating random numbers for color

            speed=1;                  //initializing speed to 1
            life=true;                               
            power=dis_power(gen);
            color=dis_color(gen);
            x=dis(gen);
            y=dis(gen);
        }
        void show(){
            cout<<"power : "<<power<<"\ncolor : "<<color<<"\nx,y : ("<<x<<","<<y<<")"<<endl;
        }
        //setters
        void set_speed(int s){
            if(s>0)
                speed = s; 
        }
        void set_life(bool l){
            life = l;
        }
        void set_power(int p){ 
            if(p>=1 && p<=5)
                power = p;
            else
                cout<<"invalid input";

        }
        void set_color(int c){ 
            color = c; 
        }
        void set_x(int x){ 
            if(x>=1 && x<=20)
                this->x = x; 
            else
                cout<<"invalid input";
        }
        void set_y(int y) { 
            if(y>=1 && y<=20)
                this->y = y; 
            else
                cout<<"invalid input";
        }
        //getters
        int get_speed(){
            return speed;
        }
        int get_x(){
            return x;
        }
        int get_y(){
            return y;
        }
        int get_power(){
            return power;
        }
        bool get_life(){
            return life;
        }
        //display function
        void display(){
            if(life){
                char c=2;
                set_cursor_at(x,y);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
                cout<<c;
            }
            else
                erase_object();
        }
        void move(Minion*track_grid[20][20]){
            erase_object();     // to erase previous position of an object/minion
            check();
            int _x=x,_y=y;
            int temp=gen()%4+1;
            if(temp==1){
                y=(y-speed)%19;     //to move upward
                // if(y<0)
                //     y=y-speed+19;    
            }
            else if(temp==2){
                y=(y+speed)%19;       //to move downward
                // if(y>20)
                //     y=abs(abs(speed-y)-19);
            }
            else if(temp==3){
                x=(x-speed)%19;        //to move right
                // if(x<1)
                //     x=x-speed+19; 
            }           
            else {
                x=(x+speed)%19;        //to move left
                // if(x>=20)
                //     x=abs(abs(speed-x)-19);
            }
            check();    
            if(track_grid[x][y]!=NULL){           //checking if there exists a minion on new position
                compete(*(track_grid[x][y]),track_grid);
                track_grid[_x][_y]=NULL;           //clearing previous position
            }
            else{
                track_grid[_x][_y]=NULL;            //clearing porevious position
                track_grid[x][y]=this;              //setting new postion
            }
            display();                              //displaying minion on new position
        }
        void check(){
            while ( x >= 20){
                x--;
            }
            while ( y >= 20){
                y--;
            }
            while(x<0){
                x++;
            }
            while(y<0){
                y++;
            }
            if(x>=0 && x<20 && y>=0 && y<20);
            else
                cout<<"array out of bound";

        }
        void erase_object(){
            set_cursor_at(x,y);         //to erase object at old position
            cout<<' ';
        }
        void compete(Minion& p,Minion*track_grid[20][20]){
            /*comparing the powers of two minions usind 2d static pointer array
            +setting the pointer to null of defeated minion*/
            check();
            if(this->power==p.get_power()){        
                p.set_life(false);
                this->set_life(false);
                track_grid[this->x][this->y]=NULL;
                track_grid[p.get_x()][p.get_y()]=NULL;
            }
            else if(this->power==1 && p.get_power()==5){
                p.set_life(false);
                this->speed++;
                track_grid[p.get_x()][p.get_y()]=NULL;
                track_grid[p.get_x()][p.get_y()]=this;
            }
            else if(this->power==5 && p.get_power()==1){
                this->set_life(false);
                track_grid[this->x][this->y]=NULL;
                track_grid[this->x][this->y]=&p;
                p.set_speed(p.get_speed()+1);
            }
            else if(this->power>p.get_power()){
                p.set_life(false);
                this->speed++;
                track_grid[p.get_x()][p.get_y()]=NULL;
                track_grid[p.get_x()][p.get_y()]=this;
            }
            else if(this->power<p.get_power()){
                this->set_life(false);            
                p.set_speed(p.get_speed()+1);
                track_grid[this->x][this->y]=NULL;
                track_grid[this->x][this->y]=&p;
            }
            check();
        }
};
int  main(){
    Minion m[100];
    int i,j;
    Minion*track_grid[20][20]={NULL}; //creating 2d static pointer array to keep track of minion positionbs on 20 x 20 area
    for(i=0;i<100;i++){
        if(track_grid[m[i].get_x()][m[i].get_y()]==NULL){//chacking and storing addresses of minions 
            track_grid[m[i].get_x()][m[i].get_y()]=&m[i];
            // cout<<"hello"<<endl;
            // m[i].show();
        }
        else{
            // cout<<"world"<<endl;      
            // m[i].show();
            int p=m[i].get_x();
            int q=m[i].get_y();
            m[i].compete(*(track_grid[p][q]),track_grid);
            m[i].check();
            // cout<<"world";      
        }
    }
    for(i=0;i<100;i++){
        if(m[i].get_life())
            m[i].display();
            
    }
    // for(i=0;i<20;i++)
    //     for(j=0;j<20;j++)
    //         cout<<"minion"<<j+1<<" = "<<track_grid[i][j]<<endl;
    // for(j=0;j<100;j++)
    //     m[j].show();
    i=1;
    while(i){
        //infinite loop 
        for(j=0;j<20;j++)
            for(int k=0;k<20;k++)
                if(track_grid[i][j]==NULL){
                    set_cursor_at(i,j);
                    cout<<" ";
                }
        for(j=0;j<100;j++){//to move reach minion
            if(m[j].get_life()){  //checking if minion is alive or just continue
                m[j].move(track_grid);
                Sleep(50);        //to visualize hustling
            }
            else{
                m[j].erase_object();
                continue;
            }
        }
    }
        
}
/*infinite loop outside +inside loop to control each minion
+trackgrid in move function+condition to call compete in move fun*/ 
/*1_firstly check and compete initial pointers
2_compete fun->two pointers delete bitcops*/   