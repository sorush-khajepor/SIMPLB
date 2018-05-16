#include <iostream>
using namespace std;


class Box{
    int begin[1];
    int end[1];
public:
    Box(){
       begin[0]=0;
       begin[1]=0;
       end[0] = 0;
       end[1] = 0;
    }
    Box(const int& iXbegin,const int& iYbegin,const int& iXend,const int& iYend){
      set(iXbegin,iYbegin,iXend,iYend);
    }
    void set (const int& iXbegin,const int& iYbegin,const int& iXend,const int& iYend){
       begin[0]=iXbegin;
       begin[1]=iYend;
       end[0] = iXend;
       end[1] = iYend;
    }
    int& getBegin(const int& i) {return begin[i];}
    int& getEnd  (const int& i){return end[i];}
    int getLength(const int& i){return end[i]-begin[i];}
    // Volume in 2D is surface area of the box
    int getVolume(){return getLength(0)*getLength(1);}
    void print(){
        cout<<"Begin vector iX="<<begin[0]<<" iY="<<begin[1];
        cout<<"End   vector iX="<<  end[0]<<" iY="<<end[1];
    }
};
