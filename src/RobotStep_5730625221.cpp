#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <stack>
#include <vector>
#include <set>
#include <queue>

#include <fstream>

using namespace std;

int n = 20;
int moveCount = -1;
int dust = 0;
int check = 0;
queue<string> state;
vector<int> beforeDirec = {1,0,0,0};
int beforeBrush = 3;

int toInt(vector<int> &x);
void countUp(vector<int> &x);
int countZero(vector<vector<int>> &m);
int countDust(vector<vector<int>> &m);
void printMap(vector<vector<int>> &m);
bool canMove(int x, pair<int,int> pos, vector<vector<int>> &m);
bool stuck(vector<vector<int>> &m,pair<int,int> d);
int findPosFromBack (vector<pair<int,int>> vec, pair<int,int> value);
vector<int> getDirector( pair<int,int> now,  pair<int,int> past);

class Robot
{
public:
    int power;
    int camera;
    int brush;
    pair <int,int> posR;
    vector<int> direction;
    pair <int,int> posPa;

    Robot(int p, int c, pair<int,int> pos, vector<int> d, pair<int,int> posP) : power(p), camera(c), posR(pos), direction(d), posPa(posP){}

    int cleanDirt(int dirt)
    {
        return abs(brush - dirt);
    }

    void toggleCamera()
    {
        if(camera==0) camera = 1;
        else camera = 0;
    }

    void setBrush(int b)
    {
        brush = b;
    }

    void standBy()
    {
        power = 1;
    }

    void turnOn()
    {
        power = 2;
    }

    void turnOff()
    {
        power = 0;
    }

    void stopPosition()
    {
        direction = {1,0,0,0};
    }

    void setDirection(vector<int> di)
    {
        direction = di;
    }

    string genState(){
        char state[15];

        // POWERT       BIT0 BIT1
        switch(power) {
            case 0 : {state[0] = '0'; state[1] = '0'; break;}
            case 1 : {state[0] = '0'; state[1] = '1'; break;}
            case 2 : {state[0] = '1'; state[1] = '0'; break;}
            default : break;
        }
        // CAMERA       BIT2
        state[2] = camera + '0';

        // BRUSH        BIT3 BIT4 BIT5
        switch(brush) {
            case 0 :    {state[3] = '0'; state[4] = '0'; state[5] = '0'; break;}
            case 1 :    {state[3] = '0'; state[4] = '0'; state[5] = '1'; break;}
            case 2 :    {state[3] = '0'; state[4] = '1'; state[5] = '0'; break;}
            case 3 :    {state[3] = '1'; state[4] = '0'; state[5] = '0'; break;}
            case 4 :    {state[3] = '1'; state[4] = '0'; state[5] = '1'; break;}
            case 5 :    {state[3] = '1'; state[4] = '1'; state[5] = '1'; break;}
            default : break;
        }
        // DIRECTION    BIT6 BIT7 BIT8 BIT9 BIT10 BIT11 BIT12 BIT13
        state[6] = '0'; state[7] = '0'; state[8] = '0'; state[9] = '0'; state[10] = '0';
        state[11] = '0'; state[12] = '0'; state[13] = '0';
        switch(toInt(direction)) {
            case 0 :    {state[6]   = '1'; break;}
            case 1 :    {state[7]   = '1'; break;}
            case 2 :    {state[8]   = '1'; break;}
            case 3 :    {state[9]   = '1'; break;}
            case 4 :    {state[10]  = '1'; break;}
            case 5 :    {state[11]  = '1'; break;}
            case 6 :    {state[12]  = '1'; break;}
            case 7 :    {state[13]  = '1'; break;}
            default : break;
        }

        //  END_BIT     BIT10
        state[14] = '\0';
        return state;
    }

};

int main()
{
    ofstream myfile;
    myfile.open ("queue.txt");
    string st;
    vector<vector<int>> mapC(n);
    vector<vector<int>> mapTranFix(n);

    cout << "INPUT MAP (SIZE 20 x 20)\n";
    for(int i = 0 ; i < n ; i++)
    {
        for(int j = 0; j < n; j++)
        {
            int k;
            cin >> k;
            mapC[i].push_back(k);
            if(k == 0) mapTranFix[i].push_back(0);
            else mapTranFix[i].push_back(1);
        }
    }

    cout << "Dust Cout : "<< countDust(mapC) << endl;

    vector<vector<int>> mapTran(mapTranFix);

    pair<int,int> pos;
    pos = make_pair(15,5);

    vector<int> x = {1,0,0,0};

    stack<Robot> s;
    vector<pair<int,int>> sPos;

    pair<int,int> pospa(pos);

    /** ROBOT OFF **/
    Robot Ro = Robot(0,0,pos,x,pospa);
    Ro.setBrush(0);
    st = Ro.genState();
    myfile << st << endl;
    state.push(st);

    /** ROBOT STANBY **/
    Ro.standBy();
    st = Ro.genState();
    myfile << st << endl;
    state.push(st);

    /** ROBOT ON **/
    Ro.turnOn();
    st = Ro.genState();
    myfile << st << endl;
    state.push(st);

    /** OPEN CAMERA **/
    Ro.toggleCamera();
    st = Ro.genState();
    myfile << st << endl;
    state.push(st);
    s.push(Ro);

    int temp = 0;

    if(Ro.camera)  //Detect Dirt By Camera
    {
        /** SET BRUSH 3 **/
        Ro.setBrush(3);
        //myfile << "-------------------------------------" << endl;
    }

    while(!s.empty())
    {
        Robot R = s.top();
        s.pop();

        if(temp > 0) {
                //myfile << "------------------------------------- POP" << endl;
                pair<int,int> posPa = R.posPa;
                int ind = findPosFromBack(sPos,posPa);
                while(ind < sPos.size()-1) {
                    pair<int,int> present = sPos[sPos.size()-1];
                    pair<int,int> past = sPos[sPos.size()-2];
                    vector<int> di = getDirector(past,present);
                    Ro = Robot(2,1,present,beforeDirec,past);
                    Ro.setBrush(beforeBrush);

                    if(beforeDirec != di) {
                        Ro.stopPosition();
                        st = Ro.genState();
                        myfile << st << endl;
                        state.push(st);
                    }

                    if(beforeBrush != 0) {
                        Ro.setBrush(0);
                        st = Ro.genState();
                        myfile << st << endl;
                        state.push(st);
                    }

                    Ro.setDirection(di);
                    st = Ro.genState();
                    myfile << st << endl;
                    state.push(st);
                    beforeDirec = di;
                    beforeBrush = Ro.brush;
                    sPos.pop_back();
                    moveCount++;
                }
                //myfile << "------------------------------------- FIN " << endl;
                temp = 0;
                if(!stuck(mapTran,R.posR)){
                    sPos.push_back(R.posR);
                    moveCount++;

                    if(check == 0 && mapC[R.posR.first][R.posR.second] != 0) R.setBrush(3);
                    else if(check > 0 && mapC[R.posR.first][R.posR.second] != 0) R.setBrush(1);
                    else R.setBrush(0);

                    if(beforeBrush != R.brush) {
                        Ro = R;
                        if(toInt(beforeDirec) != 8) {
                            Ro.stopPosition();
                            Ro.setBrush(beforeBrush);
                            st = Ro.genState();
                            myfile << st << endl;
                            state.push(st);
                        }
                        Ro.setBrush(R.brush);
                        st = Ro.genState();
                        myfile << st << endl;
                        state.push(st);
                        beforeDirec = Ro.direction;
                        beforeBrush = R.brush;  /** SET NEW BEFORE BRUSH **/
                    }
                    if(mapC[R.posR.first][R.posR.second] != 0){
                        mapC[R.posR.first][R.posR.second] = R.cleanDirt(mapC[R.posR.first][R.posR.second]); //Clean
                    }

                    mapTran[R.posR.first][R.posR.second] = 0;

                    /** CHECK BEFORE POSTION (STOP POSITION) **/
                    if(R.direction != beforeDirec && toInt(beforeDirec) != 8 ) {
                        Ro = R;
                        Ro.stopPosition();
                        st = Ro.genState();
                        //myfile << st  << "    STOP"<< endl;
                        myfile << st << endl;
                        state.push(st);
                    }

                    /** NEXT STATE **/
                    st = R.genState();
                    myfile << st << endl;
                    state.push(st);
                    beforeDirec = R.direction;
                }
        }
        else {
            sPos.push_back(R.posR);
            moveCount++;

            if(check == 0 && mapC[R.posR.first][R.posR.second] != 0) R.setBrush(3);
            else if(check > 0 && mapC[R.posR.first][R.posR.second] != 0) R.setBrush(1);
            else R.setBrush(0);

            if(beforeBrush != R.brush) {
                Ro = R;
                if(toInt(beforeDirec) != 8) {
                    Ro.stopPosition();
                    Ro.setBrush(beforeBrush);
                    st = Ro.genState();
                    myfile << st << endl;
                    state.push(st);
                }
                if(R.brush != 0 && beforeBrush != 0) {
                    Ro.setBrush(0);
                    st = Ro.genState();
                    myfile << st << endl;
                    state.push(st);
                }
                Ro.setBrush(R.brush);
                st = Ro.genState();
                myfile << st << endl;
                state.push(st);
                beforeDirec = Ro.direction;
                beforeBrush = R.brush;  /** SET NEW BEFORE BRUSH **/
            }
            if(mapC[R.posR.first][R.posR.second] != 0){
                mapC[R.posR.first][R.posR.second] = R.cleanDirt(mapC[R.posR.first][R.posR.second]); //Clean
            }

            mapTran[R.posR.first][R.posR.second] = 0;

            /** CHECK BEFORE POSTION (STOP POSITION) **/
            if(R.direction != beforeDirec && toInt(beforeDirec) != 8 ) {
                Ro = R;
                Ro.stopPosition();
                st = Ro.genState();
                //myfile << st  << "    STOP"<< endl;
                myfile << st  << endl;
                state.push(st);
            }

            /** NEXT STATE **/
            st = R.genState();
            myfile << st << endl;
            state.push(st);
            beforeDirec = R.direction;
        }

        if(countZero(mapTran)==400 ) {
                //myfile << "-------------------------------------" << endl;
                beforeDirec = R.direction;
                mapTran = mapTranFix;
                check++;
                cout << "Dust Cout : "<< countDust(mapC) << endl;
        }


        cout << "Move Count : " << moveCount << endl;
        printMap(mapC);
        getch();

        if(countZero(mapC)==400) {
                printMap(mapC);
                cout << moveCount << " " << countDust(mapC)<< endl;
                R.stopPosition();
                st = R.genState();
                myfile << st << endl;
                state.push(st);
                R.setBrush(0);
                st = R.genState();
                myfile << st << endl;
                state.push(st);
                R.toggleCamera();
                st = R.genState();
                myfile << st << endl;
                state.push(st);
                R.standBy();
                st = R.genState();
                myfile << st << endl;
                state.push(st);
                R.turnOff();
                st = R.genState();
                myfile << st << endl;
                state.push(st);
                break;
        } // Finish Cleaning

        pair<int,int> posFather = R.posR;
        vector<int> x = {0,0,0,0};
        if(!stuck(mapTran,R.posR))
        {
            for(int i = 0 ; i < 8 ; i++)
            {
                pair<int,int> pos(R.posR);

                 if(canMove(toInt(x),pos,mapTran))
                {
                    switch (toInt(x)) {
                        case 0 : { pos.first--;                 break; }
                        case 1 : { pos.first--; pos.second++;   break; }
                        case 2 : { pos.second++;                break; }
                        case 3 : { pos.first++; pos.second++;   break; }
                        case 4 : { pos.first++;                 break; }
                        case 5 : { pos.first++; pos.second--;   break; }
                        case 6 : { pos.second--;                break; }
                        case 7 : { pos.first--; pos.second--;   break; }
                        default : break;
                    }

                    Robot R1(2,1,pos,x,posFather);
                    s.push(R1);
                }
                countUp(x);
            }
        }else{ temp = 1; }
    }

    myfile.close();
}

int toInt(vector<int> &x)
{
    int total = 0;
    for(int i = 0; i < 4 ; i++)
        total += (pow(2,i))*x[3-i];
    return total;
}

void countUp(vector<int> &x)
{
    if(x[3] == 0)
    {
        x[3] = 1;
        return;
    }
    else x[3] = 0;
    for(int i = 2 ; i >=0 ; i--)
    {
        if(x[i]==0)
        {
            x[i] = 1;
            break;
        }
        else x[i] = 0;
    }
    if(x[0]==1)
    {
        x[0]=x[1]=x[2]=x[3]=0;
    }
}

int countZero(vector<vector<int>> &m)
{
    int c = 0;
    for(int i = 0 ; i < n; i++)
    {
        for(int j = 0 ; j < n ; j++)
        {
            if(m[i][j]==0) c++;
        }
    }
    return c;
}

int countDust(vector<vector<int>> &m)
{
    int c = 0;
    for(int i = 0 ; i < n; i++)
    {
        for(int j = 0 ; j < n ; j++)
        {
            c += m[i][j];
        }
    }
    return c;
}

void printMap(vector<vector<int>> &m)
{

    char ans [1000];
    int index = 0;
    for(int i = 0; i < n ; i++)
    {
        for(int j = 0; j < n; j++)
        {
           if(m[i][j] == 0) ans[index] = '0';
           else ans[index] = '0' + m[i][j];
           ans[index+1] = ' ';
           index+=2;
        }
        ans[index] = '\n';
        index++;
    }
    ans[index] = '\0';
    cout << ans << endl;
}

bool canMove(int x, pair<int,int> pos, vector<vector<int>> &m)
{
    if(x==0)
    {
        if(pos.first - 1 < 0 || m[pos.first-1][pos.second] == 0)
            return false;
        else return true;
    }
    if(x==1)
    {
        if(pos.first - 1 < 0 || pos.second + 1 > (n-1) || m[pos.first-1][pos.second+1] == 0)
            return false;
        else return true;
    }
    if(x==2)
    {
        if(pos.second + 1 > (n-1)|| m[pos.first][pos.second+1] == 0)
            return false;
        else return true;
    }
    if(x==3)
    {
        if(pos.first + 1 > (n-1) || pos.second + 1 > (n-1) || m[pos.first+1][pos.second+1] == 0)
            return false;
        else return true;
    }
    if(x==4)
    {
        if(pos.first + 1 > (n-1) || m[pos.first+1][pos.second] == 0)
            return false;
        else return true;
    }
    if(x==5)
    {
        if(pos.first + 1 > (n-1) || pos.second - 1 < 0 || m[pos.first+1][pos.second-1] == 0)
            return false;
        else return true;
    }
    if(x==6)
    {
        if(pos.second - 1 < 0 || m[pos.first][pos.second-1] == 0)
            return false;
        else return true;
    }
    if(x==7)
    {
        if(pos.first - 1 < 0 || pos.second - 1 < 0 || m[pos.first-1][pos.second-1] == 0)
            return false;
        else return true;
    }
}

bool stuck(vector<vector<int>> &m,pair<int,int> d)
{
    vector<int> x = {0,0,0,0};
    for(int i = 0 ; i < 8 ; i++)
    {
        if(!canMove(toInt(x),d,m))
        {
            countUp(x);
        }
        else return false;
    }
    return true;
}

int findPosFromBack (vector<pair<int,int>> vec, pair<int,int> value) {
    for (int i = vec.size() - 1; i >= 0; --i) {
            if(vec[i] == value) return i;
    }
}

vector<int> getDirector( pair<int,int> now,  pair<int,int> past){
    vector<int> direction = {0,0,0,0};
    int f = now.first - past.first;
    int s = now.second - past.second;
    if      (f < 0  && s == 0)   direction = {0,0,0,0};
    else if (f < 0  && s > 0)    direction = {0,0,0,1};
    else if (f == 0 && s > 0)    direction = {0,0,1,0};
    else if (f > 0  && s > 0)    direction = {0,0,1,1};
    else if (f > 0  && s == 0)   direction = {0,1,0,0};
    else if (f > 0  && s < 0)    direction = {0,1,0,1};
    else if (f == 0 && s < 0)    direction = {0,1,1,0};
    else if (f < 0  && s < 0)    direction = {0,1,1,1};
    return direction;
}
