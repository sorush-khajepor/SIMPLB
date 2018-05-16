class Box{
    int begin[1];
    int end[1];
public:
    Box(const int& iXbegin,const int& iYbegin,const int& iXend,const int& iYend){
       begin[0]=iXbegin;
       begin[1]=iYend;
       end[0] = iXend;
       end[1] = iYend;
    }
    int& getBegin(const int& i) {return begin[i];}
    int& getEnd  (const int& i){return end[i];}
    int getLength(const int& i){return end[i]-begin[i];}
};
